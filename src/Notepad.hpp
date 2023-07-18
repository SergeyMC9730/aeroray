#pragma once

#include <Application.hpp>

class NotepadWindow : public Application {
public:
    void prerender(float delta) override;

    NotepadWindow(DWM *owner);
    ~NotepadWindow();
};