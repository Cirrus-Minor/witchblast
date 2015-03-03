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

#ifndef COLLIDINGSPRITEENTITY_H_INCLUDED
#define COLLIDINGSPRITEENTITY_H_INCLUDED

#include "SpriteEntity.h"
#include "GameMap.h"

// Basis class for Jouster
class CollidingSpriteEntity : public SpriteEntity
{
public:
  CollidingSpriteEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int width = -1, int height = -1);
  ~CollidingSpriteEntity();

  GameMap* getMap();
  virtual void setMap(GameMap* map, int tileWidth, int tileHeight, int offsetX, int offsetY);
  void setTileDimensions(int width, int height);

  virtual void render(sf::RenderTarget* app);
  virtual void animate(float delay);

  virtual void calculateBB();
  virtual void makeFall();
  virtual bool collideWithMap(int direction);
  virtual bool isCollidingWithMap();
  virtual void stuck();
  virtual bool collideWithEntity(CollidingSpriteEntity* entity);

  void displayBoundingBox(sf::RenderTarget* app);
  sf::IntRect getBoundingBox();

  enum directionEnum
  {
    DIRECTION_RIGHT,
    DIRECTION_LEFT,
    DIRECTION_TOP,
    DIRECTION_BOTTOM
  };

  bool isOnGround();

protected:
  GameMap* map;   // map to test collisions with
  sf::IntRect boundingBox; // BoundingBox
  int tileWidth;
  int tileHeight;
  int offsetX;
  int offsetY;

  float normalWeight;
  float maxY;

  virtual void exitMap(int direction);

  virtual void collideMapRight();
  virtual void collideMapLeft();
  virtual void collideMapTop();
  virtual void collideMapBottom();

  virtual void collideEntity(CollidingSpriteEntity* entity);

  virtual void testSpriteCollisions();
  virtual void readCollidingEntity(CollidingSpriteEntity* entity);
};

#endif // COLLIDINGSPRITEENTITY_H_INCLUDED
