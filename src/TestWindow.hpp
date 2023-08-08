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

#include <Application.hpp>

#include <vector>
#include <queue>

#include "TypeContainer.hpp"

class TestWindow : public Application {
protected:
    void beginRendering(float delta) override;

    std::vector<TypeContainer<std::string> *> _containers;
    std::map<std::string, std::vector<Image>> _images;
    int _files;
public:
    void prerender(float delta) override;

    TestWindow(DWM *owner);
    ~TestWindow();
};