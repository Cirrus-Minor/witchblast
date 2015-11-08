#include "BlackRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BlackRatEntity::BlackRatEntity(float x, float y, ratBlackTypeEnum ratType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_RAT), x, y),
  currentTile(0, 0),
  targetTile(0, 0)
{
  this->ratType = ratType;
  imagesProLine = 10;

  if (ratType == RatBlackTypeNormal)
  {
    frame = 21;
    dyingFrame = 29;
    deathFrame = FRAME_CORPSE_BLACK_RAT;
    enemyType = EnemyTypeRatBlack;
    hp = BLACK_RAT_HP;
    creatureSpeed = BLACK_RAT_SPEED;
  }
  else //(ratType == RatBlackTypeHelmet)
  {
    frame = 41;
    dyingFrame = 49;
    deathFrame = FRAME_CORPSE_BLACK_RAT_HELMET;
    enemyType = EnemyTypeRatBlackHelmet;
    hp = BLACK_RAT_HP_HELMET;
    creatureSpeed = BLACK_RAT_SPEED_HELMET;
  }
  meleeDamages = BLACK_RAT_DAMAGES;

  bloodColor = BloodRed;
  shadowFrame = -1;
  agonizingSound = SOUND_RAT_DYING;

  currentDirection = 0;
  sprite.setOrigin(32.0f, 38.0f);

  findNextGoal();
}

void BlackRatEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
      // goal reached ?
    if (currentDirection == 6 && x > (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2) ) findNextGoal();
    else if (currentDirection == 4 && x < (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2) ) findNextGoal();
    else if (currentDirection == 2 && y > (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 - 5) ) findNextGoal();
    else if (currentDirection == 8 && y < (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 - 5) ) findNextGoal();

    checkNextFacing(delay);

    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 1;
    if (facingDirection == 4 || facingDirection == 6) frame += 3;
    isMirroring = (facingDirection == 6 );
    if (facingDirection == 8) frame += 6;
    if (ratType == RatBlackTypeHelmet) frame += 40;
    else frame += 20;
  }

  EnemyEntity::animate(delay);
  z = y + 17;
}

void BlackRatEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
  boundingBox.width = width - RAT_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - 13;
  boundingBox.height =  31;
}

void BlackRatEntity::collideMapRight()
{
  if (repulsion.active) repulsion.active = false;
  findNextGoal();
}

void BlackRatEntity::collideMapLeft()
{
  if (repulsion.active) repulsion.active = false;
  findNextGoal();
}

void BlackRatEntity::collideMapTop()
{
  if (repulsion.active) repulsion.active = false;
  findNextGoal();
}

void BlackRatEntity::collideMapBottom()
{
  if (repulsion.active) repulsion.active = false;
  findNextGoal();
}

void BlackRatEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    int oldDirection = currentDirection;

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

    if (oldDirection != currentDirection)
    {
      switch (currentDirection)
      {
        case 4: velocity.x = - creatureSpeed; velocity.y = 0.0f; break;
        case 6: velocity.x = + creatureSpeed; velocity.y = 0.0f; break;
        case 2: velocity.y = + creatureSpeed; velocity.x = 0.0f; break;
        case 8: velocity.y = - creatureSpeed; velocity.x = 0.0f; break;
        default: break;
      }
      acceleration = { 0, 0 };
      nextFacingDirection = currentDirection;
    }
  }
}

void BlackRatEntity::findNextGoal()
{
  currentTile = getCurrentTile();

  DungeonMap* dMap = game().getCurrentMap();

  int backDirection = 0;
  int oldDirection = currentDirection;

  switch (currentDirection)
  {
    case 4: backDirection = 6; break;
    case 6: backDirection = 4; break;
    case 2: backDirection = 8; break;
    case 8: backDirection = 2; break;
    default: break;
  }

  bool ok = false;
  {
    int r = 0;
    while (!ok)
    {
      r++;
      if (r == 150) // watchdog
        ok = true;
      else if (r == 40)
      {
        backDirection = 5;
      }

      int newDir = rand() % 4;
      if (newDir == 0)
      {
        if (backDirection != 4 && currentTile.x > 1 && (currentTile.y % 2 != 0) && dMap->isWalkable(currentTile.x - 1, currentTile.y))
        {
          currentDirection = 4;
          targetTile = IntCoord(currentTile.x - 1, currentTile.y);
          ok = true;
        }
      }
      else if (newDir == 1)
      {
        if (backDirection != 6 && currentTile.x < MAP_WIDTH - 2 && (currentTile.y % 2 != 0) && dMap->isWalkable(currentTile.x + 1, currentTile.y))
        {
          currentDirection = 6;
          targetTile = IntCoord(currentTile.x + 1, currentTile.y);
          ok = true;
        }
      }
      else if (newDir == 2)
      {
        if (backDirection != 8 && currentTile.y > 1 && (currentTile.x % 2 != 0) && dMap->isWalkable(currentTile.x, currentTile.y - 1))
        {
          currentDirection = 8;
          targetTile = IntCoord(currentTile.x, currentTile.y - 1);
          ok = true;
        }
      }
      else
      {
        if (backDirection != 2 && currentTile.y < MAP_HEIGHT - 2 && (currentTile.x % 2 != 0) && dMap->isWalkable(currentTile.x, currentTile.y + 1))
        {
          currentDirection = 2;
          targetTile = IntCoord(currentTile.x, currentTile.y + 1);
          ok = true;
        }
      }
    }
  }

  float accelerationAbs = (enemyType == EnemyTypeRatBlackHelmet) ? (creatureSpeed / 8) : (creatureSpeed / 16);

  switch (currentDirection)
  {
    case 4:
      velocity.x = -creatureSpeed;
      velocity.y = 0.0f;
      acceleration.x = -accelerationAbs;
      acceleration.y = 0.0f;
      break;
    case 6:
      velocity.x = creatureSpeed;
      velocity.y = 0.0f;
      acceleration.x = accelerationAbs;
      acceleration.y = 0;
      break;
    case 2:
      velocity.y = creatureSpeed;
      velocity.x = 0.0f;
      acceleration.y = accelerationAbs;
      acceleration.x = 0.0f;
      break;
    case 8:
      velocity.y = -creatureSpeed;
      velocity.x = 0.0f;
      acceleration.y = -accelerationAbs;
      acceleration.x = 0.0f;
      break;
    default: break;
  }

  if (oldDirection != currentDirection)
  {
    velocity = Vector2D {0, 0};
    doesAccelerate = true;
  }

  nextFacingDirection = currentDirection;
}

void BlackRatEntity::collideWithBolt(BoltEntity* boltEntity)
{
  if (ratType == RatBlackTypeHelmet && boltEntity->getBoltType() != ShotTypeIllusion)
  {
    int collisionDir = getCollisionDirection(boltEntity);
    bool boltCollide = true;

    switch (facingDirection)
    {
    case 4:
      if (collisionDir == 7 || collisionDir == 4 || collisionDir == 1) boltCollide = false;
      break;
    case 2:
      if (collisionDir == 1 || collisionDir == 2 || collisionDir == 3) boltCollide = false;
      break;
    case 6:
      if (collisionDir == 9 || collisionDir == 6 || collisionDir == 3) boltCollide = false;
      break;
    case 8:
      if (collisionDir == 7 || collisionDir == 8 || collisionDir == 9) boltCollide = false;
      break;
    }
    if (boltCollide) EnemyEntity::collideWithBolt(boltEntity);
    else
    {
      float xs = (x + boltEntity->getX()) / 2;
      float ys = (y + boltEntity->getY()) / 2;
      boltEntity->collide();
      SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
      star->setFading(true);
      star->setZ(y+ 100);
      star->setLifetime(0.7f);
      star->setType(ENTITY_EFFECT);
      star->setSpin(400.0f);
      SoundManager::getInstance().playSound(SOUND_CLANG_00);

      boltEntity->loseDamages(boltEntity->getDamages());

      if (boltEntity->getBoltType() == ShotTypeStone)
      {
        float factor = (boltEntity->isFromPlayer() && game().getPlayer()->isEquiped(EQUIP_RAPID_SHOT)) ? 0.25f : 1.0f;
        float recoilVelocity = factor * STONE_DECOIL_VELOCITY[boltEntity->getLevel()];
        float recoilDelay = factor * STONE_DECOIL_DELAY[boltEntity->getLevel()];

        Vector2D recoilVector = Vector2D(0, 0).vectorTo(boltEntity->getVelocity(),
                                recoilVelocity );
        giveRepulsion(true, recoilVector, recoilDelay);
      }
    }
  }
  else EnemyEntity::collideWithBolt(boltEntity);
}
