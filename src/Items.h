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
  RarityRare,    /**< Rare */
  RarityVeryRare,/**< Very rare */
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
  FamiliarFairyStone,   /**< Stone Fairy */
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
  SpellTime,
  SpellLightning,

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
  "spell_fairy",
  "spell_time",
  "spell_lightning",
};

const int NUMBER_UNIDENTIFIED = 10;

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
  ItemScrollRevelation,
  // unidentified potions
  ItemPotion01,
  ItemPotion02,
  ItemPotion03,
  ItemPotion04,
  ItemPotion05,
  ItemPotion06,
  ItemPotion07,
  ItemPotion08,
  ItemPotion09,
  ItemPotion10,
  // effect
  ItemPotionHealth,
  ItemPotionPoison,
  ItemPotionSlow,
  ItemPotionSpeed,
  ItemPotionWeakness,
  ItemPotionStrength,
  ItemPotionOblivion,
  ItemPotionConfusion,
  ItemPotionCure,
  ItemPotionRage,

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
  ItemBag,
  ItemBookAlchemy,
  ItemFairyStone,
  ItemMerchant,
  ItemSpellTime,
  ItemSideShots,
  ItemSpellLightning,
  ItemBeltAdvanced,
  ItemRearShotAdvanced,
  ItemAmuletRetaliation,

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
  bool consumable;              /**< Consumable (potion / scroll) */
};

/** Array with all the items and data */
const itemStuct items[NUMBER_ITEMS] =
{
  {
    ItemCopperCoin, "item_copper", "item_copper_desc",
    1, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSilverCoin, "item_silver", "item_silver_desc",
    5, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemGoldCoin, "item_gold", "item_gold_desc",
    20, false, FamiliarNone, false, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBossHeart, "item_titan_heart", "item_titan_heart_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBonusHealth, "item_elven_cookie", "item_elven_cookie_desc",
    250, false, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemHealthVerySmall, "item_apple", "item_apple_desc",
    2, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemHealthSmall, "item_bread", "item_bread_desc",
    4, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemHealthSmall, "item_cheese", "item_cheese_desc",
    8, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemHealthVerySmallPoison, "item_apple_green", "item_apple_green_desc",
    2, false, FamiliarNone, true, false, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  // consumables
  {
    ItemScrollRevelation, "item_scroll_revelation", "item_scroll_revelation_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, true
  },
  // potions
  {
    ItemPotion01, "item_potion_01", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion02, "item_potion_02", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion03, "item_potion_03", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion04, "item_potion_04", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion05, "item_potion_05", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion06, "item_potion_06", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion07, "item_potion_07", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion08, "item_potion_08", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion09, "item_potion_09", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotion10, "item_potion_10", "item_potion_desc",
    10, false, FamiliarNone, true, true, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  // effect
  {
    ItemPotionHealth, "item_potion_health", "item_potion_health_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionPoison, "item_potion_poison", "item_potion_poison_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionSlow, "item_potion_slow", "item_potion_slow_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionSpeed, "item_potion_speed", "item_potion_speed_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionWeakness, "item_potion_weakness", "item_potion_weakness_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionStrength, "item_potion_strength", "item_potion_strength_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionOblivion, "item_potion_oblivion", "item_potion_oblivion_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionConfusion, "item_potion_confusion", "item_potion_confusion_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionCure, "item_potion_cure", "item_potion_cure_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },
  {
    ItemPotionRage, "item_potion_rage", "item_potion_rage_desc",
    10, false, FamiliarNone, false, false, false, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, true
  },

// EQUIP ITEMS //
  {
    ItemMagicianHat, "item_sorcerer_hat", "item_sorcerer_hat_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemLeatherBoots, "item_velvet_boots", "item_velvet_boots_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookDualShots, "item_spell_dual", "item_spell_dual_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRageAmulet, "item_rage_amulet", "item_rage_amulet_desc",
    35, true, FamiliarNone, true, true, true, 3, RarityRare, false, ItemAmuletRetaliation, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBossKey, "item_boss_key", "item_boss_key_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemDisplacementGloves, "item_gloves_advanced", "item_gloves_advanced_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemMahoganyStaff, "item_staff", "item_staff_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFairy, "item_fairy", "item_fairy_desc",
    20, true, FamiliarFairy, false, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemLeatherBelt, "item_leather_belt", "item_leather_belt_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBloodSnake, "item_blood_snake", "item_blood_snake_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemGemIce, "item_gem_ice", "item_gem_ice_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, -1, ShotTypeIce, SpellNone, false
  },
  {
    ItemGemIllusion, "item_gem_illusion", "item_gem_illusion_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityRare, false, -1, ShotTypeIllusion, SpellNone, false
  },
  {
    ItemGemStone, "item_gem_stone", "item_gem_stone_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, -1, ShotTypeStone, SpellNone, false
  },
  {
    ItemGemLightning, "item_gem_lightning", "item_gem_lightning_desc",
    25, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, -1, ShotTypeLightning, SpellNone, false
  },
  {
    ItemFairyTarget, "item_fairy_target", "item_fairy_target_desc",
    40, true, FamiliarFairyTarget, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRingIce, "item_ring_ice", "item_ring_ice_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemIce, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRingStone, "item_ring_stone", "item_ring_stone_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemStone, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRingLightning, "item_ring_lightning", "item_ring_lightning_desc",
    45, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemGemLightning, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRingIllusion, "item_ring_illusion", "item_ring_illusion_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemIllusion, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookTripleShots, "item_spell_triple", "item_spell_triple_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityCommon, false, ItemBookDualShots, ShotTypeStandard, SpellNone, false
  },
  {
    ItemCriticalAdvanced, "item_critical_advanced", "item_critical_advanced_desc",
    35, true, FamiliarNone, true, true, true, 1, RarityUncommon, false, ItemCritical, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFairyFire, "item_fairy_fire", "item_fairy_fire_desc",
    30, true, FamiliarFairyFire, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFairyIce, "item_fairy_ice", "item_fairy_ice_desc",
    30, true, FamiliarFairyIce, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemMagicianRobe, "item_robe_magician", "item_robe_magician_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemGemFire, "item_gem_fire", "item_gem_fire_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, -1, ShotTypeFire, SpellNone, false
  },
  {
    ItemRingFire, "item_ring_fire", "item_ring_fire_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemFire, ShotTypeStandard, SpellNone, false
  },
  {
    ItemManualStaff, "item_manual_staff", "item_manual_staff_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSpellTeleport, "item_spell_teleport", "item_spell_teleport_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellTeleport, false
  },
  {
    ItemSpellSlimeExplode, "item_spell_slime_explode", "item_spell_slime_explode_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellSlimeExplode, false
  },
  {
    ItemSpellFireball, "item_spell_fireball", "item_spell_fireball_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFireball, false
  },
  {
    ItemSpellFreeze, "item_spell_freeze", "item_spell_freeze_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFreeze, false
  },
  {
    ItemSpellEarthquake, "item_spell_earthquake", "item_spell_earthquake_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellEarthquake, false
  },
  {
    ItemSpellProtection, "item_spell_protection", "item_spell_protection_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellProtection, false
  },
  {
    ItemManualSlime, "item_manual_slime", "item_manual_slime_desc",
    30, true, FamiliarNone, true, true, true, 3, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookDualShotsQuick, "item_spell_dual_quick", "item_spell_dual_quick_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookTripleShotsQuick, "item_spell_triple_quick", "item_spell_triple_quick_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityCommon, false, ItemBookDualShotsQuick, ShotTypeStandard, SpellNone, false
  },
  {
    ItemManualHealth, "item_manual_health", "item_manual_health_desc",
    20, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRearShot, "item_rear_shot", "item_rear_shot_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookRandomShot, "item_book_random_shot", "item_book_random_shot_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSpellWeb, "item_spell_web", "item_spell_web_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellWeb, false
  },
  {
    ItemBookMagicI, "item_book_magic_i", "item_book_magic_i_desc",
    35, true, FamiliarNone, true, true, true, 4, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookMagicII, "item_book_magic_ii", "item_book_magic_ii_desc",
    35, true, FamiliarNone, true, true, true, 4, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSulfur, "item_sulfur", "item_sulfur_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFloorMap, "item_floor_map", "item_floor_map_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemAlcohol, "item_alcohol", "item_alcohol_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemLuck, "item_luck", "item_luck_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFairyPowder, "item_fairy_powder", "item_fairy_powder_desc",
    10, true, FamiliarNone, false, false, true, 1, RarityCommon, true, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookRapidShots, "item_rapid_shots", "item_rapid_shots_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemGemPoison, "item_gem_poison", "item_gem_poison_desc",
    30, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, -1, ShotTypePoison, SpellNone, false
  },
  {
    ItemSpellFlower, "item_spell_flower", "item_spell_flower_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFlower, false
  },
  {
    ItemFairyPoison, "item_fairy_poison", "item_fairy_poison_desc",
    30, true, FamiliarFairyPoison, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRingPoison, "item_ring_poison", "item_ring_poison_desc",
    50, true, FamiliarNone, true, true, true, 4, RarityUncommon, false, ItemGemPoison, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookPrayerI, "item_book_prayer_i", "item_book_prayer_i_desc",
    35, true, FamiliarNone, true, true, true, 3, RarityVeryRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookPrayerII, "item_book_prayer_ii", "item_book_prayer_ii_desc",
    35, true, FamiliarNone, true, true, true, 3, RarityVeryRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemPetSlime, "item_pet_slime", "item_pet_slime_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemHatAdvanced, "item_hat_advanced", "item_hat_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, ItemMagicianHat, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRobeAdvanced, "item_robe_advanced", "item_robe_advanced_desc",
    40, true, FamiliarNone, true, true, true, 3, RarityUncommon, false, ItemMagicianRobe, ShotTypeStandard, SpellNone, false
  },
  {
    ItemCritical, "item_critical", "item_critical_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSpellFairy, "item_spell_fairy", "item_spell_fairy_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellFairy, false
  },
  {
    ItemGlovesAdvanced, "item_displacement_gloves", "item_displacement_gloves_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, ItemDisplacementGloves, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBootsAdvanced, "item_boots_advanced", "item_boots_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, ItemLeatherBoots, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBag, "item_bag", "item_bag_desc",
    20, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemBookAlchemy, "item_book_alchemy", "item_book_alchemy_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemFairyStone, "item_fairy_stone", "item_fairy_stone_desc",
    30, true, FamiliarFairyStone, false, true, true, 2, RarityUncommon, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemMerchant, "item_merchant", "item_merchant_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSpellTime, "item_spell_time", "item_spell_time_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellTime, false
  },
  {
    ItemSideShots, "item_side_shots", "item_side_shots_desc",
    25, true, FamiliarNone, true, true, true, 1, RarityRare, false, -1, ShotTypeStandard, SpellNone, false
  },
  {
    ItemSpellLightning, "item_spell_lightning", "item_spell_lightning_desc",
    60, true, FamiliarNone, false, false, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellLightning, false
  },
  {
    ItemBeltAdvanced, "item_belt_advanced", "item_belt_advanced_desc",
    35, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, ItemLeatherBelt, ShotTypeStandard, SpellNone, false
  },
  {
    ItemRearShotAdvanced, "item_rear_shot_advanced", "item_rear_shot_advanced_desc",
    40, true, FamiliarNone, true, true, true, 2, RarityUncommon, false, ItemRearShot, ShotTypeStandard, SpellNone, false
  },
  {
    ItemAmuletRetaliation, "item_amulet_retaliation", "item_amulet_retaliation_desc",
    20, true, FamiliarNone, true, true, true, 1, RarityCommon, false, -1, ShotTypeStandard, SpellNone, false
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
  EQUIP_FAIRY_TARGET,
  EQUIP_RING_ICE,
  EQUIP_RING_STONE,
  EQUIP_RING_LIGHTNING,
  EQUIP_RING_ILLUSION,
  EQUIP_BOOK_TRIPLE,
  EQUIP_CRITICAL_ADVANCED,
  EQUIP_FAIRY_FIRE,
  EQUIP_FAIRY_ICE,
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
  EQUIP_BAG,
  EQUIP_BOOK_ALCHEMY,
  EQUIP_FAIRY_STONE,
  EQUIP_MERCHANT,
  EQUIP_SPELL_TIME,
  EQUIP_SIDE_SHOTS,
  EQUIP_SPELL_LIGHTNING,
  EQUIP_BELT_ADVANCED,
  EQUIP_REAR_SHOT_ADVANCED,
  EQUIP_AMULET_RETALIATION,

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
  EQUIP_BELT_ADVANCED,
  EQUIP_BAG,
  EQUIP_AMULET_RETALIATION,
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
  EQUIP_REAR_SHOT_ADVANCED,
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
  EQUIP_SIDE_SHOTS,
  EQUIP_BOOK_MAGIC_I,
  EQUIP_BOOK_MAGIC_II,
  EQUIP_BOOK_PRAYER_I,
  EQUIP_BOOK_PRAYER_II,
  EQUIP_BOOK_ALCHEMY,
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
  EQUIP_FAIRY_STONE,
  EQUIP_PET_SLIME,

  // others
  EQUIP_MERCHANT,

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
  EQUIP_SPELL_TIME,
  EQUIP_SPELL_LIGHTNING,

  // level items
  EQUIP_BOSS_KEY,
  EQUIP_FLOOR_MAP,
  EQUIP_ALCOHOL,
  EQUIP_LUCK,
  EQUIP_FAIRY_POWDER
};

const std::vector<item_equip_enum> scoreEquipement =
{
  // clothes
  EQUIP_BAG,
  EQUIP_RING_ICE,
  EQUIP_RING_STONE,
  EQUIP_RING_LIGHTNING,
  EQUIP_RING_FIRE,
  EQUIP_RING_ILLUSION,
  EQUIP_RING_POISON,

  // weapons
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
  EQUIP_SIDE_SHOTS,
  EQUIP_BOOK_MAGIC_I,
  EQUIP_BOOK_MAGIC_II,
  EQUIP_BOOK_PRAYER_I,
  EQUIP_BOOK_PRAYER_II,
  EQUIP_BOOK_ALCHEMY,
  EQUIP_BOOK_DUAL,
  EQUIP_BOOK_TRIPLE,
  EQUIP_BOOK_DUAL_QUICK,
  EQUIP_BOOK_TRIPLE_QUICK,
  EQUIP_RAPID_SHOT,

  // others
  EQUIP_MERCHANT,

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
  EQUIP_SPELL_TIME,
  EQUIP_SPELL_LIGHTNING,

  // level items
  EQUIP_BOSS_KEY,
  EQUIP_FLOOR_MAP,
  EQUIP_ALCOHOL,
  EQUIP_LUCK,
  EQUIP_FAIRY_POWDER
};

#endif
