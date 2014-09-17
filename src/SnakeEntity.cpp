#include "SnakeEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SnakeEntity::SnakeEntity(float x, float y, snakeTypeEnum snakeType, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_SNAKE), x, y)
{
  this->snakeType = snakeType;
  imagesProLine = 10;
  this->invocated = invocated;

  //if (snakeType == snakeTypeNormal)
  {
    frame = 0;
    dyingFrame = 8;
    deathFrame = FRAME_CORPSE_SNAKE;
    if (invocated) enemyType = EnemyTypeSnake_invocated;
    else enemyType = EnemyTypeSnake;
    hp = SNAKE_HP;
    creatureSpeed = SNAKE_SPEED;
  }

  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();

  meleeDamages = SNAKE_DAMAGE;
  meleeType = ShotTypePoison;

  type = ENTITY_ENNEMY;
  bloodColor = BloodRed;
  shadowFrame = 9;

  timer = -1.0f;

  //agonizingSound = SOUND_RAT_DYING;
}

void SnakeEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    sprite.setColor(sf::Color(255,255,255,255));

    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = 0.8f;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      computeFacingDirection();
    }

    switch (facingDirection)
    {
      case 2: frame = 0; break;
      case 4: frame = 2; break;
      case 6: frame = 4; break;
      case 8: frame = 6; break;
    }
    frame += ((int)(age * 3.0f)) % 2;
  }

  EnemyEntity::animate(delay);
  z = y + 15;
}

void SnakeEntity::calculateBB()
{
  boundingBox.left = (int)x - 15;
  boundingBox.width = 30;
  boundingBox.top = (int)y - 15;
  boundingBox.height =  30;
}

void SnakeEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void SnakeEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void SnakeEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void SnakeEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void SnakeEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  if (recoil.active && recoil.stun) return;

  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 70.0f );
    giveRecoil(false, vel, 0.3f);

    computeFacingDirection();
  }
}

void SnakeEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}
