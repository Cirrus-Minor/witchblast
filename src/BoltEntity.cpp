#include "BoltEntity.h"
#include "ExplosionEntity.h"
#include "Constants.h"
#include "DungeonMap.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"

BoltEntity::BoltEntity(float x, float y, float boltLifeTime, enumShotType boltType, int level)
: CollidingSpriteEntity (ImageManager::getInstance().getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT)
{
  lifetime = boltLifeTime;
  setDamages(INITIAL_BOLT_DAMAGES);
  type = ENTITY_BOLT;
  viscosity = INITIAL_BOLT_VISCOSITY;
  this->level = level;
  if (boltType == ShotTypeLightning) viscosity += LIGHTNING_VISCOSITY_INCREASE[level];
  frame = 0;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  this->boltType = boltType;
  switch (boltType)
  {
    case ShotTypeDeterministic:
    case ShotTypeStandard:  frame = 0; break;
    case ShotTypeCold:
    case ShotTypeIce:       frame = 2; break;
    case ShotTypeStone:     frame = 4; break;
    case ShotTypeLightning: frame = 5; break;
    case ShotTypeIllusion:  frame = 3; break;
    case ShotTypeFire:      frame = 6; break;
    case ShotTypePoison:    frame = 7; break;
    case ShotTypeBomb:      frame = 8; damages = 0; sprite.setScale(1.0f, 1.0f); break;
  }
  testWallsCollision = false;
  flying = false;
  critical = false;

  // avoid starting in wall
  if (y > (OFFSET_Y + (MAP_HEIGHT - 1) * TILE_HEIGHT - 16))
    this->y = OFFSET_Y + (MAP_HEIGHT - 1) * TILE_HEIGHT - 16;
}

int BoltEntity::getDamages()
{
  return damages;
}

unsigned int BoltEntity::getLevel()
{
  return level;
}

void BoltEntity::setDamages(int damages)
{
  this->damages = damages;

  if (damages <= 4)  renderScale = 0.7f;
  else if (damages <= 6)  renderScale = 0.78f;
  else if (damages <= 8)  renderScale = 0.85f;
  else if (damages <= 12) renderScale = 0.9f;
  else if (damages <= 16) renderScale = 1.0f;
  else if (damages <= 20) renderScale = 1.1f;
  else if (damages <= 24) renderScale = 1.2f;
  else if (damages <= 30) renderScale = 1.3f;
  else renderScale = 1.4f;

  sprite.scale(renderScale, renderScale);
}

enumShotType BoltEntity::getBoltType()
{
  return boltType;
}

bool BoltEntity::isFlying()
{
    return flying;
}

void BoltEntity::setFlying(bool flying)
{
  this->flying = flying;
}

bool BoltEntity::isCritical()
{
    return critical;
}

void BoltEntity::setCritical(bool critical)
{
  this->critical = critical;
}

void BoltEntity::animate(float delay)
{
  if (boltType != ShotTypeBomb)
    particleGenerator.GenerateParticles(frame, IMAGE_BOLT, x, y, BOLT_WIDTH, BOLT_HEIGHT,
                        boltType == ShotTypeLightning ? Vector2D(20.0f) : Vector2D(0.0f, 0.0f),
                        10, boltType == ShotTypeIce ? renderScale * 1.3f : renderScale);

  z = y + height;

  testWallsCollision = true;
  CollidingSpriteEntity::animate(delay);
  testWallsCollision = false;
  calculateBB();

  if (boltType != ShotTypeBomb && boltType != ShotTypeStone && (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }

  if (((velocity.x)*(velocity.x) + (velocity.y)*(velocity.y)) < 1500.0f) isDying = true;
}

void BoltEntity::render(sf::RenderTarget* app)
{
  CollidingSpriteEntity::render(app);
  if (game().getShowLogical())
  {
    displayBoundingBox(app);
  }
}

void BoltEntity::calculateBB()
{
  int colSize = testWallsCollision ? 1 : 10;

  boundingBox.left = x - colSize;
  boundingBox.width = colSize * 2;
  boundingBox.top = y - colSize;
  boundingBox.height =  colSize * 2;
}

void BoltEntity::collide()
{
  isDying = true;
  if (boltType == ShotTypeBomb)
    explode();
  else
  {
    for (int i=0; i<5; i++)
    {
      Vector2D vel(40.0f + rand() % 50);
      generateParticule(vel);
    }
  }
}

void BoltEntity::generateParticule(Vector2D vel)
{
  SpriteEntity* trace = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BOLT), x, y, BOLT_WIDTH, BOLT_HEIGHT);
  trace->setFading(true);
  trace->setZ(y);
  trace->setLifetime(0.5f);
  trace->setScale(0.3f, 0.3f);
  trace->setVelocity(vel);
  trace->setViscosity(0.97f);
  trace->setType(ENTITY_EFFECT);
  trace->setFrame(frame);
}

bool BoltEntity::collideWithMap(int direction)
{
    calculateBB();

    int xTile0 = (boundingBox.left - offsetX) / tileWidth;
    int xTilef = (boundingBox.left + boundingBox.width - offsetX) / tileWidth;
    int yTile0 = (boundingBox.top - offsetY) / tileHeight;
    int yTilef = (boundingBox.top + boundingBox.height - offsetY) / tileHeight;

    if (boundingBox.top < 0) yTile0 = -1;

    for (int xTile = xTile0; xTile <= xTilef; xTile++)
        for (int yTile = yTile0; yTile <= yTilef; yTile++)
        {
          if (boltType != ShotTypeIllusion)
          {
            if (flying)
            {
              if ( dynamic_cast<DungeonMap*>(map)->isFlyable(xTile, yTile) == false ) return true;
            }
            else
            {
              if ( dynamic_cast<DungeonMap*>(map)->isShootable(xTile, yTile) == false ) return true;
            }
          }
        }

    return false;
}

void BoltEntity::onDying()
{
  isDying = true;
}

void BoltEntity::stuck()
{
  SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
  if (boltType == ShotTypeBomb)
    explode();
  else
  {
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      generateParticule(vel);
    }
  }

  onDying();
}

void BoltEntity::collideMapRight()
{
  if (boltType == ShotTypeLightning)
  {
    velocity.x = -velocity.x;
  }
  else if (boltType == ShotTypeBomb)
    explode();
  else
  {
    velocity.x = 0.0f;
    isDying = true;

    SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x > 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
  }
}

void BoltEntity::collideMapLeft()
{
  if (boltType == ShotTypeLightning)
  {
    velocity.x = -velocity.x;
  }
  else if (boltType == ShotTypeBomb)
    explode();
  else
  {
    velocity.x = 0.0f;
    isDying = true;

    SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.x < 0.0f) vel.x = - vel.x;
      generateParticule(vel);
    }
  }
}

void BoltEntity::collideMapTop()
{
  if (boltType == ShotTypeLightning)
  {
    velocity.y = -velocity.y;
  }
  else if (boltType == ShotTypeBomb)
    explode();
  else
  {
    velocity.y = 0.0f;
    isDying = true;

    SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y < 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
  }
}

void BoltEntity::collideMapBottom()
{
  if (boltType == ShotTypeLightning)
  {
    velocity.y = -velocity.y;
  }
  else if (boltType == ShotTypeBomb)
    explode();
  else
  {
    velocity.y = 0.0f;
    isDying = true;

    SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++)
    {
      Vector2D vel(100.0f + rand() % 150);
      if (vel.y > 0.0f) vel.y = - vel.y;
      generateParticule(vel);
    }
  }
}

void BoltEntity::explode()
{
  isDying = true;
  new ExplosionEntity(x, y, ExplosionTypeStandard, 12);
  game().makeShake(0.5f);
  SoundManager::getInstance().playSound(SOUND_BOOM_00);

  SpriteEntity* corpse= new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_CORPSES), x, y, 64, 64);
  corpse->setFrame(FRAME_CORPSE_SLIME_VIOLET);
  corpse->setImagesProLine(10);
  corpse->setZ(OFFSET_Y);
  corpse->setType(ENTITY_CORPSE);
}
