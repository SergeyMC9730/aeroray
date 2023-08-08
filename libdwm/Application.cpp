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

#include "Application.hpp"

#include "ButtonObject.hpp"

#include "GlassObject.hpp"

Application::Application(ApplicationType type, DWM *owner, bool spawnUnselected) {
    _old_envSize.x = GetRenderWidth();
    _old_envSize.y = GetRenderHeight();

    _owner = owner;

    int x = 640;
    int y = 440;

    if (type == ApplicationType::Windowed || type == ApplicationType::WindowedBorderless) {
        _x = 120;
        _y = 120;

        if (!spawnUnselected) owner->_selectedWindow = this;
    }

    if (type == ApplicationType::Windowed) {
        loadTexture2("ui/exitbutton.png");
        loadTexture2("ui/exitbutton_selected.png");
    }

    if (type == ApplicationType::FullscreenScalable || type == ApplicationType::Fullscreen) {
        x = GetRenderWidth();
        y = GetRenderHeight();
    }

    _windowSize.x = x;
    _windowSize.y = y;

    _framebuffer = LoadRenderTexture(x, y);

    _type = type;

    if (type == ApplicationType::Fullscreen) {
        SetWindowMinSize(x, y);
    }

    auto applicationImage = GenImageColor(39, 39, BLACK);
    auto applicationImageSmall = ImageCopy(applicationImage);

    ImageResize(&applicationImageSmall, applicationImage.width / 3, applicationImage.height / 3);

    _applicationIcon = LoadTextureFromImage(applicationImageSmall);
    _applicationIconFull = LoadTextureFromImage(applicationImage);

    UnloadImage(applicationImage);
    UnloadImage(applicationImageSmall);

    _processID = rand();

    _hidden = false;

    int i = 0;

    while(i < 4) {
        Rectangle rec = {};

        _borderRectangles.push_back(rec);

        i++;
    }

    _camera.rotation = 0.f;
    _camera.zoom = 1.f;
    _camera.target.x = 0;
    _camera.target.y = 0;
    _camera.offset.x = 0;
    _camera.offset.y = 0;

    _ObjectType = RenderObjectType::RO_APPLICATION;

    _width = _windowSize.x;
    _height = _windowSize.y;
}
Application::~Application() {
    processApplicationExit();
}

void Application::processApplicationExit() {
    for (auto [key, val] : _textureMap) {
        UnloadTexture(val);
    }
    for (auto [key, val] : _objects) {
        if (val != nullptr) {
            delete val;
            _objects[key] = nullptr;
        }
    }

    if (_framebuffer.id != 0xFFFFFFFE) {
        UnloadRenderTexture(_framebuffer);
    }

    UnloadTexture(_applicationIcon);
    UnloadTexture(_applicationIconFull);

    if(_type == ApplicationType::Fullscreen) SetWindowMinSize(_old_envSize.x, _old_envSize.y);
}

void Application::loadTexture2(std::string path) {
    loadTexture(path, _owner);
}

bool Application::prepareRendering() {
    if (_hidden) return false;

    auto mouse_rect = _owner->getMouse();

    bool collision2 = CheckCollisionRecs(_fullWindowRegion, mouse_rect);

    if (_type <= ApplicationType::WindowedBorderless) {
        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) && collision2 && _owner->_selectedWindow != this) {

            bool canSwitch = false;

            auto old_window = reinterpret_cast<Application *>(_owner->_selectedWindow);

            if (old_window != nullptr) {
                auto collidedWinRect = GetCollisionRec(_fullWindowRegion, old_window->_fullWindowRegion);

                bool collision_check = CheckCollisionRecs(collidedWinRect, mouse_rect);

                bool situation1 = !collision_check;
                bool situation2 = !CheckCollisionRecs(old_window->_fullWindowRegion, _fullWindowRegion);
                bool situation3_1 = !CheckCollisionRecs(old_window->_fullWindowRegion, mouse_rect) && CheckCollisionRecs(_fullWindowRegion, mouse_rect);
                bool situation3_2 = CheckCollisionRecs(old_window->_fullWindowRegion, mouse_rect) && !CheckCollisionRecs(_fullWindowRegion, mouse_rect);
                bool situation3 = situation3_1 || situation3_2;

                if (situation1 || situation2 || situation3) {
                    canSwitch = true;

                    int zlayer1 = old_window->_zLayer;
                    int zlayer2 = _zLayer;

                    old_window->_zLayer = zlayer2;
                    _zLayer = zlayer1;

                    _owner->rebuildApplicationList();
                }
            } else {
                canSwitch = true;
            }

            if (canSwitch) {
                _owner->_selectedWindow = this;
            }
        }
    }

    if (_type == ApplicationType::FullscreenScalable) {
        _windowSize.x = GetRenderWidth();
        _windowSize.y = GetRenderHeight();

        if (_framebuffer.texture.width != _windowSize.x || _framebuffer.texture.height != _windowSize.y) {
            updateWindowFramebuffer();
        }
    } else if (_type == ApplicationType::Windowed) {
        int posX = _x - 6;
        int posY = _y - 28;
        
        int width = 6 + _windowSize.x + 6;
        int height = 28;

        auto mouse_rect = _owner->getMouse();
        auto mouse_delta = GetMouseDelta();

        bool collision1 = CheckCollisionRecs(_headerRectangle, mouse_rect);

        if (_owner->_selectedWindow == this) {
            if (collision1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                _x += mouse_delta.x;
                _y += (mouse_delta.y * 1.f);
                _headerHolding = true;
            } else if (!collision1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT) && _headerHolding) {
                _x += mouse_delta.x;
                _y += (mouse_delta.y * 1.f);
            } else {
                _headerHolding = false;
            }
        }
    }

    BeginTextureMode(_framebuffer);

    ClearBackground((Color){255, 255, 255, 0});

    // BeginMode2D(_camera);

    return true;
}
void Application::beginRendering(float delta) {
    for (auto iter = _objects.rbegin(); iter != _objects.rend(); ++iter) {
        if (!iter->second->isHidden()) {
            if (iter->second->getRendererType() == RO_RECTGLASS) {
                auto gl = static_cast<GlassObject *>(iter->second);

                gl->_app = this;
            }
            iter->second->prerender(delta);

            BeginMode2D(_camera);

            _cameraDisabled = false;

            if (!iter->second->outputsToCamera()) {
                _cameraDisabled = true;
                EndMode2D();
            }

            iter->second->render(delta);
        }
    }
}

void Application::endRendering() {
    if (!_cameraDisabled) EndMode2D();

    EndTextureMode();
    
    // Rectangle rect = {
    //     0, 0, 
    //     (float)_framebuffer.texture.width, (float)_framebuffer.texture.height 
    // };

    // DrawTextureRec(_framebuffer.texture, rect, (Vector2){(float)this->_x, (float)this->_y}, WHITE);

    // DrawRectangleLinesEx(_fullWindowRegion, 1, RED);
    // DrawRectangleLinesEx(_headerRectangle, 1, YELLOW);
    // DrawRectangleLinesEx(_exitButtonRect, 1, BLUE);

    // int i = 0;
    // while(i < _borderRectangles.size()) {
    //     DrawRectangleLinesEx(_borderRectangles[i], 1, LIME);

    //     i++;
    // }
}

void Application::render(float delta) {
    bool result = prepareRendering();
    if (!result) return;
    beginRendering(delta);
    endRendering();
}

Rectangle Application::getMousePosition() {
    if (_owner->_selectedWindow != this && _type <= ApplicationType::WindowedBorderless) {
        Rectangle rec;

        rec.x = 99999;
        rec.y = 99999;
        rec.width = 0;
        rec.height = 0;

        return rec;
    }

    auto mouse = DWM::getMouse();

    mouse.x -= _x + _camera.offset.x + _camera.target.x;
    mouse.y -= _y + _camera.offset.x + _camera.offset.y;

    mouse.x /= _camera.zoom;
    mouse.y /= _camera.zoom;

    return mouse;
}

void Application::pushObject(std::string objname, RenderObject *obj) {
    if (obj->getRendererType() == RenderObjectType::RO_BUTTON) {
        auto btn = static_cast<ButtonObject *>(obj);

        btn->_linkedApplication = this;
    }

    _objects.insert(std::pair<std::string, RenderObject *>(objname, obj));
}

void Application::setWindowTitle(std::string title) {
    _windowName = title;
}
std::string Application::getWindowTitle() {
    return _windowName;
}

void Application::updateWindowFramebuffer() {
    auto old_fb = _framebuffer;
    auto new_fb = LoadRenderTexture(_windowSize.x, _windowSize.y);
            
    BeginTextureMode(new_fb);
    DrawTexture(old_fb.texture, 0, 0, WHITE);
    EndTextureMode();

    UnloadRenderTexture(old_fb);

    _framebuffer = new_fb;   
}

void Application::resizeWindow(Vector2 size) {
    if (_type == ApplicationType::Windowed || _type == ApplicationType::WindowedBorderless) {
        _windowSize = size;

        updateWindowFramebuffer();  
    }
}

void Application::stop() {    
   _owner->stopApplication(_processID);
}

ApplicationType Application::getAppType() {
    return _type;
}

int Application::getKeyboardChar() {
    if (_owner->_selectedWindow != this) return 0;

    return GetCharPressed();
}

int Application::getKeyboardKey() {
    if (_owner->_selectedWindow != this) return 0;

    return GetKeyPressed();
}

bool Application::isKeyPressed(int key) {
    if (_owner->_selectedWindow != this) return false;

    return IsKeyPressed(key);
}
bool Application::isKeyReleased(int key) {
    if (_owner->_selectedWindow != this) return false;

    return IsKeyReleased(key);
}
bool Application::isKeyUp(int key) {
    if (_owner->_selectedWindow != this) return false;
    
    return IsKeyUp(key);
}
bool Application::isKeyDown(int key) {
    if (_owner->_selectedWindow != this) return false;

    return IsKeyDown(key);
}

Vector2 Application::getMouseWheelMoveV() {
    if (_owner->_selectedWindow != this) return {};

    return GetMouseWheelMoveV();
}

std::map<std::string, Color> Application::getDesignTheme() {
    if (_owner->_selectedWindow == this) {
        Color c1 = {0xA9, 0xC1, 0xDA, 0xFF};
        Color c2 = {0x98, 0xB4, 0xFF, 0x00};
        Color c3 = {0xBA, 0xD2, 0xEA, 0xFF};
        Color c4 = {0x28, 0xCF, 0xE4, 0x00};

        return {
            { "c1", c1 },
            { "c2", c2 },
            { "c3", c3 },
            { "c4", c4 },
        };
    }

    Color c1 = {0xD7, 0xE4, 0xF2, 0xFF};
    Color c2 = {0xBF, 0xCD, 0xDB, 0x00};
    Color c3 = {0xD7, 0xE4, 0xF2, 0xFF};
    Color c4 = {0xE9, 0xEC, 0xF3, 0x00};

    return {
        { "c1", c1 },
        { "c2", c2 },
        { "c3", c3 },
        { "c4", c4 },
    };
}