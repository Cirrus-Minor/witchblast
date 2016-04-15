#include "MusicPlayer.h"
#include "WitchBlastGame.h"

void MusicPlayer::playMusic(musicEnum musicChoice)
{
  music.stop();

  if (game->getParameters().musicVolume <= 0) return;

  music.setLoop(true);
  music.setVolume(game->getParameters().musicVolume * VolumeModifier / 100);
  bool ok = false;

  switch (musicChoice)
  {
  case MusicDungeon:
    {
      int r = currentStandardMusic;
      while (r == currentStandardMusic) r = rand() % 3;

      switch (r)
      {
        case 0: ok = music.openFromFile("media/sound/WitchBlastTheme.ogg"); break;
        case 1: ok = music.openFromFile("media/sound/SavageLife.ogg"); break;
        case 2: ok = music.openFromFile("media/sound/HauntedLighthouse.ogg"); break;
      }

      currentStandardMusic = r;
    }
    break;

  case MusicEnding:
    ok = music.openFromFile("media/sound/AmbiantMedieval.ogg");
    break;

  case MusicBoss:
    ok = music.openFromFile("media/sound/ShowMeThePower.ogg");
    break;

  case MusicChallenge:
    ok = music.openFromFile("media/sound/HellsFire.ogg");
    break;

  case MusicIntro:
    ok = music.openFromFile("media/sound/WitchBlastTheme.ogg");
    break;
  }

  if (ok)
    music.play();
}

void MusicPlayer::pauseMusic()
{
  music.pause();
}

void MusicPlayer::resumeMusic()
{
  music.play();
}

void MusicPlayer::updateMusicVolume()
{
  if (music.getStatus() == sf::Music::Playing)
  {
    if (game->getParameters().musicVolume == 0)
      music.stop();
    else
      music.setVolume(game->getParameters().musicVolume * VolumeModifier / 100);
  }
  else
  {
    if (game->getParameters().musicVolume > 0)
    {
      bool ok = music.openFromFile("media/sound/wb.ogg");
      music.setVolume(game->getParameters().musicVolume * VolumeModifier / 100);
      if (ok) music.play();
    }
  }
}

MusicPlayer::MusicPlayer(WitchBlastGame *game)
  : game(game)
{
}
