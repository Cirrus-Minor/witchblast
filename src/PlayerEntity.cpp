#include "PlayerEntity.h"
#include "BoltEntity.h"
#include "EnnemyBoltEntity.h"
#include "ItemEntity.h"
#include "FairyEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextEntity.h"

#include <iostream>
#include <sstream>

PlayerEntity::PlayerEntity(float x, float y)
  : BaseCreatureEntity (ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE), x, y, 80, 128)
{
  currentFireDelay = -1.0f;
  canFirePlayer = true;
  type = ENTITY_PLAYER;

  imagesProLine = 8;
  playerStatus = playerStatusPlaying;
  hp = INITIAL_PLAYER_HP;
  hpDisplay = hp;
  hpMax = hp;
  gold = 0;

  boltLifeTime = INITIAL_BOLT_LIFE;
  specialBoltTimer = -1.0f;
  bloodColor = BloodRed;

  // init the equipment (to empty)
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++) equip[i] = false;
  collidingDirection = 0;

  // init the shots (to none)
  for (int i = 0; i < SPECIAL_SHOT_SLOTS; i++)
  {
    specialShots[i] = ShotTypeStandard;
    specialShotLevel[i] = 0;
  }

  specialShotIndex = 0;
  needInitShotType = false;

  computePlayer();

  firingDirection = 5;
  facingDirection = 2;

  sprite.setOrigin(40, 104);
}

void PlayerEntity::moveTo(float newX, float newY)
{
  float dx = newX - x;
  float dy = newY - y;

  x = newX;
  y = newY;

  for(int unsigned i = 0; i < fairies.size(); i++)
  {
    fairies[i]->setX(fairies[i]->getX() + dx);
    fairies[i]->setY(fairies[i]->getY() + dy);
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

PlayerEntity::playerStatusEnum PlayerEntity::getPlayerStatus()
{
  return playerStatus;
}

void PlayerEntity::setPlayerStatus(PlayerEntity::playerStatusEnum playerStatus)
{
  this->playerStatus = playerStatus;
}

bool PlayerEntity::isDead()
{
  return playerStatus==playerStatusDead;
}

void PlayerEntity::setEntering()
{
  playerStatus = playerStatusEntering;
}

void PlayerEntity::setLeavingLevel()
{
  playerStatus = playerStatusGoingUp;
}

void PlayerEntity::pay(int price)
{
  gold -= price;
  if (gold < 0) gold = 0;
  SoundManager::getSoundManager()->playSound(SOUND_PAY);
}

void PlayerEntity::acquireItemAfterStance()
{
  if (acquiredItem >= FirstEquipItem)
  {
    equip[acquiredItem - FirstEquipItem] = true;

    if (items[acquiredItem].familiar > FamiliarNone)
    {
      setEquiped(acquiredItem - FirstEquipItem, true);
    }

    if (items[acquiredItem].specialShot != (ShotTypeStandard))
      registerSpecialShot(acquiredItem);

    computePlayer();
  }
  else
  {
    if (acquiredItem == ItemBossHeart)
    {
      int hpBonus = 2 + rand() % 4;
      hpMax += hpBonus;
      hp += hpBonus;
      hpDisplay += hpBonus;

      SoundManager::getSoundManager()->playSound(SOUND_EAT);

      std::ostringstream oss;
      oss << "HP Max +" << hpBonus;
      TextEntity* text = new TextEntity(oss.str(), 15, x, y - 50.0f);
      text->setColor(TextEntity::COLOR_FADING_GREEN);
      text->setAlignment(ALIGN_CENTER);
      text->setAge(-1.0f);
      text->setLifetime(1.2f);
      text->setWeight(-60.0f);
      text->setType(ENTITY_FLYING_TEXT);
      text->setZ(2000);
    }
    else if (acquiredItem == ItemBonusHealth)
    {
      hpMax += 1;
      hp += hpMax;

      SoundManager::getSoundManager()->playSound(SOUND_EAT);

      TextEntity* text = new TextEntity("HP Max + 1", 15, x, y - 50.0f);
      text->setColor(TextEntity::COLOR_FADING_GREEN);
      text->setAlignment(ALIGN_CENTER);
      text->setAge(-1.0f);
      text->setLifetime(1.2f);
      text->setWeight(-60.0f);
      text->setType(ENTITY_FLYING_TEXT);
      text->setZ(2000);
    }
  }
  spriteItem->setDying(true);
  spriteItemStar->setDying(true);
  playerStatus = playerStatusPlaying;
}

void PlayerEntity::animate(float delay)
{
  // shot timer
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
      acquireItemAfterStance();
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
  if (playerStatus == playerStatusDead)
    velocity = Vector2D(0.0f, 0.0f);
  else
    testSpriteCollisions();

  collidingDirection = 0;
  BaseCreatureEntity::animate(delay);

  if (firingDirection != 5)
    facingDirection = firingDirection;

  if (isMoving())
  {
    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 1;
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
  else if (y > OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT)
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
  else
  {
    z = y + 13;
  }
}

void PlayerEntity::renderHead(sf::RenderTarget* app)
{
  if (playerStatus != playerStatusDead)
  {
    sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, 0, width, height));
    app->draw(sprite);

    if (equip[EQUIP_ENCHANTER_HAT])
    {
      sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
      if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
        sprite.setTextureRect(sf::IntRect( 0, 0, width, height));
      else
        sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, 0, width, height));
      app->draw(sprite);
      sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
    }
  }
}

void PlayerEntity::renderBody(sf::RenderTarget* app)
{
  if (equip[EQUIP_MAGICIAN_ROBE]) sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height, width, height));
  app->draw(sprite);
  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));

  if (equip[EQUIP_CONCENTRATION_AMULET] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_COLLAR));
    if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
      sprite.setTextureRect(sf::IntRect( 0, 0, width, height));
    else
      sprite.setTextureRect(sf::IntRect( (spriteDx / 3) * width, 0, width, height));

    app->draw(sprite);
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  }

  if (equip[EQUIP_LEATHER_BELT] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
    sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 5, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  }

  if (equip[EQUIP_BROOCH_STAR])
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
    sprite.setTextureRect(sf::IntRect( (14 + spriteDx / 3) * width, 2 * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  }
}

void PlayerEntity::renderHands(sf::RenderTarget* app)
{
  if (equip[EQUIP_VIBRATION_GLOVES] && playerStatus != playerStatusDead)
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));

  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 3, width, height));
  app->draw(sprite);

  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
}

void PlayerEntity::renderFeet(sf::RenderTarget* app)
{
  if (equip[EQUIP_LEATHER_BOOTS] && playerStatus != playerStatusDead)
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));

  sprite.setTextureRect(sf::IntRect( (frame + spriteDx) * width, height * 2, width, height));
  app->draw(sprite);

  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
}

void PlayerEntity::renderStaff(sf::RenderTarget* app)
{
  if (playerStatus == playerStatusDead) return;

  if (equip[EQUIP_MAHOGANY_STAFF]) sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
  int xSnake, ySnake;
  int xStone, yStone;

  if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
  {
    sprite.setTextureRect(sf::IntRect( 12 * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = 13 * width;
    ySnake = height * 4;
    xStone = 13 * width;
    yStone = height * 4;
  }
  else if (isMoving() || firingDirection != 5)
  {
    sprite.setTextureRect(sf::IntRect( (frame / 3 + spriteDx) * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = (frame / 3 + spriteDx + 1) * width;
    ySnake = height * 4;
    xStone = (frame / 3 + spriteDx + 1) * width;
    yStone = height * 4;
  }
  else
  {
    sprite.setTextureRect(sf::IntRect( (spriteDx / 3 + 14) * width, height * 4, width, height));
    app->draw(sprite);
    xSnake = (spriteDx / 3 + 14) * width;
    ySnake = 0;
    xStone = (spriteDx / 3 + 14) * width;
    yStone = height * 2;
  }

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_EQUIP));
    sprite.setTextureRect(sf::IntRect( xSnake, ySnake, width, height));
    app->draw(sprite);
  }
  sprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_PLAYER_BASE));
  if (getShotType() != ShotTypeStandard)
  {
    sf::Color savedColor = sprite.getColor();
    switch (getShotType())
    {
    case ShotTypeIce:
      sprite.setColor(sf::Color(100, 220, 255, 255));
      break;
    case ShotTypeStone:
      sprite.setColor(sf::Color(120, 120, 150, 255));
      break;
    case ShotTypeLightning:
      sprite.setColor(sf::Color(255, 255, 0, 255));
      break;

    case ShotTypeIllusion:
      sprite.setColor(sf::Color(240, 180, 250, 255));
      break;

    case ShotTypeStandard:
      sprite.setColor(sf::Color(255, 255, 255, 0));
      break;
    }

    sprite.setTextureRect(sf::IntRect( xStone, yStone, width, height));
    app->draw(sprite);
    sprite.setColor(savedColor);
  }
}

void PlayerEntity::render(sf::RenderTarget* app)
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

  // gems
  if ((getShotType() == ShotTypeIce || getShotType() == ShotTypeLightning)  && playerStatus != playerStatusDead)
  {
    int fade;
    sf::Color savedColor = sprite.getColor();
    if (getShotType() != ShotTypeIce || specialBoltTimer <= 0.0f) fade = 255;
    else fade = ((STATUS_FROZEN_BOLT_DELAY[getShotLevel()] - specialBoltTimer) / STATUS_FROZEN_BOLT_DELAY[getShotLevel()]) * 128;

    if (getShotType() == ShotTypeLightning)
      fade = 150 + rand() % 105;

    if (getShotType() == ShotTypeIce)
      sprite.setTextureRect(sf::IntRect(320, 0, 20, 20));
    else if (getShotType() == ShotTypeLightning)
      sprite.setTextureRect(sf::IntRect(340, 0, 20, 20));
    sprite.setColor(sf::Color(255, 255, 255, fade));

    if (isMoving() || firingDirection != 5)
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
    sprite.setColor(savedColor);
  }

  if (game().getShowLogical() && playerStatus != playerStatusDead)
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}


void PlayerEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2;
  boundingBox.width = width;
  boundingBox.top = (int)y - height / 2;
  boundingBox.height =  height;

  boundingBox.left += 25;
  boundingBox.width -= 50;
  boundingBox.top += 40.0f;
  boundingBox.height = 38.0;
}

void PlayerEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (playerStatus == playerStatusDead) return;

  EnnemyBoltEntity* boltEntity = dynamic_cast<EnnemyBoltEntity*>(entity);

  if (collideWithEntity(entity))
  {
    if (boltEntity != NULL && !boltEntity->getDying())
    {
      boltEntity->collide();
      hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel());
      game().generateBlood(x, y, bloodColor);

      float xs = (x + boltEntity->getX()) / 2;
      float ys = (y + boltEntity->getY()) / 2;
      SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
      star->setFading(true);
      star->setZ(y+ 100);
      star->setLifetime(0.7f);
      star->setType(ENTITY_EFFECT);
      star->setSpin(400.0f);
    }
  }
}

void PlayerEntity::move(int direction)
{
  if (playerStatus == playerStatusAcquire && acquireDelay < ACQUIRE_DELAY / 2)
  {
    acquireItemAfterStance();
  }
  if (playerStatus == playerStatusPlaying)
  {
    float speedx = 0.0f, speedy = 0.0f;
    if (direction == 4)
      speedx = - creatureSpeed;
    else if (direction == 1 || direction == 7)
      speedx = - creatureSpeed * 0.7f;
    else if (direction == 6)
      speedx = creatureSpeed;
    else if (direction == 3 || direction == 9)
      speedx = creatureSpeed * 0.7f;

    if (direction == 2)
      speedy = creatureSpeed;
    else if (direction == 1 || direction == 3)
      speedy = creatureSpeed * 0.7f;
    else if (direction == 8)
      speedy = - creatureSpeed;
    else if (direction == 7 || direction == 9)
      speedy = - creatureSpeed * 0.7f;

    setVelocity(Vector2D(speedx, speedy));

    {
      switch (direction)
      {
      case 8:
        facingDirection = 8;
        break;
      case 2:
        facingDirection = 2;
        break;
      case 4:
        facingDirection = 4;
        break;
      case 6:
        facingDirection = 6;
        break;
      case 7:
        if (facingDirection != 4 && facingDirection != 8) facingDirection = 4;
        break;
      case 1:
        if (facingDirection != 4 && facingDirection != 2) facingDirection = 4;
        break;
      case 9:
        if (facingDirection != 6 && facingDirection != 8) facingDirection = 6;
        break;
      case 3:
        if (facingDirection != 6 && facingDirection != 2) facingDirection = 6;
        break;
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

void PlayerEntity::setEquiped(int item, bool toggleEquipped)
{
  equip[item] = toggleEquipped;
  if (toggleEquipped && items[FirstEquipItem + item].familiar > FamiliarNone)
  {
    FairyEntity* fairy = new FairyEntity(x - 50.0f + rand() % 100,
                                         y - 50.0f + rand() % 100,
                                         items[FirstEquipItem + item].familiar);
    fairies.push_back(fairy);
  }
  computePlayer();
}

void PlayerEntity::generateBolt(float velx, float vely)
{
  enumShotType boltType = ShotTypeStandard;
  unsigned int shotLevel = 1;

  switch (getShotType())
  {
  case ShotTypeIce:
    if (getShotType() == ShotTypeIce)
    {
      if (specialBoltTimer <= 0.0f)
      {
        boltType = ShotTypeIce;
        shotLevel = getShotLevel();
        needInitShotType = true;
      }
      else boltType = ShotTypeStandard;
    }
    break;
  case ShotTypeStandard:
  case ShotTypeIllusion:
  case ShotTypeStone:
  case ShotTypeLightning:
    boltType = getShotType();
    shotLevel = getShotLevel();
    break;
  }

  BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, boltType, shotLevel);
  bolt->setDamages(fireDamages);
  bolt->setVelocity(Vector2D(velx, vely));
}

void PlayerEntity::rageFire()
{
  for (int i = -1; i <= 1; i += 2)
    for (int j = -1; j <= 1; j += 2)
    {
      BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, ShotTypeFire, 0);
      bolt->setDamages(10);
      float velx = fireVelocity * i * 0.42f;
      float vely = fireVelocity * j * 0.42f;
      bolt->setVelocity(Vector2D(velx, vely));

      if (hp <= hpMax / 5)
      {
        BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, ShotTypeFire, 0);
        bolt->setDamages(10);
        float velx = 0.0f;
        float vely = 0.0f;
        if (i == -1 && j == -1) velx = -fireVelocity * i * 0.6f;
        else if (i == -1 && j == 1) velx = fireVelocity * i * 0.6f;
        else if (i == 1 && j == -1) vely= -fireVelocity * i * 0.6f;
        else if (i == 1 && j == 1) vely = fireVelocity * i * 0.6f;
        bolt->setVelocity(Vector2D(velx, vely));
      }
    }
  SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);
}

void PlayerEntity::resestFireDirection()
{
  firingDirection = 5;
}

int PlayerEntity::getFireDirection()
{
  return firingDirection;
}

void PlayerEntity::fire(int direction)
{
  firingDirection = direction;

  if (playerStatus != playerStatusDead)
    for(int unsigned i = 0; i < fairies.size(); i++)
      fairies[i]->fire(direction);

  if (canFirePlayer && playerStatus != playerStatusDead && playerStatus != playerStatusAcquire)
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_STANDARD);

    if (equip[EQUIP_BOOK_DUAL] || equip[EQUIP_BOOK_TRIPLE])
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
      case 4:
        generateBolt(-fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
        generateBolt(-fireVelocity * cos(shoot_angle), - fireVelocity * sin(shoot_angle));
        break;
      case 6:
        generateBolt(fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
        generateBolt(fireVelocity * cos(shoot_angle), - fireVelocity * sin(shoot_angle));
        break;
      case 8:
        generateBolt(fireVelocity * sin(shoot_angle), -fireVelocity * cos(shoot_angle));
        generateBolt(-fireVelocity * sin(shoot_angle), - fireVelocity * cos(shoot_angle));
        break;
      case 2:
        generateBolt(fireVelocity * sin(shoot_angle), fireVelocity * cos(shoot_angle));
        generateBolt(-fireVelocity * sin(shoot_angle),  fireVelocity * cos(shoot_angle));
        break;
      }
    }
    if (!equip[EQUIP_BOOK_DUAL] || equip[EQUIP_BOOK_TRIPLE])
    {
      if (equip[EQUIP_VIBRATION_GLOVES])
      {
        float shoot_angle = (1000 - rand() % 2000) * 0.0001f;
        switch(direction)
        {
        case 4:
          generateBolt(-fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
          break;
        case 6:
          generateBolt(fireVelocity * cos(shoot_angle), fireVelocity * sin(shoot_angle));
          break;
        case 8:
          generateBolt(fireVelocity * sin(shoot_angle), -fireVelocity * cos(shoot_angle));
          break;
        case 2:
          generateBolt(fireVelocity * sin(shoot_angle), fireVelocity * cos(shoot_angle));
          break;
        }
      }
      else
      {
        switch(direction)
        {
        case 4:
          generateBolt(-fireVelocity, 0.0f);
          break;
        case 6:
          generateBolt(fireVelocity, 0.0f);
          break;
        case 8:
          generateBolt(0.0f, -fireVelocity);
          break;
        case 2:
          generateBolt(0.0f, fireVelocity);
          break;
        }
      }
    }

    canFirePlayer = false;
    currentFireDelay = fireDelay;
    if (needInitShotType) initShotType();
  }
}

bool PlayerEntity::canMove()
{
  return (playerStatus == playerStatusPlaying
          || (playerStatus == playerStatusAcquire && acquireDelay < ACQUIRE_DELAY / 2));
}

bool PlayerEntity::hurt(int damages, enumShotType hurtingType, int level)
{
  if (!hurting)
  {
    SoundManager::getSoundManager()->playSound(SOUND_PLAYER_HIT);
    BaseCreatureEntity::hurt(damages, hurtingType, level);
    hurtingDelay = HURTING_DELAY * 2.0f;
    game().generateBlood(x, y, bloodColor);
    game().generateBlood(x, y, bloodColor);

    if (equip[EQUIP_CONCENTRATION_AMULET]) rageFire();

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
  for (i = 0; i < gold && i < 10; i++) loseItem(ItemCopperCoin, false);

  for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    if (equip[i])
    {
      if (items[i + FirstEquipItem].familiar == FamiliarNone
          && i + FirstEquipItem != ItemMagicianRobe)
        loseItem(enumItemType(i), true);
    }
  }


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
  if (items[type].generatesStance) acquireStance(type);
  else switch (type)
    {
    case ItemCopperCoin:
      gold++;
      SoundManager::getSoundManager()->playSound(SOUND_COIN_PICK_UP);
      break;
    case ItemSilverCoin:
      gold = gold + 5;
      SoundManager::getSoundManager()->playSound(SOUND_COIN_PICK_UP);
      break;
    case ItemGoldCoin:
      gold = gold + 10;
      SoundManager::getSoundManager()->playSound(SOUND_COIN_PICK_UP);
      break;
    case ItemHealthVerySmall:
      hp += 3;
      SoundManager::getSoundManager()->playSound(SOUND_EAT);
      if (hp > hpMax) hp = hpMax;
      break;
    case ItemHealthSmall:
      hp += 7;
      SoundManager::getSoundManager()->playSound(SOUND_EAT);
      if (hp > hpMax) hp = hpMax;
      break;
    case ItemHealth:
      hp += 15;
      SoundManager::getSoundManager()->playSound(SOUND_EAT);
      if (hp > hpMax) hp = hpMax;
      break;
    default:
      break;
    }
}

void PlayerEntity::computePlayer()
{
  float boltLifeTimeBonus = 1.0f;
  float fireDelayBonus = 1.0f;
  float creatureSpeedBonus = 1.0f;
  float fireVelocityBonus = 1.0f;
  float fireDamagesBonus = 1.0f;
  armor = 0.0f;

  if (equip[EQUIP_VIBRATION_GLOVES]) fireDelayBonus -= 0.10f;
  if (equip[EQUIP_ENCHANTER_HAT]) fireDelayBonus -= 0.2f;
  if (equip[EQUIP_LEATHER_BELT]) fireDelayBonus -= 0.15f;
  if (equip[EQUIP_LEATHER_BOOTS]) creatureSpeedBonus += 0.15f;
  if (equip[EQUIP_BOOK_TRIPLE]) fireDelayBonus += 0.7f;
  else if (equip[EQUIP_BOOK_DUAL]) fireDelayBonus += 0.5f;
  if (equip[EQUIP_BROOCH_STAR]) boltLifeTimeBonus += 0.4f;
  if (equip[EQUIP_MAHOGANY_STAFF])
  {
    fireVelocityBonus += 0.15f;
    fireDamagesBonus += 0.5f;
  }
  if (equip[EQUIP_BLOOD_SNAKE]) fireDamagesBonus += 0.5f;
  if (equip[EQUIP_MAGICIAN_ROBE]) armor += 0.15f;

  fireDelay = INITIAL_PLAYER_FIRE_DELAY * fireDelayBonus;
  creatureSpeed = INITIAL_PLAYER_SPEED * creatureSpeedBonus;
  fireVelocity = INITIAL_BOLT_VELOCITY * fireVelocityBonus;
  fireDamages = INITIAL_BOLT_DAMAGES * fireDamagesBonus;
  boltLifeTime = INITIAL_BOLT_LIFE * boltLifeTimeBonus;

  // gems
  for (int i = 1; i < SPECIAL_SHOT_SLOTS; i++)
  {
    specialShotLevel[i] = 0;
    if (specialShots[i] == ShotTypeIce && equip[EQUIP_RING_ICE])
      specialShotLevel[i]++;
    if (specialShots[i] == ShotTypeStone && equip[EQUIP_RING_STONE])
      specialShotLevel[i]++;
    if (specialShots[i] == ShotTypeLightning && equip[EQUIP_RING_LIGHTNING])
      specialShotLevel[i]++;
    if (specialShots[i] == ShotTypeIllusion && equip[EQUIP_RING_ILLUSION])
      specialShotLevel[i]++;
  }
  if (getShotType() == ShotTypeIllusion) fireDamages *= ILLUSION_DAMAGES_DECREASE[getShotLevel()];
}

void PlayerEntity::acquireStance(enumItemType type)
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusAcquire;
  acquireDelay = ACQUIRE_DELAY;
  acquiredItem = (enumItemType)(type);
  SoundManager::getSoundManager()->playSound(SOUND_BONUS);

  game().showArtefactDescription(type);

  enumItemType itemFrame = type;
  int itemImage = IMAGE_ITEMS;
  if (itemFrame >= FirstEquipItem)
  {
    itemFrame = (enumItemType)(itemFrame - FirstEquipItem);
    itemImage = IMAGE_ITEMS_EQUIP;
  }

  spriteItem = new SpriteEntity(
    ImageManager::getImageManager()->getImage(itemImage),
    x, y - 100.0f, ITEM_WIDTH, ITEM_HEIGHT);
  spriteItem->setFrame((int)itemFrame);
  spriteItem->setImagesProLine(10);
  spriteItem->setZ(z);
  spriteItem->setLifetime(ACQUIRE_DELAY);

  spriteItemStar = new SpriteEntity(
    ImageManager::getImageManager()->getImage(IMAGE_STAR),
    x, y - 100.0f);
  spriteItemStar->setScale(4.0f, 4.0f);
  spriteItemStar->setZ(z-1.0f);
  spriteItemStar->setLifetime(ACQUIRE_DELAY);
  spriteItemStar->setSpin(50.0f);
}

void PlayerEntity::collideMapRight()
{
  collidingDirection = 6;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x * 0.7f;
}

void PlayerEntity::collideMapLeft()
{
  collidingDirection = 4;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x * 0.7f;
}

void PlayerEntity::collideMapTop()
{
  collidingDirection = 8;
  if (recoil.active) recoil.velocity.y= -recoil.velocity.y * 0.7f;
}

void PlayerEntity::collideMapBottom()
{
  collidingDirection = 2;
  if (recoil.active) recoil.velocity.y= -recoil.velocity.y * 0.7f;
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
    x, y - 100.0f, ITEM_WIDTH, ITEM_HEIGHT);
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

  SoundManager::getSoundManager()->playSound(SOUND_SHOT_SELECT);
  computePlayer();
}

void PlayerEntity::initShotType()
{
  specialBoltTimer = STATUS_FROZEN_BOLT_DELAY[getShotLevel()];
  needInitShotType = false;

  if (getShotType() == ShotTypeLightning)
    SoundManager::getSoundManager()->playSound(SOUND_ELECTRIC_CHARGE);
}

unsigned int PlayerEntity::getShotLevel()
{
  return specialShotLevel[specialShotIndex];
}

unsigned int PlayerEntity::getShotLevel(int index)
{
  return specialShotLevel[index];
}

int PlayerEntity::getFairieNumber()
{
  return fairies.size();
}

FairyEntity* PlayerEntity::getFairy(unsigned int n)
{
  if (n < fairies.size())
    return fairies[n];
  else
    return NULL;
}
