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
    setPosition(rect.x, rect.y);
    setSize(rect.width, rect.height);

    _ObjectType = RenderObjectType::RO;
}

Rectangle RenderObject::toRect() {
    Rectangle rect;

    rect.x = getPositionX();
    rect.y = getPositionY();
    rect.width = getSizeX();
    rect.height = getSizeY();

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
    setPosition((Vector2){(float)x, (float)y});
}

RenderObjectType RenderObject::getRendererType() {
    return _ObjectType;
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

void RenderObject::setRotation(float rot) {
    _rotation = rot;
}
float RenderObject::getRotation() {
    return _rotation;
}

void RenderObject::setSize(Vector2 size) {
    setSizeX(size.x);
    setSizeY(size.y);
}
void RenderObject::setSize(float x, float y) {
    setSizeX(x);
    setSizeY(y);
}
Vector2 RenderObject::getSize() {
    Vector2 vec;

    vec.x = getSizeX();
    vec.y = getSizeY();

    return vec;
}

void RenderObject::setSizeX(float x) {
    _width = x;
}
void RenderObject::setSizeY(float y) {
    _height = y;
}
    
float RenderObject::getSizeX() {
    return _width;
}
float RenderObject::getSizeY() {
    return _height;
}

Vector2 RenderObject::getPosition() {
    Vector2 vec;

    vec.x = getPositionX();
    vec.y = getPositionY();

    return vec;
}

void RenderObject::setScale(Vector2 scale) {
    setScaleX(scale.x);
    setScaleY(scale.y);
}
void RenderObject::setScale(float x, float y) {
    setScaleX(x);
    setScaleY(y);
}
void RenderObject::setScale(float scale) {
    setScale(scale, scale);
}
Vector2 RenderObject::getScale() {
    Vector2 vec;

    vec.x = getScaleX();
    vec.y = getScaleY();

    return vec;
}

void RenderObject::setPositionX(float x) {
    _x = x;
}
void RenderObject::setPositionY(float y) {
    _y = y;
}

float RenderObject::getPositionX() {
    return _x;
}
float RenderObject::getPositionY() {
    return _y;
}

void RenderObject::setScaleX(float x) {
    _scaleX = x;
}
float RenderObject::getScaleX() {
    return _scaleX;
}

void RenderObject::setScaleY(float y) {
    _scaleY = y;
}
float RenderObject::getScaleY() {
    return _scaleY;
}

void RenderObject::setHidden(bool hidden) {
    _hidden = hidden;
}
bool RenderObject::isHidden() {
    return _hidden;
};

void RenderObject::toggleCameraOutput(bool output) {
    _cameraOutput = output;
};
bool RenderObject::outputsToCamera() {
    return _cameraOutput;
};

RenderObject::~RenderObject() {}