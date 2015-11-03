#include "EnemyBoltEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "WitchBlastGame.h"

EnemyBoltEntity::EnemyBoltEntity(float x, float y, enumShotType boltType, int level, enemyTypeEnum enemyType) :
  BoltEntity(x, y, -1, boltType, level)
{
  damages = INITIAL_BOLT_DAMAGES;
  type = ENTITY_ENEMY_BOLT;
  this->enemyType = enemyType;
  viscosity = 1.0f;
  switch (boltType)
  {
    case ShotTypeIce: frame = 2; break;
    case ShotTypeLightning: frame = 5; break;
    case ShotTypeFire: frame = 6; break;
    case ShotTypeBomb: frame = 8; break;
    case ShotTypeIllusion:  frame = 3; break;
    default: frame = 1; break;
  }
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, 0, 0);
}

void EnemyBoltEntity::animate(float delay)
{
  if (boltType != ShotTypeBomb)
    particleGenerator.GenerateParticles(frame, IMAGE_BOLT, x, y, BOLT_WIDTH, BOLT_HEIGHT,
                        boltType == ShotTypeLightning ? Vector2D(20.0f) : Vector2D(0.0f, 0.0f),
                        10, renderScale);

  z = y + height;
  CollidingSpriteEntity::animate(delay);

  if (boltType != ShotTypeBomb && (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }

  if (boltType == ShotTypeIllusion)
  {
    float limit = 16;
    if (x < limit
        || x > TILE_WIDTH * MAP_WIDTH - limit
        || y < limit
        || y > TILE_HEIGHT * MAP_HEIGHT - limit)
    {
      isDying = true;
    }
  }
}

void EnemyBoltEntity::onDying()
{
  if (!isDying)
  {
    isDying = true;
    // TO DO effect zone ?
  }
}
