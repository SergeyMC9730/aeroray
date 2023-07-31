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

#include "DWM.hpp"

#include <bits/stdc++.h>

#include "Application.hpp"

DWM::DWM(std::string resourcesFolder) {
    _resourcesFolder = resourcesFolder;

    std::string p = resourcesFolder + "/shaders/blur.fs";

    // "resources/shaders/blur.vs"
    auto shaderblur = LoadShader(0, p.c_str());

    _shaders.push_back(shaderblur);

    _globalTexture = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());

    _ObjectType = RenderObjectType::RO_DWM;
}

Font DWM::accessFont(std::string fontName, int size) {
    std::string fullName = fontName + " " + std::to_string(size);

    if  (!_fonts.count(fullName)) {
        // std::cout << "font is not loaded: LOAD!" << std::endl;
        loadFont(fontName, size);
    } else {
        // std::cout << "font already cached" << std::endl;
    }

    return _fonts[fullName];
}

void DWM::loadFont(std::string fileName, int size) {
    std::string path = _resourcesFolder + "/fonts/" + fileName + ".ttf";
    std::string naming = fileName + " " + std::to_string(size);

    auto font = LoadFontEx(path.c_str(), size, NULL, 0);
    _fonts.insert(std::make_pair(naming, font));

    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
}

std::string DWM::getResourcesPath() {
    return _resourcesFolder;
}

DWM::~DWM() {
    int i = 0;
    while(i < _applications.size()) {
        if (_applications[i] != nullptr) {
            delete _applications[i];
            _applications[i] = nullptr;
        }
        
        i++;
    }

    for (auto [key, val] : _fonts) {
        UnloadFont(val);
    }

    UnloadRenderTexture(_globalTexture);

    i = 0;
    while (i < _shaders.size()) {
        UnloadShader(_shaders[i]);

        i++;
    }

    // RenderObject::~RenderObject();
}

void DWM::prerender(float delta) {
    int i = 0;
    while(i < _applications.size()) {
        if (_applications[i] != nullptr) {
            _applications[i]->prerender(delta);
        }
        
        i++;
    }
}

void DWM::render(float delta) {
    if (_globalTexture.texture.width != GetRenderWidth() || _globalTexture.texture.height != GetRenderHeight()) {
        auto new_fb = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());
        auto new_fb2 = LoadRenderTexture(GetRenderWidth(), GetRenderHeight());
        BeginTextureMode(new_fb);
        DrawTexture(_globalTexture.texture, 0, 0, WHITE);
        EndTextureMode();

        UnloadRenderTexture(_globalTexture);

        _globalTexture = new_fb;
    }

    Rectangle render_rec;
    render_rec.x = 0;
    render_rec.y = 0;
    render_rec.width = _globalTexture.texture.width;
    render_rec.height = -_globalTexture.texture.height;
    DrawTextureRec(this->_globalTexture.texture, render_rec, (Vector2){0, 0}, WHITE);

    int i = 0;
    while(i < _applications.size()) {
        if (_applications[i] != nullptr) {
            _applications[i]->render(delta);
        }
        
        i++;
    }
    BeginTextureMode(_globalTexture);

    ClearBackground(BLACK);

    i = 0;
    while(i < _applications.size()) {
        if (_applications[i] != nullptr && !_applications[i]->_hidden) {
            auto app = dynamic_cast<Application *>(_applications[i]);

            if (app->_hidden || app->_emptyFB) {
                i++;
                continue;
            }
            
            int posY_t = _applications[i]->getPosition().y;
            Rectangle render_rec;
            render_rec.x = 0;
            render_rec.y = 0;
            render_rec.width = app->_framebuffer.texture.width;
            render_rec.height = -app->_framebuffer.texture.height;

            EndTextureMode();

            DrawTextureRec(app->_framebuffer.texture, render_rec, (Vector2){(float)_applications[i]->getPosition().x, (float)_applications[i]->getPosition().y}, WHITE);

            BeginTextureMode(_globalTexture);

            DrawTextureRec(app->_framebuffer.texture, render_rec, (Vector2){(float)_applications[i]->getPosition().x, (float)_applications[i]->getPosition().y}, WHITE);

            render_rec.width = _globalTexture.texture.width;
            render_rec.height = -_globalTexture.texture.height;

            auto mouse_rect = DWM::getMouse();

            int posX = app->_x - 6;
            int posY = app->_y - 28;
            
            int width = 6 + app->_windowSize.x + 6;
            int height = 28;

            // auto mouse_rect = DWM::getMouse();

            if (app->getAppType() == ApplicationType::Windowed) {
                auto colors = app->getDesignTheme();

                Rectangle s1;

                // BeginTextureMode(__test_r1);
                // DrawTexture(_framebuffer.texture, 0, 0, WHITE);
                // // ClearBackground(RAYWHITE);
                // EndTextureMode();        

                // BeginTextureMode(__test_r1);
                // header gradient

                BeginBlendMode(BLEND_MULTIPLIED);

                EndTextureMode();
                BeginShaderMode(_shaders[0]);
                BeginScissorMode(posX, posY, width, height);
                DrawTextureRec(_globalTexture.texture, render_rec, (Vector2){0, 0}, WHITE);

                EndScissorMode();
                EndShaderMode();

                // DrawRectangleGradientV(posX, posY, width, height, colors["c1"], colors["c2"]);

                BeginTextureMode(_globalTexture);

                app->_headerRectangle.width = width;
                app->_headerRectangle.height = height;
                app->_headerRectangle.x = posX;
                app->_headerRectangle.y = posY;
                app->_fullWindowRegion.x = app->_headerRectangle.x;
                app->_fullWindowRegion.y = posY;
                app->_fullWindowRegion.width = app->_headerRectangle.width + 1;
                app->_fullWindowRegion.height = app->_headerRectangle.height + app->_windowSize.y + 8;

                // EndTextureMode();     

                // EndShaderMode();

                // // sidebar gradient 1

                EndTextureMode();
                
                posX = app->_x - 6;
                width = 6 + app->_windowSize.x + 6;
                posY = app->_y;
                height = app->_windowSize.y;
                DrawRectangleGradientV(posX, posY, 6, height, colors["c2"], colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = 8;
                s1.height = height;
                app->_borderRectangles[0] = s1;

                // sidebar gradient 2
                posX = app->_x + app->_windowSize.x;
                DrawRectangleGradientV(posX, posY, 6, height, colors["c2"], colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = 8;
                s1.height = height;
                app->_borderRectangles[1] = s1;

                // floor rectangle
                posX = app->_x - 6;
                posY = app->_y + app->_windowSize.y;
                height = 7;
                width = 6 + app->_windowSize.x + 6;
                DrawRectangle(posX, posY, width, height, colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = width;
                s1.height = height;
                app->_borderRectangles[2] = s1;

                // white line 1
                posY = app->_y - 29;
                DrawLine(posX, posY, posX, posY + 29 + app->_windowSize.y + 8, WHITE);

                // blue line 1
                posX = app->_x + app->_windowSize.x + 6;
                DrawRectangleGradientV(posX, posY, 1, 29 + app->_windowSize.y + 8, WHITE, colors["c4"]);

                // blue line 2
                posX = app->_x - 7;
                posY += 29 + app->_windowSize.y + 6;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 7, posY, colors["c4"]);

                // black line 1
                posX = app->_x - 8;
                posY++;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 9, posY, BLACK);

                // white line 2
                posX = app->_x - 8;
                posY = app->_y - 29;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 7, posY, WHITE);

                // black line 2
                posY--;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 8, posY, BLACK);

                // black line 3
                posX = app->_x - 7;
                posY = app->_y - 30;
                DrawLine(posX, posY, posX, posY + 30 + app->_windowSize.y + 8, BLACK);

                // black line 4
                posX = app->_x + app->_windowSize.x + 8;
                DrawLine(posX, posY, posX, posY + 30 + app->_windowSize.y + 8, BLACK);
            
                EndBlendMode();

                posX = app->_x + 4;
                posY = app->_y - 5 - app->_applicationIcon.height;
                // DrawRectangle(posX, posY, 13, 13, BLACK);
                DrawTexture(app->_applicationIcon, posX, posY, WHITE);

                // draw window title
                posX += app->_applicationIcon.width + 1 + 4;
                posY -= 2;
                auto font = accessFont("Segoe UI", 17);
                DrawTextEx(font, app->getWindowTitle().c_str(), (Vector2){(float)posX, (float)posY}, 17, 0.5f, WHITE);
            
                BeginTextureMode(_globalTexture);

                posX = app->_x - 6;
                width = 6 + app->_windowSize.x + 6;
                posY = app->_y;
                height = app->_windowSize.y;
                DrawRectangleGradientV(posX, posY, 6, height, colors["c2"], colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = 8;
                s1.height = height;
                app->_borderRectangles[0] = s1;

                // sidebar gradient 2
                posX = app->_x + app->_windowSize.x;
                DrawRectangleGradientV(posX, posY, 6, height, colors["c2"], colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = 8;
                s1.height = height;
                app->_borderRectangles[1] = s1;

                // floor rectangle
                posX = app->_x - 6;
                posY = app->_y + app->_windowSize.y;
                height = 7;
                width = 6 + app->_windowSize.x + 6;
                DrawRectangle(posX, posY, width, height, colors["c3"]);
                s1.x = posX;
                s1.y = posY;
                s1.width = width;
                s1.height = height;
                app->_borderRectangles[2] = s1;

                // white line 1
                posY = app->_y - 29;
                DrawLine(posX, posY, posX, posY + 29 + app->_windowSize.y + 8, WHITE);

                // blue line 1
                posX = app->_x + app->_windowSize.x + 6;
                DrawRectangleGradientV(posX, posY, 1, 29 + app->_windowSize.y + 8, WHITE, colors["c4"]);

                // blue line 2
                posX = app->_x - 7;
                posY += 29 + app->_windowSize.y + 6;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 7, posY, colors["c4"]);

                // black line 1
                posX = app->_x - 8;
                posY++;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 9, posY, BLACK);

                // white line 2
                posX = app->_x - 8;
                posY = app->_y - 29;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 7, posY, WHITE);

                // black line 2
                posY--;
                DrawLine(posX, posY, posX + 7 + app->_windowSize.x + 8, posY, BLACK);

                // black line 3
                posX = app->_x - 7;
                posY = app->_y - 30;
                DrawLine(posX, posY, posX, posY + 30 + app->_windowSize.y + 8, BLACK);

                // black line 4
                posX = app->_x + app->_windowSize.x + 8;
                DrawLine(posX, posY, posX, posY + 30 + app->_windowSize.y + 8, BLACK);
            
                EndBlendMode();

                posX = app->_x + 4;
                posY = app->_y - 5 - app->_applicationIcon.height;
                // DrawRectangle(posX, posY, 13, 13, BLACK);
                DrawTexture(app->_applicationIcon, posX, posY, WHITE);

                // draw window title
                posX += app->_applicationIcon.width + 1 + 4;
                posY -= 2;
                // auto font = _fonts["Segoe UI 17"];
                DrawTextEx(font, app->getWindowTitle().c_str(), (Vector2){(float)posX, (float)posY}, 17, 0.5f, WHITE);
            
                EndTextureMode();
            }

            posX = app->_x + app->_windowSize.x - app->_textureMap["ui/exitbutton.png"].width - 2;
            posY = app->_y - app->_textureMap["ui/exitbutton.png"].height - 3;

            app->_exitButtonRect.x = posX;
            app->_exitButtonRect.y = posY;
            app->_exitButtonRect.width = app->_textureMap["ui/exitbutton.png"].width;
            app->_exitButtonRect.height = app->_textureMap["ui/exitbutton.png"].height;

            auto exitButtonTexture = app->_textureMap["ui/exitbutton.png"];

            bool window_show_close = false;

            if (CheckCollisionRecs(app->_exitButtonRect, mouse_rect)) {
                exitButtonTexture = app->_textureMap["ui/exitbutton_selected.png"];
                
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    window_show_close = true;
                }
            }

            DrawTexture(exitButtonTexture, posX, posY, WHITE);

            if (window_show_close && _selectedWindow == app) {
                app->stop();
                return;
            }
        }
        
        i++;
    }
    EndTextureMode();

    for (auto [key, val] : _debuggers) {
        // DrawRectangle(val.vec.x, val.vec.y, 10, 10, val.col);
        DrawRectangleLinesEx(val.vec, 2, val.col);
    }
}

Rectangle DWM::getMouse() {
    Rectangle rect;
    Vector2 mouse = GetMousePosition();

    rect.width = 1;
    rect.height = 1;
    rect.x = mouse.x;
    rect.y = mouse.y;

    return rect;
}

void DWM::rebuildApplicationList() {
    std::vector<RenderObject *> app_list_new;

    int i = 0;
    while(i < _applications.size()) {
        if (_applications[i] != nullptr) app_list_new.push_back(_applications[i]);
        i++;
    }

    _applications = app_list_new;

    std::sort(_applications.begin(), _applications.end(), [](RenderObject *app1, RenderObject *app2) {
        auto a1 = static_cast<Application *>(app1);
        auto a2 = static_cast<Application *>(app2);
        
        return !(a1->_zLayer > a2->_zLayer);
    });
}

void DWM::pushApplication(RenderObject *application, bool pushUnselected) {
    auto app = static_cast<Application *>(application);

    app->_owner = this;

    if (!pushUnselected) currentZLayer++;

    int mul = 1;

    if (app->_type >= ApplicationType::Fullscreen) {
        mul = 1000;

        if (app->getWindowTitle().rfind("win.", 0) == 0) mul *= -1;
    }

    app->_zLayer = currentZLayer * mul;

    if (app->getWindowTitle() == "win.shell.taskbar") app->_zLayer = 9999999;

    std::cout << "app '" << app->getWindowTitle() << "' zlayer " << app->_zLayer << std::endl;

    _applications.push_back(application);

    rebuildApplicationList();
}

void DWM::stopApplication(int processID) {
    int i = 0;

    while(i < _applications.size()) {
        auto app = static_cast<Application *>(_applications[i]);
        if (app != nullptr) {
            if (app->_processID == processID) {
                if (_selectedWindow == app) {
                    _selectedWindow = nullptr;
                }
                delete app;
                _applications[i] = nullptr;
            }
        }
        i++;
    }

    rebuildApplicationList();
}