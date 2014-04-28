#include "PlayerEntity.h"
#include "BoltEntity.h"
#include "EnnemyBoltEntity.h"
#include "ItemEntity.h"
#include "FairyEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

PlayerEntity::PlayerEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f)
      : BaseCreatureEntity (image, x, y, 64, 96)
{
  currentFireDelay = -1.0f;
  canFirePlayer = true;
  type = ENTITY_PLAYER;

  imagesProLine = 8;
  playerStatus = playerStatusPlaying;
  hp = INITIAL_PLAYER_HP;
  hpDisplay = hp;
  hpMax = INITIAL_PLAYER_HP;
  gold = 0;

  boltLifeTime = INITIAL_BOLT_LIFE;

  bloodColor = bloodRed;

  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++) equip[i] = false;
  colliding = 0;

  computePlayer();

  firingDirection = 5;
  facingDirection = 2;

  // TEST
  //equip[EQUIP_BOSS_KEY] = true;
}

void PlayerEntity::moveTo(float newX, float newY)
{
  float dx = newX - x;
  float dy = newY - y;

  x = newX;
  y = newY;

  if (equip[EQUIP_FAIRY])
  {
    fairy->setX(fairy->getX() + dx);
    fairy->setY(fairy->getY() + dy);
  }
}

float PlayerEntity::getPercentFireDelay()
{
  if (canFirePlayer) return 1.0f;

  else return (1.0f - currentFireDelay / fireDelay);
}

int PlayerEntity::getColliding()
{
  return colliding;
}

bool PlayerEntity::isDead()
{
  return playerStatus==playerStatusDead;
}

void PlayerEntity::setEntering()
{
  playerStatus = playerStatusEntering;
}

void PlayerEntity::pay(int price)
{
  gold -= price;
  if (gold < 0) gold = 0;
  SoundManager::getSoundManager()->playSound(SOUND_PAY);
}

void PlayerEntity::animate(float delay)
{
  // rate of fire
  if (!canFirePlayer)
  {
    currentFireDelay -= delay;
    canFirePlayer = (currentFireDelay <= 0.0f);
  }
  // acquisition animation
  if (playerStatus == playerStatusAcquire)
  {
    acquireDelay -= delay;
    if (acquireDelay <= 0.0f)
    {
      equip[acquiredItem] = true;
      computePlayer();
      playerStatus = playerStatusPlaying;

      if (acquiredItem == (int)EQUIP_FAIRY)
      {
        fairy = new FairyEntity(x, y - 50.0f, this);
      }
    }
  }
  else if (playerStatus == playerStatusUnlocking)
  {
    acquireDelay -= delay;
    if (acquireDelay <= 0.0f)
    {
      playerStatus = playerStatusPlaying;
    }
  }
  //z = y;
  if (playerStatus != playerStatusDead) testSpriteCollisions();
  colliding = 0;
  BaseCreatureEntity::animate(delay);

  if (firingDirection != 5)
    facingDirection = firingDirection;
  firingDirection = 5;

  if (isMoving())
  {
    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 2;
    //if (frame == 3) frame = 1;
    SoundManager::getSoundManager()->playSound(SOUND_STEP);
  }
  else if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
    frame = 3;
  else if (playerStatus == playerStatusDead)
    frame = 0;
  else
    frame = 0; //1;

  if (x < OFFSET_X)
    game().moveToOtherMap(4);
  else if (x > OFFSET_X + MAP_WIDTH * TILE_WIDTH)
    game().moveToOtherMap(6);
  else if (y < OFFSET_Y)
    game().moveToOtherMap(8);
  else if (y > OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT - 5)
    game().moveToOtherMap(2);

  if (playerStatus == playerStatusEntering)
  {
    if (boundingBox.left > OFFSET_X + TILE_WIDTH
        && (boundingBox.left + boundingBox.width) < OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1)
        && boundingBox.top > OFFSET_Y + TILE_HEIGHT
        && (boundingBox.top + boundingBox.height) < OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1))
    {
      playerStatus = playerStatusPlaying;
      game().closeDoors();
    }
  }

  if (playerStatus == playerStatusDead)
  {
    z = OFFSET_Y - 2;
  }
}


void PlayerEntity::render(sf::RenderWindow* app)
{

  sprite.setPosition(x, y);

/*
  int spriteDx = 0;
  if (facingDirection == 8) spriteDx = 3;
  if (facingDirection == 4) spriteDx = 6;
  if (facingDirection == 6) spriteDx = 9;

  // body
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height, width, height));
  app->draw(sprite);

  // head
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, 0, width, height));
  app->draw(sprite);

  // feet
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 2, width, height));
  app->draw(sprite);

  // staff
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 4, width, height));
  app->draw(sprite);

  // hands
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 3, width, height));
  app->draw(sprite);
*/

    if (playerStatus == playerStatusDead)
    {
      // blood
      sprite.setTextureRect(sf::IntRect(6 * width, 0, width, height));
      app->draw(sprite);

      // body
      sprite.setTextureRect(sf::IntRect(3 * width, height, width, height));
      app->draw(sprite);

      // feet
      sprite.setTextureRect(sf::IntRect(3 * width, 2 * height, width, height));
      app->draw(sprite);

      // hand
      sprite.setTextureRect(sf::IntRect(3 * width, 3 * height, width, height));
      app->draw(sprite);
    }
    else
    {
      // shadow
      sprite.setTextureRect(sf::IntRect(7 * width, 0, width, height));
      app->draw(sprite);

      // body
      sprite.setTextureRect(sf::IntRect(frame * width, height, width, height));
      app->draw(sprite);

      // belt
      if (equip[EQUIP_LEATHER_BELT])
      {
        sprite.setTextureRect(sf::IntRect(frame * width, 6 *height, width, height));
        app->draw(sprite);
      }

      // head
      if (playerStatus != playerStatusAcquire && playerStatus != playerStatusUnlocking)
      {
        sprite.setTextureRect(sf::IntRect(0, 0, width, height));
        app->draw(sprite);

        // hat
        if (equip[EQUIP_ENCHANTER_HAT])
        {
          sprite.setTextureRect(sf::IntRect(3 * width, 0, width, height));
          app->draw(sprite);
        }
      }
      // feet
      if( equip[EQUIP_LEATHER_BOOTS])
        sprite.setTextureRect(sf::IntRect((frame + 4) * width, 2 * height, width, height));
      else
        sprite.setTextureRect(sf::IntRect(frame * width, 2 * height, width, height));
      app->draw(sprite);

      // staff
      if ( equip[EQUIP_MAHOGANY_STAFF])
        sprite.setTextureRect(sf::IntRect((frame + 4) * width + 4, 4 * height, width, height));
      else
        sprite.setTextureRect(sf::IntRect(frame * width + 4, 4 * height, width, height));
      app->draw(sprite);

      // snake
      if (equip[EQUIP_BLOOD_SNAKE])
      {
        sprite.setTextureRect(sf::IntRect(frame * width + 4, 7 *height, width, height));
        app->draw(sprite);
      }

      // ice gem
      if (equip[EQUIP_ICE_GEM])
      {
        sprite.setTextureRect(sf::IntRect(frame * width + 4, 8 *height, width, height));
        app->draw(sprite);
      }

      // hands
      if( equip[EQUIP_VIBRATION_GLOVES])
        sprite.setTextureRect(sf::IntRect((frame + 4) * width, 3 * height, width, height));
      else
        sprite.setTextureRect(sf::IntRect(frame * width, 3 * height, width, height));
      app->draw(sprite);

      // head
      if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
      {
        sprite.setTextureRect(sf::IntRect(width, 0, width, height));
        app->draw(sprite);

        // hat
        if (equip[EQUIP_ENCHANTER_HAT])
        {
          sprite.setTextureRect(sf::IntRect(3 * width, 0, width, height));
          app->draw(sprite);
        }

        // staff
        if ( equip[EQUIP_MAHOGANY_STAFF])
          sprite.setTextureRect(sf::IntRect(5 * width + 4, 4 * height, width, height));
        else
          sprite.setTextureRect(sf::IntRect(width + 4, 4 * height, width, height));
        app->draw(sprite);

        // snake
        if (equip[EQUIP_BLOOD_SNAKE])
        {
          sprite.setTextureRect(sf::IntRect(1 * width + 4, 7 *height, width, height));
          app->draw(sprite);
        }

          // ice gem
        if (equip[EQUIP_ICE_GEM])
        {
          sprite.setTextureRect(sf::IntRect(1 * width + 4, 8 *height, width, height));
          app->draw(sprite);
        }
      }

      // necklace
      if (equip[EQUIP_CONCENTRATION_AMULET])
      {
        sprite.setTextureRect(sf::IntRect(frame * width, 5 * height, width, height));
        app->draw(sprite);

        sprite.setColor(sf::Color(255,255,255, (1.0f + sin(age * 5.0f)) * 100));
        sprite.setTextureRect(sf::IntRect(5 * width, 0, width, height));
        app->draw(sprite);
        sprite.setColor(sf::Color(255,255,255,255));
      }
    }
}


void PlayerEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2;
    boundingBox.width = width;
    boundingBox.top = (int)y - height / 2;
    boundingBox.height =  height;

    float fPrez = 10.0f;
    boundingBox.left += fPrez;
    boundingBox.width -= (fPrez + fPrez);
    boundingBox.top += 52.0f;
    boundingBox.height = boundingBox.width - 10.0f;
}

void PlayerEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{

    EnnemyBoltEntity* boltEntity = dynamic_cast<EnnemyBoltEntity*>(entity);

    if (collideWithEntity(entity))
    {
      if (boltEntity != NULL && !boltEntity->getDying())
      {
        boltEntity->collide();
        hurt(boltEntity->getDamages());
        game().generateBlood(x, y, bloodColor);
      }
    }
}

void PlayerEntity::move(int direction)
{
  if (playerStatus == playerStatusPlaying)
  {
    float speedx = 0.0f, speedy = 0.0f;
    if (direction == 1 || direction == 4 || direction == 7)
      speedx = - creatureSpeed;
    else if (direction == 3 || direction == 6 || direction == 9)
      speedx = creatureSpeed;
    if (direction == 1 || direction == 2 || direction == 3)
      speedy = creatureSpeed;
    else if (direction == 7 || direction == 8 || direction == 9)
      speedy = - creatureSpeed;
    setVelocity(Vector2D(speedx, speedy));

    //if (firingDirection != 5)
    //  facingDirection = firingDirection;
    //else
    {
      switch (direction)
      {
      case 8:  facingDirection = 8; break;
      case 2:  facingDirection = 2; break;
      case 4:  facingDirection = 4; break;
      case 6:  facingDirection = 6; break;
      case 7:  if (facingDirection != 4 && facingDirection != 8) facingDirection = 4; break;
      case 1:  if (facingDirection != 4 && facingDirection != 2) facingDirection = 4; break;
      case 9:  if (facingDirection != 6 && facingDirection != 8) facingDirection = 6; break;
      case 3:  if (facingDirection != 6 && facingDirection != 2) facingDirection = 6; break;
      }
    }

    //firingDirection = 5;
  }
}

bool PlayerEntity::isMoving()
{
  if (velocity.x < -1.0f || velocity.x > 1.0f) return true;
  if (velocity.y < -1.0f || velocity.y > 1.0f) return true;
  return false;
}

bool PlayerEntity::isEquiped(int eq)
{
  return equip[eq];
}

void PlayerEntity::setEquiped(int item, bool eq)
{
  equip[item] = eq;
  if (eq && item == (int)EQUIP_FAIRY)
  {
    fairy = new FairyEntity(x, y - 50.0f, this);
  }
   computePlayer();
}

void PlayerEntity::generateBolt(float velx, float vely)
{
  BoltEntity* bolt = new BoltEntity(ImageManager::getImageManager()->getImage(1), x, y + 20, boltLifeTime, boltType);
  bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  bolt->setDamages(fireDamages);
  bolt->setVelocity(Vector2D(velx, vely));
}

void PlayerEntity::fire(int direction)
{
  firingDirection = direction;
  if (equip[EQUIP_FAIRY] && playerStatus != playerStatusDead)
    fairy->fire(direction, map);

  if (canFirePlayer && playerStatus != playerStatusDead)
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);

    if (equip[EQUIP_BOOK_DUAL])
    {
      float shoot_angle = 0.2f;

      if ((direction == 4 && velocity.x < -1.0f) || (direction == 6 && velocity.x > 1.0f)
          || (direction == 8 && velocity.y < -1.0f) || (direction == 2 && velocity.y > 1.0f))
        shoot_angle = 0.1f;
      else if ((direction == 6 && velocity.x < -1.0f) || (direction == 4 && velocity.x > 1.0f)
          || (direction == 2 && velocity.y < -1.0f) || (direction == 8 && velocity.y > 1.0f))
        shoot_angle = 0.35f;

      if (equip[EQUIP_VIBRATION_GLOVES]) shoot_angle += (1000 - rand() % 2000) * 0.0001f;

      switch(direction)
      {
        case 4: generateBolt(-fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
                generateBolt(-fireVelocity * cos(shoot_angle), - fireVelocity * sin(shoot_angle));break;
        case 6: generateBolt(fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
                generateBolt(fireVelocity * cos(shoot_angle), - fireVelocity * sin(shoot_angle));break;
        case 8: generateBolt(fireVelocity * sin(shoot_angle), -fireVelocity * cos(shoot_angle));
                generateBolt(-fireVelocity * sin(shoot_angle), - fireVelocity * cos(shoot_angle));break;
        case 2: generateBolt(fireVelocity * sin(shoot_angle), fireVelocity * cos(shoot_angle));
                generateBolt(-fireVelocity * sin(shoot_angle),  fireVelocity * cos(shoot_angle));break;
      }
    }
    else
    {
      if (equip[EQUIP_VIBRATION_GLOVES])
      {
        float shoot_angle = (1000 - rand() % 2000) * 0.0001f;
        switch(direction)
        {
          case 4: generateBolt(-fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle)); break;
          case 6: generateBolt(fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle)); break;
          case 8: generateBolt(fireVelocity * sin(shoot_angle), -fireVelocity * cos(shoot_angle)); break;
          case 2: generateBolt(fireVelocity * sin(shoot_angle), fireVelocity * cos(shoot_angle)); break;
        }
      }
      else
      {
        switch(direction)
        {
          case 4: generateBolt(-fireVelocity, 0.0f); break;
          case 6: generateBolt(fireVelocity, 0.0f); break;
          case 8: generateBolt(0.0f, -fireVelocity); break;
          case 2: generateBolt(0.0f, fireVelocity); break;
        }
      }
    }

    canFirePlayer = false;
    currentFireDelay = fireDelay;
  }
}

bool PlayerEntity::canFire()
{
  return canFirePlayer;
}

bool PlayerEntity::canMove()
{
  return (playerStatus == playerStatusPlaying);
}

bool PlayerEntity::hurt(int damages)
{
  if (!hurting)
  {
    SoundManager::getSoundManager()->playSound(SOUND_PLAYER_HIT);
    BaseCreatureEntity::hurt(damages, BoltStandard);
    game().generateBlood(x, y, bloodColor);
    game().generateBlood(x, y, bloodColor);
    return true;
  }
  return false;
}

void PlayerEntity::loseItem(enumItemType itemType, bool isEquip)
{
  CollidingSpriteEntity* itemSprite
    = new CollidingSpriteEntity(ImageManager::getImageManager()->getImage(isEquip ? IMAGE_ITEMS_EQUIP : IMAGE_ITEMS), x, y, 32, 32);
  itemSprite->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  itemSprite->setZ(OFFSET_Y - 1);
  itemSprite->setFrame(itemType);
  itemSprite->setImagesProLine(10);
  itemSprite->setType(ENTITY_BLOOD);
  itemSprite->setVelocity(Vector2D(rand()%450));
  itemSprite->setViscosity(0.95f);
  itemSprite->setSpin( (rand() % 700) - 350.0f);
}

void PlayerEntity::dying()
{
  playerStatus = playerStatusDead;
  hp = 0;
  SoundManager::getSoundManager()->playSound(SOUND_PLAYER_DIE);
  setVelocity(Vector2D(0.0f, 0.0f));

  int i;
  for (i = 0; i < gold; i++) loseItem(itemCopperCoin, false);

  for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    if (equip[i]) loseItem(enumItemType(i), true);

  for (i = 0; i < 8; i++) game().generateBlood(x, y, bloodColor);

  CollidingSpriteEntity* itemSprite
    = new CollidingSpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_PLAYER), x, y, 64, 64);
  itemSprite->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  itemSprite->setZ(OFFSET_Y - 1);
  itemSprite->setImagesProLine(10);
  itemSprite->setFrame(/*11*/1);
  itemSprite->setType(ENTITY_BLOOD);
  itemSprite->setVelocity(Vector2D(rand()%450));
  itemSprite->setViscosity(0.95f);
  itemSprite->setSpin( (rand() % 700) - 350.0f);
}

void PlayerEntity::acquireItem(enumItemType type)
{
  if (type >= itemMagicianHat) acquireStance(type);
  else switch (type)
  {
    case itemCopperCoin: gold++;
    SoundManager::getSoundManager()->playSound(SOUND_COIN_PICK_UP);
    break;
    case itemSilverCoin: gold = gold + 5; break;
    case itemGoldCoin: gold = gold + 10; break;
    case itemHealth: hp += 15;
      SoundManager::getSoundManager()->playSound(SOUND_DRINK);
      if (hp > hpMax) hp = hpMax;  break;
    default: break;
  }
}

void PlayerEntity::computePlayer()
{
  float boltLifeTimeBonus = 1.0f;
  float fireDelayBonus = 1.0f;
  float creatureSpeedBonus = 1.0f;
  float fireVelocityBonus = 1.0f;
  float fireDamagesBonus = 1.0f;
  boltType = BoltStandard;

  if (equip[EQUIP_VIBRATION_GLOVES]) fireDelayBonus -= 0.10f;
  if (equip[EQUIP_ENCHANTER_HAT]) fireDelayBonus -= 0.2f;
  if (equip[EQUIP_LEATHER_BELT]) fireDelayBonus -= 0.15f;
  if (equip[EQUIP_LEATHER_BOOTS]) creatureSpeedBonus += 0.25f;
  if (equip[EQUIP_BOOK_DUAL]) fireDelayBonus += 0.6f;
  if (equip[EQUIP_CONCENTRATION_AMULET]) boltLifeTimeBonus += 0.5f;
  if (equip[EQUIP_MAHOGANY_STAFF])
  {
    fireVelocityBonus += 0.15f;
    fireDamagesBonus += 0.5f;
  }
  if (equip[EQUIP_BLOOD_SNAKE]) fireDamagesBonus += 1.0f;
  if (equip[EQUIP_ICE_GEM]) boltType = BoltIce;

  fireDelay = INITIAL_PLAYER_FIRE_DELAY * fireDelayBonus;
  creatureSpeed = INITIAL_PLAYER_SPEED * creatureSpeedBonus;
  fireVelocity = INITIAL_BOLT_VELOCITY * fireVelocityBonus;
  fireDamages = INITIAL_BOLT_DAMAGES * fireDamagesBonus;
  boltLifeTime = INITIAL_BOLT_LIFE * boltLifeTimeBonus;
}

void PlayerEntity::acquireStance(enumItemType type)
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusAcquire;
  acquireDelay = ACQUIRE_DELAY;
  acquiredItem = (enumItemType)(type - itemMagicianHat);
  SoundManager::getSoundManager()->playSound(SOUND_BONUS);
}

void PlayerEntity::collideMapRight()
{
  colliding = 6;
}

void PlayerEntity::collideMapLeft()
{
  colliding = 4;
}

void PlayerEntity::collideMapTop()
{
  colliding = 8;
}

void PlayerEntity::collideMapBottom()
{
  colliding = 2;
}

void PlayerEntity::useBossKey()
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusUnlocking;
  acquireDelay = UNLOCK_DELAY;
  acquiredItem = (enumItemType)(type - itemMagicianHat);
  SoundManager::getSoundManager()->playSound(SOUND_BONUS);
  equip[EQUIP_BOSS_KEY] = false;


  SpriteEntity* spriteItem = new SpriteEntity(
                          ImageManager::getImageManager()->getImage(IMAGE_ITEMS_EQUIP),
                          x, y - 60.0f, ITEM_WIDTH, ITEM_HEIGHT);
          spriteItem->setFrame(EQUIP_BOSS_KEY);
          spriteItem->setZ(z);
          spriteItem->setLifetime(UNLOCK_DELAY);
}
