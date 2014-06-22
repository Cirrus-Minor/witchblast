#include "RatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RatEntity::RatEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y)
{
  creatureSpeed = RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  hp = RAT_HP;
  meleeDamages = RAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
  shadowFrame = 7;
  dyingFrame = 6;
  deathFrame = FRAME_CORPSE_RAT;
  agonizingSound = SOUND_RAT_DYING;
}

void RatEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    frame = ((int)(age * 5.0f)) % 2;
    if (facingDirection == 4 || facingDirection == 6) frame += 2;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 4;
  }

  EnnemyEntity::animate(delay);
}

void RatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
    boundingBox.width = width - RAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + RAT_BB_TOP;
    boundingBox.height =  height - RAT_BB_HEIGHT_DIFF;
}

void RatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    else computeFacingDirection();
}

void RatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    else computeFacingDirection();
}

void RatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    else computeFacingDirection();
}

void RatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    else computeFacingDirection();
}

void RatEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
    computeFacingDirection();
  }
}
