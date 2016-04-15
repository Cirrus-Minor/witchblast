#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "Enums.h"

class WitchBlastGame;

class MusicPlayer
{
public:
  static MusicPlayer &getInstance(WitchBlastGame *game)
  {
    static MusicPlayer instance(game);
    return instance;
  }

  void playMusic(musicEnum musicChoice);
  void pauseMusic();
  void resumeMusic();
  void updateMusicVolume();

private:
  MusicPlayer(WitchBlastGame *game);

  WitchBlastGame *game;
  sf::Music music;

  int currentStandardMusic = 0;
  const int VolumeModifier = 55;
};

#endif // MUSICPLAYER_H
