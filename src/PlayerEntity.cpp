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

PlayerEntity::PlayerEntity(float x = 0.0f, float y = 0.0f)
      : BaseCreatureEntity (ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE), x, y, 80, 128)
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
  specialBoltTimer = -1.0f;
  bloodColor = bloodRed;

  // init the equipment (to empty)
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++) equip[i] = false;
  collidingDirection = 0;

  // init the shots (to none)
  for (int i = 0; i < SPECIAL_SHOT_SLOTS; i++) specialShots[i] = ShotTypeStandard;
  specialShotIndex = 0;

  computePlayer();

  firingDirection = 5;
  facingDirection = 2;

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

int PlayerEntity::getFacingDirection()
{
  return facingDirection;
}

float PlayerEntity::getPercentFireDelay()
{
  if (canFirePlayer) return 1.0f;

  else return (1.0f - currentFireDelay / fireDelay);
}

int PlayerEntity::getCollidingDirection()
{
  return collidingDirection;
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
  if (specialBoltTimer >= 0.0f)
  {
    specialBoltTimer -= delay;
    if (specialBoltTimer <= 0.0f)
    {
      if (getShotType() == ShotTypeIce) SoundManager::getSoundManager()->playSound(SOUND_ICE_CHARGE);
    }
  }
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
      playerStatus = playerStatusPlaying;

      int itemID = acquiredItem + FirstEquipItem;

      if (acquiredItem == (int)EQUIP_FAIRY)
        fairy = new FairyEntity(x, y - 50.0f, this);

      if (items[itemID].specialShot != (ShotTypeStandard))
        registerSpecialShot(itemID);

      computePlayer();
    }
  }
  // unlocking animation
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
  collidingDirection = 0;
  BaseCreatureEntity::animate(delay);

  if (firingDirection != 5)
    facingDirection = firingDirection;
  firingDirection = 5;

  if (isMoving())
  {
    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 1;
    SoundManager::getSoundManager()->playSound(SOUND_STEP);
  }
  else if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
    frame = 0;
  else if (playerStatus == playerStatusDead)
    frame = 0;
  else // standing
  {
    frame = 1;
  }

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

void PlayerEntity::renderHead(sf::RenderWindow* app)
{
  if (playerStatus != playerStatusDead)
  {
    sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, 0, width, height));
    app->draw(sprite);

    if (equip[EQUIP_ENCHANTER_HAT])
    {
      sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
      sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, 0, width, height));
      app->draw(sprite);
      sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
    }
  }
}

void PlayerEntity::renderBody(sf::RenderWindow* app)
{
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_CONCENTRATION_AMULET])
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_COLLAR));
    sprite.setTextureRect(sf::IntRect( (spriteDx / 3) * width, 0, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  }

  if (equip[EQUIP_LEATHER_BELT])
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
    sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  }
}

void PlayerEntity::renderHands(sf::RenderWindow* app)
{
  if (equip[EQUIP_VIBRATION_GLOVES]) sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));

  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 3, width, height));
  app->draw(sprite);

  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
}

void PlayerEntity::renderFeet(sf::RenderWindow* app)
{
  if (equip[EQUIP_LEATHER_BOOTS]) sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));

  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 2, width, height));
  app->draw(sprite);

  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
}

void PlayerEntity::renderStaff(sf::RenderWindow* app)
{
  if (equip[EQUIP_MAHOGANY_STAFF]) sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
  int xSnake, ySnake;

  if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
  {
    sprite.setTextureRect(sf::IntRect( 12 * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = 13 * width;
    ySnake = height * 4;
  }
  else if (isMoving() || firingDirection != 5)
  {
    sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = (frame / 3 + spriteDx + 1) * width;
    ySnake = height * 4;
  }
  else
  {
    sprite.setTextureRect(sf::IntRect( (spriteDx / 3 + 14) * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = (spriteDx / 3 + 14) * width;
    ySnake = 0;
  }

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
    sprite.setTextureRect(sf::IntRect( xSnake, ySnake, width, height));
    app->draw(sprite);
  }
  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
}


void PlayerEntity::render(sf::RenderWindow* app)
{
  sprite.setPosition(x, y);

  spriteDx = 0;
  if (facingDirection == 8) spriteDx = 3;
  if (facingDirection == 4) spriteDx = 6;
  if (facingDirection == 6) spriteDx = 9;
  if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking) spriteDx = 12;

  if (playerStatus == playerStatusDead)
  {
    spriteDx = 13;
    // blood
    sprite.setTextureRect(sf::IntRect( width, 0, width, height));
    app->draw(sprite);
  }
  else
  {
    // shadow
    sprite.setTextureRect(sf::IntRect( 2 * width, 0, width, height));
    app->draw(sprite);
  }

  if (facingDirection == 8 || facingDirection == 4)
  {
    renderStaff(app);
    renderHead(app);
    renderBody(app);
    renderFeet(app);
    renderHands(app);
  }
  else
  {
    renderBody(app);
    renderHead(app);
    renderFeet(app);
    renderStaff(app);
    renderHands(app);
  }

  // ice gem
  if (getShotType() == ShotTypeIce)
  {
    int fade;
    if (specialBoltTimer <= 0.0f) fade = 255;
    else fade = ((STATUS_FROZEN_BOLT_DELAY - specialBoltTimer) / STATUS_FROZEN_BOLT_DELAY) * 128;

    sprite.setTextureRect(sf::IntRect(320, 0, 20, 20));
    sprite.setColor(sf::Color(255, 255, 255, fade));

    if (isMoving())
    {
      if (facingDirection == 2 )
        sprite.setPosition(x + 16, y + 36);
      else if (facingDirection == 8 )
        sprite.setPosition(x + 43, y + 36);
      else if (facingDirection == 4 )
        sprite.setPosition(x - 3 , y + 41);
      else
        sprite.setPosition(x + 63, y + 41);
    }
    else if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
    {
      sprite.setPosition(x + 57, y + 18);
    }
    else
    {
      if (facingDirection == 2 )
        sprite.setPosition(x + 16, y + 36);
      else if (facingDirection == 8 )
        sprite.setPosition(x + 43, y + 29);
      else if (facingDirection == 4 )
        sprite.setPosition(x + 13, y + 35);
      else
        sprite.setPosition(x + 46, y + 35);
    }

    sf::RenderStates r;
    r.blendMode = sf::BlendAdd;
    app->draw(sprite, r);

    sprite.setPosition(x, y);
    sprite.setColor(sf::Color(255, 255, 255, 255));
  }
}


void PlayerEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2;
    boundingBox.width = width;
    boundingBox.top = (int)y - height / 2;
    boundingBox.height =  height;

    float fPrez = 10.0f;
    boundingBox.left += 25;
    boundingBox.width -= 50;
    boundingBox.top += 80.0f;
    boundingBox.height = boundingBox.height - 90.0f;
}

void PlayerEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{

    EnnemyBoltEntity* boltEntity = dynamic_cast<EnnemyBoltEntity*>(entity);

    if (collideWithEntity(entity))
    {
      if (boltEntity != NULL && !boltEntity->getDying())
      {
        boltEntity->collide();
        hurt(boltEntity->getDamages(), ShotTypeStandard);
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
  enumShotType boltType;

  switch (getShotType())
  {
  case ShotTypeIce:
      if (getShotType() == ShotTypeIce)
      {
        if (specialBoltTimer <= 0.0f)
        {
          boltType = ShotTypeIce;
          initShotType();
        }
        else boltType = ShotTypeStandard;
      }
      break;
  case ShotTypeStandard:
  case ShotTypeIllusion:
    boltType = getShotType(); break;
  }


  BoltEntity* bolt = new BoltEntity(ImageManager::getImageManager()->getImage(1), x, y + 30, boltLifeTime, boltType);
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

bool PlayerEntity::canMove()
{
  return (playerStatus == playerStatusPlaying);
}

bool PlayerEntity::hurt(int damages, enumShotType hurtingType)
{
  if (!hurting)
  {
    SoundManager::getSoundManager()->playSound(SOUND_PLAYER_HIT);
    BaseCreatureEntity::hurt(damages, ShotTypeStandard);
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
  for (i = 0; i < gold; i++) loseItem(ItemCopperCoin, false);

  for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    if (equip[i]) loseItem(enumItemType(i), true);

  for (i = 0; i < 8; i++) game().generateBlood(x, y, bloodColor);

  CollidingSpriteEntity* itemSprite
    = new CollidingSpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE), x, y, 80, 120);
  itemSprite->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  itemSprite->setZ(OFFSET_Y - 1);
  itemSprite->setImagesProLine(14);
  itemSprite->setFrame(13);
  itemSprite->setType(ENTITY_BLOOD);
  itemSprite->setVelocity(Vector2D(rand()%450));
  itemSprite->setViscosity(0.95f);
  itemSprite->setSpin( (rand() % 700) - 350.0f);
}

void PlayerEntity::acquireItem(enumItemType type)
{
  if (type >= FirstEquipItem) acquireStance(type);
  else switch (type)
  {
    case ItemCopperCoin: gold++;
    SoundManager::getSoundManager()->playSound(SOUND_COIN_PICK_UP);
    break;
    case ItemSilverCoin: gold = gold + 5; break;
    case ItemGoldCoin: gold = gold + 10; break;
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
  //if (getShotType()) boltType = BoltIce;

  fireDelay = INITIAL_PLAYER_FIRE_DELAY * fireDelayBonus;
  creatureSpeed = INITIAL_PLAYER_SPEED * creatureSpeedBonus;
  fireVelocity = INITIAL_BOLT_VELOCITY * fireVelocityBonus;
  fireDamages = INITIAL_BOLT_DAMAGES * fireDamagesBonus;
  boltLifeTime = INITIAL_BOLT_LIFE * boltLifeTimeBonus;

  // gems
  if (getShotType() == ShotTypeIllusion) fireDamages *= 0.8f;
}

void PlayerEntity::acquireStance(enumItemType type)
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusAcquire;
  acquireDelay = ACQUIRE_DELAY;
  acquiredItem = (enumItemType)(type - FirstEquipItem);
  SoundManager::getSoundManager()->playSound(SOUND_BONUS);
}

void PlayerEntity::collideMapRight()
{
  collidingDirection = 6;
}

void PlayerEntity::collideMapLeft()
{
  collidingDirection = 4;
}

void PlayerEntity::collideMapTop()
{
  collidingDirection = 8;
}

void PlayerEntity::collideMapBottom()
{
  collidingDirection = 2;
}

void PlayerEntity::useBossKey()
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusUnlocking;
  acquireDelay = UNLOCK_DELAY;
  acquiredItem = (enumItemType)(type - FirstEquipItem);
  SoundManager::getSoundManager()->playSound(SOUND_BONUS);
  equip[EQUIP_BOSS_KEY] = false;


  SpriteEntity* spriteItem = new SpriteEntity(
                          ImageManager::getImageManager()->getImage(IMAGE_ITEMS_EQUIP),
                          x, y - 60.0f, ITEM_WIDTH, ITEM_HEIGHT);
  spriteItem->setFrame(EQUIP_BOSS_KEY);
  spriteItem->setZ(z);
  spriteItem->setLifetime(UNLOCK_DELAY);
}

enumShotType PlayerEntity::getShotType()
{
  return specialShots[specialShotIndex];
}

int PlayerEntity::getShotIndex()
{
  return specialShotIndex;
}

void PlayerEntity::setShotIndex(int index)
{
  specialShotIndex = index;
}

enumShotType PlayerEntity::getShotType(int slot)
{
  return specialShots[slot];
}

void PlayerEntity::setShotType(int slot, enumShotType shotType)
{
  specialShots[slot] = shotType;
}

void PlayerEntity::registerSpecialShot(int item)
{
  bool found = false;
  int index = 1;
  while (index < SPECIAL_SHOT_SLOTS && !found)
  {
    found = specialShots[index] == ShotTypeStandard;
    if (!found) index++;
  }
  if (found)
  {
    this->specialShots[index] = items[item].specialShot;
    specialShotIndex = index;
    initShotType();
  }
}

void PlayerEntity::selectNextShotType()
{
  int index = specialShotIndex + 1;
  bool found = false;
  while (index < SPECIAL_SHOT_SLOTS && !found)
  {
    if (specialShots[index] == ShotTypeStandard) index++;
    else found = true;
  }
  if (found)
  {
    specialShotIndex = index;
    initShotType();
  }
  else
    specialShotIndex = 0;

  computePlayer();
}

void PlayerEntity::initShotType()
{
  specialBoltTimer = STATUS_FROZEN_BOLT_DELAY;
}
