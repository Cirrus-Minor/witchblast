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

  NB_ACHIEVEMENTS
};

enum enumUnlockType
{
  UnlockNothing,
  UnlockItem,
  UnlockDivinity,
};

/*!
 *  \brief Item structure
 *
 *  Contains all the data for an item.
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
  { AchievementRats,        "achiev_rats",          UnlockNothing,  -1 },
  { Achievement100,         "achiev_100",           UnlockNothing,  -1 },
  { AchievementPietyMax,    "achiev_piety_max",     UnlockItem,     ItemBookPrayerII },
  { Achievement3Hits,       "achiev_3_hits",        UnlockNothing,  -1 },
  { AchievementCompleteSet, "achiev_complete_set",  UnlockItem,     ItemHatAdvanced},

  { AchievementGiantSlime,  "achiev_giant_slime",   UnlockItem,     ItemPetSlime },
  { AchievementCyclops,     "achiev_cyclops",       UnlockItem,     ItemSpellEarthquake },
  { AchievementRatKing,     "achiev_rat_king",      UnlockItem,     ItemCriticalAdvanced },
  { AchievementGiantSpider, "achiev_giant_spider",  UnlockItem,     ItemSpellWeb },
  { AchievementFrancky,     "achiev_francky",       UnlockItem,     ItemRobeAdvanced },
};


#endif
