#pragma once

#include <string>

#include <raylib.h>
#include "rvideo_fix.h"

class VideoLoader {
private:
    VideoStream *_stream;
    Texture2D _videoTexture;
public:
    VideoLoader(std::string filename);
    ~VideoLoader();

    bool isVideoGotLoaded();

    Texture2D getVideoTexture();
    VideoStream *getVideoStream();
};