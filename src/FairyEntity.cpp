#include "FairyEntity.h"
#include "BoltEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/MyTools.h"
#include <iostream>

FairyEntity::FairyEntity(float x, float y, enumFamiliar fairyType) : SpriteEntity (ImageManager::getImageManager()->getImage(IMAGE_FAIRY), x, y, 48, 72)
{
  this->x = x;
  this->y = y;

  this->setFrame(0);
  imagesProLine = 6;
  this->parentEntity = game().getPlayer();

  type = ENTITY_FAMILIAR;
  this->fairyType = fairyType;

  fireDelay = -1.0f;
  facingDirection = 2;

  fairyDamages = FAIRY_BOLT_DAMAGES;

  shotLevel = 1;
  switch (fairyType)
  {
  case FamiliarFairyTarget:
  case FamiliarFairy:
    shotType = ShotTypeStandard;
    fairyFireDelay = FAIRY_FIRE_DELAY;
    break;
  case FamiliarFairyIce:
    shotType = ShotTypeIce;
    fairyFireDelay = ICE_FAIRY_FIRE_DELAY;
    break;
  case FamiliarFairyFire:
    shotType = ShotTypeFire;
    fairyFireDelay = FAIRY_FIRE_DELAY;
    fairyDamages = FAIRY_FIRE_DAMAGES;
    break;

  case FamiliarNone:
    break;
  }
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

  checkCollisions();

  if (fairyType == FamiliarFairyTarget && !game().getCurrentMap()->isCleared()) tryToFire();
  else computeFacingDirection();

  isMirroring = false;
  frame = ((int)(age * 10.0f)) % 2;
  if (facingDirection == 8) frame += 2;
  else if (facingDirection == 4) frame += 4;
  else if (facingDirection == 6)
  {
    frame += 4;
    isMirroring = true;
  }
  frame += (int)(fairyType) * 6;
  SpriteEntity::animate(delay);
}

void FairyEntity::fire(int dir)
{
  if (x < OFFSET_X + TILE_WIDTH * 1.3) return;
  if (y < OFFSET_Y + TILE_HEIGHT * 1.3) return;
  if (x > OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1) - TILE_WIDTH * 0.3) return;
  if (y > OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1) - TILE_HEIGHT * 0.3) return;

  if (fireDelay <= 0.0f)
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);
    fireDelay = fairyFireDelay;

    float velx = 0.0f;
    float vely = 0.0f;

    if (dir == 4) velx = - FAIRY_BOLT_VELOCITY;
    if (dir == 6) velx = + FAIRY_BOLT_VELOCITY;
    if (dir == 2) vely = + FAIRY_BOLT_VELOCITY;
    if (dir == 8) vely = - FAIRY_BOLT_VELOCITY;

    BoltEntity* bolt = new BoltEntity(x, y, FAIRY_BOLT_LIFE, shotType, shotLevel);
    bolt->setDamages(fairyDamages);
    bolt->setVelocity(Vector2D(velx, vely));
    bolt->setFlying(true);

    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnnemy(x, y);
      if (target.x > -1.0f)
      {
        bolt->setVelocity(Vector2D(x, y).vectorTo(target, FAIRY_BOLT_VELOCITY));

        if ((target.x - x) * (target.x - x) > (target.y - y) *(target.y - y))
        {
          if (target.x < x) facingDirection = 4;
          else facingDirection = 6;
        }
        else
        {
          if (target.y < y) facingDirection = 8;
          else facingDirection = 2;
        }
      }
    }
  }
  else
  {
    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnnemy(x, y);
      if (target.x > -1.0f)
      {
        if ((target.x - x) * (target.x - x) > (target.y - y) *(target.y - y))
        {
          if (target.x < x) facingDirection = 4;
          else facingDirection = 6;
        }
        else
        {
          if (target.y < y) facingDirection = 8;
          else facingDirection = 2;
        }
      }
      else computeFacingDirection();
    }
  }
}

void FairyEntity::tryToFire()
{
  if (x < OFFSET_X + TILE_WIDTH * 1.3) return;
  if (y < OFFSET_Y + TILE_HEIGHT * 1.3) return;
  if (x > OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1) - TILE_WIDTH * 0.3) return;
  if (y > OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1) - TILE_HEIGHT * 0.3) return;

  if (fireDelay <= 0.0f)
  {
    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnnemy(x, y);
      if (target.x > -1.0f)
      {
        BoltEntity* bolt = new BoltEntity(x, y, FAIRY_BOLT_LIFE, shotType, shotLevel);
        bolt->setDamages(fairyDamages);
        bolt->setFlying(true);
        bolt->setVelocity(Vector2D(x, y).vectorTo(target, FAIRY_BOLT_VELOCITY));

        SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);
        fireDelay = fairyFireDelay;

        if ((target.x - x) * (target.x - x) > (target.y - y) *(target.y - y))
        {
          if (target.x < x) facingDirection = 4;
          else facingDirection = 6;
        }
        else
        {
          if (target.y < y) facingDirection = 8;
          else facingDirection = 2;
        }
      }
      else
      {
        computeFacingDirection();
      }
    }
  }
  else
  {
    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnnemy(x, y);
      if (target.x > -1.0f)
      {
        if ((target.x - x) * (target.x - x) > (target.y - y) *(target.y - y))
        {
          if (target.x < x) facingDirection = 4;
          else facingDirection = 6;
        }
        else
        {
          if (target.y < y) facingDirection = 8;
          else facingDirection = 2;
        }
      }
      else computeFacingDirection();
    }
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

void FairyEntity::checkCollisions()
{
  int n = parentEntity->getFairieNumber();
  if (n > 1)
  {
    for (int i = 0; i < n; i++)
    {
      FairyEntity* fairy = parentEntity->getFairy(i);
      if (this != fairy)
      {
        bool isColliding = true;
        int dist = 40;

        if (x > fairy->getX() + dist || x < fairy->getX() - dist
            || y > fairy->getY() + dist || y < fairy->getY() - dist)
          isColliding = false;

        if (isColliding)
        {
          Vector2D vel = (Vector2D(fairy->getX(), fairy->getY()).vectorTo(Vector2D(x, y), FAIRY_SPEED * 0.5f));
          velocity.x += vel.x;
          velocity.y += vel.y;

          fairy->setVelocity(Vector2D(fairy->getVelocity().x - vel.x, fairy->getVelocity().y - vel.y));
        }
      }
    }
  }
}
