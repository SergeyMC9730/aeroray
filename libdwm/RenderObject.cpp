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

#include "RenderObject.hpp"

RenderObject::RenderObject() {}
RenderObject::RenderObject::RenderObject(Rectangle rect) {
    _x = rect.x;
    _y = rect.y;
    _width = rect.width;
    _height = rect.height;

    _ObjectType = RenderObjectType::RO;
}

Rectangle RenderObject::toRect() {
    Rectangle rect;

    rect.x = _x;
    rect.y = _y;
    rect.width = _width;
    rect.height = _height;

    return rect;
}
Rectangle RenderObject::toBasicRect() {
    auto rect = toRect();

    rect.x = 0;
    rect.y = 0;

    return rect;
}

void RenderObject::setPosition(Vector2 vec) {
    _x = vec.x;
    _y = vec.y;
}
void RenderObject::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

void RenderObject::setWidthHeight(Vector2 vec) {
    _width = vec.x;
    _height = vec.y;
}
void RenderObject::setWidthHeight(int w, int h) {
    _width = w;
    _height = h;
}

void RenderObject::render(float delta) {}
void RenderObject::prerender(float delta) {}

bool RenderObject::checkCollision(RenderObject *obj) {
    return this->checkCollision(obj->toRect());
}
bool RenderObject::checkCollision(Rectangle rect) {
    return CheckCollisionRecs(this->toRect(), rect);
}

void RenderObject::setBlending(uint8_t blending) {
    this->_blendType = blending;
}

RenderObject::~RenderObject() {}