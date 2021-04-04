#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Constants.h"
#include "Items.h"

/** Achievement type enum
 *  All the achievements.
 */
enum enumAchievementType
{
  AchievementRats,
  Achievement100,
  AchievementPietyMax,
  Achievement4Hits,
  AchievementCompleteSet,
  AchievementGiantSlime,
  AchievementCyclops,
  AchievementRatKing,

  AchievementGiantSpider,
  AchievementFrancky,
  AchievementButcher,
  AchievementWitches,
  AchievementEggs,
  AchievementVampire,
  AchievementChallenges,
  AchievementWin,

  AchievementFairies,
  AchievementGhostbuster,
  AchievementNoob,
  AchievementNoDamage,
  AchievementNoDamage2,
  AchievementNoDamage3,
  AchievementApostate,
  AchievementFanatic,

  NB_ACHIEVEMENTS
};

enum enumUnlockType
{
  UnlockNothing,
  UnlockItem,
  UnlockDivinity,
  UnlockFunctionality,
};

enum enumFunctionalityType
{
  FunctionalityAchievementsScreen,
  FunctionalityAllAchievements,
  FunctionalityDoubleSpellDrop,
  FunctionalityNothing,

  NB_FUNCTIONALITIES
};

const std::string functionalityLabel[NB_FUNCTIONALITIES] =
{
  "func_achiev_screen",
  "func_achiev_all",
  "func_double_spell",
  "func_nothing",
};

/*!
 *  \brief Achievement structure
 *
 *  Contains all the data for an achievement.
 */
struct achievementStuct
{
  enumAchievementType id;       /**< The achievement's ID */
  std::string label;            /**< The achievement label */
  std::string gamerzilla;       /**< The achievement name for Gamerzilla */
  enumUnlockType unlockType;    /**< What the achievement unlocks (type) */
  int unlock;                   /**< What the achievement unlocks */
};

/** Array with all the achievement */
const achievementStuct achievements[NB_ACHIEVEMENTS] =
{
  { AchievementRats,        "achiev_rats",          "Rodenticide",          UnlockItem,           ItemSpellFairy },
  { Achievement100,         "achiev_100",           "Stingy",               UnlockItem,           ItemSpellFlower },
  { AchievementPietyMax,    "achiev_piety_max",     "Faithful",             UnlockItem,           ItemBookPrayerII },
  { Achievement4Hits,       "achiev_4_hits",        "Brochette",            UnlockItem,           ItemFairyTarget },
  { AchievementCompleteSet, "achiev_complete_set",  "Dressed",              UnlockItem,           ItemHatAdvanced},

  { AchievementGiantSlime,  "achiev_giant_slime",   "Giant Slime Slayer",   UnlockItem,           ItemPetSlime },
  { AchievementCyclops,     "achiev_cyclops",       "Cyclops Slayer",       UnlockItem,           ItemSpellEarthquake },
  { AchievementRatKing,     "achiev_rat_king",      "Wererat Slayer",       UnlockItem,           ItemCriticalAdvanced },
  { AchievementGiantSpider, "achiev_giant_spider",  "Giant Spider Slayer",  UnlockItem,           ItemSpellWeb },
  { AchievementFrancky,     "achiev_francky",       "Thing Slayer",         UnlockItem,           ItemRobeAdvanced },

  { AchievementButcher,     "achiev_butcher",       "Butcher Slayer",       UnlockFunctionality,  FunctionalityAchievementsScreen },
  { AchievementWitches,     "achiev_witches",       "Inquisitor",           UnlockItem,           ItemBookMagicII },
  { AchievementEggs,        "achiev_eggs",          "Bad omelette",         UnlockItem,           ItemBootsAdvanced },

  { AchievementVampire,     "achiev_vampire",       "Vampire Hunter",       UnlockItem,           ItemSpellTime },
  { AchievementChallenges,  "achiev_challenges",    "Gladiator",            UnlockFunctionality,  FunctionalityDoubleSpellDrop },
  { AchievementWin,         "achiev_win",           "Live a dream",         UnlockFunctionality,  FunctionalityAllAchievements },

  { AchievementFairies,     "achiev_fairies",       "Charlie's Angels",     UnlockItem,           ItemRageAmulet },
  { AchievementGhostbuster, "achiev_ghost",         "Ghostbuster",          UnlockItem,           ItemRearShotAdvanced },
  { AchievementNoob,        "achiev_noob",          "True noob",            UnlockFunctionality,  FunctionalityNothing },
  { AchievementNoDamage,    "achiev_no_damage",     "Tough guy",            UnlockItem,           ItemSideShots },
  { AchievementNoDamage2,   "achiev_no_damage2",    "Very tough guy",       UnlockItem,           ItemBag },
  { AchievementNoDamage3,   "achiev_no_damage3",    "Very very tough guy",  UnlockItem,           ItemGlovesAdvanced },
  { AchievementApostate,    "achiev_apostate",      "Apostate",             UnlockItem,           ItemSpellLightning },
  { AchievementFanatic,     "achiev_fanatic",       "Fanatic",              UnlockItem,           ItemBeltAdvanced },
};

const std::vector<enumAchievementType> sortedAchievements =
{
  AchievementNoob,
  AchievementNoDamage,
  AchievementNoDamage2,
  AchievementNoDamage3,
  AchievementRats,
  AchievementWitches,
  AchievementEggs,
  AchievementGhostbuster,

  Achievement100,
  AchievementPietyMax,
  AchievementApostate,
  AchievementFanatic,
  Achievement4Hits,
  AchievementCompleteSet,
  AchievementChallenges,
  AchievementFairies,

  // bosses
  AchievementButcher,
  AchievementGiantSlime,
  AchievementCyclops,
  AchievementRatKing,
  AchievementGiantSpider,
  AchievementFrancky,
  AchievementVampire,

  AchievementWin,
};
#endif
