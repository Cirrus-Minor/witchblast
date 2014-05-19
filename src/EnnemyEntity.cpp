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
  if (collideWithEntity(entity) &&
      (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT ))
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

    else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
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
}

void EnnemyEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(3), x, y, 64, 64);
  //deadRat->setZ(y + height);
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
