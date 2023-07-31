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

#include "RenderObject.hpp"
#include "RGBObject.hpp"

#include <string>

class TextObject : public RenderObject, public RGBObject {
protected:
    std::string _textLabel;
    int _fontSize = 20;
    float _spacing = 2.f;
public:
    Font _font;

    TextObject(std::string textLabel = "");
    ~TextObject() override;

    void render(float delta) override;

    void setLabel(std::string textLabel);

    int getTextSize();
    int getTextSizeY();

    void setFont(Font font);

    void setFontSize(int fontSize);

    void setSpacing(float spacing);
};