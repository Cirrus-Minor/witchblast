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

#include "GuiEntity.h"

GuiEntity::GuiEntity(float x, float y) : GameEntity(x, y)
{
    isActive = true;
}

bool GuiEntity::getActive() { return isActive; }

void GuiEntity::setActive(bool active) { isActive = active; }

void GuiEntity::render(sf::RenderTarget* app)
{
    if (app == NULL) return;
}

void GuiEntity::animate(float delay)
{
    GameEntity::animate(delay);
}

void GuiEntity::onEvent(sf::Event event)
{
    if (event.type == sf::Event::Closed) return;
}
