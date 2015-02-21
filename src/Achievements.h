#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Constants.h"

/** Achievement type enum
 *  All the achievements.
 */
enum enumAchievementType
{
  AchievementRats,
  Achievement100,
  AchievementPietyMax,
  Achievement3Hits,

  AchievementGiantSlime,
  AchievementCyclops,
  AchievementRatKing,
  AchievementGiantSpider,
  AchievementFrancky,

  NB_ACHIEVEMENTS
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
};

/** Array with all the achievement */
const achievementStuct achievements[NB_ACHIEVEMENTS] =
{
  {
    AchievementRats,        "achiev_rats",
    Achievement100,         "achiev_100",
    AchievementPietyMax,    "achiev_piety_max",
    Achievement3Hits,       "achiev_3_hits",

    AchievementGiantSlime,  "achiev_giant_slime",
    AchievementCyclops,     "achiev_cyclops",
    AchievementRatKing,     "achiev_rat_king",
    AchievementGiantSpider, "achiev_giant_spider",
    AchievementFrancky,     "achiev_francky",
  },
};


#endif
