#include "ZombieEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

ZombieEntity::ZombieEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_ZOMBIE), x, y)
{
  imagesProLine = 11;
  this->invocated = invocated;

  frame = 1;
  dyingFrame = 10;
  bloodColor = BloodRed;
  shadowFrame = 9;
  deathFrame = FRAME_CORPSE_ZOMBIE;
  if (invocated) enemyType = EnemyTypeZombie_invocated;
  else enemyType = EnemyTypeZombie;
  hp = ZOMBIE_HP;
  creatureSpeed = ZOMBIE_SPEED;

  direction = rand() % 4;
  facingDirection = direction;
  nextFacingDirection = direction;
  clockTurn = rand() % 2 == 0;
  compute(false);
  timer = 5 + rand() % 6;
  attackTimer = 0.9f;

  meleeDamages = ZOMBIE_DAMAGE;

  agonizingSound = SOUND_ZOMBIE_DYING;
  height = 80;
  sprite.setOrigin(32.0f, 60.0f);

  resistance[ResistanceFrozen] = ResistanceHigh;
  resistance[ResistanceRepulsion] = ResistanceHigh;
  resistance[ResistancePoison] = ResistanceImmune;
}

void ZombieEntity::animate(float delay)
{
  if (isAgonising)
  {
    if (hpDisplay > hp) hpDisplay--;

    if (h < -0.01f)
    {
      isAgonising = false;
      isDying = true;
      game().addCorpse(x, y, deathFrame);
      if (dyingSound != SOUND_NONE) SoundManager::getInstance().playSound(dyingSound);
    }
    else
    {
      frame = dyingFrame;
      hVelocity -= 700.0f * delay;
      h += hVelocity * delay;
    }

    return;
  }
  else if (age > 0.0f)
  {
    if (attackTimer <= 0.0f && attack())
    {
      attackTimer = 2.0f;
      giveRepulsion(false, Vector2D(velocity.x * 3.0f, velocity.y * 3.0f), 2.5f);
    }
    else
    {
      timer -= delay;
      attackTimer -= delay;
      if (timer < 0.0f)
      {
        SoundManager::getInstance().playSound(SOUND_ZOMBIE_00 + rand() % 2);
        timer = 5 + rand() % 6;
        if (rand() % 3 == 0) clockTurn = !clockTurn;
        compute(true);
      }
    }

    checkNextFacing(delay);

    frame = ((int)(age * 4.0f)) % 4;
    if (frame == 3) frame = 1;
    if (facingDirection == 4 || facingDirection == 6) frame += 3;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 6;
  }

  EnemyEntity::animate(delay);
  z = y + 17;
}

bool ZombieEntity::attack()
{
  Vector2D playerPos = game().getPlayerPosition();
  bool attacking = false;

  // left ?
  if (playerPos.x < x && playerPos.y > y - 15 && playerPos.y < y + 15 && canSee(playerPos.x, playerPos.y))
  {
    direction = 3;
    velocity.x = -creatureSpeed;
    velocity.y = 0;
    facingDirection = 4;
    attacking = true;
  }
  // right ?
  else if (playerPos.x > x && playerPos.y > y - 15 && playerPos.y < y + 15 && canSee(playerPos.x, playerPos.y))
  {
    direction = 1;
    velocity.x = creatureSpeed;
    velocity.y = 0;
    facingDirection = 6;
    attacking = true;
  }
  // down ?
  else if (playerPos.y > y && playerPos.x > x - 15 && playerPos.x < x + 15 && canSee(playerPos.x, playerPos.y))
  {
    direction = 2;
    velocity.x = 0;
    velocity.y = creatureSpeed;
    facingDirection = 2;
    attacking = true;
  }
  // up ?
  else if (playerPos.y < y && playerPos.x > x - 15 && playerPos.x < x + 15 && canSee(playerPos.x, playerPos.y))
  {
    direction = 0;
    velocity.x = 0;
    velocity.y = -creatureSpeed;
    facingDirection = 8;
    attacking = true;
  }

  if (attacking)
  {
    SoundManager::getInstance().playSound(SOUND_ZOMBIE_ATTACKING);
    facingTimer = 0.2f;
    nextFacingDirection = facingDirection;
  }

  return attacking;
}

void ZombieEntity::compute(bool turn)
{
  if (turn)
  {
    if (clockTurn)
    {
      direction++;
      if (direction == 4) direction = 0;
    }
    else
    {
      direction--;
      if (direction < 0) direction = 3;
    }
  }

  switch (direction)
  {
  case 0:
    velocity.x = 0;
    velocity.y = -creatureSpeed;
    nextFacingDirection = 8;
    break;

  case 1:
    velocity.x = creatureSpeed;
    velocity.y = 0;
    nextFacingDirection = 6;
    break;

  case 2:
    velocity.x = 0;
    velocity.y = creatureSpeed;
    nextFacingDirection = 2;
    break;

  case 3:
    velocity.x = -creatureSpeed;
    velocity.y = 0;
    nextFacingDirection = 4;
    break;
  }
}

void ZombieEntity::calculateBB()
{
  boundingBox.left = (int)x - 14;
  boundingBox.width = 28;
  boundingBox.top = (int)y - 18;
  boundingBox.height =  36;
}

void ZombieEntity::collideMapRight()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  else compute(true);
}

void ZombieEntity::collideMapLeft()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.x = -repulsion.velocity.x * 0.3f;
    else (repulsion.active = false);
  }
  else compute(true);
}

void ZombieEntity::collideMapTop()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  else compute(true);
}

void ZombieEntity::collideMapBottom()
{
  if (repulsion.active)
  {
    if (repulsion.stun) repulsion.velocity.y = -repulsion.velocity.y * 0.3f;
    else (repulsion.active = false);
  }
  else compute(true);
}

void ZombieEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    Vector2D recoilVector = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 50.0f);
    giveRepulsion(false, recoilVector, 0.2f);
    compute(true);
  }
}

void ZombieEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}

bool ZombieEntity::isAttacking()
{
  return attackTimer > 1.0f;
}
