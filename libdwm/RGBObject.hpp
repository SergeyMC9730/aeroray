#pragma once

#include <raylib.h>

class RGBObject {
protected:
    Color _color = WHITE;
public:
    Color getColor();
    void setColor(Color col);
};