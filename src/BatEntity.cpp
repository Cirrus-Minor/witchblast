#include "BatEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BatEntity::BatEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_BAT), x, y)
{
  creatureSpeed = BAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = BAT_HP;
  meleeDamages = BAT_DAMAGES;
  this->invocated = invocated;

  bloodColor = BloodRed;
  changingDelay = -0.5f;
  shadowFrame = 9;
  movingStyle = movFlying;

  dyingFrame = 8;
  deathFrame = FRAME_CORPSE_BAT;
  agonizingSound = SOUND_BAT_DYING;
  sprite.setOrigin(32.0f, 26.0f);

  enemyType = invocated ? EnemyTypeBat_invocated : EnemyTypeBat;
}

void BatEntity::animate(float delay)
{
  if (!isAgonising && !isExploding)
  {
    changingDelay -= delay;
    if (changingDelay < 0.0f)
    {
      velocity = Vector2D(creatureSpeed);
      changingDelay = 0.5f + (float)(rand() % 2500) / 1000.0f;
      computeFacingDirection();
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

  EnemyEntity::animate(delay);
}

void BatEntity::calculateBB()
{
    boundingBox.left = (int)x - 12;
    boundingBox.width = 24;
    boundingBox.top = (int)y - 8;
    boundingBox.height =  22;
}

void BatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void BatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void BatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void BatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
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
