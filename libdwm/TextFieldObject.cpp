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

#include "TextFieldObject.hpp"

TextFieldObject::TextFieldObject(Application *context, std::string text) : TextObject(text) {
    _fps = GetFPS();

    _pointer = text.size();

    _context = context;
}
TextFieldObject::~TextFieldObject() {}

void TextFieldObject::render(float delta) {
    TextObject::render(delta);

    if (_blendType != 0xFF) BeginBlendMode(_blendType);

    if (_showCursor) {
        int textSize = getTextSize();

        float fsize = ((float)_fontSize * 0.3f);
        float fsize2 = ((float)_fontSize * 0.77f);

        int posX = _x + textSize + fsize;
        int posY = _y + 1;

        DrawLine(posX, _y + 1, posX, (float)_y + fsize2 + 1, _color);
    }

    DrawRectangleLinesEx(_clickField, 1, (_allow_writing) ? LIME : BLUE);

    if (_blendType != 0xFF) EndBlendMode();
}

void TextFieldObject::prerender(float delta) {
    float fsize = ((float)_fontSize * 0.3f);

    _clickField.x = _x;
    _clickField.y = _y;
    _clickField.width = getTextSize() + 2 + fsize;
    _clickField.height = getTextSizeY() + 1;

    auto mouse_rect = _context->getMousePosition();

    if (CheckCollisionRecs(_clickField, mouse_rect)) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            _allow_writing = true;
        }
    } else {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            _allow_writing = false;
        }
    }

    _fps = GetFPS();

    _currentFrame++;

    if ((_currentFrame % _fps) == (_fps / 2)) {
        _currentFrame = 0;
        _fps = GetFPS();
        _showCursor = !_showCursor;
    }

    if (_allow_writing) {
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        int key = _context->getKeyboardChar();

        while(key > 0) {
            if ((key >= 32) && (key <= 125)) {
                _textLabel.push_back(key);
            }

            key = _context->getKeyboardChar();
        }

        if (_context->isKeyPressed(KEY_BACKSPACE)) {
            _textLabel.pop_back();
        }
    }

    TextObject::prerender(delta);
}