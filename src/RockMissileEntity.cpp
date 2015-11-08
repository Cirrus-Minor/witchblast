#include "RockMissileEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RockMissileEntity::RockMissileEntity(float x, float y, int rockType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_CYCLOP), x, y)
{

  Vector2D targetPos = game().getPlayerPosition();
  imagesProLine = 20;
  collisionDirection = -1;
  enemyType = EnemyTypeRockMissile;
  movingStyle = movFlying;
  bloodColor = BloodNone; // stones don't bleed
  hasCollided = false;

  age = 0.0f;

  this->rockType = rockType;

  if (rockType == 0)
  {
    creatureSpeed = 500.0f;
    hp = 12;
    meleeDamages = 5;
    frame = 18;
  }
  else
  {
    creatureSpeed = 450.0f;
    hp = 24;
    meleeDamages = 8;
    frame = 38;
  }

  setVelocity(Vector2D(x, y).vectorNearlyTo(targetPos, creatureSpeed, 0.4f));
  canExplode = false;
  if (y < TILE_HEIGHT) this->y = TILE_HEIGHT;
  if (!testEntityInMap()) isDying = true;
  resistance[ResistancePoison] = ResistanceImmune;
}

void RockMissileEntity::animate(float delay)
{
  EnemyEntity::animate(delay);
  if (x < -60 || x > 1050 || y < - 50 || y > 800) isDying = true;
}

void RockMissileEntity::calculateBB()
{
  int w;
  if (rockType == 0) w = 20;
  else w = 24;

  boundingBox.left = (int)x - w / 2;
  boundingBox.width = w;
  boundingBox.top = (int)y - w / 2;
  boundingBox.height =  w;
}

void RockMissileEntity::collideWall()
{
  if (rockType == 1 && !hasCollided)
  {
    hasCollided = true;
    if (collisionDirection == DIRECTION_RIGHT || collisionDirection == DIRECTION_LEFT) velocity.x = -velocity.x;
    else velocity.y = -velocity.y;
  }
  else
  {
    dying();
  }
}

void RockMissileEntity::collideMapRight()
{
  collisionDirection = DIRECTION_RIGHT;
  collideWall();
}

void RockMissileEntity::collideMapLeft()
{
  collisionDirection = DIRECTION_LEFT;
  collideWall();
}

void RockMissileEntity::collideMapTop()
{
  collisionDirection = DIRECTION_TOP;
  collideWall();
}

void RockMissileEntity::collideMapBottom()
{
  collisionDirection = DIRECTION_BOTTOM;
  collideWall();
}

void RockMissileEntity::collideWithEnemy(EnemyEntity* entity)
{
}

void RockMissileEntity::dying()
{
  isDying = true;
  game().addKilledEnemy(enemyType, hurtingType);
  SoundManager::getInstance().playSound(
    rockType == 0 ? SOUND_ROCK_IMPACT_LIGHT : SOUND_ROCK_IMPACT_MEDIUM);
  game().makeShake(0.1f);

  for (int i = 0; i < 4; i++)
  {
    displayEntityStruct& de = game().getCurrentMapEntity()->generateBlood(x, y, BloodRock);

    if ((collisionDirection == DIRECTION_LEFT) && (de.velocity.x < 0.0f))
      de.velocity.x *= -0.25f;
    else if ((collisionDirection == DIRECTION_RIGHT) && (de.velocity.x > 0.0f))
      de.velocity.x *= -0.25f;
    else if ((collisionDirection == DIRECTION_TOP) && (de.velocity.y < 0.0f))
      de.velocity.y *= -0.25f;
    else if ((collisionDirection == DIRECTION_BOTTOM) && (de.velocity.y > 0.0f))
      de.velocity.y *= -0.25f;
  }
}

void RockMissileEntity::inflictsRepulsionTo(BaseCreatureEntity* targetEntity)
{
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

  if (playerEntity != NULL && !playerEntity->isDead())
  {
    Vector2D repulsionVector = Vector2D(0, 0).vectorTo(getVelocity(), 600.0f );
    targetEntity->giveRepulsion(true, repulsionVector, 0.5f);
  }
}
