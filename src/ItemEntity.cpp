#include "ItemEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"
#include "MagnetEntity.h"
#include "WitchBlastGame.h"
#include <iostream>
#include <sstream>

ItemEntity::ItemEntity(enumItemType itemType, float x, float y)
  : CollidingSpriteEntity(ImageManager::getInstance().getImage(itemType >= FirstEquipItem ? IMAGE_ITEMS_EQUIP : IMAGE_ITEMS), x, y, ITEM_WIDTH, ITEM_HEIGHT)
{
  type = ENTITY_ITEM;
  this->itemType = itemType;
  frame = itemType;
  if (itemType >= FirstEquipItem)
  {
    frame = itemType - FirstEquipItem;
    game().addPresentItem(frame);
  }
  isMerchandise = false;
  imagesProLine = 10;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, 0, 0);
  isBeating = false;
  isFlying = false;
  jumpTimer = 1.0f + 0.1f * (rand() % 40);;
  firstJump = false;
  h = -1.0f;
}

void ItemEntity::startsJumping()
{
  firstJump = true;
  jumpTimer = 0;
}

void ItemEntity::setMerchandise(bool isMerchandise)
{
  this->isMerchandise = isMerchandise;
}
bool ItemEntity::getMerchandise()
{
  return isMerchandise;
}

int ItemEntity::getPrice()
{
  if (game().getPlayer()->isEquiped(EQUIP_MERCHANT))
    return (items[itemType].price * 0.8f);
  else
    return (items[itemType].price);
}

bool ItemEntity::canBePickedUp()
{
  // don't use health item if you don't need it
  if (game().getPlayer()->getHp() == game().getPlayer()->getHpMax())
    if (itemType >= ItemHealthVerySmall && itemType <= ItemHealthVerySmallPoison) return false;

  if (items[itemType].generatesStance && game().getPlayer()->getPlayerStatus() == PlayerEntity::playerStatusAcquire) return false;

  if (isMerchandise == true && game().getPlayer()->getGold() < getPrice()) return false;

  if (items[itemType].consumable && !game().getPlayer()->canAquireConsumable(itemType)) return false;

  if (itemType == ItemBossHeart && !game().getCurrentMap()->isCleared()) return false;

  if (itemType >= FirstEquipItem)
    if (game().getPlayer()->isEquiped(itemType - FirstEquipItem)) return false;

  if (items[itemType].spell != SpellNone && game().getPlayer()->getFairyTransmuted()) return false;

  if (itemType == ItemBookDualShots || itemType == ItemBookTripleShots)
    if (game().getPlayer()->isEquiped(EQUIP_BOOK_DUAL_QUICK)
        || game().getPlayer()->isEquiped(EQUIP_BOOK_TRIPLE_QUICK)
        || game().getPlayer()->isEquiped(EQUIP_RAPID_SHOT)) return false;

  if (itemType == ItemBookDualShotsQuick || itemType == ItemBookTripleShotsQuick)
    if (game().getPlayer()->isEquiped(EQUIP_BOOK_DUAL)
        || game().getPlayer()->isEquiped(EQUIP_BOOK_TRIPLE)
        || game().getPlayer()->isEquiped(EQUIP_RAPID_SHOT)) return false;

  if (itemType == ItemBookRapidShots)
    if (game().getPlayer()->isEquiped(EQUIP_BOOK_DUAL)
        || game().getPlayer()->isEquiped(EQUIP_BOOK_TRIPLE)
        || game().getPlayer()->isEquiped(EQUIP_BOOK_DUAL_QUICK)
        || game().getPlayer()->isEquiped(EQUIP_BOOK_TRIPLE_QUICK)) return false;

  return true;
}

bool ItemEntity::isOnMap()
{
  if (x < 0) return false;
  if (x > TILE_WIDTH * MAP_WIDTH) return false;
  if (y < 0) return false;
  if (y > TILE_HEIGHT * MAP_HEIGHT) return false;
  return true;
}

void ItemEntity::animate(float delay)
{
  // Has been identified ?
  if (itemType >= ItemPotion01 && itemType < ItemPotion01 + NUMBER_UNIDENTIFIED)
  {
    if (game().potionEffectKnown(itemType))
    {
      itemType = game().getPotion(itemType);
      frame = itemType;
    }
  }
  // Has been forgotten
  else if (itemType >= ItemPotion01 + NUMBER_UNIDENTIFIED && itemType < FirstEquipItem)
  {
    if (!game().potionEffectKnown(itemType))
    {
      itemType = game().getPotion(itemType);
      frame = itemType;
    }
  }

  if (isMerchandise)
  {
    testSpriteCollisions();
    return;
  }

  if (isFlying)
  {
    if (!isCollidingWithMap() && isOnMap())
    {
      isFlying = false;
      viscosity = 0.96f;
    }
    else
    {
      if (velocity.x < -10 || velocity.x > 10 || velocity.y < -10 || velocity.y > 10) // moving
      {
        // stay in the map
        if (velocity.x < 40 && x < TILE_WIDTH) velocity.x = 220.0f;
        else if (velocity.x > -40 && x > (TILE_WIDTH - 1) * MAP_WIDTH) velocity.x = -220.0f;
        if (velocity.y < 40 && y < TILE_HEIGHT) velocity.y = 220.0f;
        else if (velocity.y > -40 && y > (TILE_HEIGHT - 1) * MAP_HEIGHT) velocity.y = -220.0f;
      }
      else // not moving
      {
        setVelocity(Vector2D(100.0f + rand()% 250));
      }
      // make move
      x += velocity.x * delay;
      y += velocity.y * delay;
      age += delay;
    }
  }
  else // not flying
  {
    if (isCollidingWithMap() || !isOnMap())
    {
      isFlying = true;
    }
    else
    {
      CollidingSpriteEntity::animate(delay);
      if (firstJump || (canBePickedUp() && !isMerchandise))
      {
        jumpTimer -= delay;
        firstJump = false;
        if (jumpTimer <= 0.0f)
        {
          jumpTimer = 2.0f + 0.1f * (rand() % 40);
          h = 0.1f;
          hVelocity = 150.0f;
        }
      }
    }
  }

  if (h > 0.0f)
  {
    h += hVelocity * delay;
    hVelocity -= 1200.0f * delay;
  }

  if (age > 0.7f) testSpriteCollisions();

  if (isBeating)
  {
    timer -= delay;
    if (timer <= 0.0f)
    {
      timer = HEART_BEAT_DELAY;
      SoundManager::getInstance().playSound(SOUND_HEART);
    }
    float sc;
    if (timer > HEART_BEAT_DELAY - 0.25f)
    {
      sc = timer - HEART_BEAT_DELAY + 1.25f;
    }
    else
      sc = 1.0f;
    sprite.setScale(sc, sc);
  }
  if (itemType == ItemBossHeart && !isBeating && game().getCurrentMap()->isCleared())
  {
    // start beating
    isBeating = true;
    timer = HEART_BEAT_DELAY;
  }

  z = y + height / 2;
}

void ItemEntity::render(sf::RenderTarget* app)
{
  // shadow
  if (itemType < FirstEquipItem)
  {
    sprite.setTextureRect(sf::IntRect(9 * width, 3 * height, width, height));
    sprite.setPosition(x, y + 3);
    app->draw(sprite);
    sprite.setPosition(x, y);
  }
  else
  {
    sprite.setTextureRect(sf::IntRect(9 * width, 7 * height, width, height));
    app->draw(sprite);
  }

  // price
  if (isMerchandise)
  {
    std::ostringstream oss;
    oss << getPrice();
    sf::Color fontColor;
    if (getPrice() > game().getPlayer()->getGold()) fontColor = sf::Color(215, 20, 20);
    else fontColor = sf::Color(255, 255, 255);
    game().write(oss.str(), 16, x, y + 35.0f, ALIGN_CENTER, fontColor, app, 1 , 1, 0);
  }

  if (itemType < FirstEquipItem && itemType >= ItemPotion01 + NUMBER_UNIDENTIFIED)
  {
    float yItem = h > 0.1f ? y - h : y;

    int frameBottle = game().getPotion(itemType);
    sprite.setPosition(x, yItem);

    sprite.setTextureRect(sf::IntRect(frameBottle % imagesProLine * width, frameBottle / imagesProLine * height, width, height));
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(frame % imagesProLine * width, frame / imagesProLine * height, width, height));
    app->draw(sprite);

    sprite.setPosition(x, y);
  }
  else if (h > 0.1f)
  {
    sprite.setTextureRect(sf::IntRect(frame % imagesProLine * width, frame / imagesProLine * height, width, height));
    sprite.setPosition(x, y - h);
    app->draw(sprite);
    sprite.setPosition(x, y);
  }
  else
    CollidingSpriteEntity::render(app);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void ItemEntity::calculateBB()
{
  if (isMerchandise)
  {
    boundingBox.left = (int)x - width / 4;
    boundingBox.width = width / 2;
    boundingBox.top = (int)y - height / 2 - 20;
    boundingBox.height =  height + 100;
  }
  else
  {
    boundingBox.left = (int)x - width / 2;
    boundingBox.width = width;
    boundingBox.top = (int)y - height / 2;
    boundingBox.height =  height;
  }
}

void ItemEntity::dying()
{
  isDying = true;
}

void ItemEntity::buy()
{
  if (canBePickedUp() && isMerchandise)
  {
    game().getPlayer()->acquireItem(itemType);
    game().getPlayer()->pay(getPrice());
    game().getPlayer()->setItemToBuy(NULL);

    dying();

    if (!items[itemType].generatesStance)
      new MagnetEntity(x, y, game().getPlayer(), itemType);
  }
}

void ItemEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (itemType == ItemBossHeart && !game().getCurrentMap()->isCleared()) return;

  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);

  if (collideWithEntity(entity))
  {
    if (playerEntity != NULL && !playerEntity->isDead())
    {
      if (isMerchandise)
      {
        playerEntity->setItemToBuy(this);
      }
      else if (canBePickedUp())
      {
        playerEntity->acquireItem(itemType);

        if (isMerchandise) playerEntity->pay(getPrice());

        dying();

        if (!items[itemType].generatesStance)
          new MagnetEntity(x, y, playerEntity, itemType);
      }
    }
  }
}

void ItemEntity::collideMapRight()
{
  velocity.x = -velocity.x * 0.66f;
}

void ItemEntity::collideMapLeft()
{
  velocity.x = -velocity.x * 0.66f;
}

void ItemEntity::collideMapTop()
{
  velocity.y = -velocity.y * 0.66f;
}

void ItemEntity::collideMapBottom()
{
  velocity.y = -velocity.y * 0.66f;
}
