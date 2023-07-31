#include "GlassObject.hpp"

#include <iostream>

GlassObject::GlassObject(DWM *owner) {
    _manager = owner;

    _ObjectType = RO_RECTGLASS;
}
GlassObject::~GlassObject() {}

void GlassObject::render(float delta) {
    BeginScissorMode(_x, _y, _width, _height);

    Color col = _color;
    col.a = 128;

    Rectangle render_rec;
    render_rec.x = _app->_x;
    render_rec.y = _app->_y;
    render_rec.width = _width;
    render_rec.height = -_height;

    printf("%f %f %f %f %d %d\n", render_rec.x, render_rec.y, render_rec.width, render_rec.height, _app->_x, _app->_y);

    DrawRectangle(0, 0, _width, _height, col);
    
    BeginBlendMode(BLEND_ALPHA_PREMULTIPLY);
    BeginShaderMode(_manager->_shaders[0]);

    DrawTextureRec(_manager->_globalTexture.texture, render_rec, (Vector2){(float)_x, (float)_y}, WHITE);

    EndShaderMode();
    // EndBlendMode();
    EndScissorMode();

    // DrawRectangle(0, 0, _width, _height, col);
    EndBlendMode();

    // _debug1.x = render_rec.x; // щас исправлю
    // _debug1.y = render_rec.y;

    

    _manager->_debuggers["db_glass" + std::to_string((uint64_t)this)].vec = render_rec;
    _manager->_debuggers["db_glass" + std::to_string((uint64_t)this)].col = RED;

    // EndTextureMode();
    // DrawRectangle(render_rec.x, render_rec.y, 10, 10, RED);
    // BeginTextureMode(_app->_framebuffer);
}