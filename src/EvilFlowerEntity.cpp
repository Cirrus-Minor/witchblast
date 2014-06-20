#include "EvilFlowerEntity.h"
#include "BoltEntity.h"
#include "EnnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include <math.h>

EvilFlowerEntity::EvilFlowerEntity(float x, float y)
    : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_FLOWER), x, y)
{
  hp = EVIL_FLOWER_HP;
  meleeDamages = EVIL_FLOWER_MELEE_DAMAGES;

  setSpin(50.0f);
  frame = 0;

  type = 23;
  bloodColor = bloodGreen;
  //shadowFrame = 2;

  fireDelay = EVIL_FLOWER_FIRE_DELAY;
  age = -1.0f + (rand() % 2500) * 0.001f;
}

void EvilFlowerEntity::animate(float delay)
{
  float flowerDelay = delay;
  if (specialState[SpecialStateIce].active) flowerDelay = delay * STATUS_FROZEN_MULT[specialState[SpecialStateIce].level];

  if (fireDelay < 0.7f) setSpin(500.0f);
  else if (fireDelay < 1.4f) setSpin(120.0f);
  else setSpin(50.0f);

  EnnemyEntity::animate(delay);
  angle += spin * flowerDelay;

  if (age > 0.0f)
  {
    fireDelay -= flowerDelay;
    if (fireDelay <= 0.0f)
    {
      fireDelay = EVIL_FLOWER_FIRE_DELAY;
      fire();
    }
  }
}

void EvilFlowerEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + EVIL_FLOWER_BB_LEFT;
    boundingBox.width = width - EVIL_FLOWER_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + EVIL_FLOWER_BB_TOP;
    boundingBox.height =  height - EVIL_FLOWER_BB_HEIGHT_DIFF;
}

void EvilFlowerEntity::dying()
{
  isDying = true;
  SpriteEntity* deadFlower = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadFlower->setZ(OFFSET_Y);
  deadFlower->setFrame(FRAME_CORPSE_FLOWER);
  deadFlower->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);
  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}

void EvilFlowerEntity::fire()
{
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);
    EnnemyBoltEntity* bolt = new EnnemyBoltEntity
          (ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y + 10);
    bolt->setFrame(1);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), flowerFireVelocity ));
}

void EvilFlowerEntity::render(sf::RenderTarget* app)
{
    sprite.setPosition(x, y);
    float savedAngle = sprite.getRotation();
    sprite.setRotation(0.0f);

    // shadow
    sprite.setTextureRect(sf::IntRect(width * 2, 0,  width, height));
    app->draw(sprite);
    sprite.setTextureRect(sf::IntRect(width, 0,  width, height));
    app->draw(sprite);

    sprite.setRotation(savedAngle);
    EnnemyEntity::render(app);
}
