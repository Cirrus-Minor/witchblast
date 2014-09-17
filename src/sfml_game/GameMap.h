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
#ifndef GAMEMAP_H_INCLUDED
#define GAMEMAP_H_INCLUDED

#include <string>

class GameMap
{
public:
    GameMap(int width, int height);
    virtual ~GameMap();
    int getWidth();
    int getHeight();
    int getTile(int x, int y);
    bool getChanged();

    virtual bool isDownBlocking(int x, int y);
    virtual bool isUpBlocking(int x, int y);
    virtual bool isLeftBlocking(int x, int y);
    virtual bool isRightBlocking(int x, int y);

    bool inMap(int x, int y);
    void setTile(int x, int y, int n);

    virtual void randomize(int n);
    virtual void loadFromFile(const char* fileName);

protected:
    int width;
    int height;
    int** map;
    bool hasChanged;
};

#endif // GAMEMAP_H_INCLUDED
