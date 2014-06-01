#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "Constants.h"

#include "WitchBlastGame.h"

BaseCreatureEntity::BaseCreatureEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int spriteWidth = -1, int spriteHeight = -1)
  : CollidingSpriteEntity (image, x, y, spriteWidth, spriteHeight )
{
  hurting = false;
  hurtingType = ShotTypeStandard;
  shadowFrame = -1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  hpDisplay = 0;
  movingStyle = movWalking;
  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    specialState[i].type = (enumSpecialState)i;
    specialState[i].resistance = ResistanceStandard;
    specialState[i].active = false;
    specialState[i].timer = 0.0f;
  }
  recoil.active = false;
  facingDirection = 2;
}

int BaseCreatureEntity::getHp()
{
  return hp;
}

int BaseCreatureEntity::getHpMax()
{
  return hpMax;
}

void BaseCreatureEntity::setHp(int hp)
{
  this->hp = hp;
}

void BaseCreatureEntity::setHpMax(int hpMax)
{
  this->hpMax = hpMax;
}

int BaseCreatureEntity::getHpDisplay()
{
  return hpDisplay;
}

IntCoord BaseCreatureEntity::getCurrentTile()
{
  int xMap = (x - OFFSET_X) / TILE_WIDTH;
  int yMap = (y - OFFSET_Y) / TILE_HEIGHT;
  return (IntCoord(xMap, yMap));
}

BaseCreatureEntity::enumMovingStyle BaseCreatureEntity::getMovingStyle()
{
  return movingStyle;
}

float BaseCreatureEntity::animateStates(float delay)
{
  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    if (specialState[i].active)
    {
      specialState[i].timer -= delay;
      if (specialState[i].timer <= 0.0f) specialState[i].active = false;
    }
  }
  if (specialState[SpecialStateIce].active) delay *= STATUS_FROZEN_MULT;
  return delay;
}

void BaseCreatureEntity::animateColors(float delay)
{
  // no color
  sprite.setColor(sf::Color(255, 255, 255, 255 ));

  if (hurting and hp > 0)
  {
    hurtingDelay -= delay;
    if (hurtingDelay > 0.0f)
    {
      int fadeColor = (sf::Uint8)((HURTING_DELAY - hurtingDelay) * 255);
      if (hurtingType == ShotTypeIce)
        sprite.setColor(sf::Color(fadeColor, fadeColor, 255, 255 )); // blue
      else
        sprite.setColor(sf::Color(255, fadeColor, fadeColor, 255 )); // red
    }
    else
    {
      hurting = false;
      sprite.setColor(sf::Color(255, 255, 255, 255 ));
    }
  }
  if (specialState[SpecialStateIce].active) sprite.setColor(sf::Color(100, 100, 255, 255 ));
}

void BaseCreatureEntity::animateRecoil(float delay)
{
  // recoil
  if (recoil.active)
  {
    recoil.velocity.x *= 0.97f;
    recoil.velocity.y *= 0.97f;

    recoil.timer -= delay;
    if (recoil.timer <= 0.0f)
    {
      recoil.active = false;
      computeFacingDirection();
      // TODO ?
    }
  }
}

void BaseCreatureEntity::animatePhysics(float delay)
{
	velocity.x *= viscosity;
	velocity.y *= viscosity;

	float velx = velocity.x;
	float vely = velocity.y;

	if (recoil.active)
  {
    if (recoil.stun)
    {
      velx = 0.0f;
      vely = 0.0f;
    }
    velx += recoil.velocity.x;
    vely += recoil.velocity.y;
  }

  spin *= viscosity;
	angle += spin * delay;

    if ((int)velx > 0)
    {
        x += velx * delay;

        if (collideWithMap(DIRECTION_LEFT))
        {
            x = (float)((int)x);
            while (collideWithMap(DIRECTION_LEFT))
                x--;
            collideMapRight();
        }
        else if (x > map->getWidth() * tileWidth + offsetX)
        {
            exitMap(DIRECTION_RIGHT);
        }
    }
    else if ((int)velx < 0)
    {
        x += velx * delay;

        if (collideWithMap(DIRECTION_RIGHT))
        {
            x = (float)((int)x);
            while (collideWithMap(DIRECTION_RIGHT))
                x++;
            collideMapLeft();
        }
        else if (x < offsetX)
        {
            exitMap(DIRECTION_LEFT);
        }
    }

    vely += weight * delay;
    if ( vely > maxY) vely = maxY;

    if ((int)vely > 0)
    {
        y += vely * delay;

        if (collideWithMap(DIRECTION_BOTTOM))
        {
            y = (float)((int)y);
            while (collideWithMap(DIRECTION_BOTTOM))
                y--;
            collideMapBottom();
        }
    }
    else if ((int)vely < 0)
    {
        y += vely * delay;

        if (collideWithMap(DIRECTION_TOP))
        {
            y = (float)((int)y);
            while (collideWithMap(DIRECTION_TOP))
                y++;
            collideMapTop();
        }
    }

    if (lifetime > 0)
    {
        if (age >= lifetime) isDying = true;
    }
    age += delay;
}

void BaseCreatureEntity::animate(float delay)
{
  if (hpDisplay > hp) hpDisplay--;
  else if (hpDisplay < hp) hpDisplay++;

  delay = animateStates(delay);
  animateColors(delay);
  animateRecoil(delay);
  animatePhysics(delay);

  z = y + height/2;
}

void BaseCreatureEntity::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  CollidingSpriteEntity::render(app);

  #ifdef SHOW_BOUNDING_BOX
  sf::Vertex line[] =
  {
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top))
  };
  app->draw(line, 8, sf::Lines);
  #endif
}

void BaseCreatureEntity::calculateBB()
{
}

bool BaseCreatureEntity::collideWithMap(int direction)
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
      if (movingStyle == movWalking)
      {
        if ( dynamic_cast<DungeonMap*>(map)->isWalkable(xTile, yTile) == false ) return true;
      }
      else if (movingStyle == movFlying)
      {
        if ( dynamic_cast<DungeonMap*>(map)->isFlyable(xTile, yTile) == false ) return true;
      }
    }

    return false;
}

bool BaseCreatureEntity::hurt(int damages, enumShotType hurtingType)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;
  this->hurtingType = hurtingType;

  if (hurtingType == ShotTypeIce && specialState[SpecialStateIce].resistance > ResistanceImmune)
  {
    specialState[SpecialStateIce].active = true;
    specialState[SpecialStateIce].timer = STATUS_FROZEN_DELAY;
  }

  hp -= damages;
  if (hp <= 0)
  {
    hp = 0;
    prepareDying();
  }

  return true;
}

void BaseCreatureEntity::prepareDying()
{
  dying();
}

void BaseCreatureEntity::dying()
{
  isDying = true;
}

void BaseCreatureEntity::computeFacingDirection()
{
  if (abs((int)velocity.x) > 0 && abs((int)velocity.y) > 0)
  {
    if (abs((int)velocity.x) > abs((int)velocity.y))
    {
      if (velocity.x > 0.0f) facingDirection = 6;
      else facingDirection = 4;
    }
    else
    {
      if (velocity.y > 0.0f) facingDirection = 2;
      else facingDirection = 8;
    }
  }
}

void BaseCreatureEntity::giveRecoil(bool stun, Vector2D velocity, float timer)
{
  if (!(recoil.active && recoil.stun))
  {
    recoil.active = true;
    recoil.stun = stun;
    recoil.velocity = velocity;
    recoil.timer = timer;
  }
}

void BaseCreatureEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
}
