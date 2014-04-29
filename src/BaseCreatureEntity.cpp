#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "Constants.h"

#include "WitchBlastGame.h"

BaseCreatureEntity::BaseCreatureEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int spriteWidth = -1, int spriteHeight = -1)
  : CollidingSpriteEntity (image, x, y, spriteWidth, spriteHeight )
{
  hurting = false;
  hurtingType = BoltStandard;
  shadowFrame = -1;
  setMap(game().getCurrentMap(), TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  hpDisplay = 0;
  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    specialState[i].type = (enumSpecialState)i;
    specialState[i].resistance = ResistanceStandard;
    specialState[i].active = false;
    specialState[i].timer = 0.0f;
  }
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

void BaseCreatureEntity::animate(float delay)
{
  if (hpDisplay > hp) hpDisplay--;
  else if (hpDisplay < hp) hpDisplay++;

  for (int i = 0; i < NB_SPECIAL_STATES; i++)
  {
    if (specialState[i].active)
    {
      specialState[i].timer -= delay;
      if (specialState[i].timer <= 0.0f) specialState[i].active = false;
    }
  }

  if (specialState[SpecialStateIce].active) delay *= STATUS_FROZEN_MULT;

  z = y + height/2;
  sprite.setColor(sf::Color(255, 255, 255, 255 ));
  if (hurting)
  {
    hurtingDelay -= delay;
    if (hurtingDelay > 0.0f)
    {
      int fadeColor = (sf::Uint8)((HURTING_DELAY - hurtingDelay) * 255);
      if (hurtingType == BoltIce)
        sprite.setColor(sf::Color(fadeColor, fadeColor, 255, 255 ));
      else
        sprite.setColor(sf::Color(255, fadeColor, fadeColor, 255 ));
    }
    else
    {
      hurting = false;
      sprite.setColor(sf::Color(255, 255, 255, 255 ));
    }
  }
  CollidingSpriteEntity::animate(delay);

  if (specialState[SpecialStateIce].active) sprite.setColor(sf::Color(100, 100, 255, 255 ));
}

void BaseCreatureEntity::render(sf::RenderWindow* app)
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

bool BaseCreatureEntity::hurt(int damages, enumBoltType hurtingType)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;
  this->hurtingType = hurtingType;

  if (hurtingType == BoltIce && specialState[SpecialStateIce].resistance > ResistanceImmune)
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
