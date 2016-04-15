#ifndef WITCHENTITY_H
#define WITCHENTITY_H

#include "EnemyEntity.h"

class WitchEntity : public EnemyEntity
{
  public:
    enum witchTypeEnum { WitchTypeNormal, WitchTypeRed };
    WitchEntity(float x, float y, witchTypeEnum witchType);
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
    witchTypeEnum witchType;
    float timer;
    float escapeTimer;

    void fire();
    int state;
};

#endif // WITCHENTITY_H
