#ifndef ITEMS_H
#define ITEMS_H

#include "Constants.h"

/** Alignment enum
 *  Alignment of the player and equipment.
 */
enum enumAlignment
{
  AlignmentNone,  /**< No alignment */
  AlignmentLight, /**< Light (order) */
  AlignmentDark   /**< Dark (chaos) */
};
const int SPECIAL_SHOT_SLOTS_STANDARD = 2;
const int SPECIAL_SHOT_SLOTS_ADVANCED = 2;
const int SPECIAL_SHOT_SLOTS = 1 + SPECIAL_SHOT_SLOTS_STANDARD + SPECIAL_SHOT_SLOTS_ADVANCED;

/** Item type enum
 *  All the items and equipments.
 */
enum enumItemType
{
  ItemCopperCoin,
  ItemSilverCoin,
  ItemGoldCoin,
  itemHealth,
  itemBossHeart,

  ItemMagicianHat,    // first equip item
  ItemLeatherBoots,
  ItemBookDualShots,
  ItemRageAmulet,
  ItemBossKey,
  ItemVibrationGloves,
  ItemMahoganyStaff,
  ItemFairy,
  ItemLeatherBelt,
  ItemBloodSnake,
  ItemGemIce,
  ItemGemIllusion,
  ItemGemStone,
  ItemGemLightning
};

const int FirstEquipItem = (int) ItemMagicianHat; /*!< Used as an offset when creating items */

/*!
 *  \brief Item structure
 *
 *  Contains all the data for an item.
 */
struct itemStuct
{
  enumItemType type;            /**< The item ID */
  std::string name;             /**< The item name */
  std::string description;      /**< The item description */
  int price;                    /**< The item price (for shops) */
  bool equip;                   /**< True if the item is an equipment */
  bool canBeSold;               /**< True if the item is can be sold */
  bool canBeFound;              /**< True if the item is can be found */
  bool generatesStance;         /**< True if picking the item generates an acquiring stance*/
  int level;                    /**< Minimal level where the item can be found */
  enumAlignment alignment;      /**< Item alignment */
  int requirement;              /**< Pre-requisite item */
  enumShotType specialShot;     /**< Special shot */
};

const int NUMBER_ITEMS = 19;  /*!< Total number of items */
/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "Copper coin", "A copper coin (value 1)",
    1, false, false, false, false, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemSilverCoin, "Silver coin", "A silver coin (value 5)",
    5, false, false, false, false, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGoldCoin, "Gold coin", "A gold coin (value 20)",
    20, false, false, false, false, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    itemHealth, "Health potion", "A health potion",
    8, false, true, false, false, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    itemBossHeart, "Titan's Heart", "Increases Max HP",
    250, false, false, false, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMagicianHat, "Enchanter Hat", "Increases fire rate",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBoots, "Velvet Boots", "Increases speed",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBookDualShots, "Spell : Dual Bolts", "Shoots two bolts",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemRageAmulet, "Rage Amulet", "Increases fire range",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBossKey, "Boss Key", "Open the Boss gate",
    200, true, false, false, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemVibrationGloves, "Vibration Gloves", "Increases bolt's speed and damages",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMahoganyStaff, "Mahogany Staff", "Increases bolt's speed and damages",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemFairy, "Fairy", "Help you in the dungeon",
    20, true, false, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBelt, "Leather Belt", "Increases fire rate",
    20, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBloodSnake, "Blood Snake", "Increases damages",
    25, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGemIce, "Ice Gem", "Ice attack (freeze)",
    25, true, true, true, true, 1, AlignmentNone, -1, ShotTypeIce
  },
  {
    ItemGemIllusion, "Illusion Gem", "Illusion attack (ignore walls)",
    25, true, true, true, true, 4, AlignmentDark, -1, ShotTypeIllusion
  },
  {
    ItemGemStone, "Stone Gem", "Stone attack (repulse)",
    25, true, true, true, true, 1, AlignmentNone, -1, ShotTypeStone
  },
  {
    ItemGemLightning, "Lighting Gem", "Lightning attack (bouncing)",
    25, true, true, true, true, 1, AlignmentNone, -1, ShotTypeLightning
  }
};

const int NUMBER_EQUIP_ITEMS = 14;  /*!< Number of equip items */
/** Item equipment type enum
 *  All the equipments.
 */
enum item_equip_enum {
  EQUIP_ENCHANTER_HAT,
  EQUIP_LEATHER_BOOTS,
  EQUIP_BOOK_DUAL,
  EQUIP_CONCENTRATION_AMULET,
  EQUIP_BOSS_KEY,
  EQUIP_VIBRATION_GLOVES,
  EQUIP_MAHOGANY_STAFF,
  EQUIP_FAIRY,
  EQUIP_LEATHER_BELT,
  EQUIP_BLOOD_SNAKE,
  EQUIP_GEM_ICE,
  EQUIP_GEM_ILLUSION,
  EQUIP_GEM_STONE,
  EQUIP_GEM_LIGHTNING
  };

#endif
