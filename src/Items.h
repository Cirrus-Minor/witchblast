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

/** Rarity enum
 *  Rarity of the equipment.
 */
enum enumRarity
{
  RarityCommon,  /**< Common */
  RarityUnommon, /**< Uncommon */
  RarityRare     /**< Rare */
};

/** Familiar enum
 *  Familiars.
 */
enum enumFamiliar
{
  FamiliarNone = -1,     /**< No familiar */
  FamiliarFairy,    /**< Standard Fairy */
  FamiliarFairyIce  /**< Ice Fairy */
};
const int FAIRY_NUMBER = 2;

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
  ItemGemLightning,
  ItemFairyIce,
  ItemRingIce,
  ItemRingStone,
  ItemRingLightning,
  ItemRingIllusion
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
  enumFamiliar familiar;        /**< True if the "item" is a familiar */
  bool canBeSold;               /**< True if the item is can be sold */
  bool canBeFound;              /**< True if the item is can be found */
  bool generatesStance;         /**< True if picking the item generates an acquiring stance*/
  int level;                    /**< Minimal level where the item can be found */
  enumRarity rarity;            /**< Item rarity */
  enumAlignment alignment;      /**< Item alignment */
  int requirement;              /**< Pre-requisite item */
  enumShotType specialShot;     /**< Special shot */
};

const int NUMBER_ITEMS = 24;  /*!< Total number of items */
/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "Copper coin", "A copper coin (value 1)",
    1, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemSilverCoin, "Silver coin", "A silver coin (value 5)",
    5, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGoldCoin, "Gold coin", "A gold coin (value 20)",
    20, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    itemHealth, "Health potion", "A health potion",
    8, false, FamiliarNone, true, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    itemBossHeart, "Titan's Heart", "Increases Max HP",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMagicianHat, "Enchanter Hat", "Increases fire rate",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBoots, "Velvet Boots", "Increases speed",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBookDualShots, "Spell : Dual Bolts", "Shoots two bolts",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemRageAmulet, "Rage Amulet", "Increases fire range",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBossKey, "Boss Key", "Open the Boss gate",
    200, true, FamiliarNone, false, false, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemVibrationGloves, "Vibration Gloves", "Increases bolt's speed and damages",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMahoganyStaff, "Mahogany Staff", "Increases bolt's speed and damages",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemFairy, "Fairy", "Help you in the dungeon",
    20, true, FamiliarFairy, false, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBelt, "Leather Belt", "Increases fire rate",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBloodSnake, "Blood Snake", "Increases damages",
    25, true, FamiliarNone, true, true, true, 1, RarityUnommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGemIce, "Ice Gem", "Ice attack (freeze)",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeIce
  },
  {
    ItemGemIllusion, "Illusion Gem", "Illusion attack (ignore walls)",
    25, true, FamiliarNone, true, true, true, 4, RarityUnommon, AlignmentDark, -1, ShotTypeIllusion
  },
  {
    ItemGemStone, "Stone Gem", "Stone attack (repulse)",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStone
  },
  {
    ItemGemLightning, "Lighting Gem", "Lightning attack (bouncing)",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeLightning
  },
  {
    ItemFairyIce, "Ice Fairy", "Help you in the dungeon",
    40, true, FamiliarFairyIce, false, true, true, 1, RarityUnommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemRingIce, "Sapphire Ring", "Increases ice power",
    45, true, FamiliarNone, true, true, true, 2, RarityUnommon, AlignmentNone, ItemGemIce, ShotTypeStandard
  },
  {
    ItemRingStone, "Obsidian Ring", "Increases stone power",
    45, true, FamiliarNone, true, true, true, 2, RarityUnommon, AlignmentNone, ItemGemStone, ShotTypeStandard
  },
  {
    ItemRingLightning, "Opal Ring", "Increases lightning power",
    45, true, FamiliarNone, true, true, true, 2, RarityUnommon, AlignmentNone, ItemGemLightning, ShotTypeStandard
  },
  {
    ItemRingIllusion, "Quartz Ring", "Increases illusion power",
    45, true, FamiliarNone, true, true, true, 2, RarityUnommon, AlignmentNone, ItemGemIllusion, ShotTypeStandard
  }
};

const int NUMBER_EQUIP_ITEMS = 19;  /*!< Number of equip items */
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
  EQUIP_GEM_LIGHTNING,
  EQUIP_FAIRY_ICE,
  EQUIP_RING_ICE,
  EQUIP_RING_STONE,
  EQUIP_RING_LIGHTNING,
  EQUIP_RING_ILLUSION
  };

#endif
