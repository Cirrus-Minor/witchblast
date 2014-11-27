#include "PlayerEntity.h"
#include "SlimeEntity.h"
#include "FallingRockEntity.h"
#include "BoltEntity.h"
#include "SpiderWebEntity.h"
#include "EnemyBoltEntity.h"
#include "ItemEntity.h"
#include "FairyEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextEntity.h"

#include <iostream>
#include <sstream>

const int xHalo[10][3] =
{
  { 6, 6, 6},
  { 34, 34, 32},
  { 34, 36, 37},
  { 5, 5, 5},
  { 14, 14, 14},
  { 32, 34, 37},
  { 27, 26, 6},
  { 14, 14, 14},
  { 32, 34, 37},
  { 27, 26, 6}
};

const int yHalo[10][3] =
{
  { 13, 13, 19},
  { 12, 12, 12},
  { 9, 10, 12},
  { 13, 8, 6},
  { 6, 8, 10},
  { 4, 7, 10},
  { 4, 10, 14},
  { 6, 8, 10},
  { 4, 7, 10},
  { 4, 10, 14}
};

PlayerEntity::PlayerEntity(float x, float y)
  : BaseCreatureEntity (ImageManager::getInstance().getImage(IMAGE_PLAYER_BASE), x, y, 42, 80)
{
  currentFireDelay = -1.0f;
  randomFireDelay = -1.0f;
  invincibleDelay = -1.0f;
  canFirePlayer = true;
  type = ENTITY_PLAYER;

  imagesProLine = 8;
  playerStatus = playerStatusPlaying;
  hp = INITIAL_PLAYER_HP;

#ifdef TEST_MODE
  hp = INITIAL_PLAYER_HP * 100;
#endif // TEST_MODE

  hpDisplay = hp;
  hpMax = hp;
  gold = 0;
  deathAge = -1.0f;
  idleAge = 0.0f;

  boltLifeTime = INITIAL_BOLT_LIFE;
  specialBoltTimer = -1.0f;
  bloodColor = BloodRed;
  canExplode = false;

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

  sprite.setOrigin(21, 60);

  protection.active = false;

  activeSpell.delay = -1.0f;
  activeSpell.spell = SpellNone;
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

float PlayerEntity::getPercentSpellDelay()
{
  if (activeSpell.spell == SpellNone)
    return getPercentFireDelay();
  else
  {
    if (activeSpell.delay <= 0.0f) return 1.0f;
    else return (1.0f - activeSpell.delay / activeSpell.delayMax);
  }
}

bool PlayerEntity::isPoisoned()
{
  return (specialState[SpecialStatePoison].active);
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
  return playerStatus == playerStatusDead;
}

enemyTypeEnum PlayerEntity::getLastHurtingEnemy()
{
  return lastHurtingEnemy;
}

sourceTypeEnum PlayerEntity::getLastHurtingSource()
{
  return lastHurtingSource;
}

float PlayerEntity::getDeathAge()
{
  return deathAge;
}

void PlayerEntity::setDeathAge(float deathAge)
{
  this->deathAge = deathAge;
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
  SoundManager::getInstance().playSound(SOUND_PAY);
}

void PlayerEntity::acquireItemAfterStance()
{
  if (acquiredItem >= FirstEquipItem)
  {
    equip[acquiredItem - FirstEquipItem] = true;

    if (acquiredItem != ItemBossKey) game().proceedEvent(EventGetItem);

    // familiar
    if (items[acquiredItem].familiar != FamiliarNone)
    {
      setEquiped(acquiredItem - FirstEquipItem, true);
      game().proceedEvent(EventGetFamiliar);
    }

    // shot types
    if (items[acquiredItem].specialShot != (ShotTypeStandard))
    {
      registerSpecialShot(acquiredItem);
      game().proceedEvent(EventGetSpecialShot);
    }

    // spells
    if (items[acquiredItem].spell != SpellNone)
    {
      setActiveSpell(items[acquiredItem].spell, false);
      game().proceedEvent(EventGetSpell);
    }

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

      SoundManager::getInstance().playSound(SOUND_EAT);

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
      hp = hpMax;

      SoundManager::getInstance().playSound(SOUND_EAT);

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
      if (getShotType() == ShotTypeIce) SoundManager::getInstance().playSound(SOUND_ICE_CHARGE);
    }
  }
  // rate of fire
  if (!canFirePlayer)
  {
    currentFireDelay -= delay;
    canFirePlayer = (currentFireDelay <= 0.0f);
  }
  if (randomFireDelay >= 0.0f) randomFireDelay -= delay;
  // spells
  if (activeSpell.spell != SpellNone && activeSpell.delay > 0.0f)
  {
    if (game().getCurrentMap()->isCleared())
      activeSpell.delay -= 40 * delay;
    else
      activeSpell.delay -= delay;

    if (activeSpell.spell == SpellProtection && protection.active)
      activeSpell.delay = activeSpell.delayMax;

    if (activeSpell.delay <= 0.0f) SoundManager::getInstance().playSound(SOUND_SPELL_CHARGE);
  }
  // protection
  if (protection.active)
  {
    protection.timer -= delay;
    if (protection.timer <= 0.0f)
    {
      protection.active = false;
      computePlayer();
    }
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
  {
    deathAge += delay;
    velocity = Vector2D(0.0f, 0.0f);
  }
  else
    testSpriteCollisions();

  collidingDirection = 0;
  BaseCreatureEntity::animate(delay);

  if (firingDirection != 5)
    facingDirection = firingDirection;

  if (isMoving() || firingDirection != 5)
  {
    frame = ((int)(age * 7.0f)) % 4;
    if (frame == 3) frame = 1;
  }
  else if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
    frame = 3;
  else if (playerStatus == playerStatusDead)
    frame = 0;
  else // standing
  {
    frame = 1;
  }

  if (playerStatus == playerStatusDead || isMoving() || firingDirection != 5)
    idleAge = 0.0f;
  else
    idleAge += delay;

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
    z = y + 17;
    if (invincibleDelay >= 0.0f) invincibleDelay -= delay;
  }
}

void PlayerEntity::renderPlayer(sf::RenderTarget* app)
{
  sf::Color savedColor = sprite.getColor();
  if (isPoisoned()) sprite.setColor(sf::Color(180, 255, 180, 255));

  // body
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( frame * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( frame * width, spriteDy * height, width, height));
  app->draw(sprite);
  sprite.setColor(savedColor);

  if (equip[EQUIP_MAGICIAN_ROBE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_BROOCH_FINESSE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_RAGE_AMULET])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_LEATHER_BELT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_DISPLACEMENT_GLOVES])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // idle ?
  if (idleAge >= 4.0 && idleAge <= 5.0)
  {
    if (isPoisoned()) sprite.setColor(sf::Color(180, 255, 180, 255));
    int faceFrame = (5.0f - idleAge) / 0.2f;
    if (faceFrame == 3) faceFrame = 1;
    else if (faceFrame == 4) faceFrame = 0;
    if (spriteDy == 0)
    {
      sf::Sprite faceSprite;
      faceSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_FACES));
      faceSprite.setPosition(x - 21, y - 60);
      faceSprite.setTextureRect(sf::IntRect( faceFrame * width, 0 , width, height / 2));
      app->draw(faceSprite);
    }
    else if (spriteDy == 1)
    {
      sf::Sprite faceSprite;
      faceSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_FACES));
      faceSprite.setPosition(x - 21, y - 60);
      if (isMirroring)
        faceSprite.setTextureRect(sf::IntRect( faceFrame * width + width, height / 2, -width, height / 2));
      else
        faceSprite.setTextureRect(sf::IntRect( faceFrame * width, height / 2, width, height / 2));
      app->draw(faceSprite);
    }
    sprite.setColor(savedColor);
  }
  else if (idleAge >= 9.0 && idleAge <= 10.2)
  {
    if (isPoisoned()) sprite.setColor(sf::Color(180, 255, 180, 255));
    int faceFrame = (10.2f - idleAge) / 0.2f;
    if (faceFrame == 5) SoundManager::getInstance().playSound(SOUND_YAWN);
    if (faceFrame == 3) faceFrame = 2;
    else if (faceFrame == 4) faceFrame = 1;
    else if (faceFrame == 5) faceFrame = 0;
    if (spriteDy == 0)
    {
      sf::Sprite faceSprite;
      faceSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_FACES));
      faceSprite.setPosition(x - 21, y - 60);
      faceSprite.setTextureRect(sf::IntRect( faceFrame * width, height, width, height / 2));
      app->draw(faceSprite);
    }
    else if (spriteDy == 1)
    {
      sf::Sprite faceSprite;
      faceSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_FACES));
      faceSprite.setPosition(x - 21, y - 60);
      if (isMirroring)
        faceSprite.setTextureRect(sf::IntRect( faceFrame * width + width, height + height / 2, -width, height / 2));
      else
        faceSprite.setTextureRect(sf::IntRect( faceFrame * width, height + height / 2, width, height / 2));
      app->draw(faceSprite);
    }
    sprite.setColor(savedColor);
  }
  else if (idleAge > 11.0f) idleAge -= 11.0f;

  // hat
  if (equip[EQUIP_MAGICIAN_HAT] && playerStatus != playerStatusDead)
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // boots
  if (equip[EQUIP_LEATHER_BOOTS] && playerStatus != playerStatusDead)
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // staff
  int frameDx = equip[EQUIP_MAHOGANY_STAFF] ? 27 : 3;
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width, spriteDy * height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (30 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (30 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_REAR_SHOT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (33 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (33 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // shot type
  if (getShotType() != ShotTypeStandard)
  {
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

    case ShotTypeFire:
      sprite.setColor(sf::Color(255, 180, 0, 255));
      break;

    default:
      std::cout << "[WARNING] Can not render shot type: " << getShotType() << std::endl;
    }

    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (36 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (36 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setColor(savedColor);
  }
}

void PlayerEntity::renderHalo(sf::RenderTarget* app)
{
  if (frame > 2 || spriteDy > 9) return;
  // gems
  if ((getShotType() == ShotTypeIce || getShotType() == ShotTypeLightning || getShotType() == ShotTypeFire)  && playerStatus != playerStatusDead)
  {
    int fade;
    sf::Color savedColor = sprite.getColor();
    if (getShotType() != ShotTypeIce || specialBoltTimer <= 0.0f) fade = 255;
    else fade = ((STATUS_FROZEN_BOLT_DELAY[getShotLevel()] - specialBoltTimer) / STATUS_FROZEN_BOLT_DELAY[getShotLevel()]) * 128;

    if (getShotType() == ShotTypeLightning)
      fade = 150 + rand() % 105;
    if (getShotType() == ShotTypeFire)
      fade = 200 + rand() % 40;

    if (getShotType() == ShotTypeIce)
      sprite.setTextureRect(sf::IntRect(3 * width, 10 * height, 20, 20));
    else if (getShotType() == ShotTypeLightning)
      sprite.setTextureRect(sf::IntRect(3 * width + 20, 10 * height, 20, 20));
    else if (getShotType() == ShotTypeFire)
      sprite.setTextureRect(sf::IntRect(3 * width + 40, 10 * height, 20, 20));

    sprite.setColor(sf::Color(255, 255, 255, fade));
    if (isMirroring)
      sprite.setPosition(x - 10 + 42 - xHalo[spriteDy][frame], y - 10 + yHalo[spriteDy][frame]);
    else
      sprite.setPosition(x - 10 + xHalo[spriteDy][frame], y - 10 + yHalo[spriteDy][frame]);

    sf::RenderStates r;
    r.blendMode = sf::BlendAdd;
    app->draw(sprite, r);

    sprite.setPosition(x, y);
    sprite.setColor(savedColor);
  }
}

void PlayerEntity::render(sf::RenderTarget* app)
{
  sprite.setPosition(x, y);

  spriteDy = 0;
  isMirroring = false;

  if (firingDirection == 5)
  {
    if (facingDirection == 6) spriteDy = 1;
    else if (facingDirection == 8) spriteDy = 2;
    else if (facingDirection == 4)
    {
      spriteDy = 1;
      isMirroring = true;
    }
  }
  else
  {
    if (firingDirection == 2) spriteDy = 4;
    else if (firingDirection == 6) spriteDy = 5;
    else if (firingDirection == 8) spriteDy = 6;
    else if (firingDirection == 4)
    {
      spriteDy = 5;
      isMirroring = true;
    }

    if (!isMoving()) spriteDy += 3;
  }

  if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
  {
    spriteDy = 3;
    frame = ((int)(age * 4.0f)) % 4;
    if (frame == 3) frame = 1;
  }

  if (playerStatus == playerStatusDead)
  {
    int deathframe = (int)(deathAge / 0.35f);
    switch (deathframe)
    {
    case 0:
      frame = 0;
      spriteDy = 10;
      break;
    case 1:
      frame = 1;
      spriteDy = 10;
      break;
    case 2:
      frame = 2;
      spriteDy = 10;
      break;
    case 3:
      frame = 0;
      spriteDy = 11;
      break;
    default:
      frame = 1;
      spriteDy = 11;
      break;
    }
    sprite.setTextureRect(sf::IntRect( frame * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
  else
  {
    // shadow
    sprite.setTextureRect(sf::IntRect( 2 * width, 11 * height, width, height));
    app->draw(sprite);

    renderHalo(app);
    renderPlayer(app);

    // shield
    if (protection.active)
    {
      sf::Color savedColor = sprite.getColor();
      sprite.setColor(sf::Color(255, 255, 255, 100 + cos(age * (protection.timer < 2.0f ? 25 : 10)) * 30 ));
      sprite.setTextureRect(sf::IntRect( 3 * width, 11 * height, width, height));
      sprite.setScale(1.5f, 1.5f);
      app->draw(sprite);
      sprite.setScale(1.0f, 1.0f);
      sprite.setColor(savedColor);
    }
  }

  if (game().getShowLogical() && playerStatus != playerStatusDead)
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}


void PlayerEntity::calculateBB()
{
  boundingBox.left = (int)x - 10;
  boundingBox.width = 20;
  boundingBox.top = (int)y - 16;
  boundingBox.height =  33;
}

void PlayerEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (playerStatus == playerStatusDead) return;

  EnemyBoltEntity* boltEntity = dynamic_cast<EnemyBoltEntity*>(entity);

  if (collideWithEntity(entity))
  {
    if (boltEntity != NULL && !boltEntity->getDying())
    {
      boltEntity->collide();
      // TODO bolt source
      hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel(), boltEntity->isCritical(), SourceTypeBolt, boltEntity->getEnemyType());
      game().generateBlood(x, y, bloodColor);

      float xs = (x + boltEntity->getX()) / 2;
      float ys = (y + boltEntity->getY()) / 2;
      SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_STAR_2), xs, ys);
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

bool* PlayerEntity::getEquipment()
{
  return equip;
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
      else boltType = ShotTypeCold;
    }
    break;
  case ShotTypeStandard:
  case ShotTypeIllusion:
  case ShotTypeStone:
  case ShotTypeLightning:
  case ShotTypeFire:
    boltType = getShotType();
    shotLevel = getShotLevel();
    break;
  default:
    std::cout << "[WARNING] Can not generate shot type: " << getShotType() << std::endl;

  }

  BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, boltType, shotLevel);
  int boltDamage = fireDamages;
  if (criticalChance > 0)
    if (rand()% 100 < criticalChance)
    {
      boltDamage += boltDamage;
      bolt->setCritical(true);
    }
  bolt->setDamages(boltDamage);

  if (equip[EQUIP_DISPLACEMENT_GLOVES])
  {
    if (firingDirection == 2 || firingDirection == 8)
      velx += velocity.x * 0.7f;
    else if (firingDirection == 4 || firingDirection == 6)
      vely += velocity.y * 0.7f;
  }

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
  SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);
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
    SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);

    if (equip[EQUIP_BOOK_DUAL] || equip[EQUIP_BOOK_TRIPLE]
        || equip[EQUIP_BOOK_DUAL_QUICK] || equip[EQUIP_BOOK_TRIPLE_QUICK])
    {
      float shoot_angle = 0.2f;

      if ((direction == 4 && velocity.x < -1.0f) || (direction == 6 && velocity.x > 1.0f)
          || (direction == 8 && velocity.y < -1.0f) || (direction == 2 && velocity.y > 1.0f))
        shoot_angle = 0.1f;
      else if ((direction == 6 && velocity.x < -1.0f) || (direction == 4 && velocity.x > 1.0f)
               || (direction == 2 && velocity.y < -1.0f) || (direction == 8 && velocity.y > 1.0f))
        shoot_angle = (equip[EQUIP_BOOK_TRIPLE] || equip[EQUIP_BOOK_TRIPLE_QUICK]) ? 0.35f : 0.2f;
      else if (!equip[EQUIP_BOOK_TRIPLE] && !equip[EQUIP_BOOK_TRIPLE_QUICK])
        shoot_angle = 0.1f;

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
    if (!(equip[EQUIP_BOOK_DUAL] || equip[EQUIP_BOOK_DUAL_QUICK]) || (equip[EQUIP_BOOK_TRIPLE] || equip[EQUIP_BOOK_TRIPLE_QUICK]))
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

    if (equip[EQUIP_REAR_SHOT])
    {
      BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, ShotTypeStandard, 0);
      bolt->setDamages(fireDamages / 2);
      float velx = 0.0f;
      float vely = 0.0f;
      switch (direction)
      {
      case 4:
        velx = fireVelocity * 0.75f;
        break;
      case 6:
        velx = -fireVelocity * 0.75f;
        break;
      case 2:
        vely = -fireVelocity * 0.75f;
        break;
      case 8:
        vely = fireVelocity * 0.75f;
        break;
      }
      bolt->setVelocity(Vector2D(velx, vely));
    }

    if (equip[EQUIP_BOOK_RANDOM] && randomFireDelay <= 0.0f)
    {
      BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime, ShotTypeStandard, 0);
      bolt->setDamages(fireDamages);
      float shotAngle = rand() % 360;
      bolt->setVelocity(Vector2D(fireVelocity * 0.75f * cos(shotAngle), fireVelocity * 0.75f * sin(shotAngle)));
      randomFireDelay = fireDelay * 1.5f;
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

int PlayerEntity::hurt(int damages, enumShotType hurtingType, int level, bool critical, sourceTypeEnum sourceType, enemyTypeEnum enemyType)
{
  if (playerStatus == playerStatusDead) return false;

  if (invincibleDelay <= 0.0f || hurtingType == ShotTypeDeterministic)
  {
    SoundManager::getInstance().playSound(SOUND_PLAYER_HIT);
    if (BaseCreatureEntity::hurt(damages, hurtingType, level, critical, sourceType, enemyType) > 0)
    {
      if (hurtingType != ShotTypeDeterministic)
      {
        invincibleDelay = INVINCIBLE_DELAY;
        if (equip[EQUIP_RAGE_AMULET]) rageFire();
        game().generateBlood(x, y, bloodColor);
      }

      hurtingDelay = HURTING_DELAY * 2.0f;
      game().generateBlood(x, y, bloodColor);
      game().proceedEvent(EventBeingHurted);

      lastHurtingEnemy = enemyType;
      lastHurtingSource = sourceType;

      return true;
    }
  }
  return false;
}

void PlayerEntity::loseItem(enumItemType itemType, bool isEquip)
{
  CollidingSpriteEntity* itemSprite
    = new CollidingSpriteEntity(ImageManager::getInstance().getImage(isEquip ? IMAGE_ITEMS_EQUIP : IMAGE_ITEMS), x, y, 32, 32);
  itemSprite->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  itemSprite->setZ(OFFSET_Y - 1);
  itemSprite->setFrame(itemType);
  itemSprite->setImagesProLine(10);
  itemSprite->setType(ENTITY_BLOOD);
  itemSprite->setVelocity(Vector2D(200 + rand()%450));
  itemSprite->setViscosity(0.95f);
  itemSprite->setSpin( (rand() % 700) - 350.0f);
}

void PlayerEntity::dying()
{
  playerStatus = playerStatusDead;
  deathAge = 0.0f;
  hp = 0;
  SoundManager::getInstance().playSound(SOUND_PLAYER_DIE);
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
}

void PlayerEntity::displayAcquiredGold(int n)
{
  std::ostringstream oss;
  oss << "+" << n;
  TextEntity* text = new TextEntity(oss.str(), 16, x, y - 30.0f);
  text->setColor(TextEntity::COLOR_FADING_YELLOW);
  text->setAge(-0.6f);
  text->setLifetime(0.3f);
  text->setWeight(-60.0f);
  text->setZ(2000);
  text->setAlignment(ALIGN_CENTER);
  text->setType(ENTITY_FLYING_TEXT);
}

void PlayerEntity::acquireItem(enumItemType type)
{
  if (items[type].generatesStance) acquireStance(type);
  else switch (type)
    {
    case ItemCopperCoin:
      gold++;
      displayAcquiredGold(1);
      SoundManager::getInstance().playSound(SOUND_COIN_PICK_UP);
      game().proceedEvent(EventGetCoin);
      break;
    case ItemSilverCoin:
      gold = gold + 5;
      displayAcquiredGold(5);
      SoundManager::getInstance().playSound(SOUND_COIN_PICK_UP);
      game().proceedEvent(EventGetCoin);
      break;
    case ItemGoldCoin:
      gold = gold + 10;
      displayAcquiredGold(10);
      SoundManager::getInstance().playSound(SOUND_COIN_PICK_UP);
      break;
    case ItemHealthVerySmallPoison:
      specialState[SpecialStatePoison].active = false;
    case ItemHealthVerySmall:
      hp += equip[EQUIP_MANUAL_HEALTH] ? 5 : 3;
      SoundManager::getInstance().playSound(SOUND_EAT);
      if (hp > hpMax) hp = hpMax;
      break;
    case ItemHealthSmall:
      hp += equip[EQUIP_MANUAL_HEALTH] ? 10 : 7;
      SoundManager::getInstance().playSound(SOUND_EAT);
      if (hp > hpMax) hp = hpMax;
      break;
    case ItemHealth:
      hp += equip[EQUIP_MANUAL_HEALTH] ? 22 : 15;
      SoundManager::getInstance().playSound(SOUND_EAT);
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
  criticalChance = 0;

  if (equip[EQUIP_DISPLACEMENT_GLOVES]) fireDelayBonus -= 0.10f;
  if (equip[EQUIP_MAGICIAN_HAT]) fireDelayBonus -= 0.2f;
  if (equip[EQUIP_LEATHER_BELT]) fireDelayBonus -= 0.15f;
  if (equip[EQUIP_LEATHER_BOOTS]) creatureSpeedBonus += 0.15f;
  if (equip[EQUIP_BOOK_TRIPLE]) fireDelayBonus += 0.7f;
  else if (equip[EQUIP_BOOK_DUAL]) fireDelayBonus += 0.5f;
  if (equip[EQUIP_BOOK_TRIPLE_QUICK]) fireDamagesBonus -= 0.35f;
  else if (equip[EQUIP_BOOK_DUAL_QUICK]) fireDamagesBonus -= 0.25f;
  if (equip[EQUIP_BROOCH_FINESSE]) criticalChance += 5;
  if (equip[EQUIP_MANUAL_STAFF]) boltLifeTimeBonus += 0.4f;
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
    if (specialShots[i] == ShotTypeFire && equip[EQUIP_RING_FIRE])
      specialShotLevel[i]++;
  }
  if (getShotType() == ShotTypeIllusion) fireDamages *= ILLUSION_DAMAGE_DECREASE[getShotLevel()];
  else if (getShotType() == ShotTypeFire) fireDamages *= FIRE_DAMAGE_INCREASE[getShotLevel()];

  // spells
  if (protection.active) armor += protection.value;
}

void PlayerEntity::acquireStance(enumItemType type)
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusAcquire;
  acquireDelay = ACQUIRE_DELAY;
  acquiredItem = (enumItemType)(type);
  SoundManager::getInstance().playSound(SOUND_BONUS);

  game().showArtefactDescription(type);

  enumItemType itemFrame = type;
  int itemImage = IMAGE_ITEMS;
  if (itemFrame >= FirstEquipItem)
  {
    itemFrame = (enumItemType)(itemFrame - FirstEquipItem);
    itemImage = IMAGE_ITEMS_EQUIP;
  }

  spriteItem = new SpriteEntity(
    ImageManager::getInstance().getImage(itemImage),
    x, y - 100.0f, ITEM_WIDTH, ITEM_HEIGHT);
  spriteItem->setFrame((int)itemFrame);
  spriteItem->setImagesProLine(10);
  spriteItem->setZ(z);
  spriteItem->setLifetime(ACQUIRE_DELAY);

  spriteItemStar = new SpriteEntity(
    ImageManager::getInstance().getImage(IMAGE_STAR),
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
  SoundManager::getInstance().playSound(SOUND_BONUS);
  equip[EQUIP_BOSS_KEY] = false;


  SpriteEntity* spriteItem = new SpriteEntity(
    ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP),
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

  SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
  computePlayer();
}

void PlayerEntity::initShotType()
{
  specialBoltTimer = STATUS_FROZEN_BOLT_DELAY[getShotLevel()];
  needInitShotType = false;

  if (getShotType() == ShotTypeLightning)
    SoundManager::getInstance().playSound(SOUND_ELECTRIC_CHARGE);
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

bool PlayerEntity::canGetNewShot(bool advancedShot)
{
  int nbSpecial =0;
  int nbAdvanced =0;
  for (int i = 1; i < SPECIAL_SHOT_SLOTS; i++)
  {
    switch (specialShots[i])
    {
    case ShotTypeIce:
    case ShotTypeStone:
    case ShotTypeLightning:
      nbSpecial++;
      break;

    case ShotTypeFire:
    case ShotTypeIllusion:
      nbAdvanced++;
      break;

    case ShotTypeStandard:
      break;

    default:
      std::cout << "[WARNING] Can not register shot type: " << getShotType() << std::endl;
    }
  }

  if (advancedShot)
    return (nbAdvanced >= SPECIAL_SHOT_SLOTS_ADVANCED);
  else
    return (nbSpecial >= SPECIAL_SHOT_SLOTS_STANDARD);
}

castSpellStruct PlayerEntity::getActiveSpell()
{
  return activeSpell;
}

void PlayerEntity::setActiveSpell(enumCastSpell spell, bool fromSaveInFight)
{
  if (activeSpell.spell != SpellNone)
  {
    // drop the old spell
    equip[activeSpell.frame] = false;
    ItemEntity* newItem = new ItemEntity((enumItemType)(ItemMagicianHat + activeSpell.frame), x, y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
  }

  activeSpell.spell = spell;

  switch (spell)
  {
  case SpellTeleport:
    activeSpell.delayMax = 20.0f;
    activeSpell.frame = ItemSpellTeleport - FirstEquipItem;
    break;

  case SpellSlimeExplode:
    activeSpell.delayMax = 40.0f;
    activeSpell.frame = ItemSpellSlimeExplode - FirstEquipItem;
    break;

  case SpellFireball:
    activeSpell.delayMax = 20.0f;
    activeSpell.frame = ItemSpellFireball - FirstEquipItem;
    break;

  case SpellFreeze:
    activeSpell.delayMax = 40.0f;
    activeSpell.frame = ItemSpellFreeze - FirstEquipItem;
    break;

  case SpellEarthquake:
    activeSpell.delayMax = 40.0f;
    activeSpell.frame = ItemSpellEarthquake - FirstEquipItem;
    break;

  case SpellProtection:
    activeSpell.delayMax = 40.0f;
    activeSpell.frame = ItemSpellProtection - FirstEquipItem;
    break;

  case SpellWeb:
    activeSpell.delayMax = 35.0f;
    activeSpell.frame = ItemSpellWeb - FirstEquipItem;
    break;

  case SpellNone:
    break;
  }
  if (fromSaveInFight) activeSpell.delay = 1.0f;
  else activeSpell.delay = activeSpell.delayMax;
}

void PlayerEntity::castSpell()
{
  if (playerStatus != playerStatusPlaying) return;

  if (canCastSpell())
  {
    activeSpell.delay = equip[EQUIP_BOOK_MAGIC_I] ? activeSpell.delayMax * 0.8f : activeSpell.delayMax;

    switch (activeSpell.spell)
    {
    case SpellTeleport:
      castTeleport();
      break;
    case SpellSlimeExplode:
      castSummonsSlimeExplode();
      break;
    case SpellFireball:
      castFireball();
      break;
    case SpellFreeze:
      castFreeze();
      break;
    case SpellEarthquake:
      castEarthquake();
      break;
    case SpellProtection:
      castProtection();
      break;
    case SpellWeb:
      castWeb();
      break;

    case SpellNone:
      break;
    }
  }
}

bool PlayerEntity::canCastSpell()
{
  return activeSpell.spell != SpellNone && activeSpell.delay <= 0.0f;
}

void PlayerEntity::castTeleport()
{
  bool ok = false;
  int xm, ym;
  float xNew = x, yNew = y;
  invincibleDelay = equip[EQUIP_BOOK_MAGIC_II] ? 2.5f : 2.0f;
  SoundManager::getInstance().playSound(SOUND_TELEPORT);
  game().makeColorEffect(X_GAME_COLOR_VIOLET, 0.3f);

  for(int i=0; i < 6; i++)
  {
    generateStar(sf::Color(50, 50, 255, 255));
    generateStar(sf::Color(200, 200, 255, 255));
  }

  int counter = 150;
  while (!ok && counter > 0)
  {
    counter--;
    int distanceMin = 20000;
    if (counter < 50) distanceMin = 30000;
    else if (counter < 100) distanceMin = 25000;

    xm = 1 +rand() % (MAP_WIDTH - 3);
    ym = 1 +rand() % (MAP_HEIGHT - 3);

    if (game().getCurrentMap()->isWalkable(xm, ym))
    {
      // enemy or bolt ?
      EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
      EntityManager::EntityList::iterator it;

      bool isBad = false;
      xNew = OFFSET_X + xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
      yNew = OFFSET_Y + ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

      for (it = entityList->begin (); !isBad && it != entityList->end ();)
      {
        GameEntity *e = *it;
        it++;

        if ((e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX_COUNT) || e->getType() == ENTITY_ENEMY_BOLT)
          isBad = Vector2D(xNew, yNew).distance2(Vector2D(e->getX(), e->getY())) < distanceMin;
      }

      if (!isBad)
      {
        x = xNew;
        y = yNew;
      }
    }
  }

  for(int i=0; i < 6; i++)
  {
    generateStar(sf::Color(50, 50, 255, 255));
    generateStar(sf::Color(200, 200, 255, 255));
  }
}

void PlayerEntity::initFallingGrid()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
      fallingGrid[i][j] = false;
}

void PlayerEntity::fallRock()
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
                        rand() % 3,
                        true);
}

void PlayerEntity::castSummonsSlimeExplode()
{
  SlimeEntity* slime = new SlimeEntity( ((int)(x - OFFSET_X) / TILE_WIDTH) * TILE_WIDTH + TILE_WIDTH * 0.5f,
                                        y - 5, SlimeTypeViolet, true);
  slime->makePet(facingDirection);
  game().makeColorEffect(X_GAME_COLOR_VIOLET, 0.3f);
}

void PlayerEntity::castFireball()
{
  SoundManager::getInstance().playSound(SOUND_FIREBALL);
  game().makeColorEffect(X_GAME_COLOR_RED, 0.3f);
  enumShotType boltType = ShotTypeFire;
  unsigned int shotLevel = 2;

  BoltEntity* bolt = new BoltEntity(x, y - 10, boltLifeTime + 0.5f, boltType, shotLevel);

  int boltDamage = fireDamages * (equip[EQUIP_BOOK_MAGIC_II] ? 4 : 3);
  bolt->setDamages(boltDamage);
  bolt->setGoThrough(true);

  float velx = 0.0f, vely = 0.0f;
  if (facingDirection == 4) velx = -fireVelocity;
  else if (facingDirection == 8) vely = -fireVelocity;
  else if (facingDirection == 2) vely = fireVelocity;
  else velx = fireVelocity;

  bolt->setVelocity(Vector2D(velx, vely));
}

void PlayerEntity::castFreeze()
{
  int iceLevel = equip[EQUIP_BOOK_MAGIC_II] ? 2 : 1;
  for (float i = 0.0f; i < 2 * PI; i +=  PI / 8)
  {
    BoltEntity* bolt1 = new BoltEntity(x, y - 10, boltLifeTime, ShotTypeIce, iceLevel);
    bolt1->setDamages(1);
    float velx = fireVelocity * cos(i);
    float vely = fireVelocity * sin(i);
    bolt1->setVelocity(Vector2D(velx, vely));
  }

  game().makeColorEffect(X_GAME_COLOR_BLUE, 0.3f);
  SoundManager::getInstance().playSound(SOUND_SPELL_FREEZE);
}

void PlayerEntity::castEarthquake()
{
  initFallingGrid();
  int nbIterations = equip[EQUIP_BOOK_MAGIC_II] ? 24 : 22;
  for (int i = 0; i < nbIterations; i++) fallRock();

  game().makeShake(0.25f);
  game().makeColorEffect(X_GAME_COLOR_BROWN, 0.3f);
  SoundManager::getInstance().playSound(SOUND_EARTHQUAKE);
}

void PlayerEntity::castProtection()
{
  protection.active = true;
  protection.value = equip[EQUIP_BOOK_MAGIC_II] ? 0.6f : 0.4f;
  protection.timer = 10.0f;
  computePlayer();
  game().makeColorEffect(X_GAME_COLOR_BLUE, 0.3f);
  SoundManager::getInstance().playSound(SOUND_SPELL_SHIELD);
}

void PlayerEntity::castWeb()
{
  SoundManager::getInstance().playSound(SOUND_SPIDER_WEB);
  int nbWeb = equip[EQUIP_BOOK_MAGIC_II] ? 4 : 3;
  for (int i = 0; i < nbWeb; i++)
  {
    SpiderWebEntity* web = new SpiderWebEntity(x, y, true);
    float webVel = 100 + rand()% 500;
    float webAngle = -60 + rand() % 120;
    webAngle = PI * webAngle / 180.0f;

    if (facingDirection == 4) webAngle += PI;
    else if (facingDirection == 8) webAngle -= PI * 0.5;
    else if (facingDirection == 2) webAngle += PI * 0.5;

    web->setVelocity(Vector2D(webVel * cos(webAngle), webVel * sin(webAngle)));
  }
}
