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

  if (snakeType == SnakeTypeNormal)
  {
    frame = 0;
    dyingFrame = 8;
    deathFrame = FRAME_CORPSE_SNAKE;
    if (invocated) enemyType = EnemyTypeSnake_invocated;
    else enemyType = EnemyTypeSnake;
    hp = SNAKE_HP;
    creatureSpeed = SNAKE_SPEED;
    meleeDamages = SNAKE_DAMAGE;
    meleeType = ShotTypePoison;
  }
  else // snake blood
  {
    frame = 10;
    dyingFrame = 18;
    deathFrame = FRAME_CORPSE_SNAKE_BLOOD;
    if (invocated) enemyType = EnemyTypeSnakeBlood_invocated;
    else enemyType = EnemyTypeSnakeBlood;
    hp = SNAKE_BLOOD_HP;
    creatureSpeed = SNAKE_BLOOD_SPEED;

    meleeDamages = SNAKE_BLOOD_DAMAGE;
  }

  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();

  bloodColor = BloodRed;
  shadowFrame = 9;

  timer = -1.0f;

  agonizingSound = SOUND_SNAKE_DIE;
  resistance[ResistancePoison] = ResistanceImmune;
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

      if (canWalkTo(game().getPlayerPosition().x, game().getPlayerPosition().y))
      {
        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
        computeFacingDirection();
      }
    }

    switch (facingDirection)
    {
      case 2: frame = 0; break;
      case 4: frame = 2; break;
      case 6: frame = 4; break;
      case 8: frame = 6; break;
    }
    frame += ((int)(age * 3.0f)) % 2;
    if (snakeType == SnakeTypeBlood) frame += 10;
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
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
  else computeFacingDirection();
}

void SnakeEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
  else computeFacingDirection();
}

void SnakeEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
  else computeFacingDirection();
}

void SnakeEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
  else computeFacingDirection();
}

void SnakeEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed );
    if (velocity.x * vel.x < 0) velocity.x = vel.x;
    if (velocity.y * vel.y < 0) velocity.y = vel.y;
    timer = 0.05f;
  }
}

void SnakeEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        int meleeLevel = 0;
        if (snakeType == SnakeTypeBlood)
        {
          if (rand() % 3 == 0)
          {
            meleeType = ShotTypePoison;
            meleeDamages = 4;
            meleeLevel = 1;
          }
          else
          {
            meleeType = ShotTypeStandard;
            meleeDamages = 8;
          }
        }
        if (playerEntity->hurt(getHurtParams(meleeDamages, meleeType, meleeLevel, false, SourceTypeMelee, enemyType, false)))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);
        }
        inflictsRepulsionTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        collideWithBolt(boltEntity);
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX_COUNT)
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

void SnakeEntity::drop()
{
  if (!invocated)
  {
    if (rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }
    if (game().getPlayer()->isEquiped(EQUIP_LUCK) && rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }

    if (rand() % 25 == 0)
    {
      dropItem(ItemHealthVerySmallPoison);
    }
  }
}
