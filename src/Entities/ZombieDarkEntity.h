#ifndef ZOMBIEDARKENTITY_H
#define ZOMBIEDARKENTITY_H

#include "EnemyEntity.h"

class ZombieDarkEntity : public EnemyEntity
{
  public:
    ZombieDarkEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual bool isAttacking();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void collideWithBolt(BoltEntity* boltEntity);

  private:
    void findNextGoal();
    void findNextRandomGoal();
    int currentDirection;
    IntCoord currentTile;
    IntCoord targetTile;

    float attackTimer;
};

#endif // ZOMBIEDARKENTITY_H
