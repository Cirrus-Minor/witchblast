#include "RatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RatEntity::RatEntity(float x, float y, ratTypeEnum ratType)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y)
{
  this->ratType = ratType;
  imagesProLine = 8;

  if (ratType == RatTypeNormal)
  {
    frame = 0;
    dyingFrame = 6;
    deathFrame = FRAME_CORPSE_RAT;
    enemyType = EnemyTypeRat;
    hp = RAT_HP;
    creatureSpeed = RAT_SPEED;
  }
  else //(ratType == RatTypeHelmet)
  {
    frame = 24;
    dyingFrame = 30;
    deathFrame = FRAME_CORPSE_RAT_HELMET;
    enemyType = EnemyTypeRatHelmet;
    hp = RAT_HP_HELMET;
    creatureSpeed = RAT_SPEED;
  }

  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  meleeDamages = RAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = BloodRed;
  shadowFrame = 7;


  agonizingSound = SOUND_RAT_DYING;
  enemyType = EnemyTypeRat;
}

void RatEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    frame = ((int)(age * 5.0f)) % 2;
    if (facingDirection == 4 || facingDirection == 6) frame += 2;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 4;
    if (ratType == RatTypeHelmet) frame += 24;
  }

  EnemyEntity::animate(delay);
}

void RatEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
  boundingBox.width = width - RAT_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - height / 2 + RAT_BB_TOP;
  boundingBox.height =  height - RAT_BB_HEIGHT_DIFF;
}

void RatEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void RatEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void RatEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void RatEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void RatEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
    computeFacingDirection();
  }
}

void RatEntity::collideWithBolt(BoltEntity* boltEntity)
{
  if (ratType == RatTypeHelmet && boltEntity->getBoltType() != ShotTypeIllusion)
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
      SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
      star->setFading(true);
      star->setZ(y+ 100);
      star->setLifetime(0.7f);
      star->setType(ENTITY_EFFECT);
      star->setSpin(400.0f);
      SoundManager::getSoundManager()->playSound(SOUND_CLANG_00);
    }
  }
  else EnemyEntity::collideWithBolt(boltEntity);
}
