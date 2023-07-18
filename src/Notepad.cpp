#include "Notepad.hpp"

#include <TextFieldObject.hpp>

NotepadWindow::NotepadWindow(DWM *owner) : Application(Windowed, owner) {
    TextFieldObject *field = new TextFieldObject(this, "notepad!");

    field->setPosition(2, 2);
    field->setColor(BLACK);
    field->setFont(owner->accessFont("Segoe UI", 20));
    field->setFontSize(20);
    field->setSpacing(0.5f);
    field->setBlending(BLEND_MULTIPLIED);

    setWindowTitle("Notepad");

    pushObject("field", field);

    _taskbarHidden = false;
    _hidden = false;
}

NotepadWindow::~NotepadWindow() {}

void NotepadWindow::prerender(float delta) {
    _camera.offset.y += (GetMouseWheelMoveV().y * 20.f);

    if (_camera.offset.y > 0.f) {
        _camera.offset.y = 0.f;
    }
}