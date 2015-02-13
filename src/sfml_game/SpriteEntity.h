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
  *  along with Witch Blast. If not, see <http://www.gnu.org/licenses/>.
  */

#ifndef SPRITEENTITY_H_INCLUDED
#define SPRITEENTITY_H_INCLUDED

#include "GameEntity.h"

// Basis class for sprite
class SpriteEntity : public GameEntity
{
public:
  // create a sprite with the entire image
  SpriteEntity(sf::Texture* image, float x = 0.0f, float y = 0.0f, int width = -1, int height = -1, int imagesProLine = 0);

	int getFrame();
	float getScaleX();
  int getWidth();

  void setFading(bool isFading);
  void setShrinking(bool isShrinking);
  void setShrinking(bool isShrinking, float initialScaleX, float initialScaleY);
  void setVisible(bool isVisible);
  void setMirroring(bool isMirroring);
  void setFrame(int frame);
  void setImagesProLine(int n);
  void setScale(float scx, float scy);
  void setColor(sf::Color color);
  void setTexture(sf::Texture* image);

  void setRenderAdd();

  void removeCenter();

  virtual void render(sf::RenderTarget* app);
	virtual void animate(float delay);
	void displayCenterAndZ(sf::RenderTarget* app);

protected:
    sf::Sprite sprite;
    sf::Texture* image;
    sf::Color color;
    int width;
    int height;
    int frame;
    int imagesProLine;

    // for shrinking
    float initialScaleX, initialScaleY;

    bool isFading;
    bool isShrinking;
    bool isVisible;
    bool isMirroring;

    bool renderAdd;
};

#endif // SPRITEENTITY_H_INCLUDED
