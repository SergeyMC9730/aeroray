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

#include "RectangleObject.hpp"

RectangleObject::RectangleObject() {
    _ObjectType = RenderObjectType::RO_RECTANGLE;
}
RectangleObject::~RectangleObject() {
    // RenderObject::~RenderObject();
}

void RectangleObject::render(float delta) {
    auto rec = toRect();

    rec.width *= _scaleX;
    rec.height *= _scaleY;

    if (_blendType != 0xFF) BeginBlendMode(_blendType);

    DrawRectanglePro(rec, (Vector2){0, 0}, getRotation(), getColor());

    if (_blendType != 0xFF) EndBlendMode();

    return;
}