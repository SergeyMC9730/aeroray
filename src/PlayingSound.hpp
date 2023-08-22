#pragma once

#include <DWMConfig.h>
#include <raylib.h>

#include "TypeContainer.hpp"

class PlayingSound : protected TypeContainer<Music> {
protected:
    float _pitch = 1.0f;
    float _pan = 1.0f;
    float _volume = 1.0f;

    bool _paused = false;
    bool _loop = false;
public:
    PlayingSound(Music instance);
    ~PlayingSound();

    void pause();
    void resume();
    void pauseAuto();

    bool isReady();
    bool isPlaying();

    void stop();
    void play();

    void restart();

    void setPitch(float pitch);
    float getPitch();

    void setPan(float pan);
    float getPan();

    void setVolume(float pitch);
    float getVolume();

    void setLoop(bool loop);
    bool isLooping();

    float getLength();

    void setPosition(float seconds);
    // Get position in seconds
    float getPosition();
    float getPositionPercentage();
};