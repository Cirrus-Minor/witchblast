/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include <SFML/Audio.hpp>

class SoundManager
{
public:
    static SoundManager& getInstance();
    void addSound(const char *fileName);
    void playSound(int n, bool force = true);
    void playPitchModSound(int n, bool force = true);
    void stopSound(int n);
    void setMute(bool mute);
    void setVolume(int volume);

private:
    SoundManager();
    ~SoundManager();

    void checkSoundStatus();

    std::vector<sf::SoundBuffer*> soundBufferArray;
    std::vector<sf::Sound*> soundArray;

    bool mute;
    int volume;
};

#endif // SOUNDMANAGER_H_INCLUDED
