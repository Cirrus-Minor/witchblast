#include "SausageEntity.h"
#include "ExplosionEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SausageEntity::SausageEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_SAUSAGE), x, y)
{
  creatureSpeed = 0.0f;
  hp = 8;
  meleeDamages = 5;

  this->invocated = invocated;
  if (invocated) type = ENTITY_ENEMY_INVOCATED;
  age = 0.0f;

  enemyType = invocated ? EnemyTypeSausage_invocated : EnemyTypeSausage;

  frame = 1;
  shadowFrame = 3;
  deathFrame = FRAME_CORPSE_SLIME_VIOLET;
  agonizingSound = SOUND_NONE;

  width = 48;
  height = 48;
  sprite.setOrigin(24, 24);

  h = 0;
  state = 0;
  timer = 0.5f + 0.1f *( rand() % 12);
  canExplode = false;
  isNew = true;
}

void SausageEntity::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(frame * width, 0, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void SausageEntity::animate(float delay)
{
  isNew = false;
  float sausDelay = delay;
  if (specialState[SpecialStateIce].active) sausDelay = delay * specialState[SpecialStateIce].param1;

  timer -= sausDelay;
  if (timer <= 0.0f)
  {
    if (state == 0) // waiting
    {
      state++;
      hVelocity = 450.0f;
    }
    else if (state == 2) // waiting in air
    {
      state++;
    }
  }

  if (state == 1 || state == 3) // jumping / falling
  {
    hVelocity -= 750.0f * sausDelay;

    if (state == 1 && /*hVelocity <= 0.0f*/ h > 35)
    {
      state = 2;
      timer = 0.4f;
      hVelocity = -100.0f;
    }
    else
      h += hVelocity * sausDelay;

    if (h <= 0.0f)
    {
      h = 0.0f;
      state = 0;
      timer = 1.0f;
    }
  }

  if (h > 1.0f)
  {
    frame = ((int)(age * 20)) % 4;
    if (frame == 3) frame = 1;
  }
  else
    frame = 1;

  EnemyEntity::animate(delay);
  // frame
  z = y + 14;
}

bool SausageEntity::canCollide()
{
  return (!isNew);
}

void SausageEntity::calculateBB()
{
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 14;
    boundingBox.height =  28;
}

void SausageEntity::collideMapRight()
{
  velocity.x = -velocity.x * 0.8f;
}

void SausageEntity::collideMapLeft()
{
  velocity.x = -velocity.x * 0.8f;
}

void SausageEntity::collideMapTop()
{
  velocity.y = -velocity.y * 0.8f;
}

void SausageEntity::collideMapBottom()
{
  velocity.y = -velocity.y * 0.8f;
}

void SausageEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead()) dying();

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f) collideWithBolt(boltEntity);
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX)
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

void SausageEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 50.0f );
    giveRepulsion(false, vel, 0.3f);
}

void SausageEntity::dying()
{
  EnemyEntity::dying();
  new ExplosionEntity(x, y, ExplosionTypeStandard, 18, EnemyTypeNone, true);
  SoundManager::getInstance().playSound(SOUND_BOOM_00);
}

void SausageEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}
