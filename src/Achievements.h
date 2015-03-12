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
  Achievement3Hits,
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

  NB_FUNCTIONALITIES
};

const std::string functionalityLabel[NB_FUNCTIONALITIES] =
{
  "func_achiev_screen",
  "func_achiev_all",
  "func_double_spell",
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
  { Achievement3Hits,       "achiev_3_hits",        UnlockItem,           ItemFairyTarget },
  { AchievementCompleteSet, "achiev_complete_set",  UnlockItem,           ItemHatAdvanced},

  { AchievementGiantSlime,  "achiev_giant_slime",   UnlockItem,           ItemPetSlime },
  { AchievementCyclops,     "achiev_cyclops",       UnlockItem,           ItemSpellEarthquake },
  { AchievementRatKing,     "achiev_rat_king",      UnlockItem,           ItemCriticalAdvanced },
  { AchievementGiantSpider, "achiev_giant_spider",  UnlockItem,           ItemSpellWeb },
  { AchievementFrancky,     "achiev_francky",       UnlockItem,           ItemRobeAdvanced },

  { AchievementButcher,     "achiev_butcher",       UnlockFunctionality,  FunctionalityAchievementsScreen },
  { AchievementWitches,     "achiev_witches",       UnlockItem,           ItemBookMagicII },
  { AchievementEggs,        "achiev_eggs",          UnlockItem,           ItemBootsAdvanced },
  //{ AchievementFairies,     "achiev_fairies",       UnlockItem,           ItemGlovesAdvanced },
  { AchievementVampire,     "achiev_vampire",       UnlockItem,           ItemGlovesAdvanced },
  { AchievementChallenges,  "achiev_challenges",    UnlockFunctionality,  FunctionalityDoubleSpellDrop },
  { AchievementWin,         "achiev_win",           UnlockFunctionality,  FunctionalityAllAchievements },
};

const std::vector<enumAchievementType> sortedAchievements =
{
  AchievementRats,
  AchievementWitches,
  AchievementEggs,
  Achievement100,
  AchievementPietyMax,
  Achievement3Hits,
  AchievementCompleteSet,
  //AchievementFairies,

  // bosses
  AchievementButcher,
  AchievementGiantSlime,
  AchievementCyclops,
  AchievementRatKing,
  AchievementGiantSpider,
  AchievementFrancky,
  AchievementVampire,

  AchievementChallenges,
  AchievementWin,
};
#endif
