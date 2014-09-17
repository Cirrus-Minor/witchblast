#include "EnnemyBoltEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "WitchBlastGame.h"

EnnemyBoltEntity::EnnemyBoltEntity(float x, float y, enumShotType boltType, int level) :
  BoltEntity(x, y, -1, boltType, level)
{
  damages = INITIAL_BOLT_DAMAGES;
  type = ENTITY_ENNEMY_BOLT;
  viscosity = 1.0f;
  switch (boltType)
  {
    case ShotTypeIce: frame = 2; break;
    case ShotTypeFire: frame = 6; break;
    case ShotTypeBomb: frame = 8; break;
    default: frame = 1; break;
  }
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
}

void EnnemyBoltEntity::animate(float delay)
{
  if (boltType != ShotTypeBomb)
  {
    SpriteEntity* trace = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
    trace->setFading(true);
    trace->setZ(y);
    trace->setLifetime(0.2f);
    trace->setShrinking(true, renderScale, renderScale);
    trace->setType(ENTITY_EFFECT);
    trace->setFrame(frame);
  }

  z = y + height;
  CollidingSpriteEntity::animate(delay);

  if (boltType != ShotTypeBomb && (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }
}
