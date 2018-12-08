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

#include <string>

const std::string APP_NAME =      "Witch Blast";
const std::string APP_VERSION =   "0.7.6";

const std::string CONFIG_FILE =     "config.dat";
const std::string SAVE_FILE =       "game.sav";
const std::string SAVE_DATA_FILE =  "data/data.sav";
const std::string HISCORES_FILE =   "data/scores.dat";

const std::string SAVE_VERSION =    "SAVE_0.8";
const std::string SCORE_VERSION =   "V075_DEV";

const int NB_LANGUAGES = 5;
const std::string languageString[NB_LANGUAGES] = { "english", "french", "german", "spanish", "russian" };
const std::string languageState[NB_LANGUAGES] = { "", "", "", "", "" };

const unsigned int SCORES_MAX    = 10;

const int LAST_LEVEL = 8;

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
const int BOLT_PRO_LINE = 10;

const int BB_LEFT    = 22;
const int BB_RIGHT   = 22;
const int BB_TOP     = 4;
const int BB_BOTTOM  = 31;

const float FADE_IN_DELAY = 1.0f;
const float FADE_OUT_DELAY = 1.0f;

const float EXPLOSION_DELAY = 2.0f;
const float DEATH_CERTIFICATE_DELAY = 3.5f;
const float ACHIEVEMENT_DELAY_MAX = 15.0f;

const float KeyRoomFXDelay = 2.0f;

enum enum_images {
  IMAGE_PLAYER_0,
  IMAGE_PLAYER_1,
  IMAGE_PLAYER_2,
  IMAGE_BOLT,
  IMAGE_TILES,
  IMAGE_RAT,
  IMAGE_MINIMAP,
  IMAGE_MAP_BACKGROUND,
  IMAGE_ITEMS,
  IMAGE_ITEMS_EQUIP,
  IMAGE_ITEMS_PRES,
  IMAGE_ITEMS_EQUIP_PRES,
  IMAGE_CHEST,
  IMAGE_BAT,
  IMAGE_FLOWER,
  IMAGE_SLIME,
  IMAGE_IMP,
  IMAGE_SPIDER_EGG,
  IMAGE_SPIDER_WEB,
  IMAGE_LITTLE_SPIDER,
  IMAGE_BUBBLE,
  IMAGE_EYE,
  IMAGE_WITCH,
  IMAGE_CAULDRON,
  IMAGE_SNAKE,
  IMAGE_PUMPKIN,
  IMAGE_GHOST,
  IMAGE_ZOMBIE,
  IMAGE_BOGEY,
  IMAGE_SAUSAGE,

  IMAGE_BUTCHER,
  IMAGE_GIANT_SLIME,
  IMAGE_KING_RAT,
  IMAGE_CYCLOP,
  IMAGE_GIANT_SPIDER,
  IMAGE_FRANCKY,
  IMAGE_VAMPIRE,
  IMAGE_VAMPIRE_BAT,
  IMAGE_VAMPIRE_PART,

  IMAGE_BLOOD,
  IMAGE_CORPSES,
  IMAGE_CORPSES_BIG,
  IMAGE_STAR,
  IMAGE_STAR_2,
  IMAGE_HURT_IMPACT,
  IMAGE_INTERFACE,
  IMAGE_HUD_SHOTS,
  IMAGE_EXPLOSION,
  IMAGE_KEYS_QWER,
  IMAGE_KEYS_AZER,
  IMAGE_INTRO,
  IMAGE_TITLE,
  IMAGE_OVERLAY,
  IMAGE_LIGHT_CONE,
  IMAGE_DIVINITY,

  IMAGE_PNJ,
  IMAGE_FAIRY,
  IMAGE_KEY_AREA,

  IMAGE_UI_LIFE,
  IMAGE_UI_MANA,
  IMAGE_UI_SPELLS,
  IMAGE_UI_MESSAGE,
  IMAGE_UI_TOP_LAYER,
  IMAGE_UI_ACHIEV,

  IMAGE_FOG,
  IMAGE_TITLE_ANIM,
  IMAGE_SPLATTER,
  IMAGE_WITCH_INTRO,

  IMAGE_ITEM_DESCRIPTION,
  IMAGE_DEATH_CERTIFICATE,
  IMAGE_ACHIEVEMENTS,
  IMAGE_BOSS_PICTURES,
  IMAGE_PORTRAIT_PART,
  IMAGE_RANDOM_DUNGEON,
  IMAGE_DUNGEON_OBJECTS,
  IMAGE_TILES_SHADOW,
  IMAGE_TILES_SHADOW_CORNER,
  IMAGE_TILES_SHADOW_MEDIUM,
  IMAGE_TILES_SHADOW_SMALL,
  IMAGE_DOORS,
  IMAGE_DESTROYABLE,
  IMAGE_HALL_OF_FAME,
  IMAGE_LIGHTNING,
  IMAGE_WIN_SEAL,
  IMAGE_WIN_SEAL_HOF,
  IMAGE_BAG,
  IMAGE_UI_PAUSE,
  IMAGE_SCORE_FONT,
  IMAGE_EFFECT_ZONE,

  NB_IMAGES
};

enum sound_resources {
  SOUND_NONE = -1,
  SOUND_BLAST_STANDARD,
  SOUND_BLAST_FLOWER,
  SOUND_BLAST_LIGHTNING,
  SOUND_BLAST_FIRE,
  SOUND_BLAST_ICE,
  SOUND_BLAST_ILLUSION,
  SOUND_BLAST_POISON,
  SOUND_BLAST_STONE,
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
  SOUND_SLIME_IMAPCT_BOSS,
  SOUND_SLIME_SMASH,
  SOUND_ICE_CHARGE,
  SOUND_ELECTRIC_CHARGE,
  SOUND_SHOT_SELECT,
  SOUND_HEART,
  SOUND_RAT_DYING,
  SOUND_BAT_DYING,
  SOUND_IMP_HURT,
  SOUND_IMP_DYING,
  SOUND_ROCK_IMPACT_LIGHT,
  SOUND_ROCK_IMPACT_MEDIUM,
  SOUND_ROCK_IMPACT_HEAVY,
  SOUND_THROW,
  SOUND_CYCLOP_00,
  SOUND_CYCLOP_DIE,
  SOUND_CYCLOPS_IMPACT,
  SOUND_BUTCHER_00,
  SOUND_BUTCHER_01,
  SOUND_BUTCHER_HURT,
  SOUND_BUTCHER_DIE,
  SOUND_VIB,
  SOUND_BOOM_00,
  SOUND_CLANG_00,
  SOUND_BUBBLE_00,
  SOUND_BUBBLE_01,
  SOUND_TRAP,
  SOUND_EGG_SMASH_00,
  SOUND_EGG_SMASH_01,
  SOUND_SPIDER_WALKING,
  SOUND_SPIDER_WEB,
  SOUND_SPIDER_HURT,
  SOUND_SPIDER_DIE,
  SOUND_SPIDER_LITTLE_DIE,
  SOUND_WITCH_00,
  SOUND_WITCH_01,
  SOUND_WITCH_02,
  SOUND_WITCH_DIE_00,
  SOUND_WITCH_DIE_01,
  SOUND_INVOKE,
  SOUND_CAULDRON,
  SOUND_CAULDRON_DIE,
  SOUND_SNAKE_DIE,
  SOUND_PUMPKIN_00,
  SOUND_PUMPKIN_01,
  SOUND_PUMPKIN_DIE,
  SOUND_CRITICAL,
  SOUND_GONG,
  SOUND_TELEPORT,
  SOUND_SPELL_CHARGE,
  SOUND_FIREBALL,
  SOUND_MESSAGE,
  SOUND_EARTHQUAKE,
  SOUND_SPELL_FREEZE,
  SOUND_SPELL_SHIELD,
  SOUND_HEAVY_STEP_00,
  SOUND_HEAVY_STEP_01,
  SOUND_NIGHT,
  SOUND_GRUMBLE,
  SOUND_ZOMBIE_00,
  SOUND_ZOMBIE_01,
  SOUND_ZOMBIE_ATTACKING,
  SOUND_ZOMBIE_DYING,
  SOUND_GHOST,
  SOUND_GHOST_DYING,
  SOUND_ELECTRICITY,
  SOUND_ELECTRIC_BLAST,
  SOUND_FRANCKY_00,
  SOUND_FRANCKY_01,
  SOUND_FRANCKY_02,
  SOUND_FRANCKY_DYING,
  SOUND_OM,
  SOUND_GLASS,
  SOUND_HICCUP,
  SOUND_SPLATCH,
  SOUND_INTRO_WITCH,
  SOUND_FORCE_FIELD,
  SOUND_DOOR_OPENING_BOSS,
  SOUND_ACHIEVEMENT,
  SOUND_VAMPIRE_FLYING,
  SOUND_VAMPIRE_FLAP,
  SOUND_VAMPIRE_SONIC_RAY,
  SOUND_VAMPIRE_LAUGHING,
  SOUND_VAMPIRE_TRANSFORM_BOLT,
  SOUND_VAMPIRE_TRANSFORM_BAT,
  SOUND_VAMPIRE_HYPNOSIS,
  SOUND_VAMPIRE_CRY,
  SOUND_VAMPIRE_DYING,
  SOUND_ICE_BLOCK,
  SOUND_BOGEYMAN_DIE,
  SOUND_BOGEYMAN_ATTACK,
  SOUND_BOGEYMAN_VORTEX_00,
  SOUND_BOGEYMAN_VORTEX_01,
  SOUND_BARREL_HIT,
  SOUND_BARREL_SMASH,
  SOUND_SECRET,
  SOUND_SCROLL,
  SOUND_CLOCK,
  SOUND_BOTTLE,
  SOUND_THUNDER,
  SOUND_SCORE_BONUS,
  SOUND_STONE_HIT,
};

enum corpses_ressources{
  FRAME_CORPSE_RAT,
  FRAME_CORPSE_BAT,
  FRAME_CORPSE_FLOWER,
  FRAME_CORPSE_GREEN_RAT,
  FRAME_CORPSE_SLIME,
  FRAME_CORPSE_BLACK_RAT,
  FRAME_CORPSE_IMP_RED,
  FRAME_CORPSE_IMP_BLUE,
  FRAME_CORPSE_SLIME_RED,
  FRAME_CORPSE_SLIME_BLUE,
  FRAME_CORPSE_SPIDER_EGG,
  FRAME_CORPSE_LITTLE_SPIDER,
  FRAME_CORPSE_SPIDER_WEB,
  FRAME_CORPSE_RAT_HELMET,
  FRAME_CORPSE_BLACK_RAT_HELMET,
  FRAME_CORPSE_SLIME_VIOLET,
  FRAME_CORPSE_WITCH,
  FRAME_CORPSE_WITCH_RED,
  FRAME_CORPSE_CAULDRON,
  FRAME_CORPSE_SNAKE,
  FRAME_CORPSE_SNAKE_BLOOD,
  FRAME_CORPSE_PUMPKIN,
  FRAME_CORPSE_FLOWER_ICE,
  FRAME_CORPSE_GHOST,
  FRAME_CORPSE_ZOMBIE,
  FRAME_CORPSE_ZOMBIE_DARK,
  FRAME_CORPSE_FRANCKY_TORSO,
  FRAME_CORPSE_FRANCKY_HEAD,
  FRAME_CORPSE_FRANCKY_HAND,
  FRAME_CORPSE_FRANCKY_FOOT,
  FRAME_CORPSE_FLOWER_FIRE,
  FRAME_CORPSE_VAMPIRE,
  FRAME_CORPSE_BAT_SKELETON,
  FRAME_CORPSE_CAULDRON_BLUE,
  FRAME_CORPSE_CAULDRON_RED,
  FRAME_CORPSE_CAULDRON_GREEN,
  FRAME_CORPSE_LITTLE_SPIDER_TARANTULA,

  FRAME_CORPSE_KING_RAT,
  FRAME_CORPSE_GIANT_SLIME,
  FRAME_CORPSE_CYCLOP,
  FRAME_CORPSE_BUTCHER,
  FRAME_CORPSE_GIANT_SPIDER,
  FRAME_CORPSE_GIANT_SLIME_RED,
  FRAME_CORPSE_GIANT_SLIME_BLUE,
};

// chests
enum chest_type_enum {
  ChestBasic,
  ChestExit,
  ChestChallenge,
  ChestFairy,
};
const float CHEST_APPEAR_DELAY = 1.5f;

// Artefact Info
const float ARTEFACT_RECT_WIDTH = 650.0f;
const float ARTEFACT_RECT_HEIGHT = 100.0f;
const float ARTEFACT_POS_Y = 450.0f;
const float ARTEFACT_BORDER = 3.0f;
const float ARTEFACT_ZOOM_TIME = 0.5f;

// shot types
enum enumShotType {
  ShotTypeStandard,
  ShotTypeIce,
  ShotTypeIllusion,
  ShotTypeStone,
  ShotTypeLightning,
  ShotTypeFire,
  ShotTypePoison,
  ShotTypeBomb,
  ShotTypeCold,

  ShotTypeDeterministic
  };

// special shots effects
const int MAX_SHOT_LEVEL = 3;

const float STATUS_FROZEN_DELAY[MAX_SHOT_LEVEL]   // how long the freeze occurs
    = { 4.0f, 5.0f, 6.0f };
const float STATUS_FROZEN_BOLT_DELAY[MAX_SHOT_LEVEL]   // reload time
    = { 3.0f, 2.8f, 2.8f };
const float STATUS_FROZEN_MULT[MAX_SHOT_LEVEL]   // speed multiplier (= 3 times slower)
    = { 0.38f, 0.33f, 0.28f };
const float STONE_DECOIL_DELAY[MAX_SHOT_LEVEL]   // how long the stun occurs
    = { 0.15f, 0.175f, 0.2f };
const float STONE_DECOIL_VELOCITY[MAX_SHOT_LEVEL]   // Decoil velocity
    = { 270.0f, 320.0f, 450.0f };
const float ILLUSION_DAMAGE_DECREASE[MAX_SHOT_LEVEL]   // Illusion damage malus
    = { 0.8f, 0.9f, 1.0f };
const float LIGHTNING_VISCOSITY_INCREASE[MAX_SHOT_LEVEL]   // Air resistance
    = { 0.01, 0.015f, 0.02f };
const float FIRE_DAMAGE_INCREASE[MAX_SHOT_LEVEL]   // Fire damage's bonus
    = { 1.15f, 1.3f, 1.45f };

const float POISON_TIMER[MAX_SHOT_LEVEL]
    = { 10.5f, 12.5f, 12.5f };
const float POISON_DAMAGE[MAX_SHOT_LEVEL]
    = { 1, 1, 1 };
const float POISON_DELAY[MAX_SHOT_LEVEL]
    = { 2.0f, 1.5f, 1.0f };

// divinity
enum enumDivinityType {
  DivinityHealer,
  DivinityFighter,
  DivinityIce,
  DivinityStone,
  DivinityAir,
  DivinityIllusion,
  DivinityFire,
  DivinityDeath,

  NB_DIVINITY
};
const std::string divinityLabel[NB_DIVINITY]
{
  "divinity_healer",
  "divinity_fighter",
  "divinity_ice",
  "divinity_stone",
  "divinity_air",
  "divinity_illusion",
  "divinity_fire",
  "divinity_death",
};
const int MAX_DIVINITY_LEVEL = 5;
const int DIVINITY_LEVEL_TRESHOLD[MAX_DIVINITY_LEVEL] =
  { 100, 300, 500, 900, 1400};

// entity type
const int ENTITY_PLAYER = 1;
const int ENTITY_FAMILIAR = 2;
const int ENTITY_DOOR = 3;
const int ENTITY_ARTIFACT_DESCRIPTION = 9;
const int ENTITY_BLOOD    = 11;
const int ENTITY_CORPSE   = 12;
const int ENTITY_EFFECT   = 13;
const int ENTITY_FLYING_TEXT = 14;
const int ENTITY_BOLT = 15;
const int ENTITY_ENEMY_BOLT = 16;
const int ENTITY_PNJ = 17;
const int ENTITY_CHEST = 18;
const int ENTITY_ITEM = 19;
const int ENTITY_EXPLOSION = 20;
const int ENTITY_FAMILIAR_LOCAL = 21;
const int ENTITY_EFFECT_ZONE = 22;
const int ENTITY_ENEMY = 31;
const int ENTITY_ENEMY_INVOCATED = 32;
const int ENTITY_ENEMY_BOSS = 33;
const int ENTITY_ENEMY_NC = 34;

const int ENTITY_ENEMY_MAX = ENTITY_ENEMY_NC;
const int ENTITY_ENEMY_MAX_COUNT = ENTITY_ENEMY_BOSS;

const float DOOR_OPEN_TIME = 1.0f;
const float DOOR_CLOSE_TIME = 1.0f;

// Player game play
const float INITIAL_PLAYER_SPEED = 220.0f;
const int INITIAL_PLAYER_HP = 20;
const float INITIAL_PLAYER_FIRE_DELAY = 0.7f;
const float ACQUIRE_DELAY = 2.3f;
const float UNLOCK_DELAY = 1.2f;
const float WORSHIP_DELAY = 2.0f;
const float INVINCIBLE_DELAY = 1.5f;
const float HICCUP_DELAY = 5.0f;

const float INITIAL_BOLT_LIFE = 0.45f;
const int INITIAL_BOLT_DAMAGES = 8;
const float INITIAL_BOLT_VELOCITY = 700.0f;
const float INITIAL_BOLT_VISCOSITY = 0.98f;

const float FAIRY_SPEED = 220.0f;
const float FAIRY_FIRE_DELAY = 0.8f;
const float ICE_FAIRY_FIRE_DELAY = 1.3f;
const float TARGET_FAIRY_FIRE_DELAY = 1.0f;
const float FAIRY_BOLT_LIFE = 0.4f;
const int FAIRY_BOLT_DAMAGES = 8;
const int FAIRY_FIRE_DAMAGES = 12;
const float FAIRY_BOLT_VELOCITY = 700.0f;

// Rat
const float RAT_SPEED = 195.0f;
const float RAT_SPEED_HELMET = 140.0f;
const int RAT_HP = 16;
const int RAT_HP_HELMET = 24;
const int RAT_DAMAGES = 5;
const int RAT_BB_LEFT = 14;
const int RAT_BB_WIDTH_DIFF = 28;
const int RAT_BB_TOP = 22;
const int RAT_BB_HEIGHT_DIFF = 22;

// Green Rat
const float GREEN_RAT_SPEED = 180.0f;
const int GREEN_RAT_HP = 16;
const int GREEN_RAT_DAMAGES = 5;
const float GREEN_RAT_FADE = 1.0f;

// Black Rat
const float BLACK_RAT_SPEED = 195.0f;
const float BLACK_RAT_SPEED_HELMET = 125.0f;
const int BLACK_RAT_HP = 16;
const int BLACK_RAT_HP_HELMET = 24;
const int BLACK_RAT_DAMAGES = 5;

// Bat
const float BAT_SPEED = 270.0f;
const int BAT_HP = 5;
const int BAT_DAMAGES = 5;

// Zombie
const float ZOMBIE_SPEED = 80.0f;
const int ZOMBIE_HP = 80;
const int ZOMBIE_DAMAGE = 10;

// Evl Flower
const int EVIL_FLOWER_HP = 16;
const int EVIL_FLOWER_MELEE_DAMAGES = 5;
const int EVIL_FLOWER_MISSILE_DAMAGES = 6;
const int EVIL_FLOWER_FIRE_MISSILE_DAMAGES = 8;
const int EVIL_FLOWER_BB_LEFT = 14;
const int EVIL_FLOWER_BB_WIDTH_DIFF = 28;
const int EVIL_FLOWER_BB_TOP = 22;
const int EVIL_FLOWER_BB_HEIGHT_DIFF = 22;
const float EVIL_FLOWER_FIRE_DELAY = 2.2f;
const float EVIL_FLOWER_FIRE_VELOCITY = 230.0f;

// Snake
const float SNAKE_SPEED = 150.0f;
const int SNAKE_HP = 16;
const int SNAKE_DAMAGE = 5;
const float SNAKE_BLOOD_SPEED = 170.0f;
const int SNAKE_BLOOD_HP = 32;
const int SNAKE_BLOOD_DAMAGE = 8;

// Slime
const int SLIME_HP = 16;
const int SLIME_DAMAGES = 5;
const int SLIME_BB_LEFT = 13;
const int SLIME_BB_WIDTH_DIFF = 26;
const int SLIME_BB_TOP = 38;
const int SLIME_BB_HEIGHT_DIFF = 40;
const float SLIME_FIRE_VELOCITY = 240.0f;

// Imp
const float IMP_SPEED = 190.0f;
const int IMP_HP = 20;
const int IMP_MELEE_DAMAGES = 5;
const int IMP_MISSILE_DAMAGES = 8;
const float IMP_FIRE_VELOCITY = 260.0f;

// Witch
const int WITCH_HP = 80;
const int WITCH_DAMAGE = 5;
const int WITCH_VELOCITY = 140.0f;

// Cauldron
const int CAULDRON_HP = 150;

// Ghost
const float GHOST_SPEED = 140.0f;
const int GHOST_HP = 40;
const int GHOST_DAMAGE = 8;

// Butcher
const int BUTCHER_HP = 120;
const int BUTCHER_DAMAGES = 8;
const int BUTCHER_VELOCITY = 100;

// Giant Slime
const int GIANT_SLIME_HP = 250;
const int GIANT_SLIME_DAMAGES = 8;
const int GIANT_SLIME_MISSILE_DAMAGES = 6;
const float GIANT_SLIME_MISSILE_DELAY = 0.33f;
const float GIANT_SLIME_FIRE_VELOCITY = 200.0f;
const int GIANT_SLIME_BB_LEFT = 26;
const int GIANT_SLIME_BB_WIDTH_DIFF = 52;
const int GIANT_SLIME_BB_TOP = 64;
const int GIANT_SLIME_BB_HEIGHT_DIFF = 12;
const int GIANT_SLIME_SPEED = 80.0f;
const float GIANT_SLIME_ULTRA_JUMP_VELOCITY = 1700.0f;
const float GIANT_SLIME_ULTRA_FALL_VELOCITY = -2000.0f;
const float GIANT_SLIME_ULTRA_JUMP_TIMER = 2.3f;

// KingRat
const float KING_RAT_SPEED = 210.0f;
const float KING_RAT_RUNNING_SPEED = 750.0f;
const float KING_RAT_BERSERK_SPEED = 300.0f;
const float KING_RAT_RUNNING_RECOIL = 750.0f;
const int KING_RAT_HP = 700;
const int KING_RAT_DAMAGES = 8;

// Cyclops
const float CYCLOP_SPEED[4] = { 140, 150, 160, 170};
const int CYCLOP_NUMBER_ROCKS[4] = { 5, 7, 9, 12};
const float CYCLOP_FIRE_DELAY[4] = { 0.3f, 0.26f, 0.23f, 0.2f};
const int CYCLOP_HP = 880;
const int CYCLOP_DAMAGES = 8;

// Giant Spider
const float GIANT_SPIDER_SPEED[4] = { 240, 245, 250, 255 };
const int GIANT_SPIDER_NUMBER_EGGS[4] = { 12, 14, 16, 18};
const float GIANT_SPIDER_FIRE_DELAY[4] = { 0.82f, 0.72f, 0.62f, 0.52f};
const int GIANT_SPIDER_HP = 800;
const int GIANT_SPIDER_DAMAGE = 8;

// EFFECTS
const float HURTING_DELAY = 0.4f;
const float HEART_BEAT_DELAY = 1.2f;

// messages
const int NB_MSG_LINES = 3;
enum EnumMessages
{
  MsgInfoLevel1,
  MsgInfoLevel2,
  MsgInfoLevel3,
  MsgInfoLevel4,
  MsgInfoLevel5,
  MsgInfoLevel6,
  MsgInfoLevel7,
  MsgInfoRatsBats,
  MsgInfoSnakes,
  MsgInfoWitches,
  MsgInfoGold,
  MsgInfoFamiliar,
  MsgInfoButcher,
  MsgInfoGiantSlime,
  MsgInfoCyclops,
  MsgInfoWererat,
  MsgInfoGiantSpiderBefore,
  MsgInfoGiantSpiderAfter,
  MsgInfoFranky,
  MsgInfoVampire,

  MsgInfoDivHealer,
  MsgInfoDivFighter,
  MsgInfoDivIce,
  MsgInfoDivStone,
  MsgInfoDivAir,
  MsgInfoDivIllusion,
  MsgInfoDivFire,
  MsgInfoDivPoison,
  MsgInfoDivIntervention,
  MsgInfoDivGift,
  MsgInfoDivLevel,

  MsgInfoPotionId,

  MsgTutoBasics,
  MsgTutoTips,
  MsgTutoItems,
  MsgTutoShops,
  MsgTutoBossDoor,
  MsgTutoChallengeDoor,
  MsgTutoTemple,
  MsgTutoHeal,
  MsgTutoShots,
  MsgTutoSpell,
  MsgTutoAchievements,
  MsgTutoConsumables,
  MsgTutoPotions,

  NB_MESSAGES
};

enum EnumMessagesType
{
  MessageTypeInfo,
  MessageTypeTutorial,
};

struct messageStruct
{
  EnumMessagesType messageType;
  EnumMessages type;
  std::string message[NB_MSG_LINES];
  float timer;
  float timerMax;
};

enum EnumWorldEvents
{
  // info
  EventMeetRatsOrBats,
  EventMeetSnakes,
  EventMeetWitches,
  EventGetCoin,
  EventPietyMax,
  Event4Hits,

  // tuto
  EventBeingHurted,
  EventFindShop,
  EventFindBossDoor,
  EventFindChallengeDoor,
  EventFindTemple,
  EventGetItem,
  EventGetSpecialShot,
  EventGetFamiliar,
  EventGetSpell,
  EventAchievement,
  EventConsumable,
  EventPotion,

  NB_EVENTS
};

enum EnumInteractionType
{
  InteractionTypeTemple,
  InteractionTypeMerchandise,
};

#endif // CONSTANTS_H_INCLUDED
