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

#include "ButtonObject.hpp"

#include "DWM.hpp"

ButtonObject::ButtonObject(Texture2D texture, Texture2D selectedTexture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg) : TextureObject(texture) {
    _selectedTexture = selectedTexture;
    _callback = callback;
    _unselectedTexture = texture;

    _customArg = customArg;

    _hitbox.width = texture.width;
    _hitbox.height = texture.height;

    _ObjectType = RenderObjectType::RO_BUTTON; 
}
ButtonObject::ButtonObject(Texture2D texture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg) : TextureObject(texture) {
    _selectedTexture = texture;
    _callback = callback;
    _unselectedTexture = texture;

    _hitbox.width = texture.width;
    _hitbox.height = texture.height;

    _customArg = customArg;

    _ObjectType = RenderObjectType::RO_BUTTON; 
}
ButtonObject::ButtonObject(Texture2D texture, Rectangle hitboxRect, Texture2D selectedTexture, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg) : TextureObject(texture) {
    _selectedTexture = selectedTexture;
    _callback = callback;
    _hitbox = hitboxRect;
    _unselectedTexture = texture;

    _customArg = customArg;

    _ObjectType = RenderObjectType::RO_BUTTON; 
}
ButtonObject::ButtonObject(Texture2D texture, Rectangle hitboxRect, std::function<void(ButtonObject *self, void *customArg)> callback, void *customArg) : TextureObject(texture) {
    _selectedTexture = texture;
    _callback = callback;
    _hitbox = hitboxRect;
    _unselectedTexture = texture;

    _customArg = customArg;

    _ObjectType = RenderObjectType::RO_BUTTON; 
}

ButtonObject::~ButtonObject() {
    // TextureObject::~TextureObject();
}

void ButtonObject::prerender(float delta) {
    TextureObject::prerender(delta);

    _hitbox.x = _x;
    _hitbox.y = _y;

    if (_linkedApplication == nullptr) return;
    if (_linkedApplication->_owner->_selectedWindow != _linkedApplication && _linkedApplication->getAppType() <= ApplicationType::WindowedBorderless) return;

    auto mouse_rect = _linkedApplication->getMousePosition();

    if (CheckCollisionRecs(_hitbox, mouse_rect)) {
        _selected = true;
        applyTexture(_selectedTexture);

        SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !_pressLock) {
            _pressed = true;
            _pressLock = true;
            _callback(this, _customArg);
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            _pressLock = false;
        }
    } else {
        _selected = false;
        _pressed = false;
        applyTexture(_unselectedTexture);

        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    }
}