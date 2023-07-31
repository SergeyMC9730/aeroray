#include "NativeApplicationX.h"

NativeApplicationX::NativeApplicationX(DWM *owner) : Application(Windowed, owner, false) {
    _taskbarHidden = false;
    _hidden = false;
}

NativeApplicationX::~NativeApplicationX() {}

void NativeApplicationX::beginRendering(float delta) {
    ClearBackground(WHITE);

    Application::beginRendering(delta);
}

void NativeApplicationX::prerender(float delta) {
    Application::prerender(delta);
}