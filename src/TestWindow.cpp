/**
 *  libwdm - raylib framework for developing Windows 7 stylish applications.
    Copyright (C) 2023  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "TestWindow.hpp"

#include <TextObject.hpp>
#include <TextFieldObject.hpp>
#include <ButtonObject.hpp>
#include <GlassObject.hpp>

#include <iostream>
#include <filesystem>

#include "TypeContainer.hpp"

#include "Registry.hpp"

#include "Notepad.hpp"

#include <thread>
#include <chrono>

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

TestWindow::TestWindow(DWM *owner) : Application(ApplicationType::Windowed, owner) {
    setWindowTitle("raylib [core] example - basic window");

    loadTexture2("ui/taskbar_app.png");
    loadTexture2("ui/taskbar_app_selected.png");

    auto glass_obj = new GlassObject(owner);

    auto text_hi = new TextObject("Select current wallpaper for this session:");
    
    text_hi->setColor(BLACK);
    text_hi->setFont(owner->accessFont("Segoe UI Bold", 25));
    text_hi->setFontSize(25);
    text_hi->setSpacing(0.5f);
    text_hi->setBlending(BLEND_MULTIPLIED);

    auto old_dir = GetWorkingDirectory();

    _files = 0;
    for (const auto& dirEntry : recursive_directory_iterator(_owner->getResourcesPath() + "/wallpapers")) {
        if (!dirEntry.is_directory()) { 
            _files++;
        }
    }

    int i = 0;

    int posX__ = 40;
    int posY__ = 70;

    i = 0;
    for (const auto& dirEntry : recursive_directory_iterator(_owner->getResourcesPath() + "/wallpapers")) {
        if (!dirEntry.is_directory()) {
            std::string f = dirEntry.path();
            std::thread thr([&](std::string filename, TestWindow *self) {
                auto img = LoadImage(filename.c_str());
                auto img_sel = ImageCopy(img);
                
                ImageResize(&img, 60, 40);
                ImageResize(&img_sel, 60, 40);

                ImageColorBrightness(&img_sel, 64);

                self->_images[filename] = {img, img_sel};
            }, f, this);

            thr.detach();

            posX__ += 70;

            i++;

            if ((i % 8) == 0) {
                posY__ += 50 + 10;
                posX__ = 40;
            }
        }
    }

    if (posY__ != 70) posY__ += 10;
    posX__ = 40;

    auto text1 = new TextObject("Applications");
    text1->setColor(BLACK);
    text1->setFont(owner->accessFont("Segoe UI Bold", 25));
    text1->setFontSize(25);
    text1->setSpacing(0.5f);
    text1->setBlending(BLEND_ALPHA_PREMULTIPLY);
    text1->setPosition(posX__, posY__);

    text_hi->setPosition(40, 40);

    auto btn_notepad = new ButtonObject(_textureMap["ui/taskbar_app.png"], _textureMap["ui/taskbar_app_selected.png"], [&](ButtonObject *self, void *arg) {
        auto dwm = reinterpret_cast<DWM *>(arg);
        auto notepad = new NotepadWindow(dwm);

        dwm->pushApplication(notepad);
    }, owner);

    btn_notepad->setPosition(posX__, posY__ + text1->getTextSizeY() + 10);

    glass_obj->setColor(BLACK);
    glass_obj->setSize(_windowSize.x, _windowSize.y);
    glass_obj->_cameraOutput = false;

    this->pushObject("glass", glass_obj);
    this->pushObject("text", text_hi);
    this->pushObject("text1", text1);
    this->pushObject("btn_notepad", btn_notepad);
}

void TestWindow::beginRendering(float delta) {
    // ClearBackground(WHITE);

    Application::beginRendering(delta);
}

void TestWindow::prerender(float delta) {
    _taskbarHidden = false;

    _camera.offset.y += (getMouseWheelMoveV().y * 20.f);

    if (_camera.offset.y > 0.f) {
        _camera.offset.y = 0.f;
    }

    if (_images.size() == _files) {
        int posX__ = 40;
        int posY__ = 70;

        int i = 0;

        for (auto [key, val] : _images) {
            auto filename = key;

            auto img = val[0];
            auto img_sel = val[1];

            auto texture = LoadTextureFromImage(img);
            auto texture2 = LoadTextureFromImage(img_sel);
                
            UnloadImage(img);
            UnloadImage(img_sel);

            loadTexture(filename + "@unselected", texture);
            loadTexture(filename + "@selected", texture2);

            auto container = new TypeContainer<std::string>();

            auto prefix = _owner->getResourcesPath();

            container->_value = filename.substr(prefix.size());

            auto btn = new ButtonObject(texture, texture2, [&](ButtonObject *self, void *arg) {
                auto con = static_cast<TypeContainer<std::string> *>(arg);

                Registry::registry["currentWallpaperPath"] = con->_value;
            }, container);

            btn->setPosition(posX__, posY__);

            posX__ += 70;

            this->pushObject("button" + std::to_string(i), btn);

            _containers.push_back(container);

            i++;

            if ((i % 8) == 0) {
                posY__ += texture.height + 10;
                posX__ = 40;
            }
        }

        _images.clear();
    }
}

TestWindow::~TestWindow() {
    int i = 0;

    while(i < _containers.size()) {
        auto container = _containers[i];

        delete container;
        _containers[i] = nullptr;

        i++;
    }

    processApplicationExit();

    // Application::~Application(); // that line causes stuff to break
}