#include "ChestEntity.h"
#include "PlayerEntity.h"
#include "FallingRockEntity.h"
#include "ExplosionEntity.h"
#include "SnakeEntity.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"

#include <iostream>

ChestEntity::ChestEntity(float x, float y, int chestType, bool isOpen)
    : CollidingSpriteEntity(ImageManager::getInstance().getImage(IMAGE_CHEST), x, y, 48, 64)
{
  type = ENTITY_CHEST;
  imagesProLine = 2;
  this->isOpen = isOpen;
  this->chestType = chestType;
  frame = chestType * imagesProLine;
  //if (chestType > ChestFairy) frame = ChestFairy * imagesProLine;
  frame += (isOpen ? 1 : 0);
  if (isOpen && frame > ChestFairy * imagesProLine + 1) frame = ChestFairy * imagesProLine + 1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, 0, 0);

  timer = -1.0f;
  appearTimer = -1.0f;
  sprite.setOrigin(24, 40);
}

bool ChestEntity::getOpened()
{
  return isOpen;
}

int ChestEntity::getChestType()
{
  return chestType;
}

void ChestEntity::makeAppear()
{
  appearTimer = CHEST_APPEAR_DELAY;

  for(int i=0; i < 8; i++)
  {
    generateStar(sf::Color(50, 50, 255, 255));
    generateStar(sf::Color(255, 50, 50, 255));
    generateStar(sf::Color(50, 255, 50, 255));
    generateStar(sf::Color(255, 255, 255, 255));
  }
}

void ChestEntity::animate(float delay)
{
  if (appearTimer >= 0.0f) appearTimer -= delay;
  CollidingSpriteEntity::animate(delay);
  if (!isOpen) testSpriteCollisions();
  z = y + 21;

  // trap
  if (timer > 0.0f)
  {
    timer -= delay;
    if (timer <= 0.0f)
    {
      if (trap == TrapStones)
      {
        initFallingGrid();
        for (int i = 0; i < 22; i++) fallRock();
        game().makeShake(0.25f);
      }
      else if (trap == TrapExplosion)
      {
        initFallingGrid();
        new ExplosionEntity(x, y, ExplosionTypeStandard, 12, EnemyTypeNone, true);
        game().addCorpse(x, y, FRAME_CORPSE_SLIME_VIOLET);
        SoundManager::getInstance().playSound(SOUND_BOOM_00);
        game().makeShake(0.25f);
      }
      else // snakes
      {
        new SnakeEntity(x + 1, y, SnakeEntity::SnakeTypeNormal, false);
        new SnakeEntity(x - 1, y, SnakeEntity::SnakeTypeNormal, false);
        new SnakeEntity(x, y + 1, SnakeEntity::SnakeTypeNormal, false);
      }
    }
  }
}

void ChestEntity::render(sf::RenderTarget* app)
{
  if (appearTimer > 0.0f)
  {
    int fade = 255 * (1.0f - appearTimer / CHEST_APPEAR_DELAY);
    sprite.setColor(sf::Color(255, 255, 255, fade));
  }
  else
    sprite.setColor(sf::Color(255, 255, 255, 255));

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
  boundingBox.top = (int)y - 24;
  boundingBox.height =  46;
}


void ChestEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (isOpen || appearTimer > 0.5f) return;
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);

  if (collideWithEntity(entity))
  {
    if (playerEntity != NULL && !playerEntity->isDead())
    {
      open();
      frame += 1;
      if (frame > ChestFairy * imagesProLine + 1) frame = ChestFairy * imagesProLine + 1;
    }
  }
}

void ChestEntity::dropItem(enumItemType item)
{
  ItemEntity* newItem = new ItemEntity(item, x, y);
  newItem->setVelocity(Vector2D(50.0f + rand()% 140));
  if (newItem->getVelocity().y < 0.0f) newItem->setVelocity(Vector2D(newItem->getVelocity().x, -newItem->getVelocity().y));
  newItem->setViscosity(0.96f);
  newItem->startsJumping();
}

void ChestEntity::open()
{
  isOpen = true;
  if (chestType >= ChestFairy)
    SoundManager::getInstance().playSound(SOUND_GLASS);
  else
    SoundManager::getInstance().playSound(SOUND_CHEST_OPENING);

  if (chestType == ChestBasic)
  {
    int r = rand()% 50;
    if (r == 0 && !game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP))
      dropItem(ItemFloorMap);
    else if (r == 1 && !game().getPlayer()->isEquiped(EQUIP_ALCOHOL))
      dropItem(ItemAlcohol);
    else if (r == 2 && !game().getPlayer()->isEquiped(EQUIP_LUCK))
      dropItem(ItemLuck);
    else if (r == 3 && !game().getPlayer()->isEquiped(EQUIP_FAIRY_POWDER) && game().getPlayer()->getFairieNumber() > 0)
      dropItem(ItemFairyPowder);

    else if (r < 10)
    {
      // consumable
      if (rand() % 3 == 0)
      {
        // scroll
        dropItem(ItemScrollRevelation);
      }
      else
      {
        // potion
        dropItem((enumItemType)(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED));
      }
    }

    else
    {
      // gold
      int r = 2 + rand() % 6;
      if (game().getPlayer()->isEquiped(EQUIP_LUCK)) r += 1 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        dropItem(ItemCopperCoin);
      }
    }

    // trap !
    if (game().getLevel() >= 2 && !game().getPlayer()->isEquiped(EQUIP_DISPLACEMENT_GLOVES))
    {
      if (rand() % 5 == 0) // trap
      {
        SoundManager::getInstance().playSound(SOUND_TRAP);
        int r;
        // no "earthquake" trap in insel rooms
        if (game().getCurrentMap()->isWalkable(1, 1))
          r = rand() % 3;
        else
          r = 1 + rand() % 2;
        if (r == 1)
        {
          timer = 1.0f;
          trap = TrapExplosion;
        }
        else if (r == 2)
        {
          timer = 1.0f;
          trap = TrapSnakes;
        }
        else
        {
          timer = 0.5f;
          trap = TrapStones;
        }
      }
    }
  }
  else if (chestType >= ChestFairy)
  {
    enumItemType itemType = ItemFairy;
    switch (chestType - ChestFairy)
    {
      case FamiliarFairy: itemType = ItemFairy; break;
      case FamiliarFairyIce: itemType = ItemFairyIce; break;
      case FamiliarFairyFire: itemType = ItemFairyFire; break;
      case FamiliarFairyTarget: itemType = ItemFairyTarget; break;
      case FamiliarFairyPoison: itemType = ItemFairyPoison; break;
      case FamiliarFairyStone: itemType = ItemFairyStone; break;
    }

    ItemEntity* newItem = new ItemEntity(itemType, x, y);
    newItem->setVelocity(Vector2D(50.0f + rand()% 150));
    newItem->setViscosity(0.96f);
  }
  else if (chestType == ChestExit)
  {
    // Multiplayer bonus
    game().gainMultiplayerPower();
    int r = rand() % 3;
    if (r == 0)
    {
      for (int i = 0; i < (game().getPlayer()->isEquiped(EQUIP_LUCK) ? 6 : 5); i++)
      {
        dropItem(ItemSilverCoin);
      }
    }
    else if (r == 1)
    {
      for (int i = 0; i < (game().getPlayer()->isEquiped(EQUIP_LUCK) ? 4 : 3); i++)
      {
        dropItem(ItemSilverCoin);
      }
      dropItem(ItemHealth);
    }
    else
    {
      int bonusType = game().getRandomEquipItem(false, true);

      if (game().getPlayer()->isEquiped(bonusType))
        dropItem(ItemBonusHealth);
      else
        dropItem((enumItemType)(FirstEquipItem + bonusType));
    }
  }
  else if (chestType == ChestChallenge)
  {
    game().generateChallengeBonus(x, y);
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
  new FallingRockEntity(rx * TILE_WIDTH + TILE_WIDTH / 2,
                        ry * TILE_HEIGHT + TILE_HEIGHT / 2,
                        rand() % 3,
                        true);
}

void ChestEntity::generateStar(sf::Color starColor)
{
  SpriteEntity* spriteStar = new SpriteEntity(
                           ImageManager::getInstance().getImage(IMAGE_STAR_2),
                            x - 15 + rand() % 30, y - 10 + rand() % 30);
  spriteStar->setScale(0.8f, 0.8f);
  spriteStar->setZ(1000.0f);
  spriteStar->setSpin(-100 + rand()%200);
  spriteStar->setVelocity(Vector2D(50 + rand()%40));
  spriteStar->setWeight(-130);
  spriteStar->setFading(true);
  spriteStar->setAge(-0.8f);
  spriteStar->setLifetime(0.2f + (rand() % 100) * 0.005f );
  spriteStar->setColor(starColor);
  spriteStar->setType(ENTITY_EFFECT);
  spriteStar->setRenderAdd();
}
