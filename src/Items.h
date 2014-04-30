#ifndef ITEMS_H
#define ITEMS_H


enum enumItemType
{
  itemCopperCoin,
  itemSilverCoin,
  itemGoldCoin,
  itemHealth,

  itemMagicianHat,    // first equip item
  itemLeatherBoots,
  itemBookDualShots,
  itemRageAmulet,
  itemBossKey,
  itemVibrationGloves,
  itemMahoganyStaff,
  itemFairy,
  itemLeatherBelt,
  itemBloodSnake,
  itemIceGem
};

struct itemStuct
{
  enumItemType type;
  std::string name;
  std::string description;
  int price;
  bool equip;
};

const int NUMBER_ITEMS = 15;
const itemStuct items[NUMBER_ITEMS] =
{
  {
    itemCopperCoin, "Copper coin", "A copper coin (value 1)", 1, false
  },
  {
    itemSilverCoin, "Silver coin", "A silver coin (value 5)", 5, false
  },
  {
    itemGoldCoin, "Gold coin", "A gold coin (value 20)", 20, false
  },
  {
    itemHealth, "Health potion", "A health potion", 8, false
  },
  {
    itemMagicianHat, "Enchanter Hat", "Increases fire rate", 20, true
  },
  {
    itemLeatherBoots, "Leather Boots", "Increases Boots", 20, true
  },
  {
    itemBookDualShots, "Spell : Dual Bolts", "Shoots two bolts", 20, true
  },
  {
    itemRageAmulet, "Rage Amulet", "Increases fire range", 20, true
  },
  {
    itemBossKey, "Boss Key", "Open the Boss gate", 200, true
  },
  {
    itemVibrationGloves, "Vibration Gloves", "Increases bolt's speed and damages", 20, true
  },
  {
    itemMahoganyStaff, "Mahogany Staff", "Increases bolt's speed and damages", 20, true
  },
  {
    itemFairy, "Fairy", "Help you in the dungeon", 20, true
  },
  {
    itemLeatherBelt, "Leather Belt", "Increases fire rate", 20, true
  },
  {
    itemBloodSnake, "Blood Snake", "Increases damages", 25, true
  },
  {
    itemIceGem, "Ice Gem", "Ice attack", 25, true
  }
};

// Equip items
const int NUMBER_EQUIP_ITEMS = 11;
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
