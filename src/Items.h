#ifndef ITEMS_H
#define ITEMS_H

/** Alignment enum
 *  Alignment of the player and equipment.
 */
enum enumAlignment
{
  AlignmentNone,  /**< No alignment */
  AlignmentLight, /**< Light (order) */
  AlignmentDark   /**< Dark (chaos) */
};

/** Item type enum
 *  All the items and equipments.
 */
enum enumItemType
{
  ItemCopperCoin,
  ItemSilverCoin,
  ItemGoldCoin,
  itemHealth,

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
  ItemIceGem
};

const int FirstEquipItem = (int) ItemMagicianHat; /*!< Used as an offset when creating items */

/*!
 *  \brief Item structure
 *
 *  Contains all the data for an item.
 */
struct itemStuct
{
  enumItemType type;          /**< The item ID */
  std::string name;           /**< The item name */
  std::string description;    /**< The item description */
  int price;                  /**< The item price (for shops) */
  bool equip;                 /**< True if the item is an equipment */
  bool canBeSold;             /**< True if the item is can be sold */
  int level;                  /**< Minimal level where the item can be found */
  enumAlignment alignment;    /**< Item alignment */
  int requirement;            /**< Pre-requisite item */
};

const int NUMBER_ITEMS = 15;  /*!< Total number of items */

/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "Copper coin", "A copper coin (value 1)",
    1, false, false, 1, AlignmentNone, -1
  },
  {
    ItemSilverCoin, "Silver coin", "A silver coin (value 5)",
    5, false, false, 1, AlignmentNone, -1
  },
  {
    ItemGoldCoin, "Gold coin", "A gold coin (value 20)",
    20, false, false, 1, AlignmentNone, -1
  },
  {
    itemHealth, "Health potion", "A health potion",
    8, false, true, 1, AlignmentNone, -1
  },
  {
    ItemMagicianHat, "Enchanter Hat", "Increases fire rate",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemLeatherBoots, "Leather Boots", "Increases Boots",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemBookDualShots, "Spell : Dual Bolts", "Shoots two bolts",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemRageAmulet, "Rage Amulet", "Increases fire range",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemBossKey, "Boss Key", "Open the Boss gate",
    200, true, false, 1, AlignmentNone, -1
  },
  {
    ItemVibrationGloves, "Vibration Gloves", "Increases bolt's speed and damages",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemMahoganyStaff, "Mahogany Staff", "Increases bolt's speed and damages",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemFairy, "Fairy", "Help you in the dungeon",
    20, true, false, 1, AlignmentNone, -1
  },
  {
    ItemLeatherBelt, "Leather Belt", "Increases fire rate",
    20, true, true, 1, AlignmentNone, -1
  },
  {
    ItemBloodSnake, "Blood Snake", "Increases damages",
    25, true, true, 1, AlignmentNone, -1
  },
  {
    ItemIceGem, "Ice Gem", "Ice attack",
    25, true, true, 1, AlignmentNone, -1
  }
};

const int NUMBER_EQUIP_ITEMS = 11;  /*!< Number of equip items */

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
  EQUIP_ICE_GEM
  };

#endif
