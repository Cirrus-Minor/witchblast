#include "LittleSpiderEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "SpiderWebEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

LittleSpiderEntity::LittleSpiderEntity(float x, float y, EnumSpiderType spideType, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_LITTLE_SPIDER), x, y)
{
  imagesProLine = 8;
  creatureSpeed = 175.0f;
  velocity = Vector2D(creatureSpeed * 0.25f);

  meleeDamages = 5;

  this->spideType = spideType;
  if (spideType == SpiderTypeTarantula)
  {
    enemyType = invocated ? EnemyTypeSpiderTarantula_invocated : EnemyTypeSpiderTarantula;
    frame = 8;
    deathFrame = FRAME_CORPSE_LITTLE_SPIDER_TARANTULA;
    dyingFrame = 11;
    hp = 32;
    webTimer = 3 + 0.1f * (float)(rand() % 50);
    meleeType = ShotTypePoison;
    meleeLevel = 1;
  }
  else
  {
    enemyType = invocated ? EnemyTypeSpiderLittle_invocated : EnemyTypeSpiderLittle;
    frame = 0;
    deathFrame = FRAME_CORPSE_LITTLE_SPIDER;
    dyingFrame = 3;
    hp = 16;
  }


  type = invocated ? ENTITY_ENEMY_INVOCATED : ENTITY_ENEMY;

  bloodColor = BloodGreen;
  shadowFrame = 4;


  agonizingSound = SOUND_SPIDER_LITTLE_DIE;

  timer = -1.0f; //(rand() % 50) / 10.0f;
  if (invocated) age = 0.0f;

  sprite.setOrigin(32.0f, 40.0f);

  resistance[ResistancePoison] = ResistanceImmune;
  roaming = true;
}

void LittleSpiderEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = (rand() % 50) / 10.0f;
      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      roaming = false;
    }
    frame = ((int)(age * (roaming ? 1.5f : 5.0f))) % 3;
    if (spideType == SpiderTypeTarantula)
      frame += 8;

    if (spideType == SpiderTypeTarantula)
    {
      webTimer = webTimer - delay;
      if (webTimer <= 0.0f)
      {
        webTimer = 3 + 0.1f * (float)(rand() % 50);
        new SpiderWebEntity(x, y, false);
      }
    }
  }

  EnemyEntity::animate(delay);
  z = y + 21;
}

void LittleSpiderEntity::calculateBB()
{
    boundingBox.left = (int)x - 18;
    boundingBox.width = 36;
    boundingBox.top = (int)y - 10;
    boundingBox.height =  32;
}

void LittleSpiderEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
    computeFacingDirection();
}

void LittleSpiderEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRepulsion(false, vel, 0.3f);

    computeFacingDirection();
  }
}

void LittleSpiderEntity::drop()
{
  if (type == ENTITY_ENEMY)
    EnemyEntity::drop();
}
