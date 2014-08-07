#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>
#include <sstream>

BaseCreatureEntity::BaseCreatureEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int spriteWidth = -1, int spriteHeight = -1)
  : CollidingSpriteEntity (image, x, y, spriteWidth, spriteHeight )
{
  hurting = false;
  hurtingType = ShotTypeStandard;
  shadowFrame = -1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  hpDisplay = 0;
  armor = 0.0f;
  movingStyle = movWalking;
  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    specialState[i].type = (enumSpecialState)i;
    specialState[i].active = false;
    specialState[i].timer = 0.0f;
    specialState[i].parameter = 0.0f;
  }
  for (int i = 0; i < NB_RESISTANCES; i++)
  {
    resistance[i] = ResistanceStandard;
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

float BaseCreatureEntity::getCreatureSpeed()
{
  return creatureSpeed;
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

bool BaseCreatureEntity::isSpecialStateActive(enumSpecialState state)
{
  return specialState[state].active;
}

void BaseCreatureEntity::setSpecialState(enumSpecialState state, bool active, float timer, float parameter)
{
  specialState[state].active = active;
  specialState[state].timer = timer;
  specialState[state].parameter = parameter;
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
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].parameter;
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
      if (hurtingDelay > HURTING_DELAY) fadeColor = 0;
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

	if (specialState[SpecialStateSlow].active)
  {
    velx *= specialState[SpecialStateSlow].parameter;
    vely *= specialState[SpecialStateSlow].parameter;
  }

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

	if (isCollidingWithMap())
  {
    stuck();
  }
  else
  {
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
        if (age >= lifetime) dyingFromAge();
    }
    age += delay;
}

void BaseCreatureEntity::dyingFromAge()
{
  dying();
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

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
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

bool BaseCreatureEntity::determineSatusChance(enumStateResistance resistance, int level)
{
  bool hit = true;
  switch (resistance)
  {
    case ResistanceVeryLow:
    case ResistanceLow:
    case ResistanceStandard: hit = true; break;
    case ResistanceHigh: hit = rand() % 8 <= level * 2; break;
    case ResistanceVeryHigh: hit = rand() % 10 <= level * 2; break;
    case ResistanceImmune: hit = false; break;
  }
  return hit;
}

int BaseCreatureEntity::determineDamageBonus(enumStateResistance resistance, int level)
{
  int bonus = 0;
  switch (resistance)
  {
    case ResistanceVeryLow: bonus = 40 + 10 * level; break;
    case ResistanceLow: bonus = 20 + 5 * level; break;
    case ResistanceStandard: bonus = 0; break;
    case ResistanceHigh: bonus = -25 + 5 * level; break;
    case ResistanceVeryHigh: bonus = -50 + 5 * level; break;
    case ResistanceImmune: bonus = -100 + 5 * level; break;
  }
  return bonus;
}

bool BaseCreatureEntity::textTooClose(TextEntity* textEntity, float xDistMin, float yDistMin)
{
  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

		if (e->getType() == ENTITY_FLYING_TEXT)
		{
		  if (e != textEntity)
      {
        if (textEntity->getX() - e->getX() < xDistMin && textEntity->getX() - e->getX() > - xDistMin
            && textEntity->getY() - e->getY() < yDistMin && textEntity->getY() - e->getY() > - yDistMin)
              return true;
      }
		}
	}
	return false;
}

bool BaseCreatureEntity::hurt(int damages, enumShotType hurtingType, int level)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;
  this->hurtingType = hurtingType;

  if (armor > 0.01f)
  {
    int absorbedHp = damages * armor;
    if (absorbedHp == 0) absorbedHp = 1;
    damages -= absorbedHp;
  }

  if (hurtingType == ShotTypeIce
      && determineSatusChance(resistance[ResistanceFrozen], level))
  {
    // frozen ?
    specialState[SpecialStateIce].active = true;
    float frozenDelayMult = 1.0f;
    float frozenMultAdd = 0.0f;
    if (resistance[ResistanceFrozen] == ResistanceHigh)
    {
      if (level == 0)
      {
        frozenDelayMult = 0.8f;
        frozenMultAdd = 0.2f;
      }
      else
      {
        frozenDelayMult = 0.85f;
        frozenMultAdd = 0.15f;
      }
    }
    else if (resistance[ResistanceFrozen] == ResistanceVeryHigh)
    {
      if (level < 2)
      {
        frozenDelayMult = 0.7f;
        frozenMultAdd = 0.25f;
      }
      else
      {
        frozenDelayMult = 0.8f;
        frozenMultAdd = 0.2f;
      }
    }
    specialState[SpecialStateIce].timer = STATUS_FROZEN_DELAY[level] * frozenDelayMult;
    specialState[SpecialStateIce].parameter = STATUS_FROZEN_MULT[level] + frozenMultAdd;
  }

  // damages bonus
  if (hurtingType == ShotTypeIce)
    damages += (damages * determineDamageBonus(resistance[ResistanceIce], level)) / 100;
  else if (hurtingType == ShotTypeFire)
    damages += (damages * determineDamageBonus(resistance[ResistanceFire], level)) / 100;
  else if (hurtingType == ShotTypeLightning)
    damages += (damages * determineDamageBonus(resistance[ResistanceLightning], level)) / 100;
  else if (hurtingType == ShotTypeStone)
    damages += (damages * determineDamageBonus(resistance[ResistanceStone], level)) / 100;
  else if (hurtingType == ShotTypeIllusion)
    damages += (damages * determineDamageBonus(resistance[ResistanceIllusion], level)) / 100;

  hp -= damages;
  if (hp <= 0)
  {
    hp = 0;
    prepareDying();
  }

  std::ostringstream oss;
  oss << "-" << damages;
  int textSize;
  if (damages < 8) textSize = 17;
  else textSize = 17 + (damages - 4) / 4;
  TextEntity* text = new TextEntity(oss.str(), 18, x, y - 20.0f);
  text->setColor(TextEntity::COLOR_FADING_RED);
  text->setAge(-1.0f);
  text->setLifetime(1.2f);
  text->setWeight(-60.0f);
  text->setZ(2000);
  text->setType(ENTITY_FLYING_TEXT);
  while (textTooClose(text, 15, 15)) text->setY(text->getY() - 5);

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
  if (abs((int)velocity.x) > 0 || abs((int)velocity.y) > 0)
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

bool BaseCreatureEntity::canCollide()
{
  return true;
}
