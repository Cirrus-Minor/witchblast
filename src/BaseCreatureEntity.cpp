#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "Constants.h"

#include "WitchBlastGame.h"

BaseCreatureEntity::BaseCreatureEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int spriteWidth = -1, int spriteHeight = -1)
  : CollidingSpriteEntity (image, x, y, spriteWidth, spriteHeight)
{
  hurting = false;
  shadowFrame = -1;
}

void BaseCreatureEntity::setParent(WitchBlastGame* parent)
{
  parentGame = parent;
}

int BaseCreatureEntity::getHp()
{
  return hp;
}

int BaseCreatureEntity::getHpMax()
{
  return hpMax;
}

int BaseCreatureEntity::getHpDisplay()
{
  return hpDisplay;
}

void BaseCreatureEntity::animate(float delay)
{
  if (hpDisplay > hp) hpDisplay--;
  else if (hpDisplay < hp) hpDisplay++;

  z = y + height/2;
  if (hurting)
  {
    hurtingDelay -= delay;
    if (hurtingDelay > 0.0f)
    {
      int fadeColor = (sf::Uint8)((HURTING_DELAY - hurtingDelay) * 255);
       sprite.setColor(sf::Color(255, fadeColor, fadeColor, 255 ));
    }
    else
    {
      hurting = false;
      sprite.setColor(sf::Color(255, 255, 255, 255 ));
    }
  }
  CollidingSpriteEntity::animate(delay);
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

void BaseCreatureEntity::hurt(int damages)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;

  hp -= damages;
  if (hp <= 0)
  {
    hp = 0;
    dying();
  }
}

void BaseCreatureEntity::dying()
{
  isDying = true;
}
