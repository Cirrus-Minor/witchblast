#ifndef ITEMS_H
#define ITEMS_H

#include "Constants.h"

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
  FamiliarFairyTarget,  /**< Target Fairy */
  FamiliarFairyFire,    /**< Fire Fairy */
  FamiliarFairyIce,     /**< Ice Fairy */
  FamiliarFairyPoison,  /**< Poison Fairy */
};

// spells
enum enumCastSpell
{
  SpellTeleport,
  SpellSlimeExplode,
  SpellFireball,
  SpellFreeze,
  SpellEarthquake,
  SpellProtection,
  SpellWeb,
  SpellFlower,
  SpellFairy,

  SpellNone
};
const int SPELL_MAX = SpellNone;

const std::string spellLabel[SpellNone] =
{
  "spell_teleport",
  "spell_slime_explode",
  "spell_fireball",
  "spell_freeze",
  "spell_earthquake",
  "spell_protection",
  "spell_web",
  "spell_flower",
  "spell_fairy"
};

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
  ItemHealthVerySmallPoison,

  ItemMagicianHat,    // first equip item
  ItemLeatherBoots,
  ItemBookDualShots,
  ItemRageAmulet,
  ItemBossKey,
  ItemDisplacementGloves,
  ItemMahoganyStaff,
  ItemFairy,
  ItemLeatherBelt,
  ItemBloodSnake,
  ItemGemIce,
  ItemGemIllusion,
  ItemGemStone,
  ItemGemLightning,
  ItemFairyTarget,
  ItemRingIce,
  ItemRingStone,
  ItemRingLightning,
  ItemRingIllusion,
  ItemBookTripleShots,
  ItemCriticalAdvanced,
  ItemFairyFire,
  ItemFairyIce,
  ItemMagicianRobe,
  ItemGemFire,
  ItemRingFire,
  ItemManualStaff,
  ItemSpellTeleport,
  ItemSpellSlimeExplode,
  ItemSpellFireball,
  ItemSpellFreeze,
  ItemSpellEarthquake,
  ItemSpellProtection,
  ItemManualSlime,
  ItemBookDualShotsQuick,
  ItemBookTripleShotsQuick,
  ItemManualHealth,
  ItemRearShot,
  ItemBookRandomShot,
  ItemSpellWeb,
  ItemBookMagicI,
  ItemBookMagicII,
  ItemSulfur,
  ItemFloorMap,
  ItemAlcohol,
  ItemLuck,
  ItemFairyPowder,
  ItemBookRapidShots,
  ItemGemPoison,
  ItemSpellFlower,
  ItemFairyPoison,
  ItemRingPoison,
  ItemBookPrayerI,
  ItemBookPrayerII,
  ItemPetSlime,
  ItemHatAdvanced,
  ItemRobeAdvanced,
  ItemCritical,
  ItemSpellFairy,
  ItemGlovesAdvanced,
  ItemBootsAdvanced,

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
  bool isFloorItem;             /**< Floor item */
  int requirement;              /**< Pre-requisite item */
  enumShotType specialShot;     /**< Special shot */
  enumCastSpell spell;          /**< Spell */
};

/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "item_copper", "item_copper_desc",
    1, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemSilverCoin, "item_silver", "item_silver_desc",
    5, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemGoldCoin, "item_gold", "item_gold_desc",
    20, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBossHeart, "item_titan_heart", "item_titan_heart_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBonusHealth, "item_elven_cookie", "item_elven_cookie_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemHealthVerySmall, "item_apple", "item_apple_desc",
    2, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemHealthSmall, "item_bread", "item_bread_desc",
    4, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemHealthSmall, "item_cheese", "item_cheese_desc",
    8, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemHealthVerySmallPoison, "item_apple_green", "item_apple_green_desc",
    2, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },

  {
    ItemMagicianHat, "item_sorcerer_hat", "item_sorcerer_hat_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemLeatherBoots, "item_velvet_boots", "item_velvet_boots_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookDualShots, "item_spell_dual", "item_spell_dual_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemRageAmulet, "item_rage_amulet", "item_rage_amulet_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBossKey, "item_boss_key", "item_boss_key_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemDisplacementGloves, "item_displacement_gloves", "item_displacement_gloves_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemMahoganyStaff, "item_staff", "item_staff_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemFairy, "item_fairy", "item_fairy_desc",
    20, true, FamiliarFairy, false, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemLeatherBelt, "item_leather_belt", "item_leather_belt_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBloodSnake, "item_blood_snake", "item_blood_snake_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemGemIce, "item_gem_ice", "item_gem_ice_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeIce, SpellNone
  },
  {
    ItemGemIllusion, "item_gem_illusion", "item_gem_illusion_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, -1, ShotTypeIllusion, SpellNone
  },
  {
    ItemGemStone, "item_gem_stone", "item_gem_stone_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeStone, SpellNone
  },
  {
    ItemGemLightning, "item_gem_lightning", "item_gem_lightning_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeLightning, SpellNone
  },
  {
    ItemFairyTarget, "item_fairy_target", "item_fairy_target_desc",
    40, true, FamiliarFairyTarget, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemRingIce, "item_ring_ice", "item_ring_ice_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemIce, ShotTypeStandard, SpellNone
  },
  {
    ItemRingStone, "item_ring_stone", "item_ring_stone_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemStone, ShotTypeStandard, SpellNone
  },
  {
    ItemRingLightning, "item_ring_lightning", "item_ring_lightning_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemLightning, ShotTypeStandard, SpellNone
  },
  {
    ItemRingIllusion, "item_ring_illusion", "item_ring_illusion_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemIllusion, ShotTypeStandard, SpellNone
  },
  {
    ItemBookTripleShots, "item_spell_triple", "item_spell_triple_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityCommon, false, ItemBookDualShots, ShotTypeStandard, SpellNone
  },
  {
    ItemCriticalAdvanced, "item_critical_advanced", "item_critical_advanced_desc",
    35, true, FamiliarNone, true, true, true, 1, RarityCommon, false, ItemCritical, ShotTypeStandard, SpellNone
  },
  {
    ItemFairyFire, "item_fairy_fire", "item_fairy_fire_desc",
    30, true, FamiliarFairyFire, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemFairyIce, "item_fairy_ice", "item_fairy_ice_desc",
    30, true, FamiliarFairyIce, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemMagicianRobe, "item_robe_magician", "item_robe_magician_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemGemFire, "item_gem_fire", "item_gem_fire_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, -1, ShotTypeFire, SpellNone
  },
  {
    ItemRingFire, "item_ring_fire", "item_ring_fire_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemFire, ShotTypeStandard, SpellNone
  },
  {
    ItemManualStaff, "item_manual_staff", "item_manual_staff_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemSpellTeleport, "item_spell_teleport", "item_spell_teleport_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellTeleport
  },
  {
    ItemSpellSlimeExplode, "item_spell_slime_explode", "item_spell_slime_explode_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellSlimeExplode
  },
  {
    ItemSpellFireball, "item_spell_fireball", "item_spell_fireball_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFireball
  },
  {
    ItemSpellFreeze, "item_spell_freeze", "item_spell_freeze_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFreeze
  },
  {
    ItemSpellEarthquake, "item_spell_earthquake", "item_spell_earthquake_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellEarthquake
  },
  {
    ItemSpellProtection, "item_spell_protection", "item_spell_protection_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellProtection
  },
  {
    ItemManualSlime, "item_manual_slime", "item_manual_slime_desc",
    30, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookDualShotsQuick, "item_spell_dual_quick", "item_spell_dual_quick_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookTripleShotsQuick, "item_spell_triple_quick", "item_spell_triple_quick_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityCommon, false, ItemBookDualShotsQuick, ShotTypeStandard, SpellNone
  },
  {
    ItemManualHealth, "item_manual_health", "item_manual_health_desc",
    20, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemRearShot, "item_rear_shot", "item_rear_shot_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookRandomShot, "item_book_random_shot", "item_book_random_shot_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemSpellWeb, "item_spell_web", "item_spell_web_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellWeb
  },
  {
    ItemBookMagicI, "item_book_magic_i", "item_book_magic_i_desc",
    35, true, FamiliarNone, true, true, true, 4, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookMagicII, "item_book_magic_ii", "item_book_magic_ii_desc",
    35, true, FamiliarNone, true, true, true, 4, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemSulfur, "item_sulfur", "item_sulfur_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemFloorMap, "item_floor_map", "item_floor_map_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemAlcohol, "item_alcohol", "item_alcohol_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemLuck, "item_luck", "item_luck_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemFairyPowder, "item_fairy_powder", "item_fairy_powder_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookRapidShots, "item_rapid_shots", "item_rapid_shots_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemGemPoison, "item_gem_poison", "item_gem_poison_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, -1, ShotTypePoison, SpellNone
  },
  {
    ItemSpellFlower, "item_spell_flower", "item_spell_flower_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFlower
  },
  {
    ItemFairyPoison, "item_fairy_poison", "item_fairy_poison_desc",
    30, true, FamiliarFairyPoison, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemRingPoison, "item_ring_poison", "item_ring_poison_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemPoison, ShotTypeStandard, SpellNone
  },
  {
    ItemBookPrayerI, "item_book_prayer_i", "item_book_prayer_i_desc",
    35, true, FamiliarNone, true, true, true, 3, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemBookPrayerII, "item_book_prayer_ii", "item_book_prayer_ii_desc",
    35, true, FamiliarNone, true, true, true, 3, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemPetSlime, "item_pet_slime", "item_pet_slime_desc",
    50, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemHatAdvanced, "item_hat_advanced", "item_hat_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityCommon, false, ItemMagicianHat, ShotTypeStandard, SpellNone
  },
  {
    ItemRobeAdvanced, "item_robe_advanced", "item_robe_advanced_desc",
    40, true, FamiliarNone, true, true, true, 3, RarityCommon, false, ItemMagicianRobe, ShotTypeStandard, SpellNone
  },
  {
    ItemCritical, "item_critical", "item_critical_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone
  },
  {
    ItemSpellFairy, "item_spell_fairy", "item_spell_fairy_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFairy
  },
  {
    ItemGlovesAdvanced, "item_gloves_advanced", "item_gloves_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityCommon, false, ItemDisplacementGloves, ShotTypeStandard, SpellNone
  },
  {
    ItemBootsAdvanced, "item_boots_advanced", "item_boots_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityCommon, false, ItemLeatherBoots, ShotTypeStandard, SpellNone
  },
};

/** Item equipment type enum
 *  All the equipments.
 */
enum item_equip_enum {
  EQUIP_MAGICIAN_HAT,
  EQUIP_LEATHER_BOOTS,
  EQUIP_BOOK_DUAL,
  EQUIP_RAGE_AMULET,
  EQUIP_BOSS_KEY,
  EQUIP_DISPLACEMENT_GLOVES,
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
  EQUIP_CRITICAL_ADVANCED,
  EQUIP_FAIRY_FIRE,
  EQUIP_FAIRY_TARGET,
  EQUIP_MAGICIAN_ROBE,
  EQUIP_GEM_FIRE,
  EQUIP_RING_FIRE,
  EQUIP_MANUAL_STAFF,
  EQUIP_SPELL_TELEPORT,
  EQUIP_SPELL_SLIME_EXPLODE,
  EQUIP_SPELL_FIREBALL,
  EQUIP_SPELL_FREEZE,
  EQUIP_SPELL_EARTHQUAKE,
  EQUIP_SPELL_PROTECTION,
  EQUIP_MANUAL_SLIMES,
  EQUIP_BOOK_DUAL_QUICK,
  EQUIP_BOOK_TRIPLE_QUICK,
  EQUIP_MANUAL_HEALTH,
  EQUIP_REAR_SHOT,
  EQUIP_BOOK_RANDOM,
  EQUIP_SPELL_WEB,
  EQUIP_BOOK_MAGIC_I,
  EQUIP_BOOK_MAGIC_II,
  EQUIP_SULFUR,
  EQUIP_FLOOR_MAP,
  EQUIP_ALCOHOL,
  EQUIP_LUCK,
  EQUIP_FAIRY_POWDER,
  EQUIP_RAPID_SHOT,
  EQUIP_GEM_POISON,
  EQUIP_SPELL_FLOWER,
  EQUIP_FAIRY_POISON,
  EQUIP_RING_POISON,
  EQUIP_BOOK_PRAYER_I,
  EQUIP_BOOK_PRAYER_II,
  EQUIP_PET_SLIME,
  EQUIP_HAT_ADVANCED,
  EQUIP_ROBE_ADVANCED,
  EQUIP_CRITICAL,
  EQUIP_SPELL_FAIRY,
  EQUIP_GLOVES_ADVANCED,
  EQUIP_BOOTS_ADVANCED,

  NUMBER_EQUIP_ITEMS
  };

const std::vector<item_equip_enum> sortedEquipement =
{
  // clothes
  EQUIP_MAGICIAN_ROBE,
  EQUIP_ROBE_ADVANCED,
  EQUIP_MAGICIAN_HAT,
  EQUIP_HAT_ADVANCED,
  EQUIP_LEATHER_BOOTS,
  EQUIP_BOOTS_ADVANCED,
  EQUIP_DISPLACEMENT_GLOVES,
  EQUIP_GLOVES_ADVANCED,
  EQUIP_LEATHER_BELT,
  EQUIP_RAGE_AMULET,
  EQUIP_CRITICAL,
  EQUIP_CRITICAL_ADVANCED,
  EQUIP_RING_ICE,
  EQUIP_RING_STONE,
  EQUIP_RING_LIGHTNING,
  EQUIP_RING_FIRE,
  EQUIP_RING_ILLUSION,
  EQUIP_RING_POISON,

  // weapons
  EQUIP_MAHOGANY_STAFF,
  EQUIP_BLOOD_SNAKE,
  EQUIP_REAR_SHOT,
  EQUIP_SULFUR,
  EQUIP_GEM_ICE,
  EQUIP_GEM_STONE,
  EQUIP_GEM_LIGHTNING,
  EQUIP_GEM_FIRE,
  EQUIP_GEM_ILLUSION,
  EQUIP_GEM_POISON,

  // books
  EQUIP_MANUAL_STAFF,
  EQUIP_MANUAL_SLIMES,
  EQUIP_MANUAL_HEALTH,
  EQUIP_BOOK_RANDOM,
  EQUIP_BOOK_MAGIC_I,
  EQUIP_BOOK_MAGIC_II,
  EQUIP_BOOK_PRAYER_I,
  EQUIP_BOOK_PRAYER_II,
  EQUIP_BOOK_DUAL,
  EQUIP_BOOK_TRIPLE,
  EQUIP_BOOK_DUAL_QUICK,
  EQUIP_BOOK_TRIPLE_QUICK,
  EQUIP_RAPID_SHOT,

  // familiars
  EQUIP_FAIRY,
  EQUIP_FAIRY_ICE,
  EQUIP_FAIRY_FIRE,
  EQUIP_FAIRY_POISON,
  EQUIP_FAIRY_TARGET,
  EQUIP_PET_SLIME,

  // spells
  EQUIP_SPELL_TELEPORT,
  EQUIP_SPELL_SLIME_EXPLODE,
  EQUIP_SPELL_FIREBALL,
  EQUIP_SPELL_FREEZE,
  EQUIP_SPELL_EARTHQUAKE,
  EQUIP_SPELL_PROTECTION,
  EQUIP_SPELL_WEB,
  EQUIP_SPELL_FLOWER,
  EQUIP_SPELL_FAIRY,

  // others
  EQUIP_BOSS_KEY,
  EQUIP_FLOOR_MAP,
  EQUIP_ALCOHOL,
  EQUIP_LUCK,
  EQUIP_FAIRY_POWDER
};

#endif
