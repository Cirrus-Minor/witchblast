#ifndef BATENTITY_H
#define BATENTITY_H

#include "EnemyEntity.h"

enum EnumBatType { BatStandard, BatSkeleton};

class BatEntity : public EnemyEntity
{
  public:
    BatEntity(float x, float y, EnumBatType batType, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void dying();
	  virtual void drop();
  private:
    float changingDelay;
    bool invocated;
    EnumBatType batType;
};

#endif // BATENTITY_H
