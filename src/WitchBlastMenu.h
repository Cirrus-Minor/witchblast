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

#ifndef WITCHBLASTMENU_H
#define WITCHBLASTMENU_H

class SpriteEntity;
class WitchBlastGame;

class WitchBlastMenu
{
public:
  WitchBlastMenu(WitchBlastGame *game);
  ~WitchBlastMenu();

  void updateMenu();
  void renderMenu();

  void prepareIntro();
  void updateIntro();

private:
  WitchBlastGame *game;

  SpriteEntity* introSprites[8];

  int introState;
  int introSoundState;
};

#endif // WITCHBLASTMENU_H
