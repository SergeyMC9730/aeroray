#include "VideoLoader.hpp"

VideoLoader::VideoLoader(std::string filename) {
    _stream = OpenVideoStream(filename.c_str());

    _videoTexture.height = -1;
}

VideoStream *VideoLoader::getVideoStream() {
    return _stream;
}

VideoLoader::~VideoLoader() {
    UnloadVideoStream(getVideoStream());
}

bool VideoLoader::isVideoGotLoaded() {
    return getVideoStream() != NULL;
}

Texture2D VideoLoader::getVideoTexture() {
    if (_videoTexture.height == -1 && isVideoGotLoaded()) {
        _videoTexture = LoadTextureFromVideoStream(getVideoStream());
    }

    return _videoTexture;
}