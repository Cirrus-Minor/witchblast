/**  This file is part of Witch Blast.
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

#include <sstream>
#include <iostream>

#include "TextEntity.h"
#include "StaticTextEntity.h"

TextEntity::TextEntity(std::string text, int size, float x, float y) : GameEntity(x, y)
{
    this->text = text;
    this->size = size;
    alignment = ALIGN_LEFT;
    colorType = COLOR_WHITE;

    color = sf::Color(255, 255, 255);
}

void TextEntity::render(sf::RenderWindow* app)
{
    // determine the color
    StaticTextEntity::Write(app, text, size, x, y, alignment, color);
}

void TextEntity::animate(float delay)
{
  GameEntity::animate(delay);

  // TODO : other colorType
  if (colorType == COLOR_FADING_WHITE)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;
    color = sf::Color(255, 255, 255, alpha );
  }
}


void TextEntity::setText(std::string text)
{
    this->text = text;
}

void TextEntity::setColor(int colorType)
{
  // TODO : other colorType
  this->colorType = colorType;

  if (colorType == COLOR_WHITE || colorType == COLOR_FADING_WHITE)
    color = sf::Color(255, 255, 255);
}

void TextEntity::setAlignment(int alignment)
{
    this->alignment = alignment;
}
