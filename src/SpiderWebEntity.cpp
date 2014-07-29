#include "SpiderWebEntity.h"
#include "PlayerEntity.h"
#include "BoltEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SpiderWebEntity::SpiderWebEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_SPIDER_WEB), x, y)
{
  imagesProLine = 20;
  type = ENTITY_ENNEMY_INVOCATED;
  enemyType = EnemyTypeSpiderWeb;
  movingStyle = movFlying;
  bloodColor = bloodNone; // web don't bleed
  deathFrame = FRAME_CORPSE_SPIDER_WEB;

  age = 0.0f;

  frame = 0;

  setVelocity(Vector2D(80 + rand()% 500));
  width = 128.0f;
  height = 128.0f;
  sprite.setOrigin(64.0f, 64.0f);
  viscosity = 0.97f;
  hp = 40;
  hpMax = 40;

  // avoid collisions
  calculateBB();
  float x0 = OFFSET_X + TILE_WIDTH + 1;
  float xf = OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1) - 1;
  float y0 = OFFSET_Y + TILE_HEIGHT + 1;
  float yf = OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1) - 1;

  if (boundingBox.left < x0) this->x += (x0 - boundingBox.left);
  else if (boundingBox.left + boundingBox.width > xf) this->x -= (boundingBox.left + boundingBox.width - xf);

  if (boundingBox.top < y0) this->y += (y0 - boundingBox.top);
  else if (boundingBox.top + boundingBox.height > yf) this->y -= (boundingBox.top + boundingBox.height - yf);

  resistance[ResistanceFrozen] = ResistanceImmune;
  resistance[ResistanceRecoil] = ResistanceImmune;
  resistance[ResistanceFire] = ResistanceVeryLow;
  resistance[ResistanceStone] = ResistanceVeryLow;
}

void SpiderWebEntity::animate(float delay)
{
  EnnemyEntity::animate(delay);
  if (age <= 0.5f)
  {
    float scale = age * 2.0f;
    sprite.setScale(scale,scale);
  }
  else
    sprite.setScale(1.0f, 1.0f);

  int color = 177 + 78 * hp / hpMax;
  sprite.setColor(sf::Color(color, color, color, 255));
}

void SpiderWebEntity::calculateBB()
{
  boundingBox.left = (int)x - 45;
  boundingBox.width = 90;
  boundingBox.top = (int)y - 45;
  boundingBox.height =  90;
}

void SpiderWebEntity::collideMapRight()
{
  collideWall();
}

void SpiderWebEntity::collideMapLeft()
{
  collideWall();
}

void SpiderWebEntity::collideMapTop()
{
  collideWall();
}

void SpiderWebEntity::collideMapBottom()
{
  collideWall();
}

void SpiderWebEntity::collideWall()
{
  velocity.x = 0.0f;
  velocity.y = 0.0f;
}

void SpiderWebEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
}

void SpiderWebEntity::drop()
{
}

void SpiderWebEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        if (!playerEntity->isSpecialStateActive(SpecialStateSlow))
        {
          playerEntity->setSpecialState(SpecialStateSlow, true, 0.1f, 0.33f);
          hurt(2, ShotTypeStandard, 0);
        }
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        float xs = (x + boltEntity->getX()) / 2;
        float ys = (y + boltEntity->getY()) / 2;

        boltEntity->collide();

        hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel());
        if (bloodColor > bloodNone) game().generateBlood(x, y, bloodColor);
        SoundManager::getSoundManager()->playSound(SOUND_IMPACT);

        SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
        star->setFading(true);
        star->setZ(y+ 100);
        star->setLifetime(0.7f);
        star->setType(16);
        star->setSpin(400.0f);
      }
    }
  }
}

