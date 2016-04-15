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

#include "Game.h"

#include "GameEntity.h"
#include "EntityManager.h"

#include <iostream>

void Game::create(int screenWidth, int screenHeight, std::string windowsTitle, bool fullScreen, bool vsync)
{
  this->screenWidth = screenWidth;
  this->screenHeight = screenHeight;

  if (fullScreen)
    app = new sf::RenderWindow(sf::VideoMode(this->screenWidth, this->screenHeight), windowsTitle, sf::Style::Fullscreen);
  else
    app = new sf::RenderWindow(sf::VideoMode(this->screenWidth, this->screenHeight), windowsTitle);
  if (vsync) app->setVerticalSyncEnabled(true);
  else app->setFramerateLimit(60);
}

float Game::getAbsolutTime()
{
  static sf::Clock clock;
  return clock.getElapsedTime().asSeconds();
}

void Game::quitGame()
{
}

