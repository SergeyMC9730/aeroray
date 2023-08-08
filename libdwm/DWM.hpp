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

#ifndef _DWM_HPP_
#define _DWM_HPP_

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "RendererDebug.h"
#include "RenderObject.hpp"

class DWM : public RenderObject {
protected:
    int currentZLayer = 1;
    bool _drawSecondFB = false;
    std::string _resourcesFolder = "";

    std::map<RenderObject *, RenderTexture2D> _frambufferMappings = {};
public:
    DWM(std::string resourcesFolder);
    ~DWM();

    RenderObject *_selectedWindow = nullptr;

    std::vector<RenderObject *> _applications;
    std::vector<Shader> _shaders;
    std::map<std::string, Font> _fonts;

    std::map<std::string, RenderDebug> _debuggers;

    RenderTexture2D _globalTexture;

    void prerender(float delta) override;
    void render(float delta) override;

    void loadFont(std::string fileName, int size);

    Font accessFont(std::string fontName, int size);

    void pushApplication(RenderObject *application, bool pushUnselected = false);
    void stopApplication(int processID);

    void rebuildApplicationList();

    std::string getResourcesPath();

    static Rectangle getMouse();
};

#endif