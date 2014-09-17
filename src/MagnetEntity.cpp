#include "MagnetEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

MagnetEntity::MagnetEntity(float x, float y, PlayerEntity* parentEntity, enumItemType itemType) : SpriteEntity (ImageManager::getInstance().getImage(IMAGE_ITEMS), x, y, ITEM_WIDTH, ITEM_HEIGHT)
{
  this->x = x;
  this->y = y;
  this->setLifetime(0.7f);
  //itemFadingEntity->setFading(true);
  this->setShrinking(true);
  this->setVelocity(Vector2D(150.0f - rand()%300, -260.0f));
  this->setWeight(800.0f);
  this->setFrame(itemType);
  this->parentEntity = parentEntity;
  type = ENTITY_EFFECT;
}


void MagnetEntity::animate(float delay)
{
  z = y + height;

  if (age > lifetime * 0.3f)
  {
    velocity.x += (parentEntity->getX() - x) * delay * 30.0f;
    velocity.y += (parentEntity->getY() - y) * delay * 30.0f;
  }

  SpriteEntity::animate(delay);
}
