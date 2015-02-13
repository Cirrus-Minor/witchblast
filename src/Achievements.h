#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

#include "Constants.h"

/** Achievement type enum
 *  All the achievements.
 */
enum enumAchievementType
{
  AchievementGiantSlime,
  AchievementRats,

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

/** Array with all the items and data */
const achievementStuct achievements[NB_ACHIEVEMENTS] =
{
  {
    AchievementGiantSlime, "achiev_giant_slime",
    AchievementRats, "achiev_rats",
  },
};


#endif
