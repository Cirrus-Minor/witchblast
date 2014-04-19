#include "BoltEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"

BoltEntity::BoltEntity(sf::Texture* image, float x, float y, float boltLifeTime) : CollidingSpriteEntity (image, x, y, BOLT_WIDTH, BOLT_HEIGHT)
{
  lifetime = boltLifeTime;
  setDamages(INITIAL_BOLT_DAMAGES);
  type = ENTITY_BOLT;
  viscosity = 0.97f;
  frame = 0;
}

int BoltEntity::getDamages()
{
  return damages;
}

void BoltEntity::setDamages(int damages)
{
  this->damages = damages;

  if (damages <= 4)  renderScale = 0.8f;
  else if (damages <= 8)  renderScale = 0.85f;
  else if (damages <= 12) renderScale = 0.9f;
  else if (damages <= 16) renderScale = 1.0f;
  else if (damages <= 20) renderScale = 1.1f;
  else if (damages <= 24) renderScale = 1.2f;
  else if (damages <= 30) renderScale = 1.3f;
  else renderScale = 1.4f;

  sprite.scale(renderScale, renderScale);
}

void BoltEntity::animate(float delay)
{
  SpriteEntity* trace = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
  trace->setFading(true);
  trace->setZ(y);
  trace->setLifetime(0.2f);
  trace->setShrinking(true, renderScale, renderScale);
  trace->setType(16);

  z = y + height;
  //if (viscosity < 1.0f && ((velocity.x)*(velocity.x) + (velocity.y)*(velocity.y)) < 900.0f) viscosity = 1.0f;

  CollidingSpriteEntity::animate(delay);

  if ( (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }

  if (((velocity.x)*(velocity.x) + (velocity.y)*(velocity.y)) < 1500.0f) isDying = true;
}

void BoltEntity::collide()
{
  isDying = true;
  for (int i=0; i<5; i++)
    {
      Vector2D vel(40.0f + rand() % 50);
      generateParticule(vel);
    }
}

void BoltEntity::generateParticule(Vector2D vel)
{
  SpriteEntity* trace = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
  trace->setFading(true);
  trace->setZ(y);
  trace->setLifetime(0.5f);
  trace->setScale(0.3f, 0.3f);
  trace->setVelocity(vel);
  trace->setViscosity(0.97f);
  trace->setType(16);
}

void BoltEntity::collideMapRight()
{
    velocity.x = 0.0f;
    isDying = true;

    SoundManager::getSoundManager()->playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x > 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
}

void BoltEntity::collideMapLeft()
{
    velocity.x = 0.0f;
    isDying = true;

    SoundManager::getSoundManager()->playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x < 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
}

void BoltEntity::collideMapTop()
{
    velocity.y = 0.0f;
    isDying = true;

    SoundManager::getSoundManager()->playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y < 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
}

void BoltEntity::collideMapBottom()
{
    velocity.y = 0.0f;
    isDying = true;

    SoundManager::getSoundManager()->playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y > 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
}
