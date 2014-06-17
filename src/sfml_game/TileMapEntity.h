/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef TILEMAPENTITY_H_INCLUDED
#define TILEMAPENTITY_H_INCLUDED

#include "GameEntity.h"
#include "GameMap.h"

// Basis class for TileMap
class TileMapEntity : public GameEntity
{
public:
  TileMapEntity(sf::Texture* image, GameMap* gameMap, int tileWidth, int tileHeight, int tilesProLine);
  ~TileMapEntity();

  	// mutators
	void setX(float x);
	void setY(float y);

  int getTilesProLine();
  void setTileBox(int tileBoxWidth, int tileBoxHeight);
  void setMap(GameMap* gameMap);

  virtual void render(sf::RenderTarget* app);
	virtual void animate(float delay);
	virtual void computeVertices();

protected:
  int width;
  int height;
  int tileWidth;
  int tileHeight;
  int tileBoxWidth;
  int tileBoxHeight;
  int tilesProLine;

  sf::Texture* image;
  GameMap* gameMap;

  sf::VertexArray vertices;
  bool hasChanged;

  bool getChanged();
};

#endif // TILEMAPENTITY_H_INCLUDED
