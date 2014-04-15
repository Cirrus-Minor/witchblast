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

#include "SoundManager.h"
#include <iostream>


SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
    std::cout << "Releasing audio memory...\n";
    for (unsigned int i = 0; i < soundBufferArray.size(); i++)
    {
        soundArray[i]->stop();
        delete(soundArray[i]);
        delete(soundBufferArray[i]);
    }
    soundArray.clear();
    soundBufferArray.clear();
}

 SoundManager* SoundManager::getSoundManager()
 {
   static SoundManager singleton;
   return &singleton;
 }

void SoundManager::addSound(char* fileName)
{
    //std::cout << "Loading sound: " << fileName << "...\n";

    sf::SoundBuffer* newSoundBuffer = new sf::SoundBuffer;
    newSoundBuffer->loadFromFile(fileName);
    soundBufferArray.push_back(newSoundBuffer);

    sf::Sound* newSound = new sf::Sound;
    newSound->setBuffer(*newSoundBuffer);
    soundArray.push_back(newSound);
}

void SoundManager::playSound(int n)
{
    if (soundArray[n]->getStatus() != sf::Sound::Playing)
        soundArray[n]->play();
}

void SoundManager::stopSound(int n)
{
    if (soundArray[n]->getStatus() == sf::Sound::Playing)
        soundArray[n]->stop();
}
