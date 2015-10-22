#ifndef MINIMAPENTITY_H
#define MINIMAPENTITY_H

#include "sfml_game/TileMapEntity.h"

class MiniMapEntity : public TileMapEntity
{
  public:
    MiniMapEntity(sf::Texture* image, GameMap* gameMap, int tileWidth, int tileHeight, int tilesProLine);
    virtual ~MiniMapEntity();

    virtual void render(sf::RenderTarget* app);
    void display(sf::RenderTarget* app);
  protected:
  private:
};

#endif // MINIMAPENTITY_H
