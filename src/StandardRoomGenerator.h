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

#ifndef STANDARDROOMGENERATOR_H_INCLUDED
#define STANDARDROOMGENERATOR_H_INCLUDED

void generateStandardRoom01()
{
int random = rand() % 95; // (level == 1 ? 95 : 110);

 /* if (random > 0)
  {
    currentMap->generateRoom(rand()%4);

    new CyclopEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
  }
  else*/

  if (random < 16)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_RAT,4);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_BAT,4);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_EVIL_FLOWER,4);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateBonusRoom();
    new ChestEntity(v.x, v.y, CHEST_BASIC, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_RAT,3);
    game().findPlaceMonsters(MONSTER_BAT,3);
  }
  else
  {
    game().getCurrentMap()->generateRoom(4);
    game().findPlaceMonsters(MONSTER_BLACK_RAT,6);
  }
}

void generateStandardRoom02()
{
int random = rand() % 110;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_RAT, 6);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_BAT, 6);
    if (rand() % 3 == 0) game().findPlaceMonsters(MONSTER_IMP_RED, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_EVIL_FLOWER, 5);
    if (rand() % 3 == 0) game().findPlaceMonsters(MONSTER_IMP_BLUE, 1);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateBonusRoom();
    new ChestEntity(v.x, v.y, CHEST_BASIC, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_RAT,3);
    game().findPlaceMonsters(MONSTER_BAT,3);
    if (rand() % 2 == 0) game().findPlaceMonsters(MONSTER_IMP_RED, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(MONSTER_IMP_BLUE, 1);
  }
  else if (random < 95)
  {
    game().getCurrentMap()->generateRoom(4);
    game().findPlaceMonsters(MONSTER_BLACK_RAT,6);
  }
  else
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().getCurrentMap()->addRandomGrids(4);
    game().findPlaceMonsters(MONSTER_SLIME,8 + rand() % 5);
  }
}

void generateStandardRoom03()
{
int random = rand() % 110;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_RAT, 7);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_BAT, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(MONSTER_IMP_RED, 3);
    else game().findPlaceMonsters(MONSTER_IMP_BLUE, 3);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoom(rand()%5);
    game().findPlaceMonsters(MONSTER_EVIL_FLOWER, 5);
    if (rand() % 2 == 0) game().findPlaceMonsters(MONSTER_IMP_RED, 2);
    else game().findPlaceMonsters(MONSTER_IMP_BLUE, 2);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateBonusRoom();
    new ChestEntity(v.x, v.y, CHEST_BASIC, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().findPlaceMonsters(MONSTER_IMP_RED, 3);
    game().findPlaceMonsters(MONSTER_IMP_BLUE, 3);
  }
  else if (random < 95)
  {
    game().getCurrentMap()->generateRoom(4);
    game().findPlaceMonsters(MONSTER_BLACK_RAT,7);
  }
  else
  {
    game().getCurrentMap()->generateRoom(rand()%4);
    game().getCurrentMap()->addRandomGrids(4);
    game().findPlaceMonsters(MONSTER_SLIME,8 + rand() % 5);
  }
}

void generateStandardRoom(int level)
{
  switch (level)
  {
    case 1: generateStandardRoom01(); break;
    case 2: generateStandardRoom02(); break;
    case 3: generateStandardRoom03(); break;
    default: generateStandardRoom03(); break;
  }
}

#endif // STANDARDROOMGENERATOR_H_INCLUDED
