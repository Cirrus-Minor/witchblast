#include "BatEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BatEntity::BatEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_BAT), x, y)
{
  creatureSpeed = BAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = BAT_HP;
  meleeDamages = BAT_DAMAGES;
  this->invocated = invocated;

  type = ENTITY_ENNEMY;
  bloodColor = BloodRed;
  changingDelay = -0.5f;
  shadowFrame = 3;
  movingStyle = movFlying;

  dyingFrame = 2;
  deathFrame = FRAME_CORPSE_BAT;
  agonizingSound = SOUND_BAT_DYING;

  enemyType = invocated ? EnemyTypeBat_invocated : EnemyTypeBat;
}

void BatEntity::animate(float delay)
{
  changingDelay -= delay;
  if (changingDelay < 0.0f)
  {
    velocity = Vector2D(creatureSpeed);
    changingDelay = 0.5f + (float)(rand() % 2500) / 1000.0f;
  }
  if (age < 0.0f)
    frame = 1;
  else
    frame = ((int)(age * 5.0f)) % 2;

  EnemyEntity::animate(delay);
}

void BatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + BAT_BB_LEFT;
    boundingBox.width = width - BAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + BAT_BB_TOP;
    boundingBox.height =  height - BAT_BB_HEIGHT_DIFF;
}

void BatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void BatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void BatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void BatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}

void BatEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movFlying)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), BAT_SPEED ));
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
