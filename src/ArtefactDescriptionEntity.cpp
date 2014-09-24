#include "ArtefactDescriptionEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"
#include "WitchBlastGame.h"
#include "Items.h"
#include "TextMapper.h"

ArtefactDescriptionEntity::ArtefactDescriptionEntity(enumItemType itemType)
      : SpriteEntity (ImageManager::getInstance().getImage(itemType >= FirstEquipItem ? IMAGE_ITEMS_EQUIP : IMAGE_ITEMS ),
                      0, 0, ITEM_WIDTH, ITEM_HEIGHT)
{
  this->setLifetime(6.0f);
  imagesProLine = 10;
  if (itemType >= FirstEquipItem)
    this->setFrame(itemType - FirstEquipItem);
  else
    this->setFrame(itemType);
  this->setType(ENTITY_ARTIFACT_DESCRIPTION);

  float x0 = OFFSET_X + MAP_WIDTH * TILE_WIDTH * 0.5f - ARTEFACT_RECT_WIDTH * 0.5f;

  rectangle.setSize(sf::Vector2f(ARTEFACT_RECT_WIDTH, ARTEFACT_RECT_HEIGHT));
  rectangle.setPosition(sf::Vector2f(x0, ARTEFACT_POS_Y));
  rectangle.setFillColor(sf::Color(236, 222, 194, 255));
  rectangle.setOutlineThickness(ARTEFACT_BORDER);
  rectangle.setOutlineColor(sf::Color(201, 145, 95,255));

  this->x = x0 + 40.0f;
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
    artefactDescription = "Report to developper";
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
    sprite.setRotation(angle);

    if (isFading)
    {
        sprite.setColor(sf::Color(255, 255, 255, (sf::Uint8)(getFade() * 255)));
    }

    if (isShrinking)
    {
        sprite.setScale(getFade(), getFade());
    }

    app->draw(rectangle);
    game().write(artefactName, 22, 315.0f, ARTEFACT_POS_Y + 15.0f, ALIGN_LEFT, sf::Color::Black, app, 0, 0);
    game().write(artefactDescription, 19, 315.0f, ARTEFACT_POS_Y + 55.0f, ALIGN_LEFT, sf::Color::Black, app, 0, 0);
    app->draw(sprite);
}
