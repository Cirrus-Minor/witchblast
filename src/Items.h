#ifndef ITEMS_H
#define ITEMS_H


enum enumItemType
{
  itemCopperCoin,
  itemSilverCoin,
  itemGoldCoin,
  itemHealth,

  itemMagicianHat,
  itemLeatherBoots,
  itemBookDualShots,
  itemRageAmulet,
  itemBossKey,
  itemVibrationGloves,
  itemMahoganyStaff,
  itemFairy,
  itemLeatherBelt,
  itemBloodSnake
};

struct itemStuct
{
  enumItemType type;
  std::string name;
  std::string description;
  int price;
  bool equip;
};


const int NB_ITEMS = 14;
const itemStuct items[NB_ITEMS] =
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
    itemBloodSnake, "Blood Snake", "Increases damages", 20, true
  }
};

#endif
