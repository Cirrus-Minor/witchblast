#include "WitchEntity.h"
#include "BoltEntity.h"
#include "EnnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

WitchEntity::WitchEntity(float x, float y, witchTypeEnum witchType)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_WITCH), x, y)
{
  this->witchType = witchType;
  imagesProLine = 8;

  if (witchType == WitchTypeNormal)
  {
    frame = 0;
    dyingFrame = 5;
    deathFrame = FRAME_CORPSE_WITCH;
    enemyType = EnemyTypeWitch;
  }
  else
  {
    frame = 8;
    dyingFrame = 13;
    deathFrame = FRAME_CORPSE_WITCH_RED;
    enemyType = EnemyTypeWitchRed;
  }

  hp = 50;
  creatureSpeed = 120.0f;

  velocity = Vector2D(creatureSpeed);
  meleeDamages = 5;

  type = ENTITY_ENNEMY;
  bloodColor = BloodRed;
  shadowFrame = 4;

  height = 96;

  sprite.setOrigin(32, 75);

  timer = 3.0f;
  state = 0;
  agonizingSound = (sound_resources)(SOUND_WITCH_DIE_00 + rand() % 2);
}

void WitchEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    timer -= delay;
    if (timer <= 0.0f)
    {
      if (state == 0)
      {
        fire();
        if (witchType == WitchTypeNormal) fire();
        state = 1;
        velocity = Vector2D(0.0f, 0.0f);
        timer = 0.6f;
        SoundManager::getSoundManager()->playSound(SOUND_WITCH_00 + rand() % 3);
      }
      else if (state == 1)
      {
        timer = 3.5f + (rand() % 25) * 0.1f;
        velocity = Vector2D(creatureSpeed);
        state = 0;
      }
    }

    if (state == 0)
    {
      if (Vector2D(x, y).distance2(game().getPlayerPosition()) <= 36000)
      {
        velocity = game().getPlayerPosition().vectorTo(Vector2D(x, y), creatureSpeed);
      }

      frame = ((int)(age * 5.0f)) % 4;
      if (frame == 2) frame = 0;
      else if (frame == 3) frame = 2;
      if (velocity.x > 1.0f) isMirroring = true;
      else if (velocity.x < -1.0f) isMirroring = false;
    }
    else if (state == 1)
    {
      frame = 3;
      isMirroring = game().getPlayer()->getX() > x;
    }
    if (witchType == WitchTypeRed) frame += 8;
  }

  EnemyEntity::animate(delay);
  z = y + 20;
}

void WitchEntity::calculateBB()
{
  boundingBox.left = (int)x - 16;
  boundingBox.width = 32;
  boundingBox.top = (int)y - 20;
  boundingBox.height = 40;
}

void WitchEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void WitchEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void WitchEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void WitchEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void WitchEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
    computeFacingDirection();
  }
}

void WitchEntity::collideWithBolt(BoltEntity* boltEntity)
{
  EnemyEntity::collideWithBolt(boltEntity);
}

void WitchEntity::fire()
{
  if (witchType == WitchTypeNormal)
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);
    EnnemyBoltEntity* bolt = new EnnemyBoltEntity
          (x, y + 10, ShotTypeStandard, 0);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorNearlyTo(game().getPlayerPosition(), flowerFireVelocity, 1.0f ));
  }
  else
  {
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);
    EnnemyBoltEntity* bolt = new EnnemyBoltEntity
          (x, y + 10, ShotTypeBomb, 0);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY * 0.9f;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorNearlyTo(game().getPlayerPosition(), flowerFireVelocity, 1.0f ));
  }
}
