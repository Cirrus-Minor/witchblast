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
  frame = 1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
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
