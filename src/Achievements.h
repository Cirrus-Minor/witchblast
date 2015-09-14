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
  enumUnlockType unlockType;    /**< What the achievement unlocks (type) */
  int unlock;                   /**< What the achievement unlocks */
};

/** Array with all the achievement */
const achievementStuct achievements[NB_ACHIEVEMENTS] =
{
  { AchievementRats,        "achiev_rats",          UnlockItem,           ItemSpellFairy },
  { Achievement100,         "achiev_100",           UnlockItem,           ItemSpellFlower },
  { AchievementPietyMax,    "achiev_piety_max",     UnlockItem,           ItemBookPrayerII },
  { Achievement4Hits,       "achiev_4_hits",        UnlockItem,           ItemFairyTarget },
  { AchievementCompleteSet, "achiev_complete_set",  UnlockItem,           ItemHatAdvanced},

  { AchievementGiantSlime,  "achiev_giant_slime",   UnlockItem,           ItemPetSlime },
  { AchievementCyclops,     "achiev_cyclops",       UnlockItem,           ItemSpellEarthquake },
  { AchievementRatKing,     "achiev_rat_king",      UnlockItem,           ItemCriticalAdvanced },
  { AchievementGiantSpider, "achiev_giant_spider",  UnlockItem,           ItemSpellWeb },
  { AchievementFrancky,     "achiev_francky",       UnlockItem,           ItemRobeAdvanced },

  { AchievementButcher,     "achiev_butcher",       UnlockFunctionality,  FunctionalityAchievementsScreen },
  { AchievementWitches,     "achiev_witches",       UnlockItem,           ItemBookMagicII },
  { AchievementEggs,        "achiev_eggs",          UnlockItem,           ItemBootsAdvanced },

  { AchievementVampire,     "achiev_vampire",       UnlockItem,           ItemSpellTime },
  { AchievementChallenges,  "achiev_challenges",    UnlockFunctionality,  FunctionalityDoubleSpellDrop },
  { AchievementWin,         "achiev_win",           UnlockFunctionality,  FunctionalityAllAchievements },

  { AchievementFairies,     "achiev_fairies",       UnlockItem,           ItemRageAmulet },
  { AchievementGhostbuster, "achiev_ghost",         UnlockItem,           ItemRearShotAdvanced },
  { AchievementNoob,        "achiev_noob",          UnlockFunctionality,  FunctionalityNothing },
  { AchievementNoDamage,    "achiev_no_damage",     UnlockItem,           ItemSideShots },
  { AchievementNoDamage2,   "achiev_no_damage2",    UnlockItem,           ItemBag },
  { AchievementNoDamage3,   "achiev_no_damage3",    UnlockItem,           ItemGlovesAdvanced },
  { AchievementApostate,    "achiev_apostate",      UnlockItem,           ItemSpellLightning },
  { AchievementFanatic,     "achiev_fanatic",       UnlockItem,           ItemBeltAdvanced },
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
