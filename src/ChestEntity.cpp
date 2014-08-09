#include "ChestEntity.h"
#include "PlayerEntity.h"
#include "FallingRockEntity.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"

#include <iostream>

ChestEntity::ChestEntity(float x, float y, int chestType, bool isOpen)
    : CollidingSpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CHEST), x, y, 48, 48)
{
  type = ENTITY_CHEST;
  imagesProLine = 2;
  this->isOpen = isOpen;
  this->chestType = chestType;
  frame = chestType * 2;
  if (chestType > CHEST_FAIRY) frame = CHEST_FAIRY * 2;
  frame += (isOpen ? 1 : 0);
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

  timer = -1.0f;
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
  z = y + height/2 - 5;

  // trap
  if (timer > 0.0f)
  {
    timer -= delay;
    if (timer <= 0.0f)
    {
      initFallingGrid();
      for (int i = 0; i < 22; i++) fallRock();
      SoundManager::getSoundManager()->playSound(SOUND_TRAP);
      game().makeShake(0.25f);
    }
  }
}

void ChestEntity::render(sf::RenderTarget* app)
{
  CollidingSpriteEntity::render(app);
  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
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
    int r = 2 + rand() % 6;
    for (int i = 0; i < r; i++)
    {
      ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
      newItem->setVelocity(Vector2D(50.0f + rand()% 150));
      newItem->setViscosity(0.96f);
    }

    // trap !
    if (game().getLevel() >= 5)
    {
      if (rand() % 6 == 0) timer = 0.5f;
    }
  }
  else if (chestType >= CHEST_FAIRY)
  {
    enumItemType itemType = ItemFairy;
    switch (chestType - CHEST_FAIRY)
    {
      case FamiliarFairy: itemType = ItemFairy; break;
      case FamiliarFairyIce: itemType = ItemFairyIce; break;
      case FamiliarFairyFire: itemType = ItemFairyFire; break;
      case FamiliarFairyTarget: itemType = ItemFairyTarget; break;
    }

    ItemEntity* newItem = new ItemEntity(itemType, x, y);
    newItem->setVelocity(Vector2D(50.0f + rand()% 150));
    newItem->setViscosity(0.96f);
  }
  else if (chestType == CHEST_EXIT)
  {
    int r = rand() % 3;
    if (r == 0)
    {
      for (int i = 0; i < 5; i++)
      {
        ItemEntity* newItem = new ItemEntity(ItemSilverCoin, x, y);
        newItem->setVelocity(Vector2D(90.0f + rand()% 150));
        newItem->setViscosity(0.96f);
      }
    }
    else if (r == 1)
    {
      for (int i = 0; i < 3; i++)
      {
        ItemEntity* newItem = new ItemEntity(ItemSilverCoin, x, y);
        newItem->setVelocity(Vector2D(90.0f + rand()% 150));
        newItem->setViscosity(0.96f);
      }
      ItemEntity* newItem = new ItemEntity(ItemHealth, x, y);
      newItem->setVelocity(Vector2D(90.0f + rand()% 150));
      newItem->setViscosity(0.96f);
    }
    else
    {
      int bonusType = game().getRandomEquipItem(false, true);
      ItemEntity* newItem = new ItemEntity( (enumItemType)(FirstEquipItem + bonusType), x ,y);
      newItem->setVelocity(Vector2D(90.0f + rand()% 150));
      newItem->setViscosity(0.96f);
    }
  }
}
void ChestEntity::initFallingGrid()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
      fallingGrid[i][j] = false;
}

void ChestEntity::fallRock()
{
  int rx, ry;
  do
  {
    rx = 1 + rand() % (MAP_WIDTH - 2);
    ry = 1 + rand() % (MAP_HEIGHT - 2);
  }
  while (fallingGrid[rx][ry]);

  fallingGrid[rx][ry] = true;
  new FallingRockEntity(rx * TILE_WIDTH + OFFSET_X + TILE_WIDTH / 2,
                        ry * TILE_HEIGHT + OFFSET_Y + TILE_HEIGHT / 2,
                        rand() % 3);
}

