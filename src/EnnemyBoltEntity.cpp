#include "EnnemyBoltEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "WitchBlastGame.h"

EnnemyBoltEntity::EnnemyBoltEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f) : CollidingSpriteEntity (image, x, y, BOLT_WIDTH, BOLT_HEIGHT)
{
  damages = INITIAL_BOLT_DAMAGES;
  type = ENTITY_ENNEMY_BOLT;
  frame = 1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
}

int EnnemyBoltEntity::getDamages()
{
  return damages;
}

void EnnemyBoltEntity::setDamages(int damages)
{
  this->damages = damages;
}

void EnnemyBoltEntity::animate(float delay)
{
  SpriteEntity* trace = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
  trace->setFading(true);
  trace->setZ(y);
  trace->setLifetime(0.2f);
  trace->setShrinking(true);
  trace->setFrame(frame);
  trace->setType(16);

  z = y + height;
  CollidingSpriteEntity::animate(delay);

  if ( (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }
}

void EnnemyBoltEntity::collide()
{
  isDying = true;
}

void EnnemyBoltEntity::generateParticule(Vector2D vel)
{
  SpriteEntity* trace = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
  trace->setFading(true);
  trace->setZ(y);
  trace->setLifetime(0.5f);
  trace->setScale(0.3f, 0.3f);
  trace->setVelocity(vel);
  trace->setViscosity(0.97f);
  trace->setFrame(frame);
  trace->setType(16);
}

void EnnemyBoltEntity::collideMapRight()
{
    velocity.x = 0.0f;
    isDying = true;

    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x > 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
}

void EnnemyBoltEntity::collideMapLeft()
{
    velocity.x = 0.0f;
    isDying = true;

    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x < 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
}

void EnnemyBoltEntity::collideMapTop()
{
    velocity.y = 0.0f;
    isDying = true;

    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y < 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
}

void EnnemyBoltEntity::collideMapBottom()
{
    velocity.y = 0.0f;
    isDying = true;

    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y > 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
}
