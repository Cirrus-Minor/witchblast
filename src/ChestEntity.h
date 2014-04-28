#ifndef CHESTENTITY_H
#define CHESTENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"

class WitchBlastGame;

class ChestEntity : public CollidingSpriteEntity
{
  public:
    ChestEntity(float x, float y, int chestType, bool isOpen, WitchBlastGame* parent);
    virtual void animate(float delay);
    virtual void calculateBB();

    void open();
    bool getOpened();
    int getChestType();

  protected:

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

  private:
    bool isOpen;
    int chestType;
    WitchBlastGame* parentGame;
};

#endif // CHESTENTITY_H
