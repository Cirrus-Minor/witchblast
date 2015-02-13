#include "GhostEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

GhostEntity::GhostEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_GHOST), x, y)
{

  frame = 0;
  dyingFrame = 3;
  deathFrame = FRAME_CORPSE_GHOST;
  enemyType = EnemyTypeGhost;
  hp = GHOST_HP;
  creatureSpeed = GHOST_SPEED;
  meleeDamages = GHOST_DAMAGE;
  movingStyle = movFlying;

  velocity = Vector2D(creatureSpeed);
  timer = 0.0f;

  bloodColor = BloodNone;

  agonizingSound = SOUND_GHOST_DYING;

  resistance[ResistanceFrozen] = ResistanceHigh;
  resistance[ResistanceRecoil] = ResistanceHigh;
}

void GhostEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising && !isExploding)
  {
    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = 0.8f;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      computeFacingDirection();
    }

    float movAngle = Vector2D(x, y).angleTo(game().getPlayerPosition());
    movAngle += cos( 2 * age) * 0.8f;

    if (game().getPlayerPosition().y > y)
      setVelocity(Vector2D(sin(movAngle) * creatureSpeed, cos(movAngle) * creatureSpeed));
    else
      setVelocity(Vector2D(-sin(movAngle) * creatureSpeed, -cos(movAngle) * creatureSpeed));

    frame = ((int)(age * 3.0f)) % 4;
    if (frame == 3) frame = 1;
  }
  isMirroring = x > game().getPlayerPosition().x;

  EnemyEntity::animate(delay);
  z = y + 32;
}

void GhostEntity::calculateBB()
{
  boundingBox.left = (int)x - 15;
  boundingBox.width = 30;
  boundingBox.top = (int)y - 15;
  boundingBox.height =  30;
}

int GhostEntity::getFade()
{
  float dist1 = 24000.f;
  float dist2 = 40000.f;
  float dist = Vector2D(x, y).distance2(game().getPlayerPosition());

  if (dist < dist1) return 100;
  else if (dist > dist2) return 0;

  int fade =((dist2 - dist) / (dist2 - dist1)) * 100;
  if (fade < 0) fade = 0;
  if (fade > 100) fade = 100;
  return fade;
}

void GhostEntity::render(sf::RenderTarget* app)
{
  int fade = getFade();
  if (fade == 100) SoundManager::getInstance().playSound(SOUND_GHOST);
  if (fade == 100 || isAgonising || isExploding)
  {
    sf::Color color = sprite.getColor();
    color.a = 255;
    sprite.setColor(color);
    EnemyEntity::render(app);
  }
  else if (fade > 0)
  {
    sf::Color color = sprite.getColor();
    color.a = fade * 255 / 100;
    sprite.setColor(color);
    EnemyEntity::render(app);
  }
}

void GhostEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void GhostEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void GhostEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void GhostEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void GhostEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (recoil.active && recoil.stun) return;

  if (entity->getEnemyType() == EnemyTypeGhost)
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 1.5f );
    giveRecoil(false, vel, 0.3f);
  }
}

void GhostEntity::collideWithBolt(BoltEntity* boltEntity)
{
  int fade = getFade();
  if (fade <= 0) return;

  if (boltEntity->getBoltType() == ShotTypeIllusion) fade = 100;
  boltEntity->setDamages(boltEntity->getDamages() * fade / 100);

  EnemyEntity::collideWithBolt(boltEntity);
}

void GhostEntity::drop()
{
  EnemyEntity::drop();
}
