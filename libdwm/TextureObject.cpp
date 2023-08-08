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

#include "TextureObject.hpp"

TextureObject::TextureObject(Texture2D texture) {
    applyTexture(texture);

    setBlending(0xFF);

    _ObjectType = RenderObjectType::RO_TEXTURE;
}
TextureObject::TextureObject(Texture2D texture, Rectangle rect) {
    setPosition((Vector2){rect.x, rect.y});
    setSize((Vector2){rect.width, rect.height});
    
    applyTexture(texture);

    setBlending(0xFF);

    _ObjectType = RenderObjectType::RO_TEXTURE;
}

TextureObject::~TextureObject() {
    UnloadTexture(_texture);
    // RenderObject::~RenderObject();
}

void TextureObject::applyTexture(Texture2D texture, bool unloadOld) {
    if (unloadOld) {
        UnloadTexture(_texture);
    }

    _texture = texture;

    setSize(_texture.width, _texture.height);
}

void TextureObject::render(float delta) {
    Rectangle rect1 = this->toRect();
    Rectangle rect2 = this->toBasicRect();

    rect1.width *= _scaleX;
    rect1.height *= _scaleY;

    if (_blendType != 0xFF) BeginBlendMode(_blendType);

    int i = 0;
    while(i < _dencity) {
        DrawTexturePro(_texture, rect2, rect1, (Vector2){0, 0}, _rotation, _color);
        i++;
    }

    if (_blendType != 0xFF) EndBlendMode();
}

Texture2D TextureObject::getTexture() {
    return _texture;
}