#include "BoltEntity.h"
#include "ExplosionEntity.h"
#include "EffectZoneEntity.h"
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
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, 0, 0);
  this->boltType = boltType;
  enemyType = EnemyTypeNone;
  goThrough = false;
  hitNumber = 0;
  fromPlayer = true;

  switch (boltType)
  {
    case ShotTypeDeterministic:
    case ShotTypeStandard:  frame = 0; break;
    case ShotTypeCold:
    case ShotTypeIce:       frame = 2; break;
    case ShotTypeStone:     frame = 4; break;
    case ShotTypeLightning: frame = 5; goThrough = true; break;
    case ShotTypeIllusion:  frame = 3; goThrough = level >= 2; break;
    case ShotTypeFire:      frame = 6; break;
    case ShotTypePoison:    frame = 7; break;
    case ShotTypeBomb:      frame = 8; damages = 0; sprite.setScale(1.0f, 1.0f); break;
  }
  testWallsCollision = false;
  flying = false;
  critical = false;

  // avoid starting in wall
  if (y > ((MAP_HEIGHT - 1) * TILE_HEIGHT - 16))
    this->y = (MAP_HEIGHT - 1) * TILE_HEIGHT - 16;
}

int BoltEntity::getDamages()
{
  return damages;
}

void BoltEntity::setFromPlayer(bool fromPlayer)
{
  this->fromPlayer = fromPlayer;
}

bool BoltEntity::isFromPlayer()
{
  return fromPlayer;
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

void BoltEntity::loseDamages(int damages)
{
  if (this->damages > damages) setDamages(this->damages - damages);
  else setDamages(0);
  critical = false;
}

enumShotType BoltEntity::getBoltType()
{
  return boltType;
}

enemyTypeEnum BoltEntity::getEnemyType()
{
  return enemyType;
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

void BoltEntity::setGoThrough(bool goThrough)
{
  this->goThrough = goThrough;
}

bool BoltEntity::getGoThrough()
{
  return goThrough;
}

void BoltEntity::animate(float delay)
{
  if (boltType != ShotTypeBomb)
    particleGenerator.GenerateParticles(frame, IMAGE_BOLT, x, y, BOLT_WIDTH, BOLT_HEIGHT,
                        boltType == ShotTypeLightning ? Vector2D(20.0f) : Vector2D(0.0f, 0.0f),
                        10, boltType == ShotTypeIce ? renderScale * 1.3f : renderScale);

  z = y + height;

  testWallsCollision = true;
  if (isCollidingWithMap())
  {
    onDying();
    SoundManager::getInstance().playSound(SOUND_WALL_IMPACT);
    for (int i=0; i<5; i++) generateParticule(100.0f + rand() % 150);
  }
  else
  {
    float oldX = x;
    float oldY = y;
    CollidingSpriteEntity::animate(delay);
    if (boltType != ShotTypeBomb && (game().getPlayer()->isEquiped(EQUIP_RAPID_SHOT) || damages < 5))
      particleGenerator.GenerateParticles(frame, IMAGE_BOLT, (x + oldX) * 0.5f, (y + oldY) * 0.5f, BOLT_WIDTH, BOLT_HEIGHT,
                        boltType == ShotTypeLightning ? Vector2D(20.0f) : Vector2D(0.0f, 0.0f),
                        10, boltType == ShotTypeIce ? renderScale * 1.3f : renderScale);

  }


  // key room collision
  if (game().getCurrentMap()->getRoomType() == roomTypeKey && !game().getCurrentMap()->isCleared())
  {
    sf::IntRect col1;
    col1.width = 198;
    col1.height = 68;
    col1.top = 254;
    col1.left = 380;

    sf::IntRect col2;
    col2.width = 68;
    col2.height = 198;
    col2.top = 189;
    col2.left = 445;

    if (boundingBox.intersects(col1) || boundingBox.intersects(col2))
    {
      game().activateKeyRoomEffect(false);
      if (x < 390) collideMapRight();
      else if (x > 565) collideMapLeft();
      else if (y < 265) collideMapBottom();
      else collideMapTop();
    }
  }

  testWallsCollision = false;
  calculateBB();

  if (boltType != ShotTypeBomb && boltType != ShotTypeStone && (lifetime - age) < 0.2f)
  {
    if (age >= lifetime)
      sprite.setColor(sf::Color(255, 255, 255, 0));
    else
      sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)((lifetime - age) / 0.2f * 255)));
  }

  if (((velocity.x)*(velocity.x) + (velocity.y)*(velocity.y)) < 1500.0f) onDying();
}

void BoltEntity::render(sf::RenderTarget* app)
{
  if (boltType == ShotTypeBomb)
  {
    sprite.setTextureRect(sf::IntRect(8 * width, 0, width * 2, height * 2));
    sprite.setPosition(x - width / 2, y - height);
    app->draw(sprite);
  }
  else
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
  hitNumber++;
  if (fromPlayer)
  {
    if (hitNumber == 4) game().registerAchievement(Achievement4Hits);

    if (goThrough)
    {
      if (damages > 0)
      {
        if (damages > 1 && boltType == ShotTypeLightning && level == 2)
        {
          // bolt lvl3 effect
          if (damages <= 2) damages += 4;
          else if (damages <= 5) damages += 3;
          else damages += 2;

          for (int i = 0; i < 3; i++)
          {
            BoltEntity* bolt = new BoltEntity(x, y, lifetime, ShotTypeLightning, 1);
            bolt->setDamages(damages / 2);
            float shotAngle = rand() % 360;
            float fireVelocity = 400.0f;
            bolt->setVelocity(Vector2D(fireVelocity * cos(shotAngle), fireVelocity * sin(shotAngle)));
          }
          damages /= 2;
          SoundManager::getInstance().playSound(SOUND_ELECTRIC_CHARGE);
        }
        return;
      }
    }
  }

  onDying();
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
  if (!isDying)
  {
    isDying = true;

    // special attacks
    if (boltType == ShotTypeIce && level == 2)
      new EffectZoneEntity(x, y, true, 5.0f, EffectZoneTypeIce);

    else if (boltType == ShotTypePoison && level == 2)
    {
      if (!game().existsEffectZone())
        new EffectZoneEntity(x, y, true, 5.0f, EffectZoneTypePoison);
    }
  }
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

void BoltEntity::split(int direction)
{
  switch (direction)
  {
    case 4: x += 8; break;
    case 6: x -= 8; break;
    case 2: y -= 8; break;
    case 8: y += 8; break;
  }
  float currentVelocity = velocity.norm();
  lifetime *= 1.25f;
  BoltEntity* bolt2 = new BoltEntity(x, y, lifetime, ShotTypeStone, 1);
  level = 1;
  damages *= 0.75f;
  bolt2->setDamages(damages);
  bolt2->setX(x);
  bolt2->setY(y);
  bolt2->setAge(age);
  if (direction == 2 || direction == 8)
  {
    velocity.x = -currentVelocity;
    velocity.y = 0.0f;

    bolt2->setVelocity(Vector2D(currentVelocity, 0.0f));
  }
  else
  {
    velocity.y = -currentVelocity;
    velocity.x = 0.0f;

    bolt2->setVelocity(Vector2D(0.0f, currentVelocity));
  }
  SoundManager::getInstance().playSound(SOUND_STONE_HIT);
}

void BoltEntity::collideWall()
{
  if (boltType == ShotTypeFire && level == 2)
  {
    if (!game().existsEffectZone())
      new EffectZoneEntity(x, y, true, 5.0f, EffectZoneTypeFire, game().getPlayer()->getDps());
  }
}

void BoltEntity::collideMapRight()
{
  if (boltType == ShotTypeLightning)
  {
    velocity.x = -velocity.x;
  }
  else if (boltType == ShotTypeBomb)
  {
    explode();
  }
  else if (boltType == ShotTypeStone && level == 2)
  {
    split(6);
  }
  else
  {
    velocity.x = 0.0f;
    onDying();
    collideWall();

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
  {
    explode();
  }
  else if (boltType == ShotTypeStone && level == 2)
  {
    split(4);
  }
  else
  {
    velocity.x = 0.0f;
    onDying();
    collideWall();

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
  {
    explode();
  }
  else if (boltType == ShotTypeStone && level == 2)
  {
    split(8);
  }
  else
  {
    velocity.y = 0.0f;
    onDying();
    collideWall();

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
  {
    explode();
  }
  else if (boltType == ShotTypeStone && level == 2)
  {
    split(2);
  }
  else
  {
    velocity.y = 0.0f;
    onDying();
    collideWall();

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
  onDying();
  new ExplosionEntity(x, y, ExplosionTypeStandard, 12, enemyType, true);
  game().makeShake(0.5f);
  SoundManager::getInstance().playSound(SOUND_BOOM_00);
  game().addCorpse(x, y, FRAME_CORPSE_SLIME_VIOLET);
}
