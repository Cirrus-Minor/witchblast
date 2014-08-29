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
  RarityUncommon, /**< Uncommon */
  RarityRare     /**< Rare */
};

/** Familiar enum
 *  Familiars.
 */
enum enumFamiliar
{
  FamiliarNone = -1,    /**< No familiar */
  FamiliarFairy,        /**< Standard Fairy */
  FamiliarFairyIce,     /**< Ice Fairy */
  FamiliarFairyFire,    /**< Fire Fairy */
  FamiliarFairyTarget   /**< Target Fairy */
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
  ItemBossHeart,
  ItemBonusHealth,
  ItemHealthVerySmall,
  ItemHealthSmall,
  ItemHealth,

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
  ItemRingIllusion,
  ItemBookTripleShots,
  ItemBroochStar,
  ItemFairyFire,
  ItemFairyTarget,
  ItemMagicianRobe,
  ItemGemFire,
  ItemRingFire,
  ItemManualStaff,

  NUMBER_ITEMS
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

/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "item_copper", "item_copper_desc",
    1, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemSilverCoin, "item_silver", "item_silver_desc",
    5, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGoldCoin, "item_gold", "item_gold_desc",
    20, false, FamiliarNone, false, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBossHeart, "item_titan_heart", "item_titan_heart_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBonusHealth, "item_elven_cookie", "item_elven_cookie_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },{
    ItemHealthVerySmall, "item_apple", "item_apple_desc",
    2, false, FamiliarNone, true, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemHealthSmall, "item_bread", "item_bread_desc",
    4, false, FamiliarNone, true, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemHealthSmall, "item_cheese", "item_cheese_desc",
    8, false, FamiliarNone, true, false, false, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMagicianHat, "item_sorcerer_hat", "item_sorcerer_hat_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBoots, "item_velvet_boots", "item_velvet_boots_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBookDualShots, "item_spell_dual", "item_spell_dual_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemRageAmulet, "item_rage_amulet", "item_rage_amulet_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBossKey, "item_boss_key", "item_boss_key_desc",
    200, true, FamiliarNone, false, false, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemVibrationGloves, "item_vibration_gloves", "item_vibration_gloves_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMahoganyStaff, "item_staff", "item_staff_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemFairy, "item_fairy", "item_fairy_desc",
    20, true, FamiliarFairy, false, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemLeatherBelt, "item_leather_belt", "item_leather_belt_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemBloodSnake, "item_blood_snake", "item_blood_snake_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityUncommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGemIce, "item_gem_ice", "item_gem_ice_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, AlignmentNone, -1, ShotTypeIce
  },
  {
    ItemGemIllusion, "item_gem_illusion", "item_gem_illusion_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, AlignmentDark, -1, ShotTypeIllusion
  },
  {
    ItemGemStone, "item_gem_stone", "item_gem_stone_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, AlignmentNone, -1, ShotTypeStone
  },
  {
    ItemGemLightning, "item_gem_lightning", "item_gem_lightning_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, AlignmentNone, -1, ShotTypeLightning
  },
  {
    ItemFairyIce, "item_fairy_ice", "item_fairy_ice_desc",
    40, true, FamiliarFairyIce, false, true, true, 2, RarityUncommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemRingIce, "item_ring_ice", "item_ring_ice_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, AlignmentNone, ItemGemIce, ShotTypeStandard
  },
  {
    ItemRingStone, "item_ring_stone", "item_ring_stone_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, AlignmentNone, ItemGemStone, ShotTypeStandard
  },
  {
    ItemRingLightning, "item_ring_lightning", "item_ring_lightning_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, AlignmentNone, ItemGemLightning, ShotTypeStandard
  },
  {
    ItemRingIllusion, "item_ring_illusion", "item_ring_illusion_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, AlignmentDark, ItemGemIllusion, ShotTypeStandard
  },
  {
    ItemBookTripleShots, "item_spell_triple", "item_spell_triple_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityCommon, AlignmentNone, ItemBookDualShots, ShotTypeStandard
  },
  {
    ItemBroochStar, "item_brooch_star", "item_brooch_star",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemFairyFire, "item_fairy_fire", "item_fairy_fire_desc",
    40, true, FamiliarFairyFire, false, true, true, 2, RarityUncommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemFairyTarget, "item_fairy_target", "item_fairy_target_desc",
    40, true, FamiliarFairyTarget, false, true, true, 2, RarityUncommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemMagicianRobe, "item_robe_magician", "item_robe_magician_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  },
  {
    ItemGemFire, "item_gem_fire", "item_gem_fire_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, AlignmentLight, -1, ShotTypeFire
  },
  {
    ItemRingStone, "item_ring_fire", "item_ring_fire_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, AlignmentLight, ItemGemFire, ShotTypeStandard
  },
  {
    ItemManualStaff, "item_manual_staff", "item_manual_staff_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, AlignmentNone, -1, ShotTypeStandard
  }
};

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
  EQUIP_RING_ILLUSION,
  EQUIP_BOOK_TRIPLE,
  EQUIP_BROOCH_STAR,
  EQUIP_FAIRY_FIRE,
  EQUIP_FAIRY_TARGET,
  EQUIP_MAGICIAN_ROBE,
  EQUIP_GEM_FIRE,
  EQUIP_RING_FIRE,
  EQUIP_MANUAL_STAFF,

  NUMBER_EQUIP_ITEMS
  };

#endif
