#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

using namespace sf;

class SoundManager {
private:
    // Music 
    Music m_BackgroundMusic;
    Music m_SlowedBackgroundMusic;
    Music m_GameOverMusic;

    // Sound buffers and sounds
    std::unordered_map<std::string, SoundBuffer> m_SoundBuffers;
    std::unordered_map<std::string, Sound> m_Sounds;

    bool m_IsSlowedMusic = false;
    static SoundManager* m_Instance;
    SoundManager();

public:
    static SoundManager* getInstance();

    void startBackgroundMusic();
    void switchToSlowedMusic();
    void switchToNormalMusic();
    void playGameOverMusic();
    void stopAllMusic();

    void playJumpSound();
    void playDamageSound();

    void setMusicVolume(float volume);
    void setSoundVolume(float volume);

    void cleanup();
};

#endif