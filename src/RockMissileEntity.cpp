#include "RockMissileEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RockMissileEntity::RockMissileEntity(float x, float y, int rockType)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_CYCLOP), x, y)
{

  Vector2D targetPos = game().getPlayerPosition();
  imagesProLine = 20;
    collisionDirection = -1;
  type = ENTITY_ENNEMY;
  enemyType = EnemyTypeRockMissile;
  movingStyle = movFlying;
  bloodColor = bloodNone; // stones don't bleed
  hasCollided = false;

  age = 0.0f;

  this->rockType = rockType;

  if (rockType == 0)
  {
    creatureSpeed = 500.0f;
    hp = 12;
    meleeDamages = 8;
    frame = 18;
  }
  else
  {
    creatureSpeed = 450.0f;
    hp = 24;
    meleeDamages = 10;
    frame = 38;
  }

  setVelocity(Vector2D(x, y).vectorNearlyTo(targetPos, creatureSpeed, 0.4f));
}

void RockMissileEntity::animate(float delay)
{
  EnemyEntity::animate(delay);
  if (x < -60 || x > 1050 || y < - 50 || y > 800) isDying = true;
}

void RockMissileEntity::calculateBB()
{
  int w;
  if (rockType == 0) w = 20;
  else w = 24;

  boundingBox.left = (int)x - w / 2;
  boundingBox.width = w;
  boundingBox.top = (int)y - w / 2;
  boundingBox.height =  w;
}

void RockMissileEntity::collideWall()
{
  if (rockType == 1 && !hasCollided)
  {
    hasCollided = true;
    if (collisionDirection == DIRECTION_RIGHT || collisionDirection == DIRECTION_LEFT) velocity.x = -velocity.x;
    else velocity.y = -velocity.y;
  }
  else
  {
    dying();
  }
}

void RockMissileEntity::collideMapRight()
{
  collisionDirection = DIRECTION_RIGHT;
  collideWall();
}

void RockMissileEntity::collideMapLeft()
{
  collisionDirection = DIRECTION_LEFT;
  collideWall();
}

void RockMissileEntity::collideMapTop()
{
  collisionDirection = DIRECTION_TOP;
  collideWall();
}

void RockMissileEntity::collideMapBottom()
{
  collisionDirection = DIRECTION_BOTTOM;
  collideWall();
}

void RockMissileEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
}

void RockMissileEntity::dying()
{
  isDying = true;
  game().addKilledEnemy(enemyType);
  SoundManager::getSoundManager()->playSound(SOUND_ROCK_IMPACT);
  game().makeShake(0.1f);

  for (int i = 0; i < 4; i++)
  {
    SpriteEntity* blood = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BLOOD), x, y, 16, 16, 6);
    blood->setZ(OFFSET_Y - 1);

    blood->setFrame(12 + rand()%6);
    blood->setType(ENTITY_BLOOD);
    blood->setVelocity(Vector2D(rand()%150));
    blood->setViscosity(0.95f);

    float bloodScale = 1.0f + (rand() % 10) * 0.1f;
    blood->setScale(bloodScale, bloodScale);

    if ((collisionDirection == DIRECTION_LEFT) && (blood->getVelocity().x < 0.0f))
      blood->setVelocity(Vector2D(-blood->getVelocity().x * 0.25f, blood->getVelocity().y));
    else if ((collisionDirection == DIRECTION_RIGHT) && (blood->getVelocity().x > 0.0f))
      blood->setVelocity(Vector2D(-blood->getVelocity().x * 0.25f, blood->getVelocity().y));
    else if ((collisionDirection == DIRECTION_TOP) && (blood->getVelocity().y < 0.0f))
      blood->setVelocity(Vector2D(blood->getVelocity().x, -blood->getVelocity().y * 0.25f));
    else if ((collisionDirection == DIRECTION_BOTTOM) && (blood->getVelocity().y > 0.0f))
      blood->setVelocity(Vector2D(blood->getVelocity().x, -blood->getVelocity().y * 0.25f));
  }
}
