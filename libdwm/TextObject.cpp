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

#include "TextObject.hpp"

TextObject::TextObject(std::string textLabel) {
    setLabel(textLabel);
    setColor(WHITE);
    setFont(GetFontDefault());

    _ObjectType = RenderObjectType::RO_TEXT;
}

void TextObject::setLabel(std::string textLabel) {
    _textLabel = textLabel;
}

TextObject::~TextObject() {
    // RenderObject::~RenderObject();
}

void TextObject::render(float delta) {
    float fontSize = (float)_fontSize * _scaleX;
    float spacing = _spacing;

    if (_blendType != 0xFF) BeginBlendMode(_blendType);
    
    DrawTextPro(_font, _textLabel.c_str(), (Vector2){(float)_x, (float)_y}, (Vector2){0, 0}, _rotation, fontSize, spacing, _color);

    if (_blendType != 0xFF) EndBlendMode();
}

int TextObject::getTextSize() {
    float fontSize = (float)_fontSize * _scaleX;
    float spacing = _spacing;

    return MeasureTextEx(_font, _textLabel.c_str(), fontSize, spacing).x;
}
int TextObject::getTextSizeY() {
    float fontSize = (float)_fontSize * _scaleX;
    float spacing = _spacing;

    return MeasureTextEx(_font, _textLabel.c_str(), fontSize, spacing).y;
}

void TextObject::setFont(Font font) {
    _font = font;
}

void TextObject::setFontSize(int fontSize) {
    _fontSize = fontSize;
}

void TextObject::setSpacing(float spacing) {
    _spacing = spacing;
}