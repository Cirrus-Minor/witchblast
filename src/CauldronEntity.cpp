#include "CauldronEntity.h"
#include "SlimeEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

CauldronEntity::CauldronEntity(float x, float y)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_CAULDRON), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = CAULDRON_HP;
  hpMax = hp;
  meleeDamages = 0;

  type = ENTITY_ENNEMY;
  bloodColor = BloodNone;
  invokeDelay = 2.5f;
  bubbleDelay = 0.0f;
  shadowFrame = 2;

  deathFrame = FRAME_CORPSE_CAULDRON;
  dyingSound = SOUND_CAULDRON_DIE;

  enemyType = EnemyTypeCauldron;
  resistance[ResistanceRecoil] = ResistanceVeryHigh;
}

void CauldronEntity::animate(float delay)
{
  invokeDelay -= delay;
  if (invokeDelay < 0.0f)
  {
    new SlimeEntity(x, y, SlimeTypeViolet, true);
    invokeDelay = 1.5f + (float)(rand() % 2500) / 1000.0f;
  }

  bubbleDelay -= delay;
  if (bubbleDelay < 0.0f)
  {
    bubbleDelay = 0.3f;

    for (int i=0; i < 2; i++)
    {
      float xBub = x - 16 + rand() % 32;
      SpriteEntity* bubble = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CAULDRON), xBub, y - 20, 8, 8);
      bubble->setZ(z);
      bubble->setFrame(32);
      bubble->setType(ENTITY_EFFECT);
      bubble->setWeight(-20 - rand() % 40);
      bubble->setLifetime(2.0f);
      float bloodScale = 0.3f + (rand() % 20) * 0.1f;
      bubble->setScale(bloodScale, bloodScale);
    }
  }

  frame = hp > hpMax / 2 ? 0 : 1;

  EnemyEntity::animate(delay);
}

void CauldronEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        inflictsRecoilTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        collideWithBolt(boltEntity);
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENNEMY && entity->getType() <= ENTITY_ENNEMY_MAX)
      {
        if (this != entity)
        {
          EnemyEntity* ennemyEntity = static_cast<EnemyEntity*>(entity);
          if (ennemyEntity->canCollide()) collideWithEnnemy(entity);
        }
      }
    }
  }
}

void CauldronEntity::calculateBB()
{
    boundingBox.left = (int)x - 25;
    boundingBox.width = 50;
    boundingBox.top = (int)y - 10;
    boundingBox.height =  42;
}

void CauldronEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void CauldronEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void CauldronEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void CauldronEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void CauldronEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() != movFlying)
  {
    inflictsRecoilTo(entity);
  }
}

void CauldronEntity::dying()
{
  EnemyEntity::dying();
}

void CauldronEntity::drop()
{
  EnemyEntity::drop();
}

void CauldronEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 200.0f );
  targetEntity->giveRecoil(false, recoilVector, 0.1f);
}
