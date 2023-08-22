#include "PlayingSound.hpp"

#include <iostream>

PlayingSound::PlayingSound(Music instance) {
    _value = instance;

    setLoop(false);
    setPitch(_pitch);
    setPan(_pan);
    setVolume(_volume);

    while (!isReady());

    if (!isPlaying()) {
        std::cout << "Playing music stream\n";

        play();
    }
}
PlayingSound::~PlayingSound() {
    UnloadMusicStream(_value);
}

void PlayingSound::pause() {
    if (!_paused) PauseMusicStream(_value);
    _paused = true;
}
void PlayingSound::resume() {
    if (_paused) ResumeMusicStream(_value);
    _paused = false;
}
void PlayingSound::pauseAuto() {
    if (_paused) resume(); else pause(); 
}

bool PlayingSound::isReady() {
    return IsMusicReady(_value);
}
bool PlayingSound::isPlaying() {
    return IsMusicStreamPlaying(_value);
}

void PlayingSound::stop() {
    StopMusicStream(_value);
}
void PlayingSound::play() {
    PlayMusicStream(_value);
}

void PlayingSound::restart() {
    stop();
    play();
}

void PlayingSound::setPitch(float pitch) {
    _pitch = pitch;

    SetMusicPitch(_value, getPitch());
}
float PlayingSound::getPitch() {
    return _pitch;
}

void PlayingSound::setPan(float pan) {
    _pan = pan;

    SetMusicPan(_value, getPan());
}
float PlayingSound::getPan() {
    return _pan;
}

void PlayingSound::setVolume(float volume) {
    _volume = volume;

    SetMusicVolume(_value, getVolume());
}
float PlayingSound::getVolume() {
    return _volume;
}

void PlayingSound::setLoop(bool loop) {
    _loop = loop;

    _value.looping = loop;
}
bool PlayingSound::isLooping() {
    return _loop;
}

float PlayingSound::getLength() {
    return GetMusicTimeLength(_value);
}

void PlayingSound::setPosition(float seconds) {
    SeekMusicStream(_value, seconds);
}
float PlayingSound::getPosition() {
    return GetMusicTimePlayed(_value);
}
float PlayingSound::getPositionPercentage() {
    return (getPosition() / getLength()) * 100.f;
}