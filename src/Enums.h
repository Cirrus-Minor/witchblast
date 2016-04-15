#ifndef ENUMS_H
#define ENUMS_H

#include "sfml_game/Game.h"

enum EnumScoreBonus
{
  BonusSecret,
  BonusChallenge,
  BonusPerfect,
  BonusTime,
  BonusPossession,
};

enum EnumButtonType
{
  ButtonConsumable,
  ButtonShotType,
};

struct ButtonStruct
{
  sf::IntRect zone;
  EnumButtonType type;
  int index;
};

/** Music enum
 *  Identify the various music tracks of the game.
 */
enum musicEnum
{
  MusicDungeon,   /**< Main game music - played when playing the game */
  MusicEnding,    /**< Ending music - played when the player has died */
  MusicBoss,      /**< Boss music - for epic fights ! */
  MusicIntro,     /**< Main menu music */
  MusicChallenge  /**< Challenge music - for epic fights ! */
};

/** Game states enum
 *  Used for the different game states
 */
enum gameStateEnum
{
  gameStateInit,      /**< Game initialization */
  gameStateIntro,     /** < Intro animation */
  gameStateMenu,      /**< Menu */
  gameStateKeyConfig,  /**< Key config */
  gameStateJoystickConfig,  /**< Joystick config */
  gameStatePlaying,   /**< Playing */
  gameStatePlayingPause,   /**< Playing / Pause */
  gameStatePlayingDisplayBoss,   /**< Playing / DisplayBoss */
};

/** Special game states enum
 *  Used for effects such as fade in...
 */
enum xGameTypeEnum
{
  xGameTypeFade,        /**< Fade effect */
  xGameTypeShake,       /**< Shake effect */
  xGameTypeFadeColor,   /**< Color fade effect */
  NB_X_GAME
};

/** Input Keys enum
 *  Used for the input binding
 */
enum inputKeyEnum
{
  KeyUp,
  KeyDown,
  KeyLeft,
  KeyRight,
  KeyFireUp,
  KeyFireDown,
  KeyFireLeft,
  KeyFireRight,
  KeyFireSelect,
  KeySpell,
  KeyInteract,
  KeyTimeControl,
  KeyFire
};

/** Menu keys enum
 *  Identify the various keys of the menu.
 */
enum menuItemEnum
{
  MenuStartNew,     /**< When starting the game */
  MenuStartOld,     /**< When restoring the game */
  MenuConfig,       /**< When configuring the game */
  MenuKeys,         /**< When configuring keys */
  MenuJoystick,     /**< When configuring joystick */
  MenuConfigBack,   /**< Back to the main menu */
  MenuTutoReset,    /**< Reset the tutorials */
  MenuLanguage,     /**< When configuring the language */
  MenuExit,         /**< When exiting the game */
  MenuAchievements, /**< Display the Achievements */
  MenuCredits,      /**< Display the credits screen */
  MenuHiScores,     /**< Display the hi-scores screen */
  MenuPlayerName,   /**< To enter/change the player name */
  MenuVolumeSound,
  MenuVolumeMusic,

  MenuContinue,     /**< Continue the game */
  MenuSaveAndQuit,  /**< Save and return to main */
};

/** Menu states enum
 *  Identify the various states of the menu.
 */
enum menuStateEnum
{
  MenuStateMain,
  MenuStateConfig,
  MenuStateKeys,
  MenuStateJoystick,
  MenuStateHiScores,
  MenuStateChangeName,
  MenuStateCredits,
  MenuStateAchievements,
  MenuStateFirst    /**< First time, we choose language and keyboard */
};

enum achievementStatus {
  AchievementDone,
  AchievementUndone,
  AchievementPending
};

#endif // ENUMS_H
