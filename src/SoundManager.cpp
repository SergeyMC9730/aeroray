#include "SoundManager.hpp"

#include <filesystem>

SoundManager::SoundManager() {
    InitAudioDevice();
}
SoundManager::~SoundManager() {
    for (auto &snd : _sounds) {
        if (snd.snd) delete snd.snd;
        snd.snd = nullptr;
    }

    _sounds.clear();

    CloseAudioDevice();
}

PlayingSound *SoundManager::play(std::string path, bool destroy) {
    if (!std::filesystem::exists(path)) return nullptr;
    
    Music music = LoadMusicStream(path.c_str());

    auto snd = new PlayingSound(music);

    _sounds.push_back({snd, destroy, path});

    return snd; 
}

void SoundManager::update() {
    int i = 0;

    while (i < _sounds.size()) {
        bool destroy = _sounds[i].snd != nullptr && _sounds[i].snd->getPositionPercentage() == 100.f && !_sounds[i].snd->isLooping() && _sounds[i].destroy;

        printf("sound %s : %f %d\n", _sounds[i].path.c_str(), _sounds[i].snd->getPositionPercentage(), destroy);

        if (destroy) {
            delete _sounds[i].snd;
            _sounds[i].snd = nullptr;
        }

        i++;
    }
}