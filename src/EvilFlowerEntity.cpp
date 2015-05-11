#include "EvilFlowerEntity.h"
#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include <math.h>

EvilFlowerEntity::EvilFlowerEntity(float x, float y, flowerTypeEnum flowerType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_FLOWER), x, y)
{
  hp = EVIL_FLOWER_HP;
  meleeDamages = EVIL_FLOWER_MELEE_DAMAGES;

  setSpin(50.0f);
  imagesProLine = 5;

  this->flowerType = flowerType;

  bloodColor = BloodGreen;
  fireDelayMax = EVIL_FLOWER_FIRE_DELAY;
  fireDelay = EVIL_FLOWER_FIRE_DELAY;

  if (flowerType == FlowerTypeStandard)
  {
    frame = 0;
    enemyType = EnemyTypeEvilFlower;
    deathFrame = FRAME_CORPSE_FLOWER;
  }
  else if (flowerType == FlowerTypeIce)
  {
    frame = 1;
    enemyType = EnemyTypeEvilFlowerIce;
    deathFrame = FRAME_CORPSE_FLOWER_ICE;

    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceVeryHigh;
    resistance[ResistanceFire] = ResistanceVeryLow;
  }
  else if (flowerType == FlowerTypeFire)
  {
    frame = 2;
    enemyType = EnemyTypeEvilFlowerFire;
    deathFrame = FRAME_CORPSE_FLOWER_FIRE;

    resistance[ResistanceIce] = ResistanceVeryLow;
    resistance[ResistanceFire] = ResistanceVeryHigh;
  }
  else if (flowerType == FlowerTypePet)
  {
    frame = 3;
    enemyType = EnemyTypeNone;
    fireDelayMax = EVIL_FLOWER_FIRE_DELAY;
    fireDelay = EVIL_FLOWER_FIRE_DELAY;
    type = ENTITY_EFFECT;
    bloodColor = BloodNone;
  }

  age = -1.0f + (rand() % 2500) * 0.001f;
  ageFromGen = 0.0f;
  z = y + height * 0.5f;
}

void EvilFlowerEntity::animate(float delay)
{
  ageFromGen += (delay * 2.0f);
  float flowerDelay = delay;
  if (specialState[SpecialStateIce].active) flowerDelay = delay * specialState[SpecialStateIce].param1;

  if (fireDelay < 0.7f) setSpin(500.0f);
  else if (fireDelay < 1.4f) setSpin(120.0f);
  else setSpin(50.0f);

  if (flowerType == FlowerTypePet)
  {
    age += delay;
    if (age > lifetime)
    {
      isDying = true;
      SoundManager::getInstance().playSound(SOUND_INVOKE);
      for (int i = 0; i < 8; i++)
      {
        game().generateStar(sf::Color::White, x, y);
        game().generateStar(sf::Color(210, 210, 255), x, y);
      }
    }
  }
  else
    EnemyEntity::animate(flowerDelay);

  angle += spin * flowerDelay;

  if (age > 0.0f)
  {
    fireDelay -= flowerDelay;
    if (fireDelay <= 0.0f)
    {
      if (flowerType == FlowerTypePet)
      {
        fire();
      }
      else if (canSee(game().getPlayerPosition().x, game().getPlayerPosition().y))
      {
        fireDelay = EVIL_FLOWER_FIRE_DELAY;
        fire();
      }
      else
        fireDelay = 0.35f;
    }
  }
}

bool EvilFlowerEntity::canCollide()
{
  return (!isAgonising && flowerType != FlowerTypePet && ageFromGen > 0.8f);
}

void EvilFlowerEntity::setFireDelayMax(float fireDelayMax)
{
  this->fireDelayMax = fireDelayMax;
}

void EvilFlowerEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + EVIL_FLOWER_BB_LEFT;
  boundingBox.width = width - EVIL_FLOWER_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - height / 2 + EVIL_FLOWER_BB_TOP;
  boundingBox.height =  height - EVIL_FLOWER_BB_HEIGHT_DIFF;
}

void EvilFlowerEntity::fire()
{
  if (flowerType == FlowerTypeIce)
  {
    SoundManager::getInstance().playSound(SOUND_BLAST_ICE);
    for (int i = 0; i < 2; i++)
    {
      EnemyBoltEntity* bolt = new EnemyBoltEntity
      (x, y + 10, ShotTypeIce, 0, enemyType);
      bolt->setDamages(EVIL_FLOWER_MISSILE_DAMAGES);
      bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

      float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;

      float fireAngle = Vector2D(x, y).angleTo(game().getPlayerPosition());
      if (i == 0) fireAngle += 0.1f;
      else fireAngle -= 0.1f;

      if (game().getPlayerPosition().y > y)
        bolt->setVelocity(Vector2D(sin(fireAngle) * flowerFireVelocity, cos(fireAngle) * flowerFireVelocity));
      else
        bolt->setVelocity(Vector2D(-sin(fireAngle) * flowerFireVelocity, -cos(fireAngle) * flowerFireVelocity));
    }
  }
  else if (flowerType == FlowerTypeFire)
  {
    SoundManager::getInstance().playSound(SOUND_BLAST_FIRE);
    for (int i = 0; i < 2; i++)
    {
      EnemyBoltEntity* bolt = new EnemyBoltEntity
      (x, y + 10, ShotTypeFire, 0, enemyType);
      bolt->setDamages(EVIL_FLOWER_FIRE_MISSILE_DAMAGES);
      bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

      float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;

      float fireAngle = Vector2D(x, y).angleTo(game().getPlayerPosition());
      if (i == 0) fireAngle += 0.1f;
      else fireAngle -= 0.1f;

      if (game().getPlayerPosition().y > y)
        bolt->setVelocity(Vector2D(sin(fireAngle) * flowerFireVelocity, cos(fireAngle) * flowerFireVelocity));
      else
        bolt->setVelocity(Vector2D(-sin(fireAngle) * flowerFireVelocity, -cos(fireAngle) * flowerFireVelocity));
    }
  }
  else if (flowerType == FlowerTypePet)
    {
      Vector2D target = game().getNearestEnemy(x, y);
      if (target.x > -1.0f)
      {
        SoundManager::getInstance().playSound(SOUND_BLAST_STANDARD);
        BoltEntity* bolt = new BoltEntity (x, y + 10, 6, ShotTypeStandard, 0);
        bolt->setVelocity(Vector2D(x, y).vectorTo(target, 2 * EVIL_FLOWER_FIRE_VELOCITY));
        bolt->setViscosity(1.0f);
        fireDelay = fireDelayMax;
      }
      else
        fireDelay = 0.35f;
    }
  else
  {
    SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);
    EnemyBoltEntity* bolt = new EnemyBoltEntity
    (x, y + 10, ShotTypeStandard, 0, enemyType);
    bolt->setDamages(EVIL_FLOWER_MISSILE_DAMAGES);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), flowerFireVelocity ));
  }

}

void EvilFlowerEntity::render(sf::RenderTarget* app)
{
  sprite.setPosition(x, y);
  float savedAngle = sprite.getRotation();
  sprite.setRotation(0.0f);

  // shadow
  sprite.setTextureRect(sf::IntRect(width * 4, 0,  width, height));
  if (ageFromGen > 1.0f)
  {
    if (ageFromGen >= 2.0f) app->draw(sprite);
    else
    {
      float scale = ageFromGen - 1.0f;
      sprite.setScale(scale, scale);
      app->draw(sprite);
      sprite.setScale(1.0f, 1.0f);
    }
  }

  // stem
  int stemFrame = 14;
  if (ageFromGen < 1.0f) stemFrame = 5 + ageFromGen * 10;

  sprite.setTextureRect(sf::IntRect(width * (stemFrame % imagesProLine), height * (stemFrame / imagesProLine),  width, height));
  app->draw(sprite);

  sprite.setRotation(savedAngle);

  if (ageFromGen > 1.0f)
  {
    if (ageFromGen >= 2.0f) EnemyEntity::render(app);
    else
    {
      float scale = ageFromGen - 1.0f;
      sprite.setScale(scale, scale);
      EnemyEntity::render(app);
      sprite.setScale(1.0f, 1.0f);
    }
  }
}
