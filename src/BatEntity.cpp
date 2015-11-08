#include "BatEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

const int BAT_ACCELERATION = 10;

BatEntity::BatEntity(float x, float y, EnumBatType batType, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_BAT), x, y)
{
  creatureSpeed = BAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  imagesProLine = 10;

  this->invocated = invocated;
  this->batType = batType;

  changingDelay = -0.5f;
  shadowFrame = 9;
  movingStyle = movFlying;

  agonizingSound = SOUND_BAT_DYING;
  sprite.setOrigin(32.0f, 26.0f);

  if (batType == BatStandard)
  {
    enemyType = invocated ? EnemyTypeBat_invocated : EnemyTypeBat;
    hp = BAT_HP;
    meleeDamages = BAT_DAMAGES;
    bloodColor = BloodRed;
    dyingFrame = 8;
    deathFrame = FRAME_CORPSE_BAT;
  }

  else // BatSkeleton
  {
    enemyType = invocated ? EnemyTypeBatSkeleton_invocated : EnemyTypeBatSkeleton;
    hp = BAT_HP * 2;
    meleeDamages = 8;
    bloodColor = BloodNone;
    dyingFrame = 18;
    deathFrame = FRAME_CORPSE_BAT_SKELETON;
  }
}

void BatEntity::animate(float delay)
{
  if (!isAgonising)
  {
    changingDelay -= delay;
    if (changingDelay < 0.0f)
    {
      if (batType != BatSkeleton || rand() % 3 == 0)
      {
        velocity = Vector2D(creatureSpeed);
        acceleration.x = velocity.x / BAT_ACCELERATION;
        acceleration.y = velocity.y / BAT_ACCELERATION;
        computeFacingDirection();
        velocity = Vector2D {0, 0};
        doesAccelerate = true;
      }

      else
      {
        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
        acceleration.x = velocity.x / BAT_ACCELERATION;
        acceleration.y = velocity.y / BAT_ACCELERATION;
        computeFacingDirection();
        velocity = Vector2D {0, 0};
        doesAccelerate = true;
      }

      changingDelay = 0.5f + (float)(rand() % 2500) / 1000.0f;

    }

    if (age < 0.0f)
      frame = 1;
    else
    {
      switch (facingDirection)
      {
        case 2: frame = 0; break;
        case 4: frame = 2; break;
        case 6: frame = 4; break;
        case 8: frame = 6; break;
      }
      frame += ((int)(age * 7.0f)) % 2;
    }
  }
  if (batType == BatSkeleton) frame += 10;
  EnemyEntity::animate(delay);
}

void BatEntity::calculateBB()
{
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 10;
    boundingBox.height =  25;
}

void BatEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  acceleration.x = -acceleration.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
  else computeFacingDirection();
}

void BatEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  acceleration.x = -acceleration.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
  else computeFacingDirection();
}

void BatEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  acceleration.y = -acceleration.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
  else computeFacingDirection();
}

void BatEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  acceleration.y = -acceleration.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
  else computeFacingDirection();
}

void BatEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getEnemyType() != EnemyTypeSpiderWeb && entity->getMovingStyle() == movFlying)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), BAT_SPEED ));
    computeFacingDirection();
  }
}

void BatEntity::dying()
{
  EnemyEntity::dying();
  h = 25.0f;
}

void BatEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}
