#include "EnnemyEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include <iostream>
#include "WitchBlastGame.h"

EnnemyEntity::EnnemyEntity(sf::Texture* image, float x, float y)
    : BaseCreatureEntity (image, x, y, 64, 64)
{
  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;

  z = y;
  age = -0.001f * (rand()%800) - 0.4f;
}

void EnnemyEntity::animate(float delay)
{
  if (canCollide()) testSpriteCollisions();
  if (age > 0.0f)
    BaseCreatureEntity::animate(delay);
  else
    age += delay;
}

void EnnemyEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2;
    boundingBox.width = width;
    boundingBox.top = (int)y - height / 2;
    boundingBox.height =  height;
}

void EnnemyEntity::collideMapRight()
{
    velocity.x = 0.0f;
}

void EnnemyEntity::collideMapLeft()
{
    velocity.x = 0.0f;
}

void EnnemyEntity::collideMapTop()
{
    velocity.y = 0.0f;
}

void EnnemyEntity::collideMapBottom()
{
    velocity.y = 0.0f;
}

void EnnemyEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        if (playerEntity->hurt(meleeDamages, ShotTypeStandard))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(16);
          star->setSpin(400.0f);
        }
        inflictsRecoilTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && !isDying && boltEntity->getAge() > 0.05f)
      {
        boltEntity->collide();
        hurt(boltEntity->getDamages(), boltEntity->getBoltType());
        game().generateBlood(x, y, bloodColor);
        SoundManager::getSoundManager()->playSound(SOUND_IMPACT);

        float xs = (x + boltEntity->getX()) / 2;
        float ys = (y + boltEntity->getY()) / 2;
        SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
        star->setFading(true);
        star->setZ(y+ 100);
        star->setLifetime(0.7f);
        star->setType(16);
        star->setSpin(400.0f);

        if (boltEntity->getBoltType() == ShotTypeStone)
        {
          Vector2D recoilVector = Vector2D(boltEntity->getX(), boltEntity->getY()).vectorTo(Vector2D(x, y), 110 );
          giveRecoil(true, recoilVector, 0.15f);
        }
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENNEMY && entity->getType() <= ENTITY_ENNEMY_MAX)
      {
        if (this != entity)
          collideWithEnnemy(entity);
      }
    }
  }

/*
  if (x < OFFSET_X + TILE_WIDTH)
  {
    x = OFFSET_X + TILE_WIDTH;
    if (velocity.x > 1.0f) velocity.x = -velocity.x;
    else if (velocity.x > -1.0f) velocity.x = -1.0f;
  }
  else if (x > OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1))
  {
    x = OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1);
    if (velocity.x < -1.0f) velocity.x = -velocity.x;
    else if (velocity.x < 1.0f) velocity.x = 1.0f;
  }

  if (y < OFFSET_Y + TILE_HEIGHT)
  {
    y = OFFSET_Y + TILE_HEIGHT;
    if (velocity.y > 1.0f) velocity.y = -velocity.y;
    else if (velocity.y > -1.0f) velocity.y = -1.0f;
  }
  else if (y > OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1))
  {
    y = OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1);
    if (velocity.y < -1.0f) velocity.y = -velocity.y;
    else if (velocity.y < 1.0f) velocity.y = 1.0f;
  }*/
}

void EnnemyEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  // To implement the behaviour when colliding with another ennemy
}

void EnnemyEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(3), x, y, 64, 64);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(2);
  deadRat->setType(13);
}

void EnnemyEntity::drop()
{
  if (rand() % 5 == 0)
  {
    ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
    newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
  }
}

bool EnnemyEntity::canCollide()
{
  return true;
}
