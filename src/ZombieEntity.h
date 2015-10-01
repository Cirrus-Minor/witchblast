#ifndef ZOMBIEENTITY_H
#define ZOMBIEENTITY_H

#include "EnemyEntity.h"

class ZombieEntity : public EnemyEntity
{
  public:
    ZombieEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual bool isAttacking();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void drop();

  private:
    bool invocated;
    int direction;
    bool clockTurn;

    float timer;
    float attackTimer;

    void compute(bool turn);
    bool attack();
};

#endif // ZOMBIEENTITY_H
