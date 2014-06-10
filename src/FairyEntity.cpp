#include "FairyEntity.h"
#include "BoltEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include <iostream>

FairyEntity::FairyEntity(float x, float y, PlayerEntity* parentEntity) : SpriteEntity (ImageManager::getImageManager()->getImage(IMAGE_FAIRY), x, y, 48, 72)
{
  this->x = x;
  this->y = y;

  this->setFrame(0);
  this->parentEntity = parentEntity;

  type = ENTITY_FAMILIAR;
  //viscosity = 0.99f;

  fireDelay = -1.0f;
  facingDirection = 2;
}


void FairyEntity::animate(float delay)
{
  z = y + height;

  if (fireDelay > 0) fireDelay -= delay;

  float dist2 = (x - parentEntity->getX()) * (x - parentEntity->getX()) + (y - parentEntity->getY()) * (y - parentEntity->getY());

  if (dist2 > 15000.0f)
  {
    float tan = (parentEntity->getX() - x) / (parentEntity->getY() - y);
    float angle = atan(tan);

    if (parentEntity->getY() > y)
      setVelocity(Vector2D(sin(angle) * FAIRY_SPEED, cos(angle) * FAIRY_SPEED));
    else
      setVelocity(Vector2D(-sin(angle) * FAIRY_SPEED, -cos(angle) * FAIRY_SPEED));

    viscosity = 1.0f;
  }
  else if (dist2 < 50000.0f)
  {
    viscosity = 0.96f;
  }

  computeFacingDirection();

  isMirroring = false;
  frame = ((int)(age * 10.0f)) % 2;
  if (facingDirection == 8) frame += 2;
  else if (facingDirection == 4) frame += 4;
  else if (facingDirection == 6)
  {
    frame += 4;
    isMirroring = true;
  }

  SpriteEntity::animate(delay);
}

void FairyEntity::fire(int dir, GameMap* map)
{
  if (fireDelay <= 0.0f)
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);
    fireDelay = FAIRY_FIRE_DELAY;

    float velx = 0.0f;
    float vely = 0.0f;

    if (dir == 4) velx = - FAIRY_BOLT_VELOCITY;
    if (dir == 6) velx = + FAIRY_BOLT_VELOCITY;
    if (dir == 2) vely = + FAIRY_BOLT_VELOCITY;
    if (dir == 8) vely = - FAIRY_BOLT_VELOCITY;

    BoltEntity* bolt = new BoltEntity(ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y, FAIRY_BOLT_LIFE, ShotTypeStandard);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    bolt->setDamages(FAIRY_BOLT_DAMAGES);
    bolt->setVelocity(Vector2D(velx, vely));
  }
}

void FairyEntity::computeFacingDirection()
{
  if (parentEntity->getFireDirection() != 5)
  {
    facingDirection = parentEntity->getFireDirection();
  }
  else if (abs((int)velocity.x) > 0 || abs((int)velocity.y) > 0)
  {
    if (abs((int)velocity.x) > abs((int)velocity.y))
    {
      if (velocity.x > 0.0f) facingDirection = 6;
      else facingDirection = 4;
    }
    else
    {
      if (velocity.y > 0.0f) facingDirection = 2;
      else facingDirection = 8;
    }
  }
}
