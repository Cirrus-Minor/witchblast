#include "FairyEntity.h"
#include "BoltEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/MyTools.h"
#include <iostream>

float const fireDelayAdvancedMult = 0.8f;

FairyEntity::FairyEntity(float x, float y, enumFamiliar fairyType, bool isPlayerControlled) : SpriteEntity (ImageManager::getInstance().getImage(IMAGE_FAIRY), x, y, 48, 72),
  isPlayer(isPlayerControlled)
{
  this->x = x;
  this->y = y;

  this->setFrame(0);
  imagesProLine = 6;
  this->parentEntity = game().getPlayer();

  type = ENTITY_FAMILIAR;
  this->fairyType = fairyType;

  fireDelay = -1.0f;
  isFiring = false;
  facingDirection = 2;
  teleportDelay = 5.0f + 0.1f * (rand() % 35);

  fairyDamages = FAIRY_BOLT_DAMAGES;

  shotLevel = 1;
  switch (fairyType)
  {
  case FamiliarFairyTarget:
    shotType = ShotTypeStandard;
    fairyFireDelay = TARGET_FAIRY_FIRE_DELAY;
    break;
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
  case FamiliarFairyPoison:
    shotType = ShotTypePoison;
    fairyFireDelay = ICE_FAIRY_FIRE_DELAY;
    break;
  case FamiliarFairyStone:
    shotType = ShotTypeStone;
    fairyFireDelay = FAIRY_FIRE_DELAY;
    break;

  case FamiliarNone:
    break;
  }
}


void FairyEntity::animate(float delay)
{
  z = y + height;

  if (parentEntity->isEquiped(EQUIP_FAIRY_POWDER)) delay *= 1.75f;

  if (fireDelay > 0) fireDelay -= delay;

  float dist2 = (x - parentEntity->getX()) * (x - parentEntity->getX()) + (y - parentEntity->getY()) * (y - parentEntity->getY());

  float creatureSpeed = FAIRY_SPEED;

  viscosity = 1.0f;
  if (!isPlayer)
  {
    if (dist2 > 15000.0f)
    {
      float tan = (parentEntity->getX() - x) / (parentEntity->getY() - y);
      float angle = atan(tan);

      if (parentEntity->getY() > y)
        setVelocity(Vector2D(sin(angle) * creatureSpeed, cos(angle) * creatureSpeed));
      else
        setVelocity(Vector2D(-sin(angle) * creatureSpeed, -cos(angle) * creatureSpeed));
    }
    else if (dist2 < 50000.0f)
    {
      viscosity = 0.96f;
    }
  }

  if (velocity.x < 50.0f && x < TILE_WIDTH * 1.3)
  {
    velocity.x = creatureSpeed;
  }
  else if (velocity.x > -50.0f && x > TILE_WIDTH * (MAP_WIDTH - 1) - TILE_WIDTH * 0.3)
  {
    velocity.x = -creatureSpeed;
  }
  if (velocity.y < 50.0f && y < TILE_HEIGHT * 1.3)
  {
    velocity.y = creatureSpeed;
  }
  else if (velocity.y > -50.0f && y > TILE_HEIGHT * (MAP_HEIGHT - 1) - TILE_HEIGHT * 0.3)
  {
    velocity.y = -creatureSpeed;
  }

  if (parentEntity->isEquiped(EQUIP_FAIRY_POWDER))
  {
    if (!isPlayer)
    {
      teleportDelay -= delay;

      if (teleportDelay <= 0.0f)
      {
        teleportDelay = 7.0f + 0.2f * (rand() % 35);
        SoundManager::getInstance().playSound(SOUND_TELEPORT);

        for(int i=0; i < 6; i++)
        {
          game().generateStar(sf::Color(50, 50, 255, 255), x, y);
          game().generateStar(sf::Color(200, 200, 255, 255), x, y);
        }

        x = TILE_WIDTH + rand() % (TILE_WIDTH * (MAP_WIDTH - 2));
        y = TILE_HEIGHT + rand() % (TILE_HEIGHT * (MAP_HEIGHT - 2));

        for(int i=0; i < 6; i++)
        {
          game().generateStar(sf::Color(50, 50, 255, 255), x, y);
          game().generateStar(sf::Color(200, 200, 255, 255), x, y);
        }

        if (rand() % 60 == 0) parentEntity->castTeleport();
      }
    }
  }

  checkCollisions();

  if (fairyType == FamiliarFairyTarget && !game().getCurrentMap()->isCleared()) tryToFire();
  else computeFacingDirection();

  isMirroring = false;

  if (velocity.x * velocity.x + velocity.y * velocity.y > 400)
    frame = ((int)(age * 24.0f)) % 2;
  else
    frame = ((int)(age * 18.0f)) % 2;

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

void FairyEntity::move(int direction)
{
  float creatureSpeed = FAIRY_SPEED;
  float speedx = 0.0f, speedy = 0.0f;
  if (direction == 4)
    speedx = - creatureSpeed;
  else if (direction == 1 || direction == 7)
    speedx = - creatureSpeed * 0.7f;
  else if (direction == 6)
    speedx = creatureSpeed;
  else if (direction == 3 || direction == 9)
    speedx = creatureSpeed * 0.7f;

  if (direction == 2)
    speedy = creatureSpeed;
  else if (direction == 1 || direction == 3)
    speedy = creatureSpeed * 0.7f;
  else if (direction == 8)
    speedy = - creatureSpeed;
  else if (direction == 7 || direction == 9)
    speedy = - creatureSpeed * 0.7f;

  setVelocity(Vector2D{speedx, speedy});
}

void FairyEntity::fire(int dir, bool bySelf)
{
  if (isPlayer)
  {
    facingDirection = dir;
    isFiring = true;
  }

  // If the fairy is player-controlled, don't fire with the main player
  if (isPlayer && !bySelf) return;
  if (x < TILE_WIDTH * 1.3) return;
  if (y < TILE_HEIGHT * 1.3) return;
  if (x > TILE_WIDTH * (MAP_WIDTH - 1) - TILE_WIDTH * 0.3) return;
  if (y > TILE_HEIGHT * (MAP_HEIGHT - 1) - TILE_HEIGHT * 0.3) return;

  if (fireDelay <= 0.0f)
  {
    facingDirection = dir;

    switch (fairyType)
    {
    case FamiliarFairyIce:
      SoundManager::getInstance().playSound(SOUND_BLAST_ICE);
      break;
    case FamiliarFairyFire:
      SoundManager::getInstance().playSound(SOUND_BLAST_FIRE);
      break;
    default:
      SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);
      break;
    }

    fireDelay = parentEntity->isEquiped(EQUIP_BELT_ADVANCED) ? fairyFireDelay * fireDelayAdvancedMult : fairyFireDelay;

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
    bolt->setFromPlayer(false);

    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnemy(x, y);
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
      Vector2D target = game().getNearestEnemy(x, y);
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
  if (x < TILE_WIDTH * 1.3) return;
  if (y < TILE_HEIGHT * 1.3) return;
  if (x > TILE_WIDTH * (MAP_WIDTH - 1) - TILE_WIDTH * 0.3) return;
  if (y > TILE_HEIGHT * (MAP_HEIGHT - 1) - TILE_HEIGHT * 0.3) return;

  if (fireDelay <= 0.0f)
  {
    if (fairyType == FamiliarFairyTarget)
    {
      Vector2D target = game().getNearestEnemy(x, y);
      if (target.x > -1.0f)
      {
        BoltEntity* bolt = new BoltEntity(x, y, FAIRY_BOLT_LIFE, shotType, shotLevel);
        bolt->setDamages(fairyDamages);
        bolt->setFlying(true);
        bolt->setVelocity(Vector2D(x, y).vectorTo(target, FAIRY_BOLT_VELOCITY));

        SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);
        fireDelay = game().getPlayer()->isEquiped(EQUIP_BELT_ADVANCED) ? fairyFireDelay * fireDelayAdvancedMult : fairyFireDelay;

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
      Vector2D target = game().getNearestEnemy(x, y);
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
  if (isFiring)
  {
    isFiring = false;
    return;
  }
  if (!isPlayer && parentEntity->getFireDirection() != 5)
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
