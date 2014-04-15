/**  This file is part of sfmlGame.
  *
  *  FreeTumble is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  FreeTumble is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with FreeTumble.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef SOUNDMANAGER_H_INCLUDED
#define SOUNDMANAGER_H_INCLUDED

#include <SFML/Audio.hpp>

class SoundManager
{
public:
    static SoundManager* getSoundManager();
    void addSound(char* fileName);
    void playSound(int n);
    void stopSound(int n);

private:
    SoundManager();
    ~SoundManager();

    std::vector<sf::SoundBuffer*> soundBufferArray;
    std::vector<sf::Sound*> soundArray;
};

#endif // SOUNDMANAGER_H_INCLUDED
