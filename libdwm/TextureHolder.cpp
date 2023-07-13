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

#include "TextureHolder.hpp"

void TextureHolder::loadTexture(std::string texture, DWM *context) {
    std::string fullpath = context->getResourcesPath() + "/" + texture;

    auto tex2d = LoadTexture(fullpath.c_str());

    loadTexture(texture, tex2d);
}

void TextureHolder::loadTexture(std::string textureName, Texture2D texture) {
    _textureMap.insert(std::pair<std::string, Texture2D>(textureName, texture));
}

TextureHolder::TextureHolder() {}
TextureHolder::~TextureHolder() {}