#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "ExplosionEntity.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>

BaseCreatureEntity::BaseCreatureEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int spriteWidth = -1, int spriteHeight = -1)
  : CollidingSpriteEntity (image, x, y, spriteWidth, spriteHeight )
{
  hurting = false;
  displayDamage = true;
  hurtingType = ShotTypeStandard;
  shadowFrame = -1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, 0, 0);
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
    specialState[i].param3 = 0.0f;
    specialState[i].waitUnclear = false;
  }
  for (int i = 0; i < NB_RESISTANCES; i++)
  {
    resistance[i] = ResistanceStandard;
  }
  repulsion.active = false;
  facingDirection = 2;
  doesAccelerate = false;
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
  int xMap = (x) / TILE_WIDTH;
  int yMap = (y) / TILE_HEIGHT;
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

specialStateStuct BaseCreatureEntity::getSpecialState(enumSpecialState state)
{
  return specialState[state];
}

void BaseCreatureEntity::setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2,bool waitUnclear)
{
  specialState[state].active = active;
  specialState[state].timer = timer;
  specialState[state].param1 = param1;
  specialState[state].param2 = param2;
  specialState[state].waitUnclear = waitUnclear;
}

float BaseCreatureEntity::animateStates(float delay)
{
  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    if (specialState[i].active)
    {
      specialState[i].timer -= delay;
      if (specialState[i].timer <= 0.0f)
      {
        setSpecialState((enumSpecialState)i, false, 0.0f, 0.0f, 0.0f);
        game().getPlayer()->computePlayer();

        if (i == SpecialStateTime) game().resumeMusic();
      }
    }
    else if (specialState[i].waitUnclear && !game().getCurrentMap()->isCleared())
    {
      specialState[i].waitUnclear = false;
      specialState[i].active = true;
      game().getPlayer()->computePlayer();
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
      // TODO
      hurt(getHurtParams(specialState[SpecialStatePoison].param1, ShotTypeDeterministic, 0, false, SourceTypePoison, NB_ENEMY, false));
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
      if (hurtingType == ShotTypeIce || hurtingType == ShotTypeCold)
        sprite.setColor(sf::Color(fadeColor, fadeColor, 255, 255 )); // blue
      else if (hurtingType == ShotTypePoison)
        sprite.setColor(sf::Color(fadeColor, 255, fadeColor, 255 )); // green
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
  else if (specialState[SpecialStatePoison].active)
  {
    int fade = 180 + 70 * cos(age * 5);
    sprite.setColor(sf::Color(fade, 255, fade, 255 ));
  }
}

void BaseCreatureEntity::animateRecoil(float delay)
{
  // repulsion
  if (repulsion.active)
  {
    repulsion.velocity.x *= 0.97f;
    repulsion.velocity.y *= 0.97f;

    repulsion.timer -= delay;
    if (repulsion.timer <= 0.0f)
    {
      repulsion.active = false;
      computeFacingDirection();
      // TODO ?
    }
  }
}

void BaseCreatureEntity::animatePhysics(float delay)
{
  if (doesAccelerate)
  {
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;

    if (velocity.x * velocity.x + velocity.y * velocity.y > creatureSpeed * creatureSpeed)
    {
      float l = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
      velocity.x = (velocity.x / l) * creatureSpeed;
      velocity.y = (velocity.y / l) * creatureSpeed;
      doesAccelerate = false;
    }
  }

	velocity.x *= viscosity;
	velocity.y *= viscosity;

	float velx = velocity.x;
	float vely = velocity.y;

	if (specialState[SpecialStateSlow].active)
  {
    velx *= specialState[SpecialStateSlow].param1;
    vely *= specialState[SpecialStateSlow].param1;
  }

  if (specialState[SpecialStateSpeed].active)
  {
    velx *= specialState[SpecialStateSpeed].param1;
    vely *= specialState[SpecialStateSpeed].param1;
  }

	if (repulsion.active)
  {
    if (repulsion.stun)
    {
      velx = 0.0f;
      vely = 0.0f;
    }
    velx += repulsion.velocity.x;
    vely += repulsion.velocity.y;
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
    int nx = shadowFrame;
    int ny = 0;

    if (imagesProLine > 0 && shadowFrame >= imagesProLine)
    {
      nx = shadowFrame % imagesProLine;
      ny = shadowFrame / imagesProLine;
    }
    sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));
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

    switch (direction)
      {
      case DIRECTION_RIGHT:
        if (xTilef > xTile0) xTilef = xTile0;
        break;

      case DIRECTION_LEFT:
        if (xTilef > xTile0) xTile0 = xTilef;
        break;

      case DIRECTION_TOP:
        if (yTilef > yTile0) yTilef = yTile0;
        break;

      case DIRECTION_BOTTOM:
        if (yTilef > yTile0) yTile0 = yTilef;
        break;
      }

  for (int xTile = xTile0; xTile <= xTilef; xTile++)
    for (int yTile = yTile0; yTile <= yTilef; yTile++)
    {
      if (movingStyle == movWalking)
      {
        if (isAttacking())
        {
          if ( dynamic_cast<DungeonMap*>(map)->isWalkable(xTile, yTile) == false
              && dynamic_cast<DungeonMap*>(map)->getLogicalTile(xTile, yTile) != LogicalDestroyable)
                return true;
        }
        else
        {
          if ( dynamic_cast<DungeonMap*>(map)->isWalkable(xTile, yTile) == false )
            return true;
        }
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

void BaseCreatureEntity::makeExplode()
{
  new ExplosionEntity(x, y, ExplosionTypeStandard, 16, EnemyTypeNone, false);
  game().addCorpse(x, y, FRAME_CORPSE_SLIME_VIOLET);
  SoundManager::getInstance().playSound(SOUND_BOOM_00);
}

int BaseCreatureEntity::hurt(StructHurt hurtParam)
{
  int oldHp = hp;
  int absorbedHp = 0;
  bool poisoned = false;
  hurtingType = hurtParam.hurtingType;
  if (armor > 0.01f && hurtingType != ShotTypeDeterministic)
  {
    absorbedHp = hurtParam.damage * armor;
    if (absorbedHp == 0) absorbedHp = 1;
    hurtParam.damage -= absorbedHp;
  }

  if (hurtingType == ShotTypeIce
      && determineSatusChance(resistance[ResistanceFrozen], hurtParam.level))
  {
    // frozen ?
    specialState[SpecialStateIce].active = true;
    float frozenDelayMult = 1.0f;
    float frozenMultAdd = 0.0f;
    if (resistance[ResistanceFrozen] == ResistanceHigh)
    {
      if (hurtParam.level == 0)
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
      if (hurtParam.level < 2)
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
    specialState[SpecialStateIce].timer = STATUS_FROZEN_DELAY[hurtParam.level] * frozenDelayMult;
    specialState[SpecialStateIce].param1 = STATUS_FROZEN_MULT[hurtParam.level] + frozenMultAdd;
  }
  else if (hurtingType == ShotTypePoison && determineSatusChance(resistance[ResistancePoison], hurtParam.level))
  {
    specialState[SpecialStatePoison].active = true;
    specialState[SpecialStatePoison].timer = POISON_TIMER[hurtParam.level];
    specialState[SpecialStatePoison].param1 = POISON_DAMAGE[hurtParam.level];
    specialState[SpecialStatePoison].param2 = POISON_DELAY[hurtParam.level];
    specialState[SpecialStatePoison].param3 = POISON_DELAY[hurtParam.level];
    poisoned = true;
  }

  // damage bonus
  if (hurtingType == ShotTypeIce || hurtingType == ShotTypeCold)
    hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistanceIce], hurtParam.level)) / 100;
  else if (hurtingType == ShotTypeFire)
    hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistanceFire], hurtParam.level)) / 100;
  else if (hurtingType == ShotTypeLightning)
    hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistanceLightning], hurtParam.level)) / 100;
  else if (hurtingType == ShotTypeStone)
    hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistanceStone], hurtParam.level)) / 100;
  else if (hurtingType == ShotTypeIllusion)
    hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistanceIllusion], hurtParam.level)) / 100;
  //else if (hurtingType == ShotTypePoison)
  //  hurtParam.damage += (hurtParam.damage * determineDamageBonus(resistance[ResistancePoison], hurtParam.level)) / 100;

  if (hurtParam.damage > 0)
  {
    hurting = true;
    hurtingDelay = HURTING_DELAY;
    this->hurtingType = hurtingType;

    hp -= hurtParam.damage;

    if (hp <= 0)
    {
      hp = 0;

      prepareDying();
      // exploding ?
      if (game().getPlayer()->isEquiped(EQUIP_SULFUR) && canExplode)
      {
        int luck = hurtingType == ShotTypeFire ? 33 : 25;
        if (rand() % 100 < luck) makeExplode();
      }
    }

    // display damage
    if (displayDamage && oldHp > 0)
    {
      int displayedDamage = hurtParam.damage;
      if (hurtParam.goThrough && hp == 0)
      {
        hurtParam.damage = oldHp + absorbedHp;
        displayedDamage = oldHp;
      }

      std::ostringstream oss;
      oss << "-" << displayedDamage;
      int textSize;
      if (displayedDamage < 8) textSize = 17;
      else textSize = 17 + (displayedDamage - 3) / 5;
      TextEntity* text = new TextEntity(oss.str(), textSize, x, y - 20.0f);
      text->setColor(TextEntity::COLOR_FADING_RED);
      text->setAge(-0.6f);
      text->setLifetime(0.3f);
      text->setWeight(-60.0f);
      text->setZ(2000);
      text->setAlignment(ALIGN_CENTER);
      text->setType(ENTITY_FLYING_TEXT);
      while (textTooClose(text, 15, 15)) text->setY(text->getY() - 5);

      if (hurtParam.critical)
      {
        std::ostringstream crss;
        crss << tools::getLabel("critical");
        if (game().getPlayer()->isEquiped(EQUIP_CRITICAL_ADVANCED))
          crss << " X3";
        else
          crss << " X2";
        displayFlyingText(x, text->getY() - 16.0f, 16, crss.str(), TextEntity::COLOR_FADING_RED);
      }

      if (poisoned)
      {
        displayFlyingText(x, text->getY() - 16.0f, 16, tools::getLabel("poison"), TextEntity::COLOR_FADING_RED);
      }
      if (hurtParam.critical) SoundManager::getInstance().playSound(SOUND_CRITICAL);
    }
  }
  return hurtParam.damage;
}

void BaseCreatureEntity::displayFlyingText(float xText, float yText, int sizeText, std::string text, TextEntity::colorTypeEnum color)
{
  TextEntity* textEntity = new TextEntity(text, sizeText, xText, yText);
  textEntity->setColor(color);
  textEntity->setAge(-0.6f);
  textEntity->setLifetime(0.3f);
  textEntity->setWeight(-60.0f);
  textEntity->setZ(2000);
  textEntity->setAlignment(ALIGN_CENTER);
  textEntity->setType(ENTITY_FLYING_TEXT);
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

void BaseCreatureEntity::giveRepulsion(bool stun, Vector2D velocity, float timer)
{
  if (resistance[ResistanceRepulsion] == ResistanceHigh)
    {
      velocity.x *= 0.75f;
      velocity.y *= 0.75f;
      timer *= 0.75f;
    }
    else if (resistance[ResistanceRepulsion] == ResistanceVeryHigh)
    {
      velocity.x *= 0.5f;
      velocity.y *= 0.5f;
      timer *= 0.5f;
    }

  if (!(repulsion.active && repulsion.stun))
  {
    repulsion.active = true;
    repulsion.stun = stun;

    if (this->velocity.x > 1.0f && velocity.x > 1.0f) repulsion.velocity.x = velocity.x + this->velocity.x;
    else if (this->velocity.x < -1.0f && velocity.x < -1.0f) repulsion.velocity.x = velocity.x + this->velocity.x;
    else repulsion.velocity.x = velocity.x;

    if (this->velocity.y > 1.0f && velocity.y > 1.0f) repulsion.velocity.y = velocity.y + this->velocity.y;
    else if (this->velocity.y < -1.0f && velocity.y < -1.0f) repulsion.velocity.y = velocity.y + this->velocity.y;
    else repulsion.velocity.y = velocity.y;

    repulsion.timer = timer;
  }
}

void BaseCreatureEntity::inflictsRepulsionTo(BaseCreatureEntity* targetEntity)
{
}

bool BaseCreatureEntity::canCollide()
{
  return true;
}

void BaseCreatureEntity::generateStar(sf::Color starColor)
{
  game().generateStar(starColor, x, y);
}

bool BaseCreatureEntity::intersectsSegments(Vector2D a1, Vector2D a2, Vector2D b1, Vector2D b2)
{
    Vector2D b(a2.x - a1.x, a2.y - a1.y);
    Vector2D d(b2.x - b1.x, b2.y - b1.y);
    float bDotDPerp = b.x * d.y - b.y * d.x;

    // if b dot d == 0, it means the lines are parallel so have infinite intersection points
    if (bDotDPerp == 0) return false;

    Vector2D c(b1.x - a1.x, b1.y - a1.y);
    float t = (c.x * d.y - c.y * d.x) / bDotDPerp;
    if (t < 0 || t > 1) return false;

    float u = (c.x * b.y - c.y * b.x) / bDotDPerp;
    if (u < 0 || u > 1) return false;

    return true;
}

bool BaseCreatureEntity::intersectsTile(Vector2D a1, Vector2D a2, int xTile, int yTile)
{
  int posX = xTile * TILE_WIDTH;
  int posY = yTile * TILE_HEIGHT;

  if (intersectsSegments(a1, a2, Vector2D(posX, posY), Vector2D(posX + TILE_WIDTH, posY))) return true;
  else if (intersectsSegments(a1, a2, Vector2D(posX, posY + TILE_HEIGHT), Vector2D(posX + TILE_WIDTH, posY + TILE_HEIGHT))) return true;
  else if (intersectsSegments(a1, a2, Vector2D(posX + TILE_WIDTH, posY), Vector2D(posX + TILE_WIDTH, posY + TILE_HEIGHT))) return true;
  else if (intersectsSegments(a1, a2, Vector2D(posX, posY), Vector2D(posX, posY + TILE_HEIGHT))) return true;
  else

  return false;
}

bool BaseCreatureEntity::canSee(float xf, float yf)
{
  int tileX0 = x / TILE_WIDTH;
  int tileXf = xf / TILE_WIDTH;
  int tileY0 = y / TILE_HEIGHT;
  int tileYf = yf / TILE_HEIGHT;

  int xBegin, xEnd, yBegin, yEnd;
  if (tileXf < tileX0)
  {
    xBegin = tileXf;
    xEnd = tileX0;
  }
  else
  {
    xBegin = tileX0;
    xEnd = tileXf;
  }

  if (tileYf < tileY0)
  {
    yBegin = tileYf;
    yEnd = tileY0;
  }
  else
  {
    yBegin = tileY0;
    yEnd = tileYf;
  }

  for (int i = xBegin; i <= xEnd; i++ )
    for (int j = yBegin; j <= yEnd; j++ )
    {
      if (!game().getCurrentMap()->isShootable(i, j))
        if (intersectsTile(Vector2D(x, y), game().getPlayerPosition(), i , j))
          return false;
    }

  return true;
}

bool BaseCreatureEntity::canWalkTo(float xf, float yf)
{
  int tileX0 = x / TILE_WIDTH;
  int tileXf = xf / TILE_WIDTH;
  int tileY0 = y / TILE_HEIGHT;
  int tileYf = yf / TILE_HEIGHT;

  int xBegin, xEnd, yBegin, yEnd;
  if (tileXf < tileX0)
  {
    xBegin = tileXf;
    xEnd = tileX0;
  }
  else
  {
    xBegin = tileX0;
    xEnd = tileXf;
  }

  if (tileYf < tileY0)
  {
    yBegin = tileYf;
    yEnd = tileY0;
  }
  else
  {
    yBegin = tileY0;
    yEnd = tileYf;
  }

  for (int i = xBegin; i <= xEnd; i++ )
    for (int j = yBegin; j <= yEnd; j++ )
    {
      if (!game().getCurrentMap()->isWalkable(i, j))
        if (intersectsTile(Vector2D(x, y), game().getPlayerPosition(), i , j))
          return false;
    }

  return true;
}

void BaseCreatureEntity::heal(int healPoints)
{
  int savedHp = hp;
  hp += healPoints;
  if (hp > hpMax) hp = hpMax;

  int healedHp = hp - savedHp;

  if (savedHp > 0)
  {
    std::ostringstream oss;
    oss << "+" << healedHp;
    int textSize;
    if (healedHp < 8) textSize = 17;
    else textSize = 17 + (healedHp - 3) / 5;
    TextEntity* text = new TextEntity(oss.str(), textSize, x, y - 20.0f);
    text->setColor(TextEntity::COLOR_FADING_GREEN);
    text->setAge(-0.6f);
    text->setLifetime(0.3f);
    text->setWeight(-60.0f);
    text->setZ(2000);
    text->setAlignment(ALIGN_CENTER);
    text->setType(ENTITY_FLYING_TEXT);
    while (textTooClose(text, 15, 15)) text->setY(text->getY() - 5);
  }

  if (type == ENTITY_PLAYER) game().addHealingStat(healedHp);
}

bool BaseCreatureEntity::isAttacking()
{
  return false;
}
