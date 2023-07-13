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

#include "TaskbarApplication.hpp"

#include <TextureObject.hpp>
#include <TextObject.hpp>
#include <RectangleObject.hpp>
#include <ButtonObject.hpp>

#include "TestWindow.hpp"

#include <DWM.hpp>

#include <bits/stdc++.h>
#include <iostream>

TaskbarApplication::TaskbarApplication(DWM *owner) : Application(ApplicationType::FullscreenScalable, owner) {
    this->loadTexture2("taskbar_start.png");
    this->loadTexture2("show_hide_windows_button.png");
    this->loadTexture2("taskbar_middle.png");
    this->loadTexture2("taskbar_end.png");
    this->loadTexture2("winkey/frame1.png");
    this->loadTexture2("show_hide_windows_button_selected.png");
    this->loadTexture2("winkey/frame3.png");
    this->loadTexture2("ui/taskbar_app.png");
    this->loadTexture2("ui/taskbar_app_selected.png");

    auto taskbar_start = new TextureObject(_textureMap["taskbar_start.png"]);
    auto taskbar_middle = new TextureObject(_textureMap["taskbar_middle.png"]);
    auto taskbar_end = new TextureObject(_textureMap["taskbar_end.png"]);
    
    auto button_show_hide = new ButtonObject(_textureMap["show_hide_windows_button.png"], _textureMap["show_hide_windows_button_selected.png"], [&](ButtonObject *self, void *customArg) {
        auto t = static_cast<TaskbarApplication *>(customArg);

        int i = 0;

        while(i < t->_owner->_applications.size()) {
            auto app = static_cast<Application *>(t->_owner->_applications[i]);

            if (app != nullptr && app->_type <= ApplicationType::WindowedBorderless) {
                app->_hidden = !app->_hidden;
            }

            i++;
        }
    }, this);
    auto button_windows = new ButtonObject(_textureMap["winkey/frame1.png"], _textureMap["winkey/frame3.png"], [&](ButtonObject *self, void *customArg) {
        auto t = static_cast<TaskbarApplication *>(customArg);

        auto win = new TestWindow(t->_owner);
        
        t->_owner->pushApplication(win);
    }, this);

    auto blend = BLEND_MULTIPLIED;
    int dencity = 1;

    taskbar_start->setBlending(blend);
    taskbar_middle->setBlending(blend);
    taskbar_end->setBlending(blend);

    taskbar_start->_dencity = dencity;
    taskbar_middle->_dencity = dencity;
    taskbar_end->_dencity = dencity;

    this->pushObject("taskbar_middle", taskbar_middle);
    this->pushObject("taskbar_start", taskbar_start);
    this->pushObject("taskbar_end", taskbar_end);

    this->pushObject("button_windows", button_windows);
    this->pushObject("button_show_hide", button_show_hide);

    // taskbar_start->_hidden = true;
    // taskbar_middle->_hidden = true;
    // taskbar_end->_hidden = true;

    button_windows->setBlending(BLEND_ALPHA_PREMULTIPLY);
    button_show_hide->setBlending(BLEND_MULTIPLIED);

    setWindowTitle("win.shell.taskbar");

    _taskbarHidden = true;
    // _emptyFB = true;
}

TaskbarApplication::~TaskbarApplication() {
    processApplicationExit();
}

void TaskbarApplication::prerender(float delta) {
    auto taskbar_start = dynamic_cast<TextureObject *>(_objects["taskbar_start"]);
    auto taskbar_middle = dynamic_cast<TextureObject *>(_objects["taskbar_middle"]);
    auto taskbar_end = dynamic_cast<TextureObject *>(_objects["taskbar_end"]);
    
    auto button_show_hide = dynamic_cast<TextureObject *>(_objects["button_show_hide"]);
    auto button_windows = dynamic_cast<TextureObject *>(_objects["button_windows"]);

    // auto rectangle_workaround = dynamic_cast<RectangleObject *>(_objects["rectangle_workaround"]);

    auto rh = _windowSize.y;
    auto rw = _windowSize.x;

    taskbar_start->_y = rh - taskbar_start->getTexture().height;
    taskbar_start->_height = taskbar_start->getTexture().height;

    taskbar_middle->_width = rw - taskbar_end->getTexture().width - taskbar_start->getTexture().width;
    taskbar_middle->_height = taskbar_middle->getTexture().height;
    taskbar_middle->_y = taskbar_start->_y;
    taskbar_middle->_x = taskbar_start->getTexture().width;

    // rectangle_workaround->_width = rw;
    // rectangle_workaround->_height = taskbar_middle->getTexture().height;
    // rectangle_workaround->_y = taskbar_start->_y;
    // rectangle_workaround->_x = 0;

    taskbar_end->_height = taskbar_end->getTexture().height;
    taskbar_end->_x = rw - taskbar_end->getTexture().width;
    taskbar_end->_y = taskbar_middle->_y;

    button_show_hide->_x = rw - button_show_hide->getTexture().width;
    button_show_hide->_y = taskbar_end->_y;

    button_windows->_x = 1;
    button_windows->_y = taskbar_end->_y - 6;

    Rectangle mouse_rect = DWM::getMouse();

    auto btn_rect = button_windows->toRect();
    btn_rect.width -= 16;
    btn_rect.height -= 18;
    btn_rect.x += 8;
    btn_rect.y += 10;

    return;
}

void TaskbarApplication::beginRendering(float delta) {
    Rectangle render_rec;
    render_rec.x = 0;
    render_rec.y = 0;
    render_rec.width = _owner->_globalTexture.texture.width;
    render_rec.height = -_owner->_globalTexture.texture.height;

    auto taskbar_start = dynamic_cast<TextureObject *>(_objects["taskbar_start"]);
    auto taskbar_end = dynamic_cast<TextureObject *>(_objects["taskbar_end"]);

    // EndTextureMode();

    BeginScissorMode(0, GetRenderHeight() - taskbar_start->_height, GetRenderWidth(), taskbar_start->_height);
    Color col = BLACK;
    col.a = 128;
    DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(), col);
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
    BeginShaderMode(_owner->_shaders[0]);

    DrawTextureRec(_owner->_globalTexture.texture, render_rec, (Vector2){0, 0}, WHITE);

    EndShaderMode();
    EndBlendMode();
    EndScissorMode();

    // // EndTextureMode();

    // BeginTextureMode(_owner->_globalTexture);

    Application::beginRendering(delta);

    int posX = 60;
    int posY = taskbar_start->_y;

    auto _applications = _owner->_applications;
    std::sort(_applications.begin(), _applications.end(), [](RenderObject *app1, RenderObject *app2) {
        auto a1 = static_cast<Application *>(app1);
        auto a2 = static_cast<Application *>(app2);
        
        return (a1->_processID > a2->_processID);
    });
    
    int i = 0;
    int actualapps = 0;
    while(i < _applications.size()) {
        if (actualapps == 10) {
            i = _applications.size();
            continue;
        }

        auto app = static_cast<Application *>(_applications[i]);

        if (app != nullptr && !app->_taskbarHidden) {
            actualapps++;
            auto texture = _textureMap["ui/taskbar_app.png"];
            if (_owner->_selectedWindow == app) texture = _textureMap["ui/taskbar_app_selected.png"];

            float scaling = 0.7f;

            float posX2 = posX + ((float)texture.width - (float)(app->_applicationIconFull.width * scaling)) / 2.f;
            float posY2 = posY + ((float)texture.height - (float)(app->_applicationIconFull.height * scaling)) / 2.f;

            BeginBlendMode(BLEND_ADDITIVE);
            DrawTexture(texture, posX, posY, WHITE);
            EndBlendMode();
            DrawTextureEx(app->_applicationIconFull, (Vector2){posX2, posY2}, 0.f, 0.7f, WHITE);

            Rectangle miniapp_rect;
            miniapp_rect.x = posX;
            miniapp_rect.y = posY;
            miniapp_rect.width = texture.width;
            miniapp_rect.height = texture.height;

            // DrawRectangleLinesEx(miniapp_rect, 2, GREEN);

            auto mouse_rect = DWM::getMouse();

            if (CheckCollisionRecs(miniapp_rect, mouse_rect)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && _owner->_selectedWindow != nullptr) {
                    auto selWindow = static_cast<Application *>(_owner->_selectedWindow);
                    
                    int zlayer1 = selWindow->_zLayer;
                    int zlayer2 = app->_zLayer;

                    app->_zLayer = zlayer1;
                    selWindow->_zLayer = zlayer2;

                    _owner->_selectedWindow = app;
                }
            }
 
            posX += texture.width + 2;
        }

        i++;
    }

    auto font = _owner->accessFont("Segoe UI", 17);

    auto currentTime = getCurrentTime();
    auto currentDay = getCurrentDay();

    float mes1 = MeasureTextEx(font, currentTime.c_str(), 17, 0.3f).x;
    float mes2 = MeasureTextEx(font, currentDay.c_str(), 17, 0.3f).x;

    float test1 = (taskbar_end->_width - mes1) / 2.f;
    float test2 = (taskbar_end->_width - mes2) / 2.f;

    BeginBlendMode(BLEND_ADDITIVE);

    DrawTextEx(font, currentTime.c_str(), (Vector2){_windowSize.x - 25 - (test1 * 0.87f), (float)taskbar_start->_y + 4.f}, 17, 0.3F, WHITE);
    DrawTextEx(font, currentDay.c_str(), (Vector2){_windowSize.x - 25 - test2, (float)taskbar_start->_y + 21.f}, 17, 0.3F, WHITE);

    EndBlendMode();
}


#include <ctime>

std::string TaskbarApplication::getCurrentTime() {
    // get unix timestamp
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    // parse it
    char buffer[64] = {};
    std::strftime(buffer, 64, "%I:%M", &tm);

    std::string currentTime = buffer;

    if (tm.tm_hour < 12) {
        currentTime += " AM";
    } else {
        currentTime += " PM";
    }

    return currentTime;
};

std::string TaskbarApplication::getCurrentDay() {
    // get unix timestamp
    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    // parse it
    char buffer[64] = {};
    std::strftime(buffer, 64, "%m/%d/%Y", &tm);

    return buffer;
};