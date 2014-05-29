#include "BlackRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

BlackRatEntity::BlackRatEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y),
  currentTile(0, 0),
  targetTile(0, 0)
{
  imagesProLine = 6;
  creatureSpeed = BLACK_RAT_SPEED;
  hp = BLACK_RAT_HP;
  meleeDamages = BLACK_RAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
  shadowFrame = 6;
  frame = 12;

  currentDirection = 0;

  findNextGoal();
}

void BlackRatEntity::animate(float delay)
{
  // goal reached ?
  if (currentDirection == 6 && x > (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2 + OFFSET_X) ) findNextGoal();
  else if (currentDirection == 4 && x < (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2 + OFFSET_X) ) findNextGoal();
  else if (currentDirection == 2 && y > (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 + OFFSET_Y - 5) ) findNextGoal();
  else if (currentDirection == 8 && y < (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 + OFFSET_Y - 5) ) findNextGoal();

  if (age > 0.0f)
  {
    frame = 12 + ((int)(age * 5.0f)) % 2;
    if (facingDirection == 4 || facingDirection == 6) frame += 2;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 4;
  }

  EnnemyEntity::animate(delay);
}

void BlackRatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
    boundingBox.width = width - RAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + RAT_BB_TOP;
    boundingBox.height =  height - RAT_BB_HEIGHT_DIFF;
}

void BlackRatEntity::collideMapRight()
{
  findNextGoal();
}

void BlackRatEntity::collideMapLeft()
{
  findNextGoal();
}

void BlackRatEntity::collideMapTop()
{
  findNextGoal();
}

void BlackRatEntity::collideMapBottom()
{
  findNextGoal();
}

void BlackRatEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    if (currentDirection == 6 && entity->getX() > x)
    {
      currentDirection = 4;
      targetTile = IntCoord(currentTile.x - 1, currentTile.y);
    }
    else if (currentDirection == 4 && entity->getX() < x)
    {
      currentDirection = 6;
      targetTile = IntCoord(currentTile.x + 1, currentTile.y);
    }
    else if (currentDirection == 8 && entity->getY() < y)
    {
      currentDirection = 2;
      targetTile = IntCoord(currentTile.x, currentTile.y + 1);
    }
    else if (currentDirection == 2 && entity->getY() > y)
    {
      currentDirection = 8;
      targetTile = IntCoord(currentTile.x, currentTile.y - 1);
    }
    switch (currentDirection)
    {
      case 4: velocity.x = - BLACK_RAT_SPEED; velocity.y = 0.0f; break;
      case 6: velocity.x = + BLACK_RAT_SPEED; velocity.y = 0.0f; break;
      case 2: velocity.y = + BLACK_RAT_SPEED; velocity.x = 0.0f; break;
      case 8: velocity.y = - BLACK_RAT_SPEED; velocity.x = 0.0f; break;
      default: break;
    }
    facingDirection = currentDirection;
  }
}

void BlackRatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_BLACK_RAT);
  deadRat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);
  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}

void BlackRatEntity::findNextGoal()
{
  currentTile = getCurrentTile();

  int backDirection = 0;
  switch (currentDirection)
  {
    case 4: backDirection = 6; break;
    case 6: backDirection = 4; break;
    case 2: backDirection = 8; break;
    case 8: backDirection = 2; break;
    default: break;
  }

  {
    bool ok = false;
    int r = 0;
    while (!ok)
    {
      r++;
      if (r == 150) // watchdog
        ok = true;

      int newDir = rand() % 4;
      if (newDir == 0)
      {
        if (backDirection != 4 && currentTile.x > 2 && (currentTile.y % 2 != 0))
        {
          currentDirection = 4;
          targetTile = IntCoord(currentTile.x - 2, currentTile.y);
          ok = true;
        }
      }
      else if (newDir == 1)
      {
        if (backDirection != 6 && currentTile.x < MAP_WIDTH - 2 && (currentTile.y % 2 != 0))
        {
          currentDirection = 6;
          targetTile = IntCoord(currentTile.x + 2, currentTile.y);
          ok = true;
        }
      }
      else if (newDir == 2)
      {
        if (backDirection != 8 && currentTile.y > 1 && (currentTile.x % 2 != 0))
        {
          currentDirection = 8;
          targetTile = IntCoord(currentTile.x, currentTile.y - 2);
          ok = true;
        }
      }
      else
      {
        if (backDirection != 2 && currentTile.y < MAP_HEIGHT - 2 && (currentTile.x % 2 != 0))
        {
          currentDirection = 2;
          targetTile = IntCoord(currentTile.x, currentTile.y + 2);
          ok = true;
        }
      }
    }
  }

  switch (currentDirection)
  {
    case 4: velocity.x = - BLACK_RAT_SPEED; velocity.y = 0.0f; break;
    case 6: velocity.x = + BLACK_RAT_SPEED; velocity.y = 0.0f; break;
    case 2: velocity.y = + BLACK_RAT_SPEED; velocity.x = 0.0f; break;
    case 8: velocity.y = - BLACK_RAT_SPEED; velocity.x = 0.0f; break;
    default: break;
  }

  facingDirection = currentDirection;
}
