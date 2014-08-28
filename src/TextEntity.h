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

#ifndef TEXTENTITY_H_INCLUDED
#define TEXTENTITY_H_INCLUDED

#include "sfml_game/GameEntity.h"

// Basis class for text
class TextEntity : public GameEntity
{
public:
  TextEntity(std::string text, int size, float x = 0.0f, float y = 0.0f);

  virtual void render(sf::RenderTarget* app);
	virtual void animate(float delay);

	virtual void setText(std::string text);

	void setAlignment(int alignment);
	void setColor(int colorType);
	enum alignmentEnum { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };
	enum colorTypeEnum { COLOR_WHITE,
                         COLOR_BLUE,
                         COLOR_RED,
                         COLOR_GREEN,
                         COLOR_YELLOW,
                         COLOR_FADING_WHITE,
                         COLOR_FADING_BLUE,
                         COLOR_FADING_GREEN,
                         COLOR_FADING_RED,
                         COLOR_FADING_YELLOW,
                         COLOR_BLINKING_WHITE };

protected:
  std::string text;
  int size;
  int alignment;
  int colorType;
  sf::Color color;
};

#endif // TEXTENTITY_H_INCLUDED
