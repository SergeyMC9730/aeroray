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

#include <raylib.h>
#include <cstdint>

enum RenderObjectType {
    RO = 0,
    RO_APPLICATION, RO_BUTTON, RO_RECTANGLE, 
    RO_TEXT, RO_TEXTURE, RO_DWM,
    RO_TEXTFIELD
};

class RenderObject {
protected:
    RenderObjectType _ObjectType;
    uint8_t _blendType = 0xFF;
public:
    int _x = 0;
    int _y = 0;
    int _width = 0;
    int _height = 0;
    
    float _scaleX = 1.f;
    float _scaleY = 1.f;
    float _rotation = 0.f;

    bool _hidden = false;

    RenderObject();
    RenderObject(Rectangle rect);

    virtual ~RenderObject();

    Rectangle toRect();
    Rectangle toBasicRect();

    virtual void prerender(float delta);
    virtual void render(float delta);

    void setPosition(Vector2 vec);
    void setPosition(int x, int y);

    void setWidthHeight(Vector2 vec);
    void setWidthHeight(int w, int h);

    bool checkCollision(RenderObject *obj);
    bool checkCollision(Rectangle rect);

    void setBlending(uint8_t blending);

    friend class Application;
};