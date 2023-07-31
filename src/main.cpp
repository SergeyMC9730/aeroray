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

#include <raylib.h>

#include <DWM.hpp>

#include "TaskbarApplication.hpp"
#include "BackgroundApplication.hpp"
#include "TestWindow.hpp"

#include "Registry.hpp"

#define TEXTINFO_ID 0xA1198DD2

DWM *dwm = nullptr;

int main(int, char**){
    Registry::registry["currentWallpaperPath"] = "wallpapers/neon_win7.png";

    srand(time(0));

    bool display_fb = false;

    InitWindow(1024, 768, "WDM Demo");

    #if PLATFORM_DESKTOP == 1
    SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetWindowMinSize(640, 480);
    #endif

    SetTargetFPS(144);

    dwm = new DWM("resources");
    auto taskbar = new TaskbarApplication(dwm);
    auto bg = new BackgroundApplication(dwm, "wallpapers/Windows/img0.png");

    // dwm->_applications.push_back(bg);

    // auto win = new TestWindow(dwm);
    // win->_scaleX = 10.f;

    dwm->pushApplication(taskbar);
    // dwm->pushApplication(win);
    dwm->pushApplication(bg);

    while (!WindowShouldClose())
    {    
        float delta = GetFrameTime();

        if (IsKeyPressed(KEY_TAB)) {
            display_fb = !display_fb;
        }

        dwm->prerender(delta);

        BeginDrawing();
            ClearBackground(BLACK);

            dwm->render(delta);

            std::string t = std::to_string(delta);

            DrawText(t.c_str(), 4, 4, 20, LIME);
            DrawFPS(4, 25);

            // if (display_fb) {
            //     ClearBackground(BLACK);
            //     BeginShaderMode(dwm->_shaders[0]);
            //     Rectangle rec;
            //     rec.x = 0;
            //     rec.y = 0;
            //     rec.height = -dwm->_globalTexture.texture.height;
            //     rec.width = dwm->_globalTexture.texture.width;
            //     DrawTextureRec(dwm->_globalTexture2.texture, rec, (Vector2){0, 0}, WHITE);
            //     EndShaderMode();
            //     DrawText("DISPLAYING DWM FB", 4, 45, 20, LIME);
            // } else {
            //     DrawText("DISPLAYING RL FB", 4, 45, 20, LIME);
            // }
        EndDrawing();
    }

    delete dwm;
    dwm = nullptr;

    CloseWindow();
}
