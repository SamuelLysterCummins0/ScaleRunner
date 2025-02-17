#include "SoundManager.h"
#include <iostream>

SoundManager* SoundManager::m_Instance = nullptr;

SoundManager::SoundManager() {
    //Sound Buffers used for efficiency 
    m_SoundBuffers["jump"].loadFromFile("audio/jump.mp3");
    m_SoundBuffers["damage"].loadFromFile("audio/robotDamage.mp3");

    m_Sounds["jump"].setBuffer(m_SoundBuffers["jump"]);
    m_Sounds["damage"].setBuffer(m_SoundBuffers["damage"]);
    m_Sounds["damage"].setVolume(100.0f);

    m_BackgroundMusic.openFromFile("audio/spaceshipNormal.wav");
    m_SlowedBackgroundMusic.openFromFile("audio/spaceshipSlow.wav");
    m_GameOverMusic.openFromFile("audio/end.mp3");

    m_BackgroundMusic.setLoop(true);
    m_SlowedBackgroundMusic.setLoop(true);
}

SoundManager* SoundManager::getInstance() {
    if (m_Instance == nullptr) {
        m_Instance = new SoundManager();
    }
    return m_Instance;
}

void SoundManager::startBackgroundMusic() {
    m_BackgroundMusic.play();
}

void SoundManager::switchToSlowedMusic() {
    if (!m_IsSlowedMusic) {
        float position = m_BackgroundMusic.getPlayingOffset().asSeconds();
        m_BackgroundMusic.stop();
        m_SlowedBackgroundMusic.play();
        m_SlowedBackgroundMusic.setPlayingOffset(seconds(position));
        m_IsSlowedMusic = true;
    }
}

void SoundManager::switchToNormalMusic() {
    if (m_IsSlowedMusic) {
        float position = m_SlowedBackgroundMusic.getPlayingOffset().asSeconds();
        m_SlowedBackgroundMusic.stop();
        m_BackgroundMusic.play();
        m_BackgroundMusic.setPlayingOffset(seconds(position));
        m_IsSlowedMusic = false;
    }
}

void SoundManager::playGameOverMusic() {
    stopAllMusic();
    m_GameOverMusic.play();
}

void SoundManager::playJumpSound() {
    m_Sounds["jump"].play();
}

void SoundManager::playDamageSound() {
    m_Sounds["damage"].play();
}

void SoundManager::setMusicVolume(float volume) {
    m_BackgroundMusic.setVolume(volume);
    m_SlowedBackgroundMusic.setVolume(volume);
    m_GameOverMusic.setVolume(volume);
}

void SoundManager::stopAllMusic() {
    m_BackgroundMusic.stop();
    m_SlowedBackgroundMusic.stop();
    m_GameOverMusic.stop();
}

void SoundManager::cleanup() {
    stopAllMusic();
}