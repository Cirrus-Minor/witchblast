#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "ExplosionEntity.h"
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
  explosive = false;

  dyingSound = SOUND_BARREL_SMASH;
  hurtingSound = SOUND_BARREL_HIT;
  bloodColor = BloodNone;
  obstacleBloodType = BloodBarrel;
  deathFrame = FRAME_CORPSE_SLIME_VIOLET;

  age = 0.0f;
  frame = 0;
  objectIndex = objectFrame;

  xGrid = x / TILE_WIDTH;
  yGrid = y / TILE_HEIGHT;

  game().getCurrentMap()->setObjectTile(xGrid, yGrid, objectFrame);
  game().getCurrentMap()->setLogicalTile(xGrid, yGrid, LogicalDestroyable);

  // hp
  if (objectIndex == MAPOBJ_BARREL || objectIndex == MAPOBJ_BARREL_NO_DROP
      || objectIndex == MAPOBJ_BARREL_EXPL || objectIndex == MAPOBJ_SKULL)
  {
    hp = 18;
    hpMax = 18;
  }
  else if (objectIndex == MAPOBJ_BARREL + 1 || objectIndex == MAPOBJ_BARREL_NO_DROP + 1
           || objectIndex == MAPOBJ_BARREL_EXPL + 1 || objectIndex == MAPOBJ_SKULL + 1)
  {
    hp = 12;
    hpMax = 12;
  }
  else if (objectIndex == MAPOBJ_BARREL + 2 || objectIndex == MAPOBJ_BARREL_NO_DROP + 2
           || objectIndex == MAPOBJ_BARREL_EXPL + 2 || objectIndex == MAPOBJ_SKULL + 2)
  {
    hp = 6;
    hpMax = 6;
  }

  if (objectIndex >= MAPOBJ_BARREL_EXPL && objectIndex < MAPOBJ_BARREL_EXPL + 3)
  {
    obstacleBloodType = BloodBarrelPowder;
    explosive = true;

    initialFrame = 3;
    initialObjectIndex = MAPOBJ_BARREL_EXPL;
    frame = 3 + objectIndex - MAPOBJ_BARREL_EXPL;
  }
  else if (objectIndex >= MAPOBJ_BARREL && objectIndex < MAPOBJ_BARREL + 3)
  {
    initialFrame = 0;
    initialObjectIndex = MAPOBJ_BARREL;
    frame = objectIndex - MAPOBJ_BARREL;
  }
  else if (objectIndex >= MAPOBJ_BARREL_NO_DROP && objectIndex < MAPOBJ_BARREL_NO_DROP + 3)
  {
    initialFrame = 0;
    initialObjectIndex = MAPOBJ_BARREL_NO_DROP;
    frame = objectIndex - MAPOBJ_BARREL_NO_DROP;
  }
  else if (objectIndex >= MAPOBJ_SKULL && objectIndex < MAPOBJ_SKULL + 3)
  {
    obstacleBloodType = BloodSkull;

    initialFrame = 6;
    initialObjectIndex = MAPOBJ_SKULL;
    frame = 6 + objectIndex - MAPOBJ_SKULL;
  }

  resistance[ResistanceFrozen] = ResistanceImmune;
  resistance[ResistanceRepulsion] = ResistanceImmune;
  resistance[ResistanceFire] = ResistanceVeryLow;
  resistance[ResistancePoison] = ResistanceImmune;

  canExplode = false;
}

int ObstacleEntity::getObjectIndex()
{
  return objectIndex;
}

void ObstacleEntity::animate(float delay)
{
  age += delay;
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

  for (int i = 0; i < 10; i++)
    game().generateBlood(x, y, obstacleBloodType);

  if (explosive && age > 2.0f)
  {
    new ExplosionEntity(x, y, ExplosionTypeStandard, 16, EnemyTypeNone, true);
    SoundManager::getInstance().playSound(SOUND_BOOM_00);
    game().addCorpse(x, y, deathFrame);
  }
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
  if (initialObjectIndex == MAPOBJ_BARREL || initialObjectIndex == MAPOBJ_SKULL)
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

      if (!enemyEntity->getDying() && enemyEntity->canCollide()&& enemyEntity->getMovingStyle() != movFlying)
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
    if ( (hp - 1) / 6 == 1)
    {
      frame = initialFrame + 1;
      objectIndex = initialObjectIndex + 1;
    }
    else if ( (hp - 1) / 6 == 0)
    {
      frame = initialFrame + 2;
      objectIndex = initialObjectIndex + 2;
    }
    game().getCurrentMap()->setObjectTile(xGrid, yGrid, objectIndex);
  }
}

int ObstacleEntity::hurt(StructHurt hurtParam)
{
  int oldHp = hp;
  int result = EnemyEntity::hurt(hurtParam);
  int diff = oldHp - hp;
  for (int i = 0; i < diff; i++)
    game().generateBlood(x, y, obstacleBloodType);
  return result;
}
