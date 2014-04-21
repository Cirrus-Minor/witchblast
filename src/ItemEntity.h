#ifndef ITEMENTITY_H
#define ITEMENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"

class WitchBlastGame;

class ItemEntity : public CollidingSpriteEntity
{
  public:
    enum enumItemType
    {
      itemCopperCoin,
      itemSilverCoin,
      itemGoldCoin,
      itemHealth,

      itemMagicianHat,
      itemLeatherBoots,
      itemBookDualShots,
      itemConcentrationAmulet,
      itemBossKey,
      itemVibrationGloves,
      itemMahoganyStaff,
      itemFairy,
      itemLeatherBelt,
      itemBloodSnake
    };



    void setMerchandise(bool isMerchandise);
    bool getMerchandise();
    int getPrice();
    /*ItemEntity(sf::Texture* image, float x, float y, int itemType, int spriteWidth, int spriteHeight);*/
    ItemEntity(enumItemType itemType, float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
    virtual void calculateBB();
    virtual void dying();
    void setParent(WitchBlastGame* parent);
    enumItemType getItemType() { return itemType; };

  protected:
    WitchBlastGame* parentGame;
    enumItemType itemType;

    bool isMerchandise;

    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

  private:
};

#endif // ITEMENTITY_H
