#include "CauldronEntity.h"
#include "SlimeEntity.h"
#include "ExplosionEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

CauldronEntity::CauldronEntity(float x, float y, cauldronTypeEnum cauldronType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_CAULDRON), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = CAULDRON_HP;
  hpMax = hp;
  meleeDamages = 0;
  this->cauldronType = cauldronType;

  bloodColor = BloodNone;
  invokeDelay = 2.5f;
  bubbleDelay = 0.0f;
  shadowFrame = 2;
  sprite.setOrigin(32, 28);
  imagesProLine = 2;

  deathFrame = FRAME_CORPSE_CAULDRON;
  dyingSound = SOUND_CAULDRON_DIE;

  if (cauldronType == CauldronTypeElemental)
  {
    enemyType = EnemyTypeCauldronElemental;
    colorChangeDelay = 4.0f + rand()% 40 * 0.1f;
    colorState = rand() % 3;
  }
  else
  {
    enemyType = EnemyTypeCauldron;
  }

  enemyType = cauldronType == CauldronTypeStandard ? EnemyTypeCauldron : EnemyTypeCauldronElemental;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceImmune;
  canExplode = false;
}

void CauldronEntity::animate(float delay)
{
  if (isAgonising)
  {
    agonizingDelay -= delay;
    if (agonizingDelay <= 0.0f)
    {
      isDying = true;
      game().addCorpse(x, y, deathFrame);
    }
  }
  else
  {
    SoundManager::getInstance().playSound(SOUND_CAULDRON, false);

    invokeDelay -= delay;
    if (invokeDelay < 0.0f)
    {
      if (cauldronType == CauldronTypeElemental)
      {
        if (game().getEnemyCount() < 9)
        {
          slimeTypeEnum slimeType = SlimeTypeStandard;
          switch (colorState)
          {
            case 0: slimeType = SlimeTypeBlue; break;
            case 1: slimeType = SlimeTypeRed; break;
            case 2: slimeType = SlimeTypeStandard; break;
          }
          SlimeEntity* slime = new SlimeEntity(x, y, slimeType, true);
          slime->disableCollidingTemporary();
          invokeDelay = 3.75f + (float)(rand() % 3000) / 1000.0f;
        }
      }
      else
      {
        SlimeEntity* slime = new SlimeEntity(x, y, SlimeTypeViolet, true);
        slime->disableCollidingTemporary();
        invokeDelay = 1.5f + (float)(rand() % 2500) / 1000.0f;
      }
    }

    if (cauldronType == CauldronTypeElemental)
    {
      colorChangeDelay -= delay;
      if (colorChangeDelay < 0.0f)
      {
        colorChangeDelay = 4.0f + rand()% 50 * 0.1f;
        if (rand() % 2 == 0)
        {
          colorState++;
          if (colorState > 2) colorState = 0;
        }
        else
        {
          colorState--;
          if (colorState < 0) colorState = 2;
        }
      }
    }

    bubbleDelay -= delay;
    if (bubbleDelay < 0.0f)
    {
      bubbleDelay = 0.3f;
      int bubbleFrame = 32;
      if (cauldronType == CauldronTypeElemental)
      {
        switch (colorState)
        {
          case 0: bubbleFrame = 33; break;
          case 1: bubbleFrame = 34; break;
          case 2: bubbleFrame = 35; break;
        }
      }

      for (int i=0; i < 2; i++)
      {
        float xBub = x - 16 + rand() % 32;
        SpriteEntity* bubble = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_CAULDRON), xBub, y - 20, 8, 8);
        bubble->setZ(z);
        bubble->setFrame(bubbleFrame);
        bubble->setType(ENTITY_EFFECT);
        bubble->setWeight(-20 - rand() % 40);
        bubble->setLifetime(2.0f);
        float bloodScale = 0.3f + (rand() % 20) * 0.1f;
        bubble->setScale(bloodScale, bloodScale);
      }
    }
    frame = hp > hpMax / 2 ? 0 : 1;
    if (cauldronType == CauldronTypeElemental)
    {
      switch (colorState)
      {
        case 0: frame += 2; break;
        case 1: frame += 4; break;
        case 2: frame += 6; break;
      }
    }

    EnemyEntity::animate(delay);
  }
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
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void CauldronEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void CauldronEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void CauldronEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void CauldronEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() != movFlying)
    inflictsRecoilTo(entity);
}

void CauldronEntity::dying()
{
  new ExplosionEntity(x, y, ExplosionTypeViolet, 0, EnemyTypeNone, true);

  if (cauldronType == CauldronTypeElemental)
    {
      switch (colorState)
      {
        case 0: deathFrame = FRAME_CORPSE_CAULDRON_RED; break;
        case 1: deathFrame = FRAME_CORPSE_CAULDRON_BLUE; break;
        case 2: deathFrame = FRAME_CORPSE_CAULDRON_GREEN; break;
      }
    }

  EnemyEntity::dying();
  return;
}

void CauldronEntity::drop()
{
  EnemyEntity::drop();
}

void CauldronEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 200.0f );
  targetEntity->giveRepulsion(false, recoilVector, 0.1f);
}
