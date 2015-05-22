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
    hp = 24;
    hpMax = 24;
    initialFrame = 0;
    break;
  case MAPOBJ_BARREL + 1:
    hp = 16;
    hpMax = 16;
    initialFrame = 1;
    break;
  case MAPOBJ_BARREL + 2:
    hp = 8;
    hpMax = 8;
    initialFrame = 2;
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
}

void ObstacleEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
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
  }
}

void ObstacleEntity::correctFrame()
{
  if (hp > 0)
  {
    if ( (hp - 1) / 8 == 1) frame = initialFrame + 1;
    else if ( (hp - 1) / 8 == 0) frame = initialFrame + 2;
    game().getCurrentMap()->setObjectTile(xGrid, yGrid, frame);
  }
}

