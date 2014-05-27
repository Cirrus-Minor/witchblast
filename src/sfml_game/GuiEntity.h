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

#ifndef GUIENTITY_H_INCLUDED
#define GUIENTITY_H_INCLUDED

#include "GameEntity.h"

// Basis class for GUI
class GuiEntity : public GameEntity
{
public:
    GuiEntity(float x = 0.0f, float y = 0.0f);

    bool getActive();

    void setActive(bool active);

    virtual void render(sf::RenderTarget* app);
	virtual void animate(float delay);

    virtual void onEvent(sf::Event event);

protected:
    bool isActive;
};

#endif // GUIENTITY_H_INCLUDED
