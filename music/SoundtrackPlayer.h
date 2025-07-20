#ifndef SOUNDTRACKPLAYER_H
#define SOUNDTRACKPLAYER_H

#include "raylib.h"

class SoundtrackPlayer {
private:
  Music music;
  bool isPlaying = false;
  bool isLoaded = false;

  SoundtrackPlayer() = default;

public:
  SoundtrackPlayer(const SoundtrackPlayer&) = delete;
  SoundtrackPlayer& operator=(const SoundtrackPlayer&) = delete;

  static SoundtrackPlayer* getInstance() {
    static SoundtrackPlayer instance;
    return &instance;
  }
  // Load the soundtrack from a file
  bool load(const char* filename) {
    if (!IsAudioDeviceReady()) InitAudioDevice();
    music = LoadMusicStream(filename);

    // Check if music loaded successfully
    // In Raylib, a failed music load typically results in stream.buffer being 0
    if (music.stream.buffer == 0) {
      isLoaded = false;
      return false;
    }

    isLoaded = true;
    isPlaying = false;
    return true;
  }

  // Start playing the soundtrack (loops by default)
  void play() {
    if (isLoaded && !isPlaying) {
      PlayMusicStream(music);
      isPlaying = true;
    }
  }

  // Call this in your game loop to update music stream
  void update() {
    if (isLoaded && isPlaying) {
      UpdateMusicStream(music);
    }
  }

  // Stop and unload
  void stop() {
    if (isLoaded) {
      if (isPlaying) {
        StopMusicStream(music);
        isPlaying = false;
      }
      UnloadMusicStream(music);
      isLoaded = false;
    }
    if (IsAudioDeviceReady()) CloseAudioDevice();
  }

  // Check if music is currently loaded
  bool loaded() const {
    return isLoaded;
  }

  // Check if music is currently playing
  bool playing() const {
    return isPlaying;
  }

  void setVolume(float volume) {
    if (isLoaded) {
      if (volume < 0.0f) volume = 0.0f;
      if (volume > 1.0f) volume = 1.0f;

      SetMusicVolume(music, volume);
    }
  }

  ~SoundtrackPlayer() {
    stop();
  }
};

#endif //SOUNDTRACKPLAYER_H