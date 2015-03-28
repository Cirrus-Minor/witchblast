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

#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdio.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

//#include "Entity/EntityManager.h"

class Game
{
public:
  Game();
  virtual ~Game();

  virtual void startGame();
  virtual void quitGame();

  static float getAbsolutTime();
  void create(int screenWidth, int screenHeight, std::string windowsTitle = "Generic sfmlGame", bool fullScreen = false, bool vsync = true);

protected:
  virtual void onRender();     // screen and game items rendering
  virtual void onUpdate();

  int screenWidth;
  int screenHeight;

  float lastTime;

  sf::RenderWindow* app;
};

#endif // GAME_H_INCLUDED
