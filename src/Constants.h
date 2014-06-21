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

#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

// uncomment to show bounding box in the app
// #define SHOW_BOUNDING_BOX

#include <string>

const std::string APP_NAME =      "Witch Blast";
const std::string APP_VERSION =   "0.1.2";

const std::string CONFIG_FILE =   "config.dat";
const std::string SAVE_FILE =     "game.sav";

const std::string SAVE_VERSION =  "SAVE_0.1.2.1";

// Client size
const int SCREEN_WIDTH = 970;
const int SCREEN_HEIGHT = 720;

// Tile set
const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;

// Tile map offset
const int OFFSET_X = 5;
const int OFFSET_Y = 5;

const int MAP_WIDTH = 15;
const int MAP_HEIGHT = 9;

const int MENU_MAP_WIDTH = 16;
const int MENU_MAP_HEIGHT = 13;

const int GAME_WIDTH = MAP_WIDTH * TILE_WIDTH;
const int GAME_HEIGHT = MAP_HEIGHT * TILE_HEIGHT;

const int FLOOR_WIDTH = 13;
const int FLOOR_HEIGHT = 7;

const int ITEM_WIDTH = 32;
const int ITEM_HEIGHT = 32;

const int BOLT_WIDTH = 24;
const int BOLT_HEIGHT = 24;

const int BB_LEFT    = 22;
const int BB_RIGHT   = 22;
const int BB_TOP     = 4;
const int BB_BOTTOM  = 31;

const float FADE_IN_DELAY = 1.0f;
const float FADE_OUT_DELAY = 1.0f;

enum item_images {
  IMAGE_PLAYER_BASE,
  IMAGE_PLAYER_EQUIP,
  IMAGE_PLAYER_COLLAR,
  IMAGE_BOLT,
  IMAGE_TILES,
  IMAGE_RAT,
  IMAGE_MINIMAP,
  IMAGE_DOOR,
  IMAGE_ITEMS,
  IMAGE_ITEMS_EQUIP,
  IMAGE_CHEST,
  IMAGE_BAT,
  IMAGE_FLOWER,
  IMAGE_SLIME,

  IMAGE_GIANT_SLIME,
  IMAGE_KING_RAT,

  IMAGE_BLOOD,
  IMAGE_CORPSES,
  IMAGE_CORPSES_BIG,
  IMAGE_STAR,
  IMAGE_STAR_2,
  IMAGE_INTERFACE,
  IMAGE_HUD_SHOTS,

  IMAGE_PNJ,
  IMAGE_FAIRY
};

enum sound_resources {
  SOUND_STEP,
  SOUND_BLAST_STANDARD,
  SOUND_BLAST_FLOWER,
  SOUND_DOOR_CLOSING,
  SOUND_DOOR_OPENING,
  SOUND_CHEST_OPENING,
  SOUND_IMPACT,
  SOUND_BONUS,
  SOUND_DRINK,
  SOUND_EAT,
  SOUND_PLAYER_HIT,
  SOUND_PLAYER_DIE,
  SOUND_ENNEMY_DYING,
  SOUND_COIN_PICK_UP,
  SOUND_PAY,
  SOUND_WALL_IMPACT,
  SOUND_BIG_WALL_IMPACT,
  SOUND_KING_RAT_1,
  SOUND_KING_RAT_2,
  SOUND_KING_RAT_DIE,
  SOUND_SLIME_JUMP,
  SOUND_SLIME_IMAPCT,
  SOUND_SLIME_IMAPCT_WEAK,
  SOUND_SLIME_SMASH,
  SOUND_ICE_CHARGE,
  SOUND_ELECTRIC_CHARGE,
  SOUND_SHOT_SELECT,
  SOUND_HEART
};

enum corpses_ressources{
  FRAME_CORPSE_RAT,
  FRAME_CORPSE_BAT,
  FRAME_CORPSE_FLOWER,
  FRAME_CORPSE_GREEN_RAT,
  FRAME_CORPSE_SLIME,
  FRAME_CORPSE_BLACK_RAT,

  FRAME_CORPSE_KING_RAT,
  FRAME_CORPSE_GIANT_SLIME
};

// Player game play
const float INITIAL_PLAYER_SPEED = 180.0f;
const int INITIAL_PLAYER_HP = 20;
const float INITIAL_PLAYER_FIRE_DELAY = 0.7f;
const float ACQUIRE_DELAY = 2.8f;
const float UNLOCK_DELAY = 1.0f;

const float INITIAL_BOLT_LIFE = 0.4f;
const int INITIAL_BOLT_DAMAGES = 8;
const float INITIAL_BOLT_VELOCITY = 700.0f;

const float FAIRY_SPEED = 180.0f;
const float FAIRY_FIRE_DELAY = 0.8f;
const float ICE_FAIRY_FIRE_DELAY = 1.3f;
const float FAIRY_BOLT_LIFE = 0.4f;
const int FAIRY_BOLT_DAMAGES = 8;
const float FAIRY_BOLT_VELOCITY = 700.0f;

enum chest_type_enum {
  CHEST_BASIC,
  CHEST_EXIT,
  CHEST_FAIRY
};
// Artefact Info
const float ARTEFACT_RECT_WIDTH = 600.0f;
const float ARTEFACT_RECT_HEIGHT = 100.0f;
const float ARTEFACT_POS_Y = 450.0f;
const float ARTEFACT_BORDER = 8.0f;
const float ARTEFACT_ZOOM_TIME = 0.5f;

// shot types
enum enumShotType {
  ShotTypeStandard,
  ShotTypeIce,
  ShotTypeIllusion,
  ShotTypeStone,
  ShotTypeLightning
  };

// special shots effects
const int MAX_SHOT_LEVEL = 3;

const float STATUS_FROZEN_DELAY[MAX_SHOT_LEVEL]   // how long the freeze occurs
    = { 4.0f, 5.0f, 6.0f };
const float STATUS_FROZEN_BOLT_DELAY[MAX_SHOT_LEVEL]   // reload time
    = { 3.0f, 2.6f, 2.0f };
const float STATUS_FROZEN_MULT[MAX_SHOT_LEVEL]   // speed multiplier (= 3 times slower)
    = { 0.38f, 0.33f, 0.28f };
// entity type
const int ENTITY_PLAYER = 1;
const int ENTITY_FAMILIAR = 2;
const int ENTITY_DOOR = 3;
const int ENTITY_ARTIFACT_DESCRIPTION = 9;
const int ENTITY_BLOOD    = 11;
const int ENTITY_CORPSE   = 12;
const int ENTITY_EFFECT   = 13;
const int ENTITY_BOLT = 15;
const int ENTITY_ENNEMY_BOLT = 16;
const int ENTITY_PNJ = 17;
const int ENTITY_CHEST = 18;
const int ENTITY_ITEM = 19;
const int ENTITY_ENNEMY = 21;
const int ENTITY_ENNEMY_INVOCATED = 22;
const int ENTITY_ENNEMY_BOSS = 23;

const int ENTITY_ENNEMY_MAX = 23;

// monster type
enum monster_type_enum
{
  MONSTER_RAT,
  MONSTER_BAT,
  MONSTER_EVIL_FLOWER,
  MONSTER_SLIME,
  MONSTER_BLACK_RAT,

  MONSTER_KING_RAT
};

const float DOOR_OPEN_TIME = 1.0f;
const float DOOR_CLOSE_TIME = 1.0f;

// Rat
const float RAT_SPEED = 160.0f;
const int RAT_HP = 24;
const int RAT_DAMAGES = 5;
const int RAT_BB_LEFT = 14;
const int RAT_BB_WIDTH_DIFF = 28;
const int RAT_BB_TOP = 22;
const int RAT_BB_HEIGHT_DIFF = 22;

// Green Rat
const float GREEN_RAT_SPEED = 170.0f;
const int GREEN_RAT_HP = 16;
const int GREEN_RAT_DAMAGES = 5;
const float GREEN_RAT_FADE = 1.0f;

// Black Rat
const float BLACK_RAT_SPEED = 160.0f;
const int BLACK_RAT_HP = 24;
const int BLACK_RAT_DAMAGES = 5;

// Bat
const float BAT_SPEED = 250.0f;
const int BAT_HP = 8;
const int BAT_DAMAGES = 5;
const int BAT_BB_LEFT = 5;
const int BAT_BB_WIDTH_DIFF = 10;
const int BAT_BB_TOP = 2;
const int BAT_BB_HEIGHT_DIFF = 32;

// Evl Flower
const int EVIL_FLOWER_HP = 16;
const int EVIL_FLOWER_MELEE_DAMAGES = 8;
const int EVIL_FLOWER_MISSILE_DAMAGES = 5;
const int EVIL_FLOWER_BB_LEFT = 14;
const int EVIL_FLOWER_BB_WIDTH_DIFF = 28;
const int EVIL_FLOWER_BB_TOP = 22;
const int EVIL_FLOWER_BB_HEIGHT_DIFF = 22;
const float EVIL_FLOWER_FIRE_DELAY = 2.7f;
const float EVIL_FLOWER_FIRE_VELOCITY = 220.0f;

// Slime
const int SLIME_HP = 16;
const int SLIME_DAMAGES = 5;
const int SLIME_BB_LEFT = 13;
const int SLIME_BB_WIDTH_DIFF = 26;
const int SLIME_BB_TOP = 38;
const int SLIME_BB_HEIGHT_DIFF = 40;

// Giant Slime
const int GIANT_SLIME_HP = 650;
const int GIANT_SLIME_DAMAGES = 8;
const int GIANT_SLIME_MISSILE_DAMAGES = 6;
const float GIANT_SLIME_MISSILE_DELAY = 0.33f;
const float GIANT_SLIME_FIRE_VELOCITY = 200.0f;
const int GIANT_SLIME_BB_LEFT = 26;
const int GIANT_SLIME_BB_WIDTH_DIFF = 52;
const int GIANT_SLIME_BB_TOP = 64;
const int GIANT_SLIME_BB_HEIGHT_DIFF = 12;
const int GIANT_SLIME_SPEED = 85.0f;

// KingRat
const float KING_RAT_SPEED = 200.0f;
const float KING_RAT_RUNNING_SPEED = 600.0f;
const float KING_RAT_BERSERK_SPEED = 250.0f;
const float KING_RAT_RUNNING_RECOIL = 750.0f;
const int KING_RAT_HP = 700;
const int KING_RAT_DAMAGES = 8;

// EFFECTS
const float HURTING_DELAY = 0.4f;
const float HEART_BEAT_DELAY = 1.2f;

#endif // CONSTANTS_H_INCLUDED
