#include "LittleSpiderEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

LittleSpiderEntity::LittleSpiderEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_LITTLE_SPIDER), x, y)
{
  imagesProLine = 8;
  creatureSpeed = 175.0f;
  velocity = Vector2D(creatureSpeed);
  hp = 16;
  meleeDamages = 5;

  type = ENTITY_ENEMY_INVOCATED;
  enemyType = EnemyTypeSpiderLittle_invocated;
  bloodColor = BloodGreen;
  shadowFrame = 4;
  dyingFrame = 3;
  deathFrame = FRAME_CORPSE_LITTLE_SPIDER;
  agonizingSound = SOUND_SPIDER_LITTLE_DIE;

  timer = (rand() % 50) / 10.0f;
  age = 0.0f;
  frame = 8;
}

void LittleSpiderEntity::animate(float delay)
{
  z = y + boundingBox.top + boundingBox.height;

  if (age > 0.0f && !isAgonising)
  {
    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = (rand() % 50) / 10.0f;
      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
    }
    frame = ((int)(age * 5.0f)) % 3;
  }

  EnemyEntity::animate(delay);
  z = y + 25;
}

void LittleSpiderEntity::calculateBB()
{
    boundingBox.left = (int)x - 20;
    boundingBox.width = 40;
    boundingBox.top = (int)y - 10;
    boundingBox.height =  35;
}

void LittleSpiderEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    computeFacingDirection();
}

void LittleSpiderEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    computeFacingDirection();
}

void LittleSpiderEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (recoil.active && recoil.stun) return;

  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRecoil(false, vel, 0.3f);

    computeFacingDirection();
  }
}

void LittleSpiderEntity::drop()
{
  // no drop
}
