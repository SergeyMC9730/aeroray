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

#include "TextObject.hpp"

#include "Application.hpp"

class TextFieldObject : public TextObject {
private:
    int _pointer = 0;
    int _fps = 60;
    int _currentFrame = 0;

    bool _showCursor = false;
    bool _allow_writing = false;

    Rectangle _clickField;

    Application *_context;
public:
    TextFieldObject(Application *context, std::string text);
    ~TextFieldObject();

    void render(float delta) override;

    void prerender(float delta) override;
};