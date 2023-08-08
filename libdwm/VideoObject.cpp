#include "rvideo_fix.h"

#include "VideoObject.hpp"

VideoObject::VideoObject(std::string filename) : VideoLoader(filename), TextureObject(getVideoTexture()) {}

void VideoObject::prerender(float delta) {
    UpdateTextureFromVideoStream(&_texture, getVideoStream());

    TextureObject::prerender(delta);
}