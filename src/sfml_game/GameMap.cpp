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

#include <cstdlib>
#include <fstream>

#include "GameMap.h"

GameMap::GameMap(int width, int height)
{
  hasChanged = true;

  this->width = width;
  this->height = height;

  map = new int* [width];
  int i = 0;
  for ( i = 0 ; i < width ; i++)
      map[i] = new int [height];

  for ( i = 0 ; i < width ; i++)
    for ( int j = 0 ; j < height ; j++)
      map[i][j] = 0;
}

GameMap::~GameMap()
{
    for ( int i = 0 ; i < width ; i++)
        delete[] map [i];
    delete[] map;
}

int GameMap::getWidth() { return width; }
int GameMap::getHeight() { return height; }

int GameMap::getTile(int x, int y)
{
    if (!inMap(x, y)) return -1;
    return map[x][y];
}

bool GameMap::getChanged()
{
  bool result = hasChanged;
  hasChanged = false;
  return result;
}

bool GameMap::inMap(int x, int y)
{
  if (x < 0) return false;
  if (y < 0) return false;
  if (x >= width) return false;
  if (y >= height) return false;
  return true;
}

bool GameMap::isDownBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (map[x][y] > 0);
}

bool GameMap::isUpBlocking(int x, int y)
{
  if (y < 0) return true;
  if (!inMap(x, y)) return false;
  return (map[x][y] > 0);
}

bool GameMap::isLeftBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (map[x][y] > 0);
}

bool GameMap::isRightBlocking(int x, int y)
{
  if (!inMap(x, y)) return false;
  return (map[x][y] > 0);
}

void GameMap::setTile(int x, int y, int n)
{
  if (!inMap(x, y)) return;
  map[x][y] = n;
  hasChanged = true;
}

void GameMap::randomize(int n)
{
  hasChanged = true;
    for ( int i = 0 ; i < width ; i++)
        for ( int j = 0 ; j < height ; j++)
            map[i][j] = (int) (((float) rand() / (float)RAND_MAX * ((float)n)));

}

void GameMap::loadFromFile(const char* fileName)
{
  hasChanged = true;
  std::ifstream f(fileName);
  if (!f.is_open()) return;

  int n;
  int i;

	// dimensions
	f >> n;
	if (n != width) return;
	f >> n;
	if (n != height) return;

  for (int j = 0; j < height; j++)
  {
    for (i = 0; i < width; i++)
    {
      f >> n;
      map[i][j] = n;
    }
  }

  f.close();
}
