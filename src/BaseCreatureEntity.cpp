#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

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
    specialState[i].param1 = 0.0f;
    specialState[i].param2 = 0.0f;
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

void BaseCreatureEntity::setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2)
{
  specialState[state].active = active;
  specialState[state].timer = timer;
  specialState[state].param1 = param1;
  specialState[state].param2 = param2;
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
  // ice
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  // poison
  if (specialState[SpecialStatePoison].active)
  {
    specialState[SpecialStatePoison].param3 -= delay;
    if (specialState[SpecialStatePoison].param3 <= 0.0f)
    {
      specialState[SpecialStatePoison].param3 += specialState[SpecialStatePoison].param2;
      hurt(specialState[SpecialStatePoison].param1, ShotTypeDeterministic, 0, false);
    }
  }

  return delay;
}

void BaseCreatureEntity::animateColors(float delay)
{
  // no color
  sprite.setColor(sf::Color(255, 255, 255, 255 ));

  if (hurting && hp > 0)
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
    velx *= specialState[SpecialStateSlow].param1;
    vely *= specialState[SpecialStateSlow].param1;
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
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
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

bool BaseCreatureEntity::hurt(int damages, enumShotType hurtingType, int level, bool critical)
{
  if (armor > 0.01f && hurtingType != ShotTypeDeterministic)
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
    specialState[SpecialStateIce].param1 = STATUS_FROZEN_MULT[level] + frozenMultAdd;
  }
  else if (hurtingType == ShotTypePoison
      /*&& determineSatusChance(resistance[ResistanceFrozen], level)*/)
  {
    specialState[SpecialStatePoison].active = true;
    specialState[SpecialStatePoison].timer = 10.5f;
    specialState[SpecialStatePoison].param1 = 1.0f;
    specialState[SpecialStatePoison].param2 = 2.0f;
    specialState[SpecialStatePoison].param3 = 2.0f;
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
  else if (hurtingType == ShotTypePoison)
    damages += (damages * determineDamageBonus(resistance[ResistancePoison], level)) / 100;

  if (damages > 0)
  {
    hurting = true;
    hurtingDelay = HURTING_DELAY;
    this->hurtingType = hurtingType;

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
    else textSize = 17 + (damages - 3) / 5;
    TextEntity* text = new TextEntity(oss.str(), textSize, x, y - 20.0f);
    text->setColor(TextEntity::COLOR_FADING_RED);
    text->setAge(-0.6f);
    text->setLifetime(0.3f);
    text->setWeight(-60.0f);
    text->setZ(2000);
    text->setAlignment(ALIGN_CENTER);
    text->setType(ENTITY_FLYING_TEXT);
    while (textTooClose(text, 15, 15)) text->setY(text->getY() - 5);

    if (critical)
    {
      TextEntity* textCrit = new TextEntity(tools::getLabel("critical"), 16, x, text->getY() - 16.0f);
      textCrit->setColor(TextEntity::COLOR_FADING_RED);
      textCrit->setAge(-0.6f);
      textCrit->setLifetime(0.3f);
      textCrit->setWeight(-60.0f);
      textCrit->setZ(2000);
      textCrit->setAlignment(ALIGN_CENTER);
      textCrit->setType(ENTITY_FLYING_TEXT);
    }

    if (hurtingType == ShotTypePoison)
    {
      TextEntity* textCrit = new TextEntity(tools::getLabel("poison"), 16, x, text->getY() - 16.0f);
      textCrit->setColor(TextEntity::COLOR_FADING_RED);
      textCrit->setAge(-0.6f);
      textCrit->setLifetime(0.3f);
      textCrit->setWeight(-60.0f);
      textCrit->setZ(2000);
      textCrit->setAlignment(ALIGN_CENTER);
      textCrit->setType(ENTITY_FLYING_TEXT);
    }

    if (critical) SoundManager::getInstance().playSound(SOUND_CRITICAL);

    return true;
  }
  return false;
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
  if (resistance[ResistanceRecoil] == ResistanceHigh)
    {
      velocity.x *= 0.75f;
      velocity.y *= 0.75f;
      timer *= 0.75f;
    }
    else if (resistance[ResistanceRecoil] == ResistanceVeryHigh)
    {
      velocity.x *= 0.5f;
      velocity.y *= 0.5f;
      timer *= 0.5f;
    }

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

void BaseCreatureEntity::generateStar(sf::Color starColor)
{
  SpriteEntity* spriteStar = new SpriteEntity(
                           ImageManager::getInstance().getImage(IMAGE_STAR_2),
                            x, y);
  spriteStar->setScale(0.8f, 0.8f);
  spriteStar->setZ(z-1.0f);
  spriteStar->setLifetime(0.8f);
  spriteStar->setSpin(-100 + rand()%200);
  spriteStar->setVelocity(Vector2D(10 + rand()%40));
  spriteStar->setWeight(-150);
  spriteStar->setFading(true);
  spriteStar->setColor(starColor);
  spriteStar->setType(ENTITY_EFFECT);
}
