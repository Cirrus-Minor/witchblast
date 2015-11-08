#include "GreenRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

GreenRatEntity::GreenRatEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_RAT), x, y)
{
  imagesProLine = 10;
  creatureSpeed = GREEN_RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  hp = GREEN_RAT_HP;
  meleeDamages = GREEN_RAT_DAMAGES;

  type = ENTITY_ENEMY_INVOCATED;
  enemyType = EnemyTypeRatGreen;
  bloodColor = BloodRed;
  shadowFrame = -1;
  dyingFrame = 19;
  deathFrame = FRAME_CORPSE_GREEN_RAT;
  agonizingSound = SOUND_RAT_DYING;

  timer = (rand() % 50) / 10.0f;
  age = -GREEN_RAT_FADE;
  frame = 11;
  sprite.setOrigin(32.0f, 38.0f);

  canExplode = false; // TO SEE
}

void GreenRatEntity::animate(float delay)
{
  z = y + boundingBox.top + boundingBox.height;

  if (age > 0.0f && !isAgonising)
  {
    sprite.setColor(sf::Color(255,255,255,255));

    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = (rand() % 50) / 10.0f;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), GREEN_RAT_SPEED ));
      computeFacingDirection();
    }

    frame = 8 + ((int)(age * 5.0f)) % 2;
    if (facingDirection == 4 || facingDirection == 6) frame += 2;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 4;

    frame = ((int)(age * 5.0f)) % 4;
    if (frame == 3) frame = 1;
    if (facingDirection == 4 || facingDirection == 6) frame += 3;
    isMirroring = (facingDirection == 6 );
    if (facingDirection == 8) frame += 6;
    frame += 10;
  }
  else if (!isAgonising)
  {
    sprite.setColor(sf::Color(255,255,255,255 * (1.0 + age)));
  }
  else if (isAgonising)
  {
    if (hVelocity < -1.0f)
    {
      int fade = h * 200 / 25;
      if (fade > 200) fade = 200;
      else if (fade < 0) fade = 0;
      sprite.setColor(sf::Color(255, 255, 255, fade));
    }
  }

  EnemyEntity::animate(delay);
  z = y + 17;
}

void GreenRatEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
  boundingBox.width = width - RAT_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - 13;
  boundingBox.height =  31;
}

void GreenRatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
    computeFacingDirection();
}

void GreenRatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
    computeFacingDirection();
}

void GreenRatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
    computeFacingDirection();
}

void GreenRatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
    computeFacingDirection();
}

bool GreenRatEntity::canCollide()
{
  return (age >= 0.0f);
}

void GreenRatEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRepulsion(false, vel, 0.3f);

    computeFacingDirection();
  }
}

void GreenRatEntity::drop()
{
  // no drop
}
