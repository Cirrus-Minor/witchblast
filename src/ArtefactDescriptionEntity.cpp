#include "ArtefactDescriptionEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "WitchBlastGame.h"
#include "Items.h"
#include "TextMapper.h"

ArtefactDescriptionEntity::ArtefactDescriptionEntity(enumItemType itemType)
      : SpriteEntity (ImageManager::getInstance().getImage(itemType >= FirstEquipItem ? IMAGE_ITEMS_EQUIP_PRES : IMAGE_ITEMS_PRES ),
                      0, 0, ITEM_WIDTH, ITEM_HEIGHT)
{
  this->setLifetime(6.0f);
  imagesProLine = 10;
  if (itemType >= FirstEquipItem)
    this->setFrame(itemType - FirstEquipItem);
  else
    this->setFrame(itemType);
  this->setType(ENTITY_ARTIFACT_DESCRIPTION);

  rectangle.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEM_DESCRIPTION));
  float x0 = 20 + MAP_WIDTH * TILE_WIDTH * 0.5f - rectangle.getTextureRect().width * 0.5f;
  rectangle.setPosition(x0, ARTEFACT_POS_Y);

  this->x = x0;
  this->y = 500.0f;
  sprite.setScale(3.5f, 3.5f);

  z = 5000.0f;

  if (items[itemType].type == itemType)
  {
    artefactName = tools::getLabel(items[itemType].name);
    artefactDescription = tools::getLabel(items[itemType].description);
  }
  else
  {
    artefactName = "Unknown";
    artefactDescription = "Report to developer";
  }
}

ArtefactDescriptionEntity::~ArtefactDescriptionEntity()
{
}

void ArtefactDescriptionEntity::animate(float delay)
{

  if (age < ARTEFACT_ZOOM_TIME)
  {
    float perc = 1.0f - age / ARTEFACT_ZOOM_TIME;
    sprite.setScale(4.0f + perc * 50.0f, 4.0f + perc * 50.0f);
  }
  else
  {
    sprite.setScale(4.0f, 4.0f);
  }
  SpriteEntity::animate(delay);
}

void ArtefactDescriptionEntity::render(sf::RenderTarget* app)
{
    int nx = frame;
    int ny = 0;

    if (imagesProLine > 0)
    {
        nx = frame % imagesProLine;
        ny = frame / imagesProLine;
    }

    sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));

    sprite.setPosition(x, y);

    if (isFading)
    {
        sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(getFade() * 255)));
    }

    if (isShrinking)
    {
        sprite.setScale(getFade(), getFade());
    }

    sf::Color fadeColor = sf::Color::White;
    if (age > lifetime - 0.5f)
    {
      int fade = 500 * (lifetime - age);
      if (fade < 0) fade = 0;
      fadeColor = sf::Color(255, 255, 255, fade);
      rectangle.setColor(fadeColor);
      sprite.setColor(fadeColor);
    }

    app->draw(rectangle);
    game().write(artefactName, 19, 470.0f, ARTEFACT_POS_Y + 15.0f, ALIGN_CENTER, fadeColor, app, 0, 0, 0);
    game().write(artefactDescription, 17, 470.0f, ARTEFACT_POS_Y + 55.0f, ALIGN_CENTER, fadeColor, app, 0, 0, 340);
    app->draw(sprite);
}
