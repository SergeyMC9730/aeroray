#pragma once

#include <string>
#include <vector>

#include "PlayingSound.hpp"

class SoundManager {
public:
    struct SMSound {
        PlayingSound *snd;
        bool destroy;
        std::string path;
    };
private:
    std::vector<SMSound> _sounds = {};
public:
    SoundManager();
    ~SoundManager();

    // returns id of the playing sound
    PlayingSound *play(std::string path, bool destroy = true);

    void update();
};