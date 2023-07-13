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

#include "TextureObject.hpp"
#include "Application.hpp"

#include <functional>

class ButtonObject : public TextureObject {
protected:
    void *_customArg;
    std::function<void(ButtonObject *self, void *customArg)> _callback;
    Texture2D _unselectedTexture;
    Texture2D _selectedTexture;

    Application *_linkedApplication = nullptr;

    bool _pressLock = false;
public:
    Rectangle _hitbox;

    bool _selected = false;
    bool _pressed = false;

    ButtonObject(Texture2D texture, Texture2D selectedTexture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg);
    ButtonObject(Texture2D texture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg);
    ButtonObject(Texture2D texture, Rectangle hitboxRect, Texture2D selectedTexture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg);
    ButtonObject(Texture2D texture, Rectangle hitboxRect, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg);

    ~ButtonObject() override;

    void prerender(float delta) override;

    friend class Application;
};