#include "MiniMapEntity.h"

MiniMapEntity::MiniMapEntity(sf::Texture* image, GameMap* gameMap, int tileWidth, int tileHeight, int tilesProLine)
    : TileMapEntity(image, gameMap, tileWidth, tileHeight, tilesProLine)
{
  //ctor
}

MiniMapEntity::~MiniMapEntity()
{
  //dtor
}

void MiniMapEntity::render(sf::RenderTarget* app)
{
  // nothing
}

void MiniMapEntity::display(sf::RenderTarget* app)
{
  TileMapEntity::render(app);
}
