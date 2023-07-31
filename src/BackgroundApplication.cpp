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

#include "BackgroundApplication.hpp"

#include <TextureObject.hpp>
#include <TextObject.hpp>

#include "Registry.hpp"

BackgroundApplication::BackgroundApplication(DWM *owner, std::string wallpaperPath) : Application(ApplicationType::FullscreenScalable, owner) {
    this->loadTexture2(wallpaperPath);

    auto bg = new TextureObject(_textureMap[wallpaperPath]);
    auto t = new TextObject("DWM Impl 1.0");

    t->setSpacing(0.5F);

    SetTextureFilter(bg->getTexture(), TEXTURE_FILTER_BILINEAR);

    // t->setS

    this->pushObject("wallpaper", bg);
    this->pushObject("text", t);

    setWindowTitle("win.shell.wallpaper");

    _taskbarHidden = true;
    _wallpaper = wallpaperPath;

    Registry::registry["currentWallpaperPath"] = wallpaperPath;
}

BackgroundApplication::~BackgroundApplication() {
    processApplicationExit();
}

void BackgroundApplication::prerender(float delta) {
    auto texture = _textureMap[_wallpaper];
    auto bg = dynamic_cast<TextureObject *>(_objects["wallpaper"]);

    if (Registry::registry.count("currentWallpaperPath")) {
        auto current_wallpaper = std::get<std::string>(Registry::registry["currentWallpaperPath"]);
    
        if (current_wallpaper != _wallpaper) {
            if (!_textureMap.count(current_wallpaper)) {
                loadTexture2(current_wallpaper);
            }

            _textureMap.erase(_wallpaper);

            _wallpaper = current_wallpaper;

            texture = _textureMap[_wallpaper];

            SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

            bg->applyTexture(texture, true);
        }
    }

    auto t = dynamic_cast<TextObject *>(_objects["text"]);

    t->setFont(_owner->accessFont("Segoe UI Semibold", 30));
    t->setPosition(_windowSize.x - t->getTextSize() - 10, _windowSize.y - 70);

    float ratio = 1.f;

    if (texture.height > _windowSize.y) {
        ratio = (float)_windowSize.y / (float)texture.height;
        bg->setScale(ratio);
    }
    if (((float)texture.width * ratio) < (float)_windowSize.x) {
        ratio = (float)_windowSize.x / (float)texture.width;
        bg->setScale(ratio);
    }

    int posX = (_windowSize.x - ((float)texture.width * ratio)) / 2;
    int posY = (_windowSize.y - ((float)texture.height * ratio)) / 2;

    bg->setPosition(posX, posY);

    _fullWindowRegion.width = _windowSize.x + texture.width;
    _fullWindowRegion.height = _windowSize.y + texture.height;
    _fullWindowRegion.x = posX;
    _fullWindowRegion.y = posY;
}