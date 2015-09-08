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
  int random = rand() % 64;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 2);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeBat, 2);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 2);
  }
  else
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(MAP_WIDTH * TILE_WIDTH / 2, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
}

void generateStandardRoom02()
{
  int random = rand() % 95;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSnake, 1);
    else game().findPlaceMonsters(EnemyTypeBat, 2);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSnake, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 4);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSnake, 4);
  }
  else
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 6);
  }
}

void generateStandardRoom03()
{
  int random = rand() % 110;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 4);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 5);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 4 + rand() % 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypePumpkin, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 6; i++)
      game().findPlaceMonsters(rand() % 2 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);

    game().findPlaceMonsters(EnemyTypeRatHelmet, 1);
    if (rand() % 2 == 0)
    {
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 1);
    }
    else if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSnake, 2);
  }
  else if (random < 95)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    int r = rand() % 2;
    game().findPlaceMonsters(EnemyTypeRatBlack, 5 - r);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 1 + r);
  }
  else
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().getCurrentMap()->addRandomGrids(4);
    game().findPlaceMonsters(EnemyTypeSlime, 7 + rand() % 5);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeSlimeBlue, 1);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeSlimeRed, 1);
  }
}

void generateStandardRoom04()
{
  int random = rand() % 110;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 4);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 3);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeSnakeBlood, 1);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 3);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 3);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 2);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 2);

    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 2);
      game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1 + rand() % 2);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeImpBlue, 3);
      game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
    }
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeImpRed, 3);
    game().findPlaceMonsters(EnemyTypeImpBlue, 3);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = 0; i < 6; i++)
      game().findPlaceMonsters(rand() % 2 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
    else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 100)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 4);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 3);
  }
  else
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
      int r = 3 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 3:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 8 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 3:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        }
      }
    }
  }
}

void generateStandardRoom05()
{
  int random = rand() % 110;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 3);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 4);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 4);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitch, 2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitchRed, 1 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeCauldron, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 3);
    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 4);
      game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeImpBlue, 3);
      game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
    }
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    int r = rand() % 3;
    if (r == 0)
    {
      game().findPlaceMonsters(EnemyTypeImpRed, 3 + rand() % 2);
      game().findPlaceMonsters(EnemyTypeImpBlue, 3 + rand() % 2);
    }
    else if (r == 1)
      game().findPlaceMonsters(EnemyTypeImpRed, 7);
    else
      game().findPlaceMonsters(EnemyTypeImpBlue, 7);

    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 4);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
  }
  else if (random < 100)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 6; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 2; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      int r = rand() % 3;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
        break;
      }

      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 8 + rand() % 5;

      if (rand() % 3 == 0)
      {
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
        r -= 4;
      }

      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
      if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    }
  }
}

void generateStandardRoom06()
{
  int random = rand() % 100;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombie, 6 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeZombie, 2);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 2);
    game().findPlaceMonsters(EnemyTypeGhost, 5);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 1);
  }
  else if (random < 30)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitch, 2 +  + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitchRed, 2 +  + rand() % 2);
    if (rand() % 5 == 0)
      game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
    else
      game().findPlaceMonsters(EnemyTypeCauldron, 1);
  }
  else if (random < 40)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 2);
    game().findPlaceMonsters(EnemyTypePumpkin, 3 + rand() % 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 2);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 2);
  }
  else if (random < 50)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    int r = rand() % 3;
    if (r == 0)
    {
      game().findPlaceMonsters(EnemyTypeImpRed, 4);
      game().findPlaceMonsters(EnemyTypeImpBlue, 4);
    }
    else if (r == 1)
      game().findPlaceMonsters(EnemyTypeImpRed, 8);
    else
      game().findPlaceMonsters(EnemyTypeImpBlue, 8);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 60)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombieDark, 5);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
  }
  else if (random < 70)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 6; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 2; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSpiderLittle, 8);
    game().findPlaceMonsters(EnemyTypeSpiderEgg, 14);
  }
  else if (random < 90)
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      int r = rand() % 4;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
        break;
      case 3:
        game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 1);
        break;
      }

      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 8 + rand() % 5;

      if (rand() % 3 == 0)
      {
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
        r -= 4;
      }

      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
      if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    }
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
}

void generateStandardRoom07()
{
  int random = rand() % 100;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombie, 8 + rand() % 3);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeZombie, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeBogeyman, 1);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 2);
    game().findPlaceMonsters(EnemyTypeGhost, 6);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 2);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeBogeyman, 1);
  }
  else if (random < 30)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitch, 2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitchRed, 2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeCauldron, 1);

    if (rand() % 2 == 0)
    {
      if (rand() % 3 == 0)
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
      else
        game().findPlaceMonsters(EnemyTypeCauldron, 1);
    }
  }
  else if (random < 40)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 2);
    game().findPlaceMonsters(EnemyTypePumpkin, 3 + rand() % 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 3);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 3);
  }
  else if (random < 50)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeBogeyman, 5 + rand() % 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 60)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombieDark, 7);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
  }
  else if (random < 70)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 8; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 2; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSpiderLittle, 8);
    game().findPlaceMonsters(EnemyTypeSpiderTarantula, 2);
    game().findPlaceMonsters(EnemyTypeSpiderEgg, 16);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(-1);
    game().getCurrentMap()->addRandomGrids(4);

    int r = rand() % 4;
    switch (r)
    {
    case 0:
      game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
      break;
    case 1:
      game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
      break;
    case 2:
      game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
      break;
    case 3:
      game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 1);
      break;
    }

    if (rand() % 2 == 0)
    {
      r = rand() % 4;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
        break;
      case 3:
        game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 1);
        break;
      }
      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      int r = 8 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
}

void generateStandardRoom08()
{
  int random = rand() % 100;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombie, 9 + rand() % 3);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeZombie, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeBogeyman, 1);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 3);
    game().findPlaceMonsters(EnemyTypeGhost, 6);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 2);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeBogeyman, 1);
  }
  else if (random < 30)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitch, 2 + rand() % 3);
    game().findPlaceMonsters(EnemyTypeWitchRed, 2 + rand() % 3);
    game().findPlaceMonsters(EnemyTypeCauldron, 1+ rand() % 2);

    if (rand() % 2 == 0)
    {
      if (rand() % 3 == 0)
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
      else
        game().findPlaceMonsters(EnemyTypeCauldron, 1);
    }
  }
  else if (random < 40)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 2);
    game().findPlaceMonsters(EnemyTypePumpkin, 4 + rand() % 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 4);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 4);
  }
  else if (random < 50)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeBogeyman, 5 + rand() % 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 60)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombieDark, 9);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
  }
  else if (random < 70)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 12; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 2; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSpiderLittle, 6);
    game().findPlaceMonsters(EnemyTypeSpiderTarantula, 4);
    game().findPlaceMonsters(EnemyTypeSpiderEgg, 20);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(-1);
    game().getCurrentMap()->addRandomGrids(4);

    int r;

    for (int i = 0; i < 2; i++)
    {
      r = rand() % 4;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
        break;
      case 3:
        game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 1);
        break;
      }
    }

    if (rand() % 2 == 0)
    {
      r = rand() % 4;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
        break;
      case 3:
        game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 1);
        break;
      }
      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      int r = 8 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
}

/////// ADVANCED ! ////////

void generateAdvancedRoom01()
{
  int random = rand() % 64;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 2);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 1 + rand() % 2);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeBat, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(MAP_WIDTH * TILE_WIDTH / 2, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypeWitch, 1);
      game().findPlaceMonsters(EnemyTypeBat, 1);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeZombie, 1);
      game().findPlaceMonsters(EnemyTypeBat, 1);
    }
  }
}

void generateAdvancedRoom02()
{
  int random = rand() % 100;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 2);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSnake, 2);
    else game().findPlaceMonsters(EnemyTypeBat, 3);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSnake, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 2);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 2);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSnake, 4);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeSnakeBlood, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeSnakeBlood, 1);
  }
  else if (random < 95)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 6);
  }
  else
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      game().findPlaceMonsters(EnemyTypeBat, 3);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeZombie, 2);
      game().findPlaceMonsters(EnemyTypeBat, 1);
    }
  }
}

void generateAdvancedRoom03()
{
  int random = rand() % 115;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 2);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    if (rand() % 2 == 0)
      game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    else
      game().findPlaceMonsters(EnemyTypeZombie, 1);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBat, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 2);
    else game().findPlaceMonsters(EnemyTypeBatSkeleton, 3);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 4 + rand() % 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypePumpkin, 2);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 7; i++)
      game().findPlaceMonsters(rand() % 2 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);

    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);

    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpBlue, 1);

  }
  else if (random < 95)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    int r = rand() % 2;
    game().findPlaceMonsters(EnemyTypeRatBlack, 5 - r);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 1 + r);
    game().findPlaceMonsters(EnemyTypeZombieDark, 1);
  }
  else if (random < 110)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().getCurrentMap()->addRandomGrids(4);
    game().findPlaceMonsters(EnemyTypeSlime, 7 + rand() % 5);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeSlimeBlue, 1);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeSlimeRed, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSlimeViolet, 1);
  }
  else
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 3);
    else game().findPlaceMonsters(EnemyTypeWitchRed, 3);
  }
}

void generateAdvancedRoom04()
{
  int random = rand() % 115;

  if (random < 16)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 3);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 5);
    if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
    else game().findPlaceMonsters(EnemyTypeSnakeBlood, 2);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 5);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 3);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 3);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 4);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 2);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 2);

    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 2);
      game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 1 + rand() % 2);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeImpBlue, 3);
      game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
    }
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeImpRed, 3);
    game().findPlaceMonsters(EnemyTypeImpBlue, 3);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = 0; i < 9; i++)
      game().findPlaceMonsters(rand() % 2 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
    else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 100)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 3);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 5);
  }
  else if (random < 110)
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      game().findPlaceMonsters(EnemyTypeSlimeLarge, 1);
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 1);
      else game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 1);
      int r = 3 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 3:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 11 + rand() % 5;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 3:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        }
      }
    }
  }
  else
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().getCurrentMap()->addRandomGrids(4);
    int r = 4 + rand() % 2;
    for (int i = 0; i < r; i++)
    {
      int rtype = rand() % 3;
      switch (rtype)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlime,1);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeRed,1);
        break;
      }
    }
    game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
  }
}

void generateAdvancedRoom05()
{
  int random = rand() % 115;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRat, 3);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 5);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 2);
    else game().findPlaceMonsters(EnemyTypeWitchRed, 2);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 5);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 5);
  }
  else if (random < 32)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 4);
    game().findPlaceMonsters(EnemyTypeWitch, 2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitchRed, 1 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeCauldron, 1);
  }
  else if (random < 48)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 3);
    if (rand() % 2 == 0)
    {
      game().findPlaceMonsters(EnemyTypePumpkin, 2 + rand() % 4);
      game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 3);
    }
    else
    {
      game().findPlaceMonsters(EnemyTypeImpBlue, 3);
      game().findPlaceMonsters(EnemyTypeEvilFlowerFire, 3);
    }
  }
  else if (random < 64)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 3);
    int r = rand() % 3;
    if (r == 0)
    {
      game().findPlaceMonsters(EnemyTypeImpRed, 3 + rand() % 2);
      game().findPlaceMonsters(EnemyTypeImpBlue, 3 + rand() % 2);
    }
    else if (r == 1)
      game().findPlaceMonsters(EnemyTypeImpRed, 7);
    else
      game().findPlaceMonsters(EnemyTypeImpBlue, 7);

    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 90)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeRatBlack, 4);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
    game().findPlaceMonsters(EnemyTypeZombie, 3);
  }
  else if (random < 100)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 7; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 3; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 110)
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      int r = rand() % 3;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 2);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 2);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 2);
        break;
      }

      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 8 + rand() % 5;

      if (rand() % 3 == 0)
      {
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
        r -= 4;
      }

      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
      if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    }
  }
  else
  {
    game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
    game().getCurrentMap()->addRandomGrids(4);

    game().findPlaceMonsters(EnemyTypeCauldron, 3);
  }
}

void generateAdvancedRoom06()
{
  int random = rand() % 105;

  if (random < 10)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombie, 10 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeRatHelmet, 2);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    else game().findPlaceMonsters(EnemyTypeZombie, 2);
  }
  else if (random < 20)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 4);
    game().findPlaceMonsters(EnemyTypeGhost, 5);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeImpRed, 1);
    else game().findPlaceMonsters(EnemyTypeImpBlue, 1);
  }
  else if (random < 30)
  {
    game().getCurrentMap()->generateRoomWithoutHoles(2 + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitch, 2 +  + rand() % 2);
    game().findPlaceMonsters(EnemyTypeWitchRed, 2 +  + rand() % 2);
    if (rand() % 5 == 0)
      game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
    else
      game().findPlaceMonsters(EnemyTypeCauldron, 1);
    game().findPlaceMonsters(EnemyTypeSausage, 6);
  }
  else if (random < 40)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeEvilFlower, 5);
    game().findPlaceMonsters(EnemyTypePumpkin, 3 + rand() % 4);
    if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 2);
    else game().findPlaceMonsters(EnemyTypeEvilFlowerIce, 2);
  }
  else if (random < 50)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    int r = rand() % 3;
    if (r == 0)
    {
      game().findPlaceMonsters(EnemyTypeImpRed, 5);
      game().findPlaceMonsters(EnemyTypeImpBlue, 5);
    }
    else if (r == 1)
      game().findPlaceMonsters(EnemyTypeImpRed, 10);
    else
      game().findPlaceMonsters(EnemyTypeImpBlue, 10);
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
  }
  else if (random < 60)
  {
    game().getCurrentMap()->generateRoomRandom(ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeZombieDark, 8);
    game().findPlaceMonsters(EnemyTypeRatBlackHelmet, 4);
  }
  else if (random < 70)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    for (int i = rand() % 2; i < 6; i++)
      game().findPlaceMonsters(rand() % 3 == 0 ? EnemyTypeSnake : EnemyTypeSnakeBlood, 1);
    for (int i = 0; i < 4; i++)
      if (rand() % 2 == 0) game().findPlaceMonsters(EnemyTypeWitchRed, 1);
      else game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 80)
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_CHECKER);
    game().findPlaceMonsters(EnemyTypeSpiderLittle, 5);
    game().findPlaceMonsters(EnemyTypeSpiderTarantula, 3);
    game().findPlaceMonsters(EnemyTypeSpiderEgg, 18);
  }
  else if (random < 90)
  {
    if (rand() % 2 == 0)
    {
      game().getCurrentMap()->generateRoomWithoutHoles(-1);
      game().getCurrentMap()->addRandomGrids(4);
      int r = rand() % 4;
      switch (r)
      {
      case 0:
        game().findPlaceMonsters(EnemyTypeSlimeLarge, 2);
        break;
      case 1:
        game().findPlaceMonsters(EnemyTypeSlimeRedLarge, 2);
        break;
      case 2:
        game().findPlaceMonsters(EnemyTypeSlimeBlueLarge, 2);
        break;
      case 3:
        game().findPlaceMonsters(EnemyTypeSlimeVioletLarge, 2);
        break;
      }

      r = 2 + rand() % 4;
      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
    }
    else
    {
      game().getCurrentMap()->generateRoomWithoutHoles(rand() % ROOM_TYPE_CHECKER);
      game().getCurrentMap()->addRandomGrids(4);
      int r = 10 + rand() % 5;

      if (rand() % 3 == 0)
      {
        game().findPlaceMonsters(EnemyTypeCauldronElemental, 1);
        r -= 4;
      }

      for (int i = 0; i < r; i++)
      {
        int rtype = rand() % 5;
        switch (rtype)
        {
        case 0:
        case 4:
          game().findPlaceMonsters(EnemyTypeSlime,1);
          break;
        case 1:
          game().findPlaceMonsters(EnemyTypeSlimeBlue,1);
          break;
        case 2:
          game().findPlaceMonsters(EnemyTypeSlimeRed,1);
          break;
        case 3:
          game().findPlaceMonsters(EnemyTypeSlimeViolet,1);
          break;
        }
      }
      if (rand() % 4 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
    }
    if (rand() % 3 == 0) game().findPlaceMonsters(EnemyTypeWitch, 1);
  }
  else if (random < 100)
  {
    Vector2D v = game().getCurrentMap()->generateChestRoom();
    new ChestEntity(v.x, v.y, ChestBasic, false);
    game().getCurrentMap()->setCleared(true);
  }
  else
  {
    game().getCurrentMap()->generateRoomRandom(rand() % ROOM_TYPE_ALL);
    game().findPlaceMonsters(EnemyTypeZombie, 10);
    game().findPlaceMonsters(EnemyTypeBatSkeleton, 5);
    game().findPlaceMonsters(EnemyTypeGhost, 1);
  }
}

void generateStandardRoom(int level, bool advanced)
{
  if (advanced)
  {
    switch (level)
    {
    case 1:
      generateAdvancedRoom01();
      break;
    case 2:
      generateAdvancedRoom02();
      break;
    case 3:
      generateAdvancedRoom03();
      break;
    case 4:
      generateAdvancedRoom04();
      break;
    case 5:
      generateAdvancedRoom05();
      break;
    case 6:
      generateAdvancedRoom06();
      break;
    case 7:
      generateStandardRoom07();
      break;

    default:
      generateStandardRoom08();
      break;
    }
  }
  else
  {
    switch (level)
    {
    case 1:
      generateStandardRoom01();
      break;
    case 2:
      generateStandardRoom02();
      break;
    case 3:
      generateStandardRoom03();
      break;
    case 4:
      generateStandardRoom04();
      break;
    case 5:
      generateStandardRoom05();
      break;
    case 6:
      generateStandardRoom06();
      break;
    case 7:
      generateStandardRoom07();
      break;

    default:
      generateStandardRoom08();
      break;
    }
  }
}

#endif // STANDARDROOMGENERATOR_H_INCLUDED
