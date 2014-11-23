#include "RatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RatEntity::RatEntity(float x, float y, ratTypeEnum ratType, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_RAT), x, y)
{
  this->ratType = ratType;
  imagesProLine = 10;
  this->invocated = invocated;

  if (ratType == RatTypeNormal)
  {
    frame = 1;
    dyingFrame = 9;
    deathFrame = FRAME_CORPSE_RAT;
    if (invocated) enemyType = EnemyTypeRat_invocated;
    else enemyType = EnemyTypeRat;
    hp = RAT_HP;
    creatureSpeed = RAT_SPEED;
  }
  else //(ratType == RatTypeHelmet)
  {
    frame = 31;
    dyingFrame = 39;
    deathFrame = FRAME_CORPSE_RAT_HELMET;
    if (invocated) enemyType = EnemyTypeRatHelmet_invocated;
    else enemyType = EnemyTypeRatHelmet;
    hp = RAT_HP_HELMET;
    creatureSpeed = RAT_SPEED_HELMET;
  }

  direction = rand() % 4;
  clockTurn = rand() % 2 == 0;
  compute(false);
  timer = 5 + rand() % 6;

  meleeDamages = RAT_DAMAGES;

  bloodColor = BloodRed;
  shadowFrame = -1;

  agonizingSound = SOUND_RAT_DYING;
  sprite.setOrigin(32.0f, 38.0f);
}

void RatEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising && !isExploding)
  {
    timer -= delay;
    if (timer < 0.0f)
    {
      timer = 5 + rand() % 6;
      if (rand() % 3 == 0) clockTurn = !clockTurn;
      compute(true);
    }

    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 1;
    if (facingDirection == 4 || facingDirection == 6) frame += 3;
    isMirroring = (facingDirection == 6 );
    if (facingDirection == 8) frame += 6;
    if (ratType == RatTypeHelmet) frame += 30;
  }

  EnemyEntity::animate(delay);
  z = y + 17;
}

void RatEntity::compute(bool turn)
{
  if (turn)
  {
    if (clockTurn)
    {
      direction++;
      if (direction == 4) direction = 0;
    }
    else
    {
      direction--;
      if (direction < 0) direction = 3;
    }
  }

  switch (direction)
  {
  case 0:
    velocity.x = 0;
    velocity.y = -creatureSpeed;
    facingDirection = 8;
    break;

  case 1:
    velocity.x = creatureSpeed;
    velocity.y = 0;
    facingDirection = 6;
    break;

  case 2:
    velocity.x = 0;
    velocity.y = creatureSpeed;
    facingDirection = 2;
    break;

  case 3:
    velocity.x = -creatureSpeed;
    velocity.y = 0;
    facingDirection = 4;
    break;
  }
}

void RatEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
  boundingBox.width = width - RAT_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - 13;
  boundingBox.height =  31;
}

void RatEntity::collideMapRight()
{
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else compute(true);
}

void RatEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else compute(true);
}

void RatEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else compute(true);
}

void RatEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else compute(true);
}

void RatEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    Vector2D recoilVector = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 50.0f);
    giveRecoil(false, recoilVector, 0.2f);
    compute(true);
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
      SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_STAR_2), xs, ys);
      star->setFading(true);
      star->setZ(y+ 100);
      star->setLifetime(0.7f);
      star->setType(ENTITY_EFFECT);
      star->setSpin(400.0f);
      SoundManager::getInstance().playSound(SOUND_CLANG_00);

      boltEntity->loseDamages(boltEntity->getDamages());
    }
  }
  else EnemyEntity::collideWithBolt(boltEntity);
}

void RatEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}
