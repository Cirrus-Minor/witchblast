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
  imagesProLine = 3;

  this->flowerType = flowerType;

  bloodColor = BloodGreen;

  if (flowerType == FlowerTypeStandard)
  {
    frame = 0;
    enemyType = EnemyTypeEvilFlower;
    deathFrame = FRAME_CORPSE_FLOWER;
  }
  else if (flowerType == FlowerTypeIce)
  {
    frame = 3;
    enemyType = EnemyTypeEvilFlowerIce;
    deathFrame = FRAME_CORPSE_FLOWER_ICE;

    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceHigh;
    resistance[ResistanceFire] = ResistanceLow;
  }

  fireDelay = EVIL_FLOWER_FIRE_DELAY;
  age = -1.0f + (rand() % 2500) * 0.001f;
}

void EvilFlowerEntity::animate(float delay)
{
  float flowerDelay = delay;
  if (specialState[SpecialStateIce].active) flowerDelay = delay * specialState[SpecialStateIce].param1;

  if (fireDelay < 0.7f) setSpin(500.0f);
  else if (fireDelay < 1.4f) setSpin(120.0f);
  else setSpin(50.0f);

  EnemyEntity::animate(delay);
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

void EvilFlowerEntity::fire()
{
    SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);

    if (flowerType == FlowerTypeIce)
    {
      for (int i = 0; i < 2; i++)
      {
        EnemyBoltEntity* bolt = new EnemyBoltEntity
            (x, y + 10, ShotTypeIce, 0, enemyType);
        bolt->setDamages(EVIL_FLOWER_MISSILE_DAMAGES);
        bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

        float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;

        float fireAngle = Vector2D(x, y).angleTo(game().getPlayerPosition());
        std::cout << fireAngle << " - " << fireAngle * 360 / (2 * PI) << std::endl;
        if (i == 0) fireAngle += 0.1f;
        else fireAngle -= 0.1f;

        if (game().getPlayerPosition().y > y)
          bolt->setVelocity(Vector2D(sin(fireAngle) * flowerFireVelocity, cos(fireAngle) * flowerFireVelocity));
        else
          bolt->setVelocity(Vector2D(-sin(fireAngle) * flowerFireVelocity, -cos(fireAngle) * flowerFireVelocity));
      }
    }
    else
    {
      EnemyBoltEntity* bolt = new EnemyBoltEntity
            (x, y + 10, ShotTypeStandard, 0, enemyType);
      bolt->setDamages(EVIL_FLOWER_MISSILE_DAMAGES);
      bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

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
    sprite.setTextureRect(sf::IntRect(width * 2, 0,  width, height));
    app->draw(sprite);
    sprite.setTextureRect(sf::IntRect(width, 0,  width, height));
    app->draw(sprite);

    sprite.setRotation(savedAngle);
    EnemyEntity::render(app);
}
