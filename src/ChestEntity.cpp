#include "ChestEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"

ChestEntity::ChestEntity(float x, float y, int chestType, bool isOpen)
    : CollidingSpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CHEST), x, y, 48, 48)
{
  type = ENTITY_CHEST;
  imagesProLine = 2;
  this->isOpen = isOpen;
  this->chestType = chestType;
  frame = chestType * 2 + (isOpen ? 1 : 0);
}

bool ChestEntity::getOpened()
{
  return isOpen;
}

int ChestEntity::getChestType()
{
  return chestType;
}

void ChestEntity::animate(float delay)
{
  CollidingSpriteEntity::animate(delay);
  if (!isOpen) testSpriteCollisions();
  z = y + height/2;
}

void ChestEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2;
  boundingBox.width = width;
  boundingBox.top = (int)y - height / 2;
  boundingBox.height =  height;
}


void ChestEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
    PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);

    if (collideWithEntity(entity))
    {
      if (!isOpen && playerEntity != NULL && !playerEntity->isDead())
      {
        open();
        frame += 1;
      }
    }
}

void ChestEntity::open()
{
  isOpen = true;
  SoundManager::getSoundManager()->playSound(SOUND_CHEST_OPENING);

  if (chestType == CHEST_BASIC)
  {
    for (int i = 0; i < 5; i++)
    {
      ItemEntity* newItem = new ItemEntity(ItemEntity::itemCopperCoin, x, y);
      newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
      newItem->setVelocity(Vector2D(50.0f + rand()% 150));
      newItem->setViscosity(0.96f);
    }
  }
  else if (chestType == CHEST_FAIRY)
  {
    ItemEntity* newItem = new ItemEntity(ItemEntity::itemFairy, x, y);
      newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
      newItem->setVelocity(Vector2D(50.0f + rand()% 150));
      newItem->setViscosity(0.96f);
  }
}

