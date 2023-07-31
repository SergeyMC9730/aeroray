#include "Notepad.hpp"

#include <TextFieldObject.hpp>
#include <RectangleObject.hpp>

NotepadWindow::NotepadWindow(DWM *owner) : Application(Windowed, owner) {
    TextFieldObject *field = new TextFieldObject(this, "notepad!");

    field->setPosition(2, 2);
    field->setColor(BLACK);
    field->setFont(owner->accessFont("Segoe UI", 20));
    field->setFontSize(20);
    field->setSpacing(0.5f);
    field->setBlending(BLEND_ALPHA_PREMULTIPLY);

    setWindowTitle("Notepad");

    auto rect_bg = new RectangleObject();
    rect_bg->setSize(_width, _height);

    _taskbarHidden = false;
    _hidden = false;

    pushObject("bg", rect_bg);
    pushObject("field", field);
}

NotepadWindow::~NotepadWindow() {}

void NotepadWindow::prerender(float delta) {
    _camera.offset.y += (getMouseWheelMoveV().y * 20.f);

    if (_camera.offset.y > 0.f) {
        _camera.offset.y = 0.f;
    }
}