#include "ZombieDarkEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

ZombieDarkEntity::ZombieDarkEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_ZOMBIE), x, y),
  currentTile(0, 0),
  targetTile(0, 0)
{
  imagesProLine = 11;

  frame = 11;
  dyingFrame = 21;
  bloodColor = BloodRed;
  shadowFrame = 9;
  deathFrame = FRAME_CORPSE_ZOMBIE_DARK;
  enemyType = EnemyTypeZombieDark;
  hp = ZOMBIE_HP;
  creatureSpeed = ZOMBIE_SPEED;
  meleeDamages = ZOMBIE_DAMAGE;

  agonizingSound = SOUND_ZOMBIE_DYING;

  currentDirection = 2 + 2 * rand()%4;
  facingDirection = currentDirection;
  nextFacingDirection = currentDirection;

  height = 80;
  sprite.setOrigin(32.0f, 60.0f);
  attackTimer = 0.9f;

  resistance[ResistanceFrozen] = ResistanceHigh;
  resistance[ResistanceRepulsion] = ResistanceHigh;
  resistance[ResistancePoison] = ResistanceImmune;

  findNextGoal();
}

void ZombieDarkEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
      // goal reached ?
    if (currentDirection == 6 && x > (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2) ) findNextGoal();
    else if (currentDirection == 4 && x < (targetTile.x * TILE_WIDTH + TILE_WIDTH / 2) ) findNextGoal();
    else if (currentDirection == 2 && y > (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 - 5) ) findNextGoal();
    else if (currentDirection == 8 && y < (targetTile.y * TILE_HEIGHT + TILE_HEIGHT / 2 - 5) ) findNextGoal();

    checkNextFacing(delay);

    frame = ((int)(age * 4.0f)) % 4;
    if (frame == 3) frame = 1;
    if (facingDirection == 4 || facingDirection == 6) frame += 3;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 6;
    frame += 11;

    attackTimer -= delay;
  }

  EnemyEntity::animate(delay);
  z = y + 17;
}

void ZombieDarkEntity::calculateBB()
{
  boundingBox.left = (int)x - 14;
  boundingBox.width = 28;
  boundingBox.top = (int)y - 18;
  boundingBox.height =  36;
}

void ZombieDarkEntity::collideMapRight()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  findNextRandomGoal();
}

void ZombieDarkEntity::collideMapLeft()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  findNextRandomGoal();
}

void ZombieDarkEntity::collideMapTop()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  findNextRandomGoal();
}

void ZombieDarkEntity::collideMapBottom()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  findNextRandomGoal();
}

void ZombieDarkEntity::collideWithEnemy(EnemyEntity* entity)
{
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
      case 4: velocity.x = - creatureSpeed; velocity.y = 0.0f; break;
      case 6: velocity.x = + creatureSpeed; velocity.y = 0.0f; break;
      case 2: velocity.y = + creatureSpeed; velocity.x = 0.0f; break;
      case 8: velocity.y = - creatureSpeed; velocity.x = 0.0f; break;
      default: break;
    }
    nextFacingDirection = currentDirection;
  }
}

void ZombieDarkEntity::findNextGoal()
{
  currentTile = getCurrentTile();

  float xPlayer = game().getPlayerPosition().x;
  float yPlayer = game().getPlayerPosition().y;

  float xDist = abs(x - xPlayer);
  float yDist = abs(y - yPlayer);

  int playerDirecion;
  if (xDist >= yDist)
  {
    if (xPlayer > x) playerDirecion = 6;
    else playerDirecion = 4;
  }
  else
  {
    if (yPlayer > y) playerDirecion = 2;
    else playerDirecion = 8;
  }

  switch (currentDirection)
  {
    case 4:
      if (playerDirecion != 6)
        currentDirection = playerDirecion;
      else
      {
        if (yPlayer > y) currentDirection = 2;
        else currentDirection = 8;
      }
      break;

    case 6:
      if (playerDirecion != 4)
        currentDirection = playerDirecion;
      else
      {
        if (yPlayer > y) currentDirection = 2;
        else currentDirection = 8;
      }
      break;

    case 2:
      if (playerDirecion != 8)
        currentDirection = playerDirecion;
      else
      {
        if (xPlayer > x) currentDirection = 6;
        else currentDirection = 4;
      }
      break;
    case 8:
      if (playerDirecion != 2)
        currentDirection = playerDirecion;
      else
      {
        if (xPlayer > x) currentDirection = 6;
        else currentDirection = 4;
      }
      break;
    default: break;
  }

  switch (currentDirection)
  {
    case 4:
      velocity.x = - creatureSpeed;
      velocity.y = 0.0f;
      targetTile = IntCoord(currentTile.x - 2, currentTile.y);
      break;
    case 6:
      velocity.x = + creatureSpeed;
      velocity.y = 0.0f;
      targetTile = IntCoord(currentTile.x + 2, currentTile.y);
      break;
    case 2:
      velocity.y = + creatureSpeed;
      velocity.x = 0.0f;
      targetTile = IntCoord(currentTile.x, currentTile.y + 2);
      break;
    case 8:
      velocity.y = - creatureSpeed;
      velocity.x = 0.0f;
      targetTile = IntCoord(currentTile.x, currentTile.y - 2);
      break;
    default: break;
  }
  nextFacingDirection = currentDirection;

  if (currentDirection == playerDirecion && attackTimer <= 0.0f)
  {
    giveRepulsion(false, Vector2D(velocity.x * 2.0f, velocity.y * 2.0f), 1.5f);
    attackTimer = 2.0f;
    SoundManager::getInstance().playSound(SOUND_ZOMBIE_ATTACKING);
    facingTimer = -1.0f;
  }
  else
    SoundManager::getInstance().playSound(SOUND_ZOMBIE_00 + rand() % 2);
}

void ZombieDarkEntity::findNextRandomGoal()
{
  currentTile = getCurrentTile();

  DungeonMap* dMap = game().getCurrentMap();

  int backDirection = 0;
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

  switch (currentDirection)
  {
    case 4: velocity.x = - creatureSpeed; velocity.y = 0.0f; break;
    case 6: velocity.x = + creatureSpeed; velocity.y = 0.0f; break;
    case 2: velocity.y = + creatureSpeed; velocity.x = 0.0f; break;
    case 8: velocity.y = - creatureSpeed; velocity.x = 0.0f; break;
    default: break;
  }

  nextFacingDirection = currentDirection;
}

void ZombieDarkEntity::collideWithBolt(BoltEntity* boltEntity)
{
  EnemyEntity::collideWithBolt(boltEntity);
}

bool ZombieDarkEntity::isAttacking()
{
  return attackTimer > 1.0f;
}
