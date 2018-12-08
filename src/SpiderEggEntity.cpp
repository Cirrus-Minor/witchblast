#include "SpiderEggEntity.h"
#include "PlayerEntity.h"
#include "LittleSpiderEntity.h"
#include "BoltEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SpiderEggEntity::SpiderEggEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_SPIDER_EGG), x, y)
{
  imagesProLine = 20;
  type = invocated ? ENTITY_ENEMY_INVOCATED : ENTITY_ENEMY;
  enemyType = invocated ? EnemyTypeSpiderEgg_invocated : EnemyTypeSpiderEgg;
  this->invocated = invocated;
  movingStyle = movFlying;
  bloodColor = invocated ? BloodNone : BloodGreen;
  deathFrame = FRAME_CORPSE_SPIDER_EGG;
  dyingSound = rand() %  2 == 0 ? SOUND_EGG_SMASH_00 : SOUND_EGG_SMASH_01;
  meleeDamages = 5;

  age = 0.0f;
  h = 1800 + rand() % 1000;
  hp = 24;
  jumping = false;
  hVelocity = 0.0f;

  lifetime = 12.0f + (rand() % 700) / 100;
}

void SpiderEggEntity::animate(float delay)
{
  if (movingStyle == movFlying)
  {
    if (jumping)
    {
      hVelocity -= 300.0f * delay;
      h += hVelocity * delay;

      if (h <= 0.0f)
      {
        movingStyle = movWalking;
        h = 0.0f;
      }
    }
    else
    {
      h -= delay * 750.0f;
      if (h < 0.0f)
      {
        h = 0.0f;
        hVelocity = 100.0f;
        jumping = true;
      }
    }
  }
  EnemyEntity::animate(delay);
  z = y + 25;

  frame = 0;
  if (lifetime - age < 1.0f) frame = 3;
  else if (lifetime - age < 2.5f) frame = 2;
  else if (lifetime - age < 5.0f) frame = 1;
}

void SpiderEggEntity::dyingFromAge()
{
  LittleSpiderEntity* ls = new LittleSpiderEntity(x, y, SpiderTypeStandard, invocated);
  ls->setAge(-0.3f);

  for (int i = 0; i < 4; i++)
  {
    SpriteEntity* blood = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BLOOD), x, y, 16, 16, 6);
    blood->setZ(-1);
    blood->setFrame(18 + rand()%6);
    blood->setType(ENTITY_BLOOD);
    blood->setVelocity(Vector2D(rand()%250));
    blood->setViscosity(0.95f);
    float bloodScale = 1.0f + (rand() % 10) * 0.1f;
    blood->setScale(bloodScale, bloodScale);
  }

  SoundManager::getInstance().playSound(SOUND_EGG_SMASH_00 + rand() %  2);

  isDying = true;
}

void SpiderEggEntity::render(sf::RenderTarget* app)
{
  int nx = frame % imagesProLine;
  int ny = frame / imagesProLine;

  // shadow
  if (h <= 1600)
  {
    int f = 1600 - h;
    if (f > 255) f = 255;

    sprite.setColor(sf::Color(255, 255, 255, f));
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(4 * width, 0, width, height));
    app->draw(sprite);
    sprite.setColor(sf::Color(255, 255, 255, 255));
  }

  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void SpiderEggEntity::calculateBB()
{
  boundingBox.left = (int)x - 20;
  boundingBox.width = 40;
  boundingBox.top = (int)y - 5;
  boundingBox.height =  30;
}

bool SpiderEggEntity::canCollide()
{
  return h < 70;
}

void SpiderEggEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (movingStyle == movFlying && playerEntity != NULL && !playerEntity->isDead())
      {
        if (playerEntity->hurt(getHurtParams(meleeDamages, ShotTypeStandard, 0, false, SourceTypeMelee, enemyType, false)))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);
        }
        inflictsRepulsionTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        EnemyEntity::collideWithBolt(boltEntity);
      }
    }
  }
}
