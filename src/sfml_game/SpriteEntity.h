/**  This file is part of sfmlGame.
  *
  *  FreeTumble is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  FreeTumble is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with FreeTumble.  If not, see <http://www.gnu.org/licenses/>.
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
  void setVisible(bool isVisible);
  void setFrame(int frame);
  void setImagesProLine(int n);
  void setScale(float scx, float scy);

  void removeCenter();

  virtual void render(sf::RenderWindow* app);
	virtual void animate(float delay);

protected:
    sf::Sprite sprite;
    sf::Texture* image;
    int width;
    int height;
    int frame;
    int imagesProLine;

    bool isFading;
    bool isShrinking;
    bool isVisible;
};

#endif // SPRITEENTITY_H_INCLUDED
