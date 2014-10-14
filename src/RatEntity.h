#ifndef RATENTITY_H
#define RATENTITY_H

#include "EnemyEntity.h"

class RatEntity : public EnemyEntity
{
  public:
    enum ratTypeEnum { RatTypeNormal, RatTypeHelmet };
    RatEntity(float x, float y, ratTypeEnum ratType, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void collideWithBolt(BoltEntity* boltEntity);
    virtual void drop();
  private:
    ratTypeEnum ratType;
    bool invocated;
    int direction;
    bool clockTurn;

    float timer;

    void compute(bool turn);
};

#endif // RATENTITY_H
