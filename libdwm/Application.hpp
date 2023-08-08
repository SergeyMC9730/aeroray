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

#pragma once

#include <map>
#include <string>
#include <vector>

#include "RenderObject.hpp"
#include "TextureHolder.hpp"
#include "DWM.hpp"

enum ApplicationType {
    Windowed = 0,
    WindowedBorderless, Fullscreen, FullscreenScalable, FS_Switchable
};

class Application : public RenderObject, public TextureHolder {
protected:  
    std::map<std::string, RenderObject *> _objects = {};

    RenderTexture2D _framebuffer;

    ApplicationType _type;

    Vector2 _windowSize;

    std::string _windowName;

    Vector2 _old_envSize;

    Rectangle _headerRectangle;
    Rectangle _fullWindowRegion;
    Rectangle _windowRegion;
    Rectangle _exitButtonRect;

    std::vector<Rectangle> _borderRectangles;

    bool _headerHolding;

    int _zLayer;

    int _processID;

    bool _taskbarHidden;
    bool _hidden;

    bool _cameraDisabled = false;

    DWM *_owner;

    bool _emptyFB = false;

    Camera2D _camera;

    void loadTexture2(std::string path);

    virtual bool prepareRendering();
    virtual void beginRendering(float delta);
    virtual void endRendering();

    virtual void pushObject(std::string objname, RenderObject *obj);

    void updateWindowFramebuffer();

    std::map<std::string, Color> getDesignTheme();

    void processApplicationExit();
public:
    Texture2D _applicationIcon;
    Texture2D _applicationIconFull;
    
    Application(ApplicationType type, DWM *owner, bool spawnUnselected = false);
    ~Application();

    void render(float delta) override;

    void setWindowTitle(std::string title);
    std::string getWindowTitle();

    void resizeWindow(Vector2 size);

    virtual void stop();

    Rectangle getMousePosition();

    int getKeyboardChar();
    int getKeyboardKey();

    bool isKeyPressed(int key);
    bool isKeyReleased(int key);
    bool isKeyUp(int key);
    bool isKeyDown(int key);

    Vector2 getMouseWheelMoveV();

    ApplicationType getAppType();

    friend class DWM;
    friend class ButtonObject;
    friend class GlassObject;
    friend class TaskbarApplication;
};