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

#include <sstream>
#include <iostream>
#include "TextEntity.h"
#include "WitchBlastGame.h"

TextEntity::TextEntity(std::string text, int size, float x, float y) : GameEntity(x, y)
{
    this->text = text;
    this->size = size;
    alignment = ALIGN_LEFT;
    colorType = COLOR_WHITE;

    color = sf::Color(255, 255, 255);
}

void TextEntity::render(sf::RenderTarget* app)
{
  if (colorType == COLOR_FADING_RED)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x - 1, y - 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x + 1, y + 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(255, 50, 20, alpha );
    game().write(text, size, x, y, alignment, color, app, 0 , 0, 0);
  }
  else if (colorType == COLOR_FADING_GREEN)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x - 1, y - 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x + 1, y + 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(20, 255, 50, alpha );
    game().write(text, size, x, y, alignment, color, app, 0 , 0, 0);
  }
  else if (colorType == COLOR_FADING_YELLOW)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x - 1, y - 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(0, 0, 0, alpha );
    game().write(text, size, x + 1, y + 1, alignment, color, app, 0 , 0, 0);

    color = sf::Color(255, 255, 128, alpha );
    game().write(text, size, x, y, alignment, color, app, 0 , 0, 0);
  }
  else
    game().write(text, size, x, y, alignment, color, app, 0 , 0, 0);
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
  else if (colorType == COLOR_FADING_GREEN)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;
    color = sf::Color(0, 255, 0, alpha );
  }
  else if (colorType == COLOR_FADING_RED)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;
    color = sf::Color(255, 0, 0, alpha );
  }
  else if (colorType == COLOR_FADING_YELLOW)
  {
    int alpha = 255 * (lifetime - age) / lifetime;
    if (alpha > 255) alpha = 255;
    else if (alpha < 0) alpha = 0;
    color = sf::Color(255, 255, 0, alpha );
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
  else if (colorType == COLOR_GREEN || colorType == COLOR_FADING_GREEN)
    color = sf::Color(0, 255, 0);
  else if (colorType == COLOR_RED || colorType == COLOR_FADING_RED)
    color = sf::Color(255, 0, 0);
  else if (colorType == COLOR_YELLOW || colorType == COLOR_FADING_YELLOW)
    color = sf::Color(255, 255, 0);
}

void TextEntity::setAlignment(int alignment)
{
    this->alignment = alignment;
}
