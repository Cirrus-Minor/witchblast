#include "SpiderWebEntity.h"
#include "PlayerEntity.h"
#include "BoltEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SpiderWebEntity::SpiderWebEntity(float x, float y, bool isFromPlayer)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_SPIDER_WEB), x, y)
{
  imagesProLine = 20;
  if (isFromPlayer) type = ENTITY_ENEMY_NC;
  else type = ENTITY_ENEMY_INVOCATED;
  enemyType = EnemyTypeSpiderWeb;
  movingStyle = movFlying;
  bloodColor = BloodNone; // web don't bleed
  deathFrame = FRAME_CORPSE_SPIDER_WEB;

  this->isFromPlayer = isFromPlayer;

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

  canExplode = false;
}

void SpiderWebEntity::animate(float delay)
{
  EnemyEntity::animate(delay);
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

void SpiderWebEntity::collideWithEnemy(EnemyEntity* enemyEntity)
{
  if (enemyEntity->getMovingStyle() != movFlying
      && enemyEntity->getEnemyType() != EnemyTypeSpiderLittle_invocated
      && enemyEntity->getEnemyType() != EnemyTypeSpiderGiant
      && enemyEntity->getEnemyType() != EnemyTypeSpiderEgg_invocated
      && enemyEntity->getEnemyType() != EnemyTypeSpiderWeb)
  {
    if (!enemyEntity->isSpecialStateActive(SpecialStateSlow))
    {
      enemyEntity->setSpecialState(SpecialStateSlow, true, 0.15f, 0.25f, 0.0f);
      hurt(2, ShotTypeStandard, 0, false, -1);
    }
  }
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

      if (!isFromPlayer && playerEntity != NULL && !playerEntity->isDead())
      {
        if (!playerEntity->isSpecialStateActive(SpecialStateSlow))
        {
          playerEntity->setSpecialState(SpecialStateSlow, true, 0.1f, 0.33f, 0.0f);
          // TODO
          hurt(2, ShotTypeStandard, 0, false, -1);
        }
      }

      else if (!isFromPlayer && boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        float xs = (x + boltEntity->getX()) / 2;
        float ys = (y + boltEntity->getY()) / 2;

        boltEntity->collide();

        // TODO
        hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel(), boltEntity->isCritical(), -1);
        if (bloodColor > BloodNone) game().generateBlood(x, y, bloodColor);
        SoundManager::getInstance().playSound(SOUND_IMPACT);

        SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_STAR_2), xs, ys);
        star->setFading(true);
        star->setZ(y+ 100);
        star->setLifetime(0.7f);
        star->setType(ENTITY_EFFECT);
        star->setSpin(400.0f);
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX)
      {
        if (this != entity)
        {
          EnemyEntity* enemyEntity = static_cast<EnemyEntity*>(entity);
          if (enemyEntity->canCollide()) collideWithEnemy(enemyEntity);
        }
      }
    }
  }
}

