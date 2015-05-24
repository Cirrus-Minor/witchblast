#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "ObstacleEntity.h"

ObstacleEntity::ObstacleEntity(float x, float y, int objectFrame)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_DESTROYABLE), x, y)
{
  imagesProLine = 3;
  type = ENTITY_ENEMY_NC;
  enemyType = EnemyTypeDestroyable;
  movingStyle = movWalking;
  bloodColor = BloodBarrel;

  age = 0.0f;
  frame = 0;

  xGrid = x / TILE_WIDTH;
  yGrid = y / TILE_HEIGHT;

  game().getCurrentMap()->setObjectTile(xGrid, yGrid, objectFrame);
  game().getCurrentMap()->setLogicalTile(xGrid, yGrid, LogicalDestroyable);

  switch (objectFrame)
  {
  case MAPOBJ_BARREL:
    hp = 18;
    hpMax = 18;
    initialFrame = 0;
    break;
  case MAPOBJ_BARREL + 1:
    hp = 12;
    hpMax = 12;
    initialFrame = 0;
    frame = 1;
    break;
  case MAPOBJ_BARREL + 2:
    hp = 6;
    hpMax = 6;
    initialFrame = 0;
    frame = 2;
    break;
  default:
    std::cout << "ERROR: unknown obstacle (" << objectFrame << ")\n";
    isDying = true;
    break;
  }

  resistance[ResistanceFrozen] = ResistanceImmune;
  resistance[ResistanceRecoil] = ResistanceImmune;
  resistance[ResistanceFire] = ResistanceVeryLow;
  resistance[ResistancePoison] = ResistanceImmune;

  canExplode = false;
}

void ObstacleEntity::animate(float delay)
{
  testSpriteCollisions();
}

void ObstacleEntity::render(sf::RenderTarget* app)
{
  EnemyEntity::render(app);
}

void ObstacleEntity::dying()
{
  EnemyEntity::dying();
  game().getCurrentMap()->setObjectTile(xGrid, yGrid, 0);
  game().getCurrentMap()->setLogicalTile(xGrid, yGrid, LogicalFloor);
}

void ObstacleEntity::calculateBB()
{
  boundingBox.left = (int)x - 30;
  boundingBox.width = 60;
  boundingBox.top = (int)y - 30;
  boundingBox.height =  60;
}


void ObstacleEntity::drop()
{
  EnemyEntity::drop();
}

void ObstacleEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (entity == this) return;
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_BOLT )
    {
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (!boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        EnemyEntity::collideWithBolt(boltEntity);
        correctFrame();
      }
    }
    else if (entity->getType() == ENTITY_ENEMY_BOLT )
    {
      EnemyBoltEntity* boltEntity = dynamic_cast<EnemyBoltEntity*>(entity);

      if (!boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        EnemyEntity::collideWithBolt(boltEntity);
        correctFrame();
      }
    }
    else if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX)
    {
      EnemyEntity* enemyEntity = dynamic_cast<EnemyEntity*>(entity);

      if (!enemyEntity->getDying())
      {
        hurt(getHurtParams(hp, ShotTypeStandard, 0, false, SourceTypeMelee, enemyEntity->getEnemyType(), false));
      }
    }
  }
}

void ObstacleEntity::correctFrame()
{
  if (hp > 0)
  {
    if ( (hp - 1) / 6 == 1) frame = initialFrame + 1;
    else if ( (hp - 1) / 6 == 0) frame = initialFrame + 2;
    game().getCurrentMap()->setObjectTile(xGrid, yGrid, MAPOBJ_BARREL + frame);
  }
}

