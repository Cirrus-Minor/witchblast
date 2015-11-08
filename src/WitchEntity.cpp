#include "WitchEntity.h"
#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "RatEntity.h"
#include "BatEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

WitchEntity::WitchEntity(float x, float y, witchTypeEnum witchType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_WITCH), x, y)
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

  hp = WITCH_HP;
  hpMax = hp;
  creatureSpeed = WITCH_VELOCITY;

  velocity = Vector2D(creatureSpeed);
  meleeDamages = WITCH_DAMAGE;

  bloodColor = BloodRed;
  shadowFrame = 4;

  height = 96;

  sprite.setOrigin(32, 71);

  timer = 3.0f;
  escapeTimer = -1.0f;
  state = 0;
  agonizingSound = (sound_resources)(SOUND_WITCH_DIE_00 + rand() % 2);
}

void WitchEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    if (escapeTimer > 0.0f) escapeTimer -= delay;
    timer -= delay;
    if (timer <= 0.0f)
    {
      if (state == 0)
      {

        state = 1;
        velocity = Vector2D(0.0f, 0.0f);
        SoundManager::getInstance().playSound(SOUND_WITCH_00 + rand() % 3);
        timer = 0.6f;

        if (rand() % 7 == 0 || !canSee(game().getPlayerPosition().x, game().getPlayerPosition().y))
        {
          // invoke
          int x0 = x / TILE_WIDTH;
          if (x0 < 1) x0 = 1;
          else if(x0 > MAP_WIDTH - 2) x0 = MAP_WIDTH - 2;
          x0 = x0 * TILE_WIDTH + TILE_WIDTH / 2;
          int y0 = y / TILE_HEIGHT;
          if (y0 < 1) y0 = 1;
          else if(y0 > MAP_HEIGHT - 2) y0 = MAP_HEIGHT - 2;
          y0 = y0 * TILE_HEIGHT + TILE_HEIGHT / 2;
          if (witchType == WitchTypeNormal) new RatEntity(x0, y0, RatEntity::RatTypeNormal, true);
          else new BatEntity(x0, y0, BatStandard, true);
          SoundManager::getInstance().playSound(SOUND_INVOKE);

          for(int i=0; i < 6; i++)
          {
            generateStar(sf::Color(200, 50, 200, 255));
            generateStar(sf::Color(255, 255, 255, 255));
          }
        }
        else
        {
          // fire
          fire();
          if (witchType == WitchTypeNormal) fire();
        }
      }
      else if (state == 1)
      {
        timer = 3.5f + (rand() % 25) * 0.1f;
        if (!canSee(game().getPlayerPosition().x, game().getPlayerPosition().y)) timer += 1.2f;
        velocity = Vector2D(creatureSpeed);
        state = 0;
      }
    }

    if (state == 0)
    {
      if (escapeTimer < 0.0f && Vector2D(x, y).distance2(game().getPlayerPosition()) <= 36000)
      {
        velocity = game().getPlayerPosition().vectorTo(Vector2D(x, y), creatureSpeed);
        escapeTimer = 2.5f;
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
  boundingBox.top = (int)y - 25;
  boundingBox.height = 45;
}

void WitchEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  else computeFacingDirection();
}

void WitchEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  else computeFacingDirection();
}

void WitchEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  else computeFacingDirection();
}

void WitchEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  else computeFacingDirection();
}

void WitchEntity::collideWithEnemy(EnemyEntity* entity)
{
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
    SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);
    EnemyBoltEntity* bolt = new EnemyBoltEntity
          (x, y + 10, ShotTypeStandard, 0, enemyType);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.7f;
    bolt->setVelocity(Vector2D(x, y).vectorNearlyTo(game().getPlayerPosition(), flowerFireVelocity, 1.0f ));
  }
  else
  {
    SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);
    EnemyBoltEntity* bolt = new EnemyBoltEntity
          (x, y + 10, ShotTypeBomb, 0, enemyType);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

    float flowerFireVelocity = EVIL_FLOWER_FIRE_VELOCITY * 0.9f;
    if (specialState[SpecialStateIce].active) flowerFireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorNearlyTo(game().getPlayerPosition(), flowerFireVelocity, 1.0f ));
  }
}

void WitchEntity::drop()
{
  if (rand() % 12 == 0)
  {
    if (rand() % 5 == 0)
    {
      dropItem(ItemScrollRevelation);
    }
    else
    {
      dropItem((enumItemType)(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED));
    }
    return;
  }
  else
  {
    if (rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }
    if (game().getPlayer()->isEquiped(EQUIP_LUCK) && rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }
  }
}
