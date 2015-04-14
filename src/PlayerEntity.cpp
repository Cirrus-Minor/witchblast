#include "PlayerEntity.h"
#include "SlimeEntity.h"
#include "SlimePetEntity.h"
#include "FallingRockEntity.h"
#include "BoltEntity.h"
#include "SpiderWebEntity.h"
#include "EvilFlowerEntity.h"
#include "EnemyBoltEntity.h"
#include "ItemEntity.h"
#include "FairyEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextEntity.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>

#define SPIRAL_STAIRCASE

const int xHalo[9][3] =
{
  { 13, 13, 18},
  { 47, 45, 37},
  { 48, 47, 44},
  { 15, 15, 16},
  { 45, 45, 45},
  { 46, 44, 42},
  { 13, 13, 13},
  { 13, 24, 36},
  { 13, 17, 13}
};

const int yHalo[9][3] =
{
  { 26, 25, 25},
  { 23, 23, 23},
  { 23, 24, 25},
  { 31, 32, 34},
  { 23, 23, 23},
  { 23, 22, 21},
  { 26, 26, 26},
  { 26, 15, 6},
  { 25, 27, 25}
};

PlayerEntity::PlayerEntity(float x, float y)
  : BaseCreatureEntity (ImageManager::getInstance().getImage(IMAGE_PLAYER_0), x, y, 64, 96)
{
  currentFireDelay = -1.0f;
  randomFireDelay = -1.0f;
  invincibleDelay = -1.0f;
  divineInterventionDelay = -1.0f;
  fireAnimationDelay = -1.0f;
  fireAnimationDelayMax = 0.4f;
  spellAnimationDelay = -1.0f;
  spellAnimationDelayMax = 0.7f;

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
  hiccupDelay = HICCUP_DELAY;
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

  sprite.setOrigin(32, 80);

  protection.active = false;
  armor = 0.0f;

  activeSpell.delay = -1.0f;
  activeSpell.spell = SpellNone;

  divinity.divinity = -1;
  divinity.piety = 0;
  divinity.level = 0;
  divinity.interventions = 0;
  divinity.percentsToNextLevels = 0.0f;
  isRegeneration = false;
  isFairyTransmuted = false;

  itemToBuy = NULL;
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

void PlayerEntity::setFacingDirection(int facingDirection)
{
  if (facingDirection == 4 || facingDirection == 6 || facingDirection == 2  || facingDirection == 8)
    this->facingDirection = facingDirection;
}

float PlayerEntity::getPercentFireDelay()
{
  if (canFirePlayer) return 1.0f;

  else return (1.0f - currentFireDelay / fireDelay);
}

float PlayerEntity::getLightCone()
{
  if (playerStatus == playerStatusPraying)
  {
    float result = 1.0f;
    if (statusTimer < 0.25f)
      result = 4 * statusTimer;
    else if (statusTimer > WORSHIP_DELAY - 0.25f)
      result = (WORSHIP_DELAY - statusTimer) * 4;
    return result;
  }
  else if (divineInterventionDelay > 0.0f)
  {
    float result = 1.0f;
    if (divineInterventionDelay < 0.25f)
      result = 4 * divineInterventionDelay;
    else if (divineInterventionDelay > WORSHIP_DELAY - 0.25f)
      result = (WORSHIP_DELAY - divineInterventionDelay) * 4;

    if (isRegeneration) result *= 0.4f;

    return result;
  }
  else return -1.0f;
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

bool PlayerEntity::getFairyTransmuted()
{
  return isFairyTransmuted;
}

divinityStruct PlayerEntity::getDivinity()
{
  return divinity;
}

int PlayerEntity::getPiety()
{
  return divinity.piety;
}

void PlayerEntity::stuck()
{
  castTeleport();
}

void PlayerEntity::setEntering()
{
  playerStatus = playerStatusEntering;
}

void PlayerEntity::setLeavingLevel()
{
  playerStatus = playerStatusGoingNext;
}

void PlayerEntity::pay(int price)
{
  gold -= price;
  displayAcquiredGold(-price);
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
    else if (items[acquiredItem].specialShot != (ShotTypeStandard))
    {
      registerSpecialShot(acquiredItem);
      game().proceedEvent(EventGetSpecialShot);
    }

    // spells
    else if (items[acquiredItem].spell != SpellNone)
    {
      setActiveSpell(items[acquiredItem].spell, false);
      game().proceedEvent(EventGetSpell);
    }

    // spells
    else if (items[acquiredItem].spell != SpellNone)
    {
      setActiveSpell(items[acquiredItem].spell, false);
      game().proceedEvent(EventGetSpell);
    }

    // pet slime
    else if (acquiredItem == ItemPetSlime)
    {
      new SlimePetEntity();
    }

    // floor item
    else if (acquiredItem == ItemFloorMap)
      game().revealFloor();
    else if (acquiredItem == ItemAlcohol)
      hiccupDelay = HICCUP_DELAY;

    // acquirement
    if (equip[EQUIP_DISPLACEMENT_GLOVES] && equip[EQUIP_LEATHER_BOOTS] && equip[EQUIP_MAGICIAN_HAT] && equip[EQUIP_MAGICIAN_ROBE])
      game().registerAchievement(AchievementCompleteSet);

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

void PlayerEntity::resetFloorItem()
{
  equip[EQUIP_BOSS_KEY] = false;
  equip[EQUIP_FLOOR_MAP] = false;
  equip[EQUIP_ALCOHOL] = false;
  equip[EQUIP_FAIRY_POWDER] = false;
  equip[EQUIP_LUCK] = false;
  computePlayer();
}

void PlayerEntity::setItemToBuy(ItemEntity* item)
{
  itemToBuy = item;
}

ItemEntity* PlayerEntity::getItemToBuy()
{
  return itemToBuy;
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
  if (playerStatus == playerStatusGoingNext)
  {
    return;
  }
  if (playerStatus == playerStatusStairs)
  {
    x += velocity.x * delay;
    y += velocity.y * delay;
    age += delay;
    frame = ((int)(age * 7.0f)) % 4;
    if (frame == 3) frame = 1;

    if (x < (MAP_WIDTH / 2) * TILE_WIDTH - TILE_WIDTH / 2)
    {
      facingDirection = 8;
      game().moveToOtherMap(8);
    }

    return;
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
    statusTimer -= delay;
    if (statusTimer <= 0.0f)
    {
      acquireItemAfterStance();
    }
  }
  else if (equip[EQUIP_ALCOHOL] && playerStatus == playerStatusPlaying)
  {
    hiccupDelay -= delay;
    if (hiccupDelay <= 0.0f)
    {
      hiccupDelay = 4.0f;

      // hiccup
      recoil.active = true;
      recoil.stun = true;
      recoil.velocity = Vector2D(350.0f);
      recoil.timer = 0.4f;

      for (int i = 0; i < 4; i++)
      {
        BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypePoison, 0);
        bolt->setDamages(4);
        bolt->setFlying(isFairyTransmuted);
        //bolt->setVelocity(recoil.velocity.vectorTo(Vector2D(0, 0), fireVelocity));
        bolt->setVelocity(Vector2D(fireVelocity));
      }

      TextEntity* text = new TextEntity("*hic*", 16, x, y - 30.0f);
      text->setColor(TextEntity::COLOR_FADING_GREEN);
      text->setAge(-0.6f);
      text->setLifetime(0.3f);
      text->setWeight(-60.0f);
      text->setZ(2000);
      text->setAlignment(ALIGN_CENTER);
      text->setType(ENTITY_FLYING_TEXT);

      SoundManager::getInstance().playSound(SOUND_HICCUP);
      SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);
    }
  }

  if (divineInterventionDelay > 0.0f) divineInterventionDelay -= delay;
  if (fireAnimationDelay > 0.0f)
  {
    fireAnimationDelay -= delay;
    if (fireAnimationDelay <= 0.0f) fireAnimationDelay = -1.0f;
  }
  if (spellAnimationDelay > 0.0f)
  {
    spellAnimationDelay -= delay;
    if (spellAnimationDelay <= 0.0f) spellAnimationDelay = -1.0f;
  }

  // unlocking animation
  else if (playerStatus == playerStatusUnlocking || playerStatus == playerStatusPraying)
  {
    statusTimer -= delay;
    if (statusTimer <= 0.0f)
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

  // key room collision
  if (game().getCurrentMap()->getRoomType() == roomTypeKey && !game().getCurrentMap()->isCleared())
  {
    sf::IntRect col1;
    col1.width = 198;
    col1.height = 68;
    col1.top = 254;
    col1.left = 380;

    sf::IntRect col2;
    col2.width = 68;
    col2.height = 198;
    col2.top = 189;
    col2.left = 445;

    if (boundingBox.intersects(col1) || boundingBox.intersects(col2))
    {
      recoil.active = true;
      recoil.stun = true;
      recoil.velocity = Vector2D(GAME_WIDTH / 2, GAME_HEIGHT /2).vectorTo(Vector2D(x, y), 650.0f);
      recoil.timer = 0.4f;

      game().activateKeyRoomEffect(true);
    }
  }

  collidingDirection = 0;
  BaseCreatureEntity::animate(delay);

  if (firingDirection != 5)
    facingDirection = firingDirection;

  // find the frame
  if  (firingDirection != 5)
  {
    if (fireAnimationDelay < 0.0f)
      fireAnimationDelay = fireAnimationDelayMax;
    fireAnimationDirection = firingDirection;
  }
  else if (isMoving())
  {
    frame = ((int)(age * 7.0f)) % 4;
    if (frame == 3) frame = 1;
  }
  else if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking || playerStatus == playerStatusPraying)
    frame = 3;
  else if (playerStatus == playerStatusDead)
    frame = 0;
  else // standing
  {
    frame = 1;
  }

  if (playerStatus != playerStatusPlaying || isMoving() || firingDirection != 5)
    idleAge = 0.0f;
  else
    idleAge += delay;

  if (x < 0)
    game().moveToOtherMap(4);
  else if (x > MAP_WIDTH * TILE_WIDTH)
    game().moveToOtherMap(6);
  else if (y < 0)
    game().moveToOtherMap(8);
  else if (y > MAP_HEIGHT * TILE_HEIGHT)
    game().moveToOtherMap(2);
#ifdef SPIRAL_STAIRCASE
  else if (playerStatus == playerStatusPlaying
           && game().getCurrentMap()->getRoomType() == roomTypeExit && y < TILE_HEIGHT * 0.6f)
  {
    playerStatus = playerStatusStairs;
    velocity.y = creatureSpeed / 12;
    velocity.x = -creatureSpeed / 3;
    facingDirection = 4;

    SpriteEntity* doorEntity = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TILES),
        (MAP_WIDTH / 2) * TILE_WIDTH - TILE_WIDTH / 2,
        TILE_HEIGHT / 2, 64, 64, 1);
    doorEntity->setZ(TILE_HEIGHT);
    doorEntity->setImagesProLine(10);
    doorEntity->setFrame(189);
    doorEntity->setType(ENTITY_EFFECT);
  }
#endif

  if (playerStatus == playerStatusEntering)
  {
    if (boundingBox.left > TILE_WIDTH
        && (boundingBox.left + boundingBox.width) < TILE_WIDTH * (MAP_WIDTH - 1)
        && boundingBox.top > TILE_HEIGHT
        && (boundingBox.top + boundingBox.height) < TILE_HEIGHT * (MAP_HEIGHT - 1))
    {
      playerStatus = playerStatusPlaying;
      game().closeDoors();
    }
  }

  if (playerStatus != playerStatusDead)
  {
    if (invincibleDelay >= 0.0f) invincibleDelay -= delay;
    if (specialState[SpecialStateConfused].active)
      SoundManager::getInstance().playSound(SOUND_VAMPIRE_HYPNOSIS, false);
  }
  z = y + 4;
}

bool PlayerEntity::canCollide()
{
  return invincibleDelay <= 0.0f;
}

void PlayerEntity::setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2)
{
  BaseCreatureEntity::setSpecialState(state, active, timer, param1, param2);
  computePlayer();
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

  // boots
  if (equip[EQUIP_BOOTS_ADVANCED] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_LEATHER_BOOTS] && playerStatus != playerStatusDead)
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_ROBE_ADVANCED] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);

    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width, spriteDy * height, width, height));
    sprite.setColor(sf::Color(255, 255, 255, 100 + 100 * cosf(game().getAbsolutTime() * 3.5f)));
    app->draw(sprite, sf::BlendAdd);
    sprite.setColor(sf::Color(255, 255, 255, 255));

    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_MAGICIAN_ROBE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_GLOVES_ADVANCED] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_DISPLACEMENT_GLOVES])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_CRITICAL_ADVANCED])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
  else if (equip[EQUIP_CRITICAL])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);

    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
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

  // staff
  int frameDx = equip[EQUIP_MAHOGANY_STAFF] ? 6 : 3;
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width, spriteDy * height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_REAR_SHOT])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
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

    case ShotTypePoison:
      sprite.setColor(sf::Color(50, 255, 50, 255));
      break;

    default:
      std::cout << "[WARNING] Can not render shot type: " << getShotType() << std::endl;
    }

    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setColor(savedColor);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }

  // hat
  if (equip[EQUIP_HAT_ADVANCED] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_MAGICIAN_HAT] && playerStatus != playerStatusDead)
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
}

void PlayerEntity::renderHalo(sf::RenderTarget* app)
{
  if (frame > 2 || spriteDy > 8) return;
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
      sprite.setTextureRect(sf::IntRect(448, 864, 20, 20));
    else if (getShotType() == ShotTypeLightning)
      sprite.setTextureRect(sf::IntRect(448 + 20, 864, 20, 20));
    else if (getShotType() == ShotTypeFire)
      sprite.setTextureRect(sf::IntRect(448 + 40, 864, 20, 20));

    sprite.setColor(sf::Color(255, 255, 255, fade));
    if (isMirroring)
      sprite.setPosition(x - 10 + 64 - xHalo[spriteDy][frame], y - 10 + yHalo[spriteDy][frame]);
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

  if (idleAge > 8.5f)
  {
    idleAge -= 8.5f;
  }
  else if (idleAge >= 7.5)
  {
    spriteDy = 8;
    frame = 2;
  }
  else if (idleAge >= 7.0)
  {
    spriteDy = 8;
    frame = 1;
  }
  else if (idleAge >= 6.0)
  {
    spriteDy = 8;
    frame = 0;
  }
  else if (idleAge >= 5.5f && facingDirection != 2)
  {
    facingDirection = 2;
    idleAge -= 2.0f;
  }
  else if (fireAnimationDelay <= 0.0f && spellAnimationDelay <= 0.0f)
  {
    if (facingDirection == 6) spriteDy = 1;
    else if (facingDirection == 8) spriteDy = 2;
    else if (facingDirection == 4)
    {
      spriteDy = 1;
      isMirroring = true;
    }
  }
  else if (spellAnimationDelay >= 0.0f)
  {
    spriteDy = 7;
    if (spellAnimationDelay < spellAnimationDelayMax * 0.1f) frame = 0;
    else if (spellAnimationDelay < spellAnimationDelayMax * 0.2f) frame = 1;
    else if (spellAnimationDelay < spellAnimationDelayMax * 0.7f) frame = 2;
    else if (spellAnimationDelay < spellAnimationDelayMax * 0.85f) frame = 1;
    else frame = 0;
  }
  else
  {
    if (fireAnimationDirection == 2) spriteDy = 3;
    else if (fireAnimationDirection == 6) spriteDy = 4;
    else if (fireAnimationDirection == 8) spriteDy = 5;
    else if (fireAnimationDirection == 4)
    {
      spriteDy = 4;
      isMirroring = true;
    }
    if (fireAnimationDelay < fireAnimationDelayMax * 0.2f) frame = 0;
    else if (fireAnimationDelay < fireAnimationDelayMax * 0.4f) frame = 1;
    else if (fireAnimationDelay < fireAnimationDelayMax * 0.6f) frame = 2;
    else if (fireAnimationDelay < fireAnimationDelayMax * 0.8f) frame = 1;
    else frame = 0;
  }

  if (playerStatus == playerStatusAcquire || playerStatus == playerStatusUnlocking)
  {
    spriteDy = 6;
    frame = ((int)(age * 10.0f)) % 4;
    if (frame == 3) frame = 1;
  }
  else if (playerStatus == playerStatusPraying)
  {
    spriteDy = 7;
    frame = ((int)(age * 10.0f)) % 4;
    if (frame == 3) frame = 1;
    float delay = playerStatus == playerStatusPraying ? WORSHIP_DELAY : UNLOCK_DELAY;
    if (statusTimer < delay * 0.1f) frame = 0;
    else if (statusTimer < delay * 0.2f) frame = 1;
    else if (statusTimer < delay * 0.7f) frame = 2;
    else if (statusTimer < delay * 0.85f) frame = 1;
    else frame = 0;
  }

  if (playerStatus == playerStatusDead)
  {
    frame = (int)(deathAge / 0.35f);
    if (frame > 6) frame = 6;
    spriteDy = 9;

    sprite.setTextureRect(sf::IntRect( frame * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
  else
  {
    if (isFairyTransmuted)
    {
      frame = 0;
      if (velocity.x * velocity.x + velocity.y * velocity.y > 400)
        frame = ((int)(age * 24.0f)) % 2;
      else
        frame = ((int)(age * 18.0f)) % 2;

      sf::Sprite fairySprite;
      fairySprite.setColor(sprite.getColor());
      fairySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
      fairySprite.setPosition(sprite.getPosition());
      fairySprite.setOrigin(26, 62);

      switch (facingDirection)
      {
        case 8: fairySprite.setTextureRect(sf::IntRect( (2 + frame) * 48, 5 * 72, 48, 72)); break;
        case 4: fairySprite.setTextureRect(sf::IntRect( (4 + frame) * 48, 5 * 72, 48, 72)); break;
        case 6: fairySprite.setTextureRect(sf::IntRect( (5 + frame) * 48, 5 * 72, - 48, 72)); break;
        default: fairySprite.setTextureRect(sf::IntRect( frame * 48, 5 * 72, 48, 72)); break;
      }
      app->draw(fairySprite);
    }
    else
    {
      renderHalo(app);
      renderPlayer(app);
    }

    // shield
    if (specialState[DivineStateProtection].active || protection.active)
    {
      int firstFrame = 8;
      if (specialState[DivineStateProtection].active && divinity.divinity == DivinityStone) firstFrame = 16;
      float timer = specialState[DivineStateProtection].active ? specialState[DivineStateProtection].timer : protection.timer;

      sprite.setTextureRect(sf::IntRect( firstFrame * width, 9 * height, width, height));
      app->draw(sprite);

      sf::Color savedColor = sprite.getColor();
      sprite.setColor(sf::Color(255, 255, 255, 100 + cos(age * (timer < 2.0f ? 25 : 10)) * 30 ));
      sprite.setTextureRect(sf::IntRect( (firstFrame + 1) * width, 9 * height, width, height));
      app->draw(sprite);
      sprite.setColor(savedColor);
    }

    // divine field
    if (divinity.level > 1)
    {
      bool displayField = false;
      int fieldFrame;
      int fieldFade;

      switch (divinity.divinity)
      {
      case DivinityHealer:
        {
          displayField = true;
          fieldFrame = 10;
          fieldFade = 40 * (divinity.level - 1);
          break;
        }
      case DivinityFighter:
        {
          displayField = true;
          fieldFrame = 12;
          fieldFade = 40 * (divinity.level - 1);
          break;
        }
      case DivinityIce:
        {
          if (divinity.level > 2)
          {
            displayField = true;
            fieldFrame = 14;
            fieldFade = 80 * (divinity.level - 3);
          }
          break;
        }
      case DivinityStone:
        {
          if (divinity.level > 2)
          {
            displayField = true;
            fieldFrame = 16;
            fieldFade = 80 * (divinity.level - 3);
          }
          break;
        }
      case DivinityAir:
        {
          if (divinity.level > 2)
          {
            displayField = true;
            fieldFrame = 14;
            fieldFade = 80 * (divinity.level - 3);
          }
          break;
        }
      }

      if (displayField)
      {
        sf::Color savedColor = sprite.getColor();
        sprite.setColor(sf::Color(255, 255, 255, fieldFade ));
        sprite.setTextureRect(sf::IntRect( fieldFrame * width, 9 * height, width, height));
        app->draw(sprite);

        if (divinity.divinity != DivinityStone && divinity.divinity != DivinityHealer)
        {
          sprite.setColor(sf::Color(255, 255, 255, 2 + fieldFade / 2 + cos(age * 15) * fieldFade / 2 ));
          sprite.setTextureRect(sf::IntRect( (fieldFrame + 1) * width, 9 * height, width, height));
          app->draw(sprite);
          sprite.setColor(savedColor);
        }
        if (divinity.divinity == DivinityHealer && divineInterventionDelay > 0.0f && isRegeneration)
        {
          sprite.setTextureRect(sf::IntRect( (fieldFrame + 1) * width, 9 * height, width, height));
          app->draw(sprite);
        }
        sprite.setColor(savedColor);
      }
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
  if (isFairyTransmuted)
  {
    boundingBox.left = (int)x - 10;
    boundingBox.width = 20;
    boundingBox.top = (int)y - 29;
    boundingBox.height =  20;
  }
  else
  {
    boundingBox.left = (int)x - 10;
    boundingBox.width = 20;
    boundingBox.top = (int)y - 29;
    boundingBox.height =  33;
  }
}

void PlayerEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (playerStatus == playerStatusDead || !canCollide()) return;

  EnemyBoltEntity* boltEntity = dynamic_cast<EnemyBoltEntity*>(entity);

  if (collideWithEntity(entity))
  {
    if (boltEntity != NULL && !boltEntity->getDying())
    {
      boltEntity->collide();
      // TODO bolt source
      hurt(getHurtParams(boltEntity->getDamages(),
                         boltEntity->getBoltType(),
                         boltEntity->getLevel(),
                         boltEntity->isCritical(),
                         SourceTypeBolt,
                         boltEntity->getEnemyType(),
                         false));
      game().generateBlood(x, y, bloodColor);

      float xs = (x + boltEntity->getX()) / 2;
      float ys = (y + boltEntity->getY()) / 2;
      SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
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
  if (specialState[SpecialStateConfused].active)
  {
    switch (direction)
    {
      case 4: direction = 6; break;
      case 6: direction = 4; break;
      case 2: direction = 8; break;
      case 8: direction = 2; break;

      case 1: direction = 9; break;
      case 9: direction = 1; break;
      case 7: direction = 3; break;
      case 3: direction = 7; break;
    }
  }

  if (playerStatus == playerStatusAcquire && statusTimer < ACQUIRE_DELAY / 2)
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
    //if (fairies.size() == 3) game().registerAchievement(AchievementFairies);
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
  case ShotTypePoison:
    boltType = getShotType();
    shotLevel = getShotLevel();
    break;
  default:
    std::cout << "[WARNING] Can not generate shot type: " << getShotType() << std::endl;

  }

  BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, boltType, shotLevel);
  bolt->setFlying(isFairyTransmuted);
  int boltDamage = fireDamages;
  if (criticalChance > 0)
    if (rand()% 100 < criticalChance)
    {
      boltDamage *= equip[EQUIP_CRITICAL_ADVANCED] ? 3 : 2;
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
      BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypeFire, 0);
      bolt->setDamages(10);
      bolt->setFlying(isFairyTransmuted);
      float velx = fireVelocity * i * 0.42f;
      float vely = fireVelocity * j * 0.42f;
      bolt->setVelocity(Vector2D(velx, vely));

      if (hp <= hpMax / 5)
      {
        BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypeFire, 0);
        bolt->setDamages(10);
        bolt->setFlying(isFairyTransmuted);
        float velx = 0.0f;
        float vely = 0.0f;
        if (i == -1 && j == -1) velx = -fireVelocity * i * 0.6f;
        else if (i == -1 && j == 1) velx = fireVelocity * i * 0.6f;
        else if (i == 1 && j == -1) vely= -fireVelocity * i * 0.6f;
        else if (i == 1 && j == 1) vely = fireVelocity * i * 0.6f;
        bolt->setVelocity(Vector2D(velx, vely));
      }
    }
  SoundManager::getInstance().playSound(SOUND_BLAST_FIRE);
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
    switch (getShotType())
    {
    case ShotTypeCold:
    case ShotTypeIce:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_ICE);
      break;
    case ShotTypeFire:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_FIRE);
      break;
    case ShotTypeIllusion:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_ILLUSION);
      break;
    case ShotTypeLightning:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_LIGHTNING);
      break;
    case ShotTypePoison:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_POISON);
      break;
    case ShotTypeStone:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_STONE);
      break;
    default:
      SoundManager::getInstance().playPitchModSound(SOUND_BLAST_STANDARD);
      break;
    }

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
    if (equip[EQUIP_RAPID_SHOT])
    {
      Vector2D boltDirection;
      switch(direction)
      {
      case 4:
        boltDirection = Vector2D(0, 0).vectorNearlyTo(Vector2D(-1, 0), fireVelocity, 0.2f);
        break;
      case 6:
        boltDirection = Vector2D(0, 0).vectorNearlyTo(Vector2D(1, 0), fireVelocity, 0.2f);
        break;
      case 8:
        boltDirection = Vector2D(0, 0).vectorNearlyTo(Vector2D(0, -1), fireVelocity, 0.2f);
        break;
      case 2:
        boltDirection = Vector2D(0, 0).vectorNearlyTo(Vector2D(0, 1), fireVelocity, 0.2f);
        break;
      }
      generateBolt(boltDirection.x, boltDirection.y);
    }
    else if (!(equip[EQUIP_BOOK_DUAL] || equip[EQUIP_BOOK_DUAL_QUICK]) || (equip[EQUIP_BOOK_TRIPLE] || equip[EQUIP_BOOK_TRIPLE_QUICK]))
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
      BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypeStandard, 0);
      bolt->setDamages(fireDamages / 2);
      bolt->setFlying(isFairyTransmuted);
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
      BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypeStandard, 0);
      bolt->setDamages(fireDamages);
      bolt->setFlying(isFairyTransmuted);
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
          || (playerStatus == playerStatusAcquire && statusTimer < ACQUIRE_DELAY / 2));
}

int PlayerEntity::hurt(StructHurt hurtParam)
{
  if (playerStatus == playerStatusDead) return false;

  bool divinityInvoked = false;
  if (hp - hurtParam.damage <= hpMax / 4 && divinity.divinity >= 0)
  {
    divinityInvoked = triggerDivinityBefore();
    if (divinityInvoked)
      game().testAndAddMessageToQueue((EnumMessages)(MsgInfoDivIntervention));
  }

  if (invincibleDelay <= 0.0f || hurtParam.hurtingType == ShotTypeDeterministic)
  {
    SoundManager::getInstance().playSound(SOUND_PLAYER_HIT);
    int oldHp = hp;
    if (BaseCreatureEntity::hurt(hurtParam) > 0)
    {
      if (hurtParam.hurtingType != ShotTypeDeterministic)
      {
        invincibleDelay = INVINCIBLE_DELAY;
        if (equip[EQUIP_RAGE_AMULET]) rageFire();
        game().generateBlood(x, y, bloodColor);
      }

      hurtingDelay = HURTING_DELAY * 2.0f;
      game().generateBlood(x, y, bloodColor);
      game().proceedEvent(EventBeingHurted);

      lastHurtingEnemy = hurtParam.enemyType;
      lastHurtingSource = hurtParam.sourceType;

      // divinity
      offerHealth(oldHp - hp);
      if (!divinityInvoked && hp <= hpMax / 4 && divinity.divinity >= 0)
      {
        triggerDivinityAfter();
      }

      return true;
    }
  }

  return false;
}

void PlayerEntity::setMap(GameMap* map, int tileWidth, int tileHeight, int offsetX, int offsetY)
{
  CollidingSpriteEntity::setMap(map, tileWidth, tileHeight, offsetX, offsetY);
  //if (slimePet != NULL) slimePet->setMap(map, tileWidth, tileHeight, offsetX, offsetY);
}

void PlayerEntity::loseItem(enumItemType itemType, bool isEquip)
{
  CollidingSpriteEntity* itemSprite
    = new CollidingSpriteEntity(ImageManager::getInstance().getImage(isEquip ? IMAGE_ITEMS_EQUIP : IMAGE_ITEMS), x, y, 32, 32);
  itemSprite->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);
  itemSprite->setZ(-1);
  itemSprite->setFrame(itemType);
  itemSprite->setImagesProLine(10);
  itemSprite->setType(ENTITY_BLOOD);
  itemSprite->setVelocity(Vector2D(200 + rand()%450));
  itemSprite->setViscosity(0.95f);
  itemSprite->setSpin( (rand() % 700) - 350.0f);
}

void PlayerEntity::dying()
{
  if (divinity.divinity > -1 && divinity.interventions < divinity.level - 1)
  {
    hp = 1;
    return;
  }

  playerStatus = playerStatusDead;
  deathAge = 0.0f;
  hp = 0;
  SoundManager::getInstance().playSound(SOUND_PLAYER_DIE);
  setVelocity(Vector2D(0.0f, 0.0f));

  int i;
  for (i = 0; i < gold && i < 10; i++) loseItem(ItemCopperCoin, false);
  for (i = 0; i < 5; i++) game().generateBlood(x, y, BloodRed);
  for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    if (equip[i])
    {
      if (items[i + FirstEquipItem].familiar == FamiliarNone)
        loseItem(enumItemType(i), true);
    }
  }
  remove(SAVE_FILE.c_str());

  game().calculateScore();
}

void PlayerEntity::displayAcquiredGold(int n)
{
  std::ostringstream oss;
  if (n > 0) oss << "+";
  oss << n;
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
      game().proceedEvent(EventGetCoin);
      break;
    case ItemHealthVerySmallPoison:
      specialState[SpecialStatePoison].active = false;
    case ItemHealthVerySmall:
      heal(equip[EQUIP_MANUAL_HEALTH] ? 5 : 3);
      SoundManager::getInstance().playSound(SOUND_EAT);
      break;
    case ItemHealthSmall:
      heal(equip[EQUIP_MANUAL_HEALTH] ? 10 : 7);
      SoundManager::getInstance().playSound(SOUND_EAT);
      break;
    case ItemHealth:
      heal(equip[EQUIP_MANUAL_HEALTH] ? 22 : 15);
      SoundManager::getInstance().playSound(SOUND_EAT);
      break;
    default:
      break;
    }
}

void PlayerEntity::onClearRoom()
{
  if (divinity.divinity == DivinityHealer)
  {
    if (divinity.level > 1 && hp < hpMax)
    {
      divineInterventionDelay = WORSHIP_DELAY / 2;
      isRegeneration = true;
      if (divinity.level >= 5) heal(4);
      else if (divinity.level >= 4) heal(3);
      else if (divinity.level >= 3) heal(2);
      else if (divinity.level >= 2) heal(1);
    }
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

  for (int i = 0; i < NB_RESISTANCES; i++) resistance[i] = ResistanceStandard;

  if (equip[EQUIP_GLOVES_ADVANCED]) fireDelayBonus -= 0.15f;
  else if (equip[EQUIP_DISPLACEMENT_GLOVES]) fireDelayBonus -= 0.10f;
  if (equip[EQUIP_HAT_ADVANCED])
  {
    fireDelayBonus -= 0.3f;
    resistance[ResistanceIce] = (enumStateResistance)(resistance[ResistanceIce] - 1);
    resistance[ResistanceStone] = (enumStateResistance)(resistance[ResistanceStone] - 1);
    resistance[ResistanceLightning] = (enumStateResistance)(resistance[ResistanceLightning] - 1);
    resistance[ResistanceFire] = (enumStateResistance)(resistance[ResistanceFire] - 1);
  }

  else if (equip[EQUIP_MAGICIAN_HAT]) fireDelayBonus -= 0.2f;
  if (equip[EQUIP_LEATHER_BELT]) fireDelayBonus -= 0.15f;
  if (equip[EQUIP_BOOTS_ADVANCED]) creatureSpeedBonus += 0.25f;
  else if (equip[EQUIP_LEATHER_BOOTS]) creatureSpeedBonus += 0.15f;
  if (equip[EQUIP_BOOK_TRIPLE]) fireDelayBonus += 0.7f;
  else if (equip[EQUIP_BOOK_DUAL]) fireDelayBonus += 0.5f;

  if (equip[EQUIP_CRITICAL]) criticalChance += 5;
  if (equip[EQUIP_MANUAL_STAFF]) boltLifeTimeBonus += 0.4f;
  if (equip[EQUIP_MAHOGANY_STAFF])
  {
    fireVelocityBonus += 0.15f;
    fireDamagesBonus += 0.5f;
  }
  if (equip[EQUIP_BLOOD_SNAKE]) fireDamagesBonus += 0.5f;

  if (equip[EQUIP_ROBE_ADVANCED]) armor += 0.2f;
  else if (equip[EQUIP_MAGICIAN_ROBE]) armor += 0.15f;

  // divinity
  switch (divinity.divinity)
  {
  case (DivinityHealer):
    {
      break;
    }
  case (DivinityFighter):
    {
      if (divinity.level >= 5)
        fireDamagesBonus += 0.5f;
      else if (divinity.level >= 4)
        fireDamagesBonus += 0.375f;
      else if (divinity.level >= 3)
        fireDamagesBonus += 0.25f;
      else if (divinity.level >= 2)
        fireDamagesBonus += 0.125f;
      break;
    }
  case (DivinityIce):
    {
      if (divinity.level >= 5) resistance[ResistanceFrozen] = ResistanceVeryHigh;
      if (divinity.level >= 3) resistance[ResistanceIce] = (enumStateResistance)(resistance[ResistanceIce] - 1);
      break;
    }
  case (DivinityStone):
    {
      if (divinity.level >= 5) resistance[ResistanceRecoil] = ResistanceVeryHigh;
      if (divinity.level >= 3) resistance[ResistanceStone] = (enumStateResistance)(resistance[ResistanceStone] - 1);
      break;
    }
  case (DivinityAir):
    {
      if (divinity.level > 1) creatureSpeedBonus += (divinity.level - 1) * 0.04f;
      if (divinity.level >= 3) resistance[ResistanceLightning] = (enumStateResistance)(resistance[ResistanceLightning] - 1);
      break;
    }
  }

  fireDelay = INITIAL_PLAYER_FIRE_DELAY * fireDelayBonus;
  creatureSpeed = INITIAL_PLAYER_SPEED * creatureSpeedBonus;
  fireVelocity = INITIAL_BOLT_VELOCITY * fireVelocityBonus;
  fireDamages = INITIAL_BOLT_DAMAGES * fireDamagesBonus;
  boltLifeTime = INITIAL_BOLT_LIFE * boltLifeTimeBonus;

  // gems
  for (int i = 1; i < SPECIAL_SHOT_SLOTS; i++)
  {
    specialShotLevel[i] = 0;

    switch (specialShots[i])
    {
    case ShotTypeIce:
      if (equip[EQUIP_RING_ICE]) specialShotLevel[i]++;
      if (divinity.divinity == DivinityIce && divinity.level >= 4) specialShotLevel[i]++;
      break;

    case ShotTypeStone:
      if (equip[EQUIP_RING_STONE]) specialShotLevel[i]++;
      if (divinity.divinity == DivinityStone && divinity.level >= 4) specialShotLevel[i]++;
      break;

    case ShotTypeLightning:
      if (equip[EQUIP_RING_LIGHTNING]) specialShotLevel[i]++;
      if (divinity.divinity == DivinityAir && divinity.level >= 4) specialShotLevel[i]++;
      break;

    case ShotTypeIllusion:
      if (equip[EQUIP_RING_ILLUSION]) specialShotLevel[i]++;
      break;

    case ShotTypeFire:
      if (equip[EQUIP_RING_FIRE]) specialShotLevel[i]++;
      break;

    case ShotTypePoison:
      if (equip[EQUIP_RING_POISON]) specialShotLevel[i]++;
      break;

    default:
      break;
    }
  }
  if (getShotType() == ShotTypeIllusion) fireDamages *= ILLUSION_DAMAGE_DECREASE[getShotLevel()];
  else if (getShotType() == ShotTypeFire) fireDamages *= FIRE_DAMAGE_INCREASE[getShotLevel()];

  // divinity
  if (specialState[DivineStateProtection].active)
    armor += specialState[DivineStateProtection].param1;

  // post-computation
  if (equip[EQUIP_BOOK_TRIPLE_QUICK]) fireDamages *= 0.65f;
  else if (equip[EQUIP_BOOK_DUAL_QUICK]) fireDamages *= 0.75f;
  else if (equip[EQUIP_RAPID_SHOT])
  {
    fireDelay *= 0.20f;
    fireDamages *= 0.25f;
  }
  if (equip[EQUIP_ALCOHOL]) fireDamages *= 1.25f;

  // spells
  if (protection.active) armor += protection.value;

  if (armor > 1.0f) armor = 1.0f;

  // fairy ?
  if (isFairyTransmuted)
  {
    fireDamages *= 0.5f;
    creatureSpeed *= 1.5f;
    movingStyle = movFlying;
  }
  else
  {
    movingStyle = movWalking;
  }
}

void PlayerEntity::acquireStance(enumItemType type)
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
  playerStatus = playerStatusAcquire;
  statusTimer = ACQUIRE_DELAY;
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
  statusTimer = UNLOCK_DELAY;
  acquiredItem = (enumItemType)(type - FirstEquipItem);
  SoundManager::getInstance().playSound(SOUND_DOOR_OPENING_BOSS);
  equip[EQUIP_BOSS_KEY] = false;


  SpriteEntity* spriteItem = new SpriteEntity(
    ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP),
    x, y - 80.0f, ITEM_WIDTH, ITEM_HEIGHT);
  spriteItem->setFrame(EQUIP_BOSS_KEY);
  spriteItem->setZ(z);
  spriteItem->setAge(-UNLOCK_DELAY * 0.5f);
  spriteItem->setLifetime(UNLOCK_DELAY * 0.5f);
  spriteItem->setFading(true);
  spriteItem->setSpin(300);
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
    case ShotTypePoison:
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

void PlayerEntity::interact(EnumInteractionType interaction, int id)
{
  if (playerStatus == playerStatusPlaying)
  {
    // praying at the temple
    if (interaction == InteractionTypeTemple)
    {
      if (divinity.divinity == id)
      {
        // donation
        if (gold >= 10)
        {
          donate(10);
        }
      }
      else
      {
        worship((enumDivinityType)id);
      }
    }
    else if (interaction == InteractionTypeMerchandise)
    {
      if (itemToBuy != NULL) itemToBuy->buy();
    }
  }
}

float PlayerEntity::getBolPositionY()
{
  if (isFairyTransmuted)
    return y - 25;
  else
    return y - 20;
}

// DIVINITY
void PlayerEntity::donate(int n)
{
  if (gold >= n)
  {
    gold -= n;
    displayAcquiredGold(-n);
    SoundManager::getInstance().playSound(SOUND_PAY);

    // standard : 1 gold = 3 piety
    int pietyProGold = 3;
    if (divinity.divinity == DivinityHealer)
      pietyProGold = 5;

    addPiety(pietyProGold * n);

    // check item invoke
    bool divineGift = false;
    enumItemType itemType = ItemCopperCoin;

    if (divinity.level >= 3 && game().getItemsCount() == 0)
    {
      if (divinity.divinity == DivinityHealer && !equip[EQUIP_MANUAL_HEALTH])
      {
        // Healer + level 3 = Health manual
        divineGift = true;
        itemType = ItemManualHealth;
      }
      else if (divinity.divinity == DivinityIce && !equip[EQUIP_GEM_ICE])
      {
        divineGift = true;
        itemType = ItemGemIce;
      }
      else if (divinity.divinity == DivinityStone && !equip[EQUIP_GEM_STONE])
      {
        divineGift = true;
        itemType = ItemGemStone;
      }
      else if (divinity.divinity == DivinityAir && !equip[EQUIP_GEM_LIGHTNING])
      {
        divineGift = true;
        itemType = ItemGemLightning;
      }
    }

    if (divineGift)
    {
      float xItem = GAME_WIDTH / 2;
      float yItem = GAME_HEIGHT * 0.8f;
      new ItemEntity(itemType, xItem, yItem);
      SoundManager::getInstance().playSound(SOUND_OM);
      divineInterventionDelay = WORSHIP_DELAY / 2;
      isRegeneration = false;
      for (int i = 0; i < 8; i++)
      {
        game().generateStar(sf::Color::White, xItem, yItem);
        game().generateStar(sf::Color(255, 255, 210), xItem, yItem);
      }

      game().testAndAddMessageToQueue((EnumMessages)(MsgInfoDivGift));
    }
  }
}

void PlayerEntity::offerMonster(enemyTypeEnum monster, enumShotType hurtingType)
{
  if (divinity.divinity > -1)
  {
    // standard : 1 monster = 2 piety - 1 boss = 20 piety
    int pietyProMonster   = 2;
    int pietyProBoss      = 20;

    switch (divinity.divinity)
    {
    case DivinityHealer:
      if (monster == EnemyTypeGhost
          || monster == EnemyTypeZombie
          || monster == EnemyTypeZombieDark
          || monster == EnemyTypeImpBlue
          || monster == EnemyTypeImpRed
          || monster == EnemyTypeWitch
          || monster == EnemyTypeWitchRed
          || monster == EnemyTypeBogeyman)
        pietyProMonster   = 4;
      else
        pietyProMonster   = 0;
      break;

    case DivinityFighter:
      pietyProMonster = 3;
      pietyProBoss    = 30;
      break;

    case DivinityIce:
      if (monster == EnemyTypeSlimeRed
          || monster == EnemyTypeImpRed
          || monster == EnemyTypeEvilFlowerFire)
        pietyProMonster = 4;

      if (hurtingType == ShotTypeCold || hurtingType == ShotTypeIce)
      {
        pietyProMonster *= 1.5f;
        pietyProBoss = 25;
      }
      break;

    case DivinityStone:
      if (hurtingType == ShotTypeCold || hurtingType == ShotTypeIce)
      {
        pietyProMonster = 3;
        pietyProBoss    = 30;
      }
      else
      {
        pietyProBoss    = 25;
      }
      break;
    }


    if (monster < EnemyTypeButcher) // normal or mini-boss
    {
      addPiety(pietyProMonster);
    }
    else if (monster < EnemyTypeBat_invocated) // boss
    {
      addPiety(pietyProBoss);
    }
  }
}

void PlayerEntity::offerHealth(int lostHp)
{
  if (divinity.divinity == DivinityHealer)
  {
    addPiety(lostHp * 2.5f);
  }

}

void PlayerEntity::offerChallenge()
{
  if (divinity.divinity >= 0)
    addPiety(30);
}

void PlayerEntity::divineFury()
{
  enumShotType shotType = ShotTypeStandard;
  if (divinity.divinity == DivinityIce) shotType = ShotTypeIce;
  else if (divinity.divinity == DivinityStone) shotType = ShotTypeStone;
  else if (divinity.divinity == DivinityAir) shotType = ShotTypeLightning;

  int multBonus = 6;
  if (divinity.divinity == DivinityFighter) multBonus = 8;

  for (float i = 0.0f; i < 2 * PI; i +=  PI / 16)
  {
    BoltEntity* bolt = new BoltEntity(TILE_WIDTH * 1.5f + rand() % (MAP_WIDTH - 3) * TILE_WIDTH ,
                                      TILE_HEIGHT * 1.5f + rand() % (MAP_HEIGHT - 3) * TILE_HEIGHT,
                                      boltLifeTime, shotType, 0);
    bolt->setDamages(8 + divinity.level * multBonus);
    float velx = 400 * cos(i);
    float vely = 400 * sin(i);
    bolt->setVelocity(Vector2D(velx, vely));
    if (divinity.divinity == DivinityAir)
    {
      bolt->setFlying(true);
      bolt->setLifetime(-1.0f);
    }
    else
    {
      bolt->setLifetime(8.0f);
    }

    bolt->setViscosity(1.0f);
    bolt->setGoThrough(true);
    bolt->setFromPlayer(false);
  }
}

void PlayerEntity::divineDestroyUndead()
{
  game().destroyUndead(40);
}

void PlayerEntity::divineIce()
{
  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;
  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX)
    {
      EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(e);
      enemy->setSpecialState(SpecialStateIce, true, 10.0f, 0.1f, 0.0f);
    }
  }
}

void PlayerEntity::divineRepulse()
{
  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;
  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX)
    {
      EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(e);

      enemy->giveRecoil(true, Vector2D(x, y).vectorTo(Vector2D(enemy->getX(), enemy->getY()), 700.0f), 2.0f);
    }
  }

  // effect
  for (int i = 0; i < 40; i++)
  {
    SpriteEntity* spriteRock = new SpriteEntity(
                           ImageManager::getInstance().getImage(IMAGE_CYCLOP),
                            x, y, 64, 64);
    spriteRock->setZ(1000.0f);
    spriteRock->setImagesProLine(20);
    spriteRock->setFrame(rand() % 2 == 0 ? 38 : 58);
    spriteRock->setSpin(-100 + rand()%200);
    spriteRock->setVelocity(Vector2D(400 + rand()%400));
    spriteRock->setFading(true);
    spriteRock->setAge(-0.8f);
    spriteRock->setLifetime(2.0f);
    spriteRock->setType(ENTITY_EFFECT);
  }
  game().makeShake(1.0f);
  SoundManager::getInstance().playSound(SOUND_EARTHQUAKE);
}

void PlayerEntity::divineProtection(float duration, float armorBonus)
{
  setSpecialState(DivineStateProtection, true, 4.0f, 0.8f, 0.0f);
}

void PlayerEntity::divineHeal(int hpHealed)
{
  hp += hpHealed;
  if (hp > hpMax) hp = hpMax;
  specialState[SpecialStatePoison].active = false;
  divineInterventionDelay = WORSHIP_DELAY;
  isRegeneration = false;
}

bool PlayerEntity::triggerDivinityBefore()
{
  if (divinity.divinity > -1 && divinity.interventions < divinity.level - 1)
  {
    switch (divinity.divinity)
    {
    case DivinityHealer:
    {
      if (game().getUndeadCount() > 0 && rand() % 2 == 0)
      {
        SoundManager::getInstance().playSound(SOUND_OM);
        divinity.interventions ++;
        divineHeal(hpMax / 2);
        divineDestroyUndead();
        game().makeColorEffect(X_GAME_COLOR_WHITE, 0.45f);
        return true;
      }
      break;
    }
    case DivinityFighter:
    {
      int r = rand() % 3;
      if (r == 0) return false;

      SoundManager::getInstance().playSound(SOUND_OM);
      divinity.interventions ++;
      divineHeal(hpMax / 3);
      if (r == 1) divineProtection(5.0f, 0.8f);
      else divineFury();
      game().makeColorEffect(X_GAME_COLOR_RED, 0.45f);
      return true;
      break;
    }
    case DivinityIce:
    {
      int r = rand() % 3;
      if (r == 0) return false;

      SoundManager::getInstance().playSound(SOUND_OM);
      divinity.interventions ++;
      divineHeal(hpMax / 3);
      if (r == 1)
      {
        divineIce();
        game().makeColorEffect(X_GAME_COLOR_BLUE, 7.5f);
      }
      else
      {
        divineFury();
        game().makeColorEffect(X_GAME_COLOR_BLUE, 0.5f);
      }
      return true;
      break;
    }
    case DivinityStone:
    {
      int r = rand() % 3;
      r = 1;
      divineProtection(10.0f, 0.5f);
      if (r == 0) return false;

      SoundManager::getInstance().playSound(SOUND_OM);
      divinity.interventions ++;
      divineHeal(hpMax / 3);
      if (r == 1)
      {
        divineRepulse();
        game().makeColorEffect(X_GAME_COLOR_BROWN, 3.0f);
      }
      else
      {
        divineFury();
        game().makeColorEffect(X_GAME_COLOR_BROWN, 0.5f);
      }
      return true;
      break;
    }
    case DivinityAir:
    {
      int r = rand() % 3;
      if (r == 0) return false;

      SoundManager::getInstance().playSound(SOUND_OM);
      divinity.interventions ++;
      divineHeal(hpMax / 3);
      /*if (r == 1)
      {
        divineIce();
        game().makeColorEffect(X_GAME_COLOR_BLUE, 7.5f);
      }
      else*/
      {
        divineFury();
        game().makeColorEffect(X_GAME_COLOR_VIOLET, 0.5f);
      }
      return true;
      break;
    }
    }
  }
  return false;
}

void PlayerEntity::triggerDivinityAfter()
{
  if (divinity.divinity > -1 && divinity.interventions < divinity.level - 1)
  {
    switch (divinity.divinity)
    {
    case DivinityHealer:
      {
        SoundManager::getInstance().playSound(SOUND_OM);
        divinity.interventions ++;
        divineHeal(hpMax);
        break;
      }
    //case DivinityFighter:
    default:
      {
        SoundManager::getInstance().playSound(SOUND_OM);
        divinity.interventions ++;
        divineHeal(hpMax / 2);
        break;
      }
    }
    game().testAndAddMessageToQueue((EnumMessages)(MsgInfoDivIntervention));
  }
}

void PlayerEntity::addPiety(int n)
{
  if (equip[EQUIP_BOOK_PRAYER_I]) n *= 1.5f;

  int oldLevel = divinity.level;
  divinity.piety += n;
  if (divinity.piety >= DIVINITY_LEVEL_TRESHOLD[MAX_DIVINITY_LEVEL - 1])
  {
    divinity.piety = DIVINITY_LEVEL_TRESHOLD[MAX_DIVINITY_LEVEL - 1];
    divinity.percentsToNextLevels = 1.0f;
    game().registerAchievement(AchievementPietyMax);
    divinity.level = MAX_DIVINITY_LEVEL + 1;
  }
  else
  {
    int i = 0;
    while (divinity.piety > DIVINITY_LEVEL_TRESHOLD[i] && i < (MAX_DIVINITY_LEVEL + 1)) i++;
    divinity.level = i + 1;

    if (divinity.level == 1)
      divinity.percentsToNextLevels = (float)divinity.piety / (float)DIVINITY_LEVEL_TRESHOLD[0];
    else
      divinity.percentsToNextLevels
        = (float)(divinity.piety - DIVINITY_LEVEL_TRESHOLD[divinity.level - 2])
          / (float)(DIVINITY_LEVEL_TRESHOLD[divinity.level - 1] - DIVINITY_LEVEL_TRESHOLD[divinity.level - 2]);
  }

  if (divinity.level > oldLevel)
  {
    SoundManager::getInstance().playSound(SOUND_OM);
    divineInterventionDelay = WORSHIP_DELAY / 2;
    isRegeneration = false;
    pietyLevelUp();
    computePlayer();
  }
}

void PlayerEntity::pietyLevelUp()
{
  std::string label = "";
  switch (divinity.divinity)
  {
  case DivinityFighter:
    label = "div_fighter_lvl";
    break;
  case DivinityHealer:
    if (divinity.level == 2) label = "div_healer_lvl_2";
    else label = "div_healer_lvl_3";
    break;
  case DivinityIce:
    if (divinity.level == 3) label = "div_ice_lvl_3";
    else if (divinity.level == 4) label = "div_ice_lvl_4";
    else if (divinity.level == 5) label = "div_ice_lvl_5";
    break;
  case DivinityStone:
    if (divinity.level == 3) label = "div_stone_lvl_3";
    else if (divinity.level == 4) label = "div_stone_lvl_4";
    else if (divinity.level == 5) label = "div_stone_lvl_5";
    break;
  case DivinityAir:
    if (divinity.level == 4) label = "div_air_lvl_4";
    else label = "div_air_lvl";
    break;
  }

  if (label.compare("") != 0) game().addDivLevelMessageToQueue(label);
}

void PlayerEntity::worship(enumDivinityType id)
{
  int oldPiety = divinity.piety;
  bool isReconversion = divinity.divinity > -1;

  playerStatus = playerStatusPraying;
  statusTimer = WORSHIP_DELAY;
  SoundManager::getInstance().playSound(SOUND_OM);
  divinity.divinity = id;
  divinity.piety = 0;
  divinity.level = 1;
  divinity.percentsToNextLevels = 0.0f;
  facingDirection = 2;

  // text
  float x0 = MAP_WIDTH * 0.5f * TILE_WIDTH;
  float y0 = MAP_HEIGHT * 0.5f * TILE_HEIGHT + 140.0f;
  std::stringstream ss;
  ss << tools::getLabel("worshipping") << " ";
  ss << tools::getLabel(divinityLabel[divinity.divinity] + "_0");
  TextEntity* text = new TextEntity(ss.str(), 24, x0, y0);
  text->setAlignment(ALIGN_CENTER);
  text->setLifetime(2.5f);
  text->setWeight(-36.0f);
  text->setZ(1200);
  text->setColor(TextEntity::COLOR_FADING_WHITE);

  // reconversion
  if (isReconversion)
  {
    addPiety((equip[EQUIP_BOOK_PRAYER_I]) ? 0.66 * oldPiety : 0.5 * oldPiety);
    if (divinity.interventions > divinity.level - 1)
      divinity.interventions = divinity.level - 1;
  }
  else
    divinity.interventions = 0;

  // message
  game().testAndAddMessageToQueue((EnumMessages)(MsgInfoDivHealer + (int)id));
}

void PlayerEntity::loadDivinity(int id, int piety, int level, int interventions)
{
  divinity.divinity = id;
  divinity.piety = piety;
  divinity.level = level;
  divinity.interventions = interventions;
  if (id >= 0) addPiety(0);
}

// MAGIC

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

  case SpellFlower:
    activeSpell.delayMax = 80.0f;
    activeSpell.frame = ItemSpellFlower - FirstEquipItem;
    break;

  case SpellFairy:
    activeSpell.delayMax = 10.0f;
    activeSpell.frame = ItemSpellFairy - FirstEquipItem;
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
      spellAnimationDelay = spellAnimationDelayMax;
      castFreeze();
      break;
    case SpellEarthquake:
      spellAnimationDelay = spellAnimationDelayMax;
      castEarthquake();
      break;
    case SpellProtection:
      spellAnimationDelay = spellAnimationDelayMax;
      castProtection();
      break;
    case SpellWeb:
      castWeb();
      break;
    case SpellFlower:
      spellAnimationDelay = spellAnimationDelayMax;
      castSummonsFlower();
      break;
    case SpellFairy:
      castTransmuteFairy();
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
      xNew = xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
      yNew = ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

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
  new FallingRockEntity(rx * TILE_WIDTH + TILE_WIDTH / 2,
                        ry * TILE_HEIGHT + TILE_HEIGHT / 2,
                        rand() % 3,
                        true);
}

void PlayerEntity::castSummonsSlimeExplode()
{
  SlimeEntity* slime = new SlimeEntity( ((int)(x) / TILE_WIDTH) * TILE_WIDTH + TILE_WIDTH * 0.5f,
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

  BoltEntity* bolt = new BoltEntity(x, getBolPositionY(), boltLifeTime + 0.5f, boltType, shotLevel);

  int boltDamage = fireDamages * (equip[EQUIP_BOOK_MAGIC_II] ? 4 : 3);
  if (equip[EQUIP_BOOK_MAGIC_II] && boltDamage < 16) boltDamage = 16;
  else if (!equip[EQUIP_BOOK_MAGIC_II] && boltDamage < 12) boltDamage = 12;
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
    BoltEntity* bolt1 = new BoltEntity(x, getBolPositionY(), boltLifeTime, ShotTypeIce, iceLevel);
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

void PlayerEntity::castSummonsFlower()
{
  SoundManager::getInstance().playSound(SOUND_INVOKE);
  EvilFlowerEntity* flower = new EvilFlowerEntity(x, y, FlowerTypePet);
  flower->setLifetime(equip[EQUIP_BOOK_MAGIC_II] ? 45 : 35);
  if (equip[EQUIP_BOOK_MAGIC_II]) flower->setFireDelayMax(EVIL_FLOWER_FIRE_DELAY * 0.8f);
}

void PlayerEntity::castTransmuteFairy()
{
  if (isFairyTransmuted)
  {
    movingStyle = movWalking;
    if (isCollidingWithMap())
      movingStyle = movFlying;
    else
    {
      SoundManager::getInstance().playSound(SOUND_INVOKE);
      isFairyTransmuted = false;
      computePlayer();
      for(int i=0; i < 6; i++)
      {
        generateStar(sf::Color(50, 50, 255, 255));
        generateStar(sf::Color(200, 200, 255, 255));
      }
    }
  }
  else
  {
    SoundManager::getInstance().playSound(SOUND_INVOKE);
    isFairyTransmuted = true;
    computePlayer();
    for(int i=0; i < 6; i++)
    {
      generateStar(sf::Color(50, 50, 255, 255));
      generateStar(sf::Color(200, 200, 255, 255));
    }
  }
}
