#ifndef BLACKRATSPRITE_H
#define BLACKRATSPRITE_H

#include "EnemyEntity.h"

class BlackRatEntity : public EnemyEntity
{
  public:
    enum ratBlackTypeEnum { RatBlackTypeNormal, RatBlackTypeHelmet };
    BlackRatEntity(float x, float y, ratBlackTypeEnum ratType);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void collideWithBolt(BoltEntity* boltEntity);

  private:
    void findNextGoal();
    int currentDirection;
    IntCoord currentTile;
    IntCoord targetTile;

    ratBlackTypeEnum ratType;
};

#endif // BLACKRATSPRITE_H
