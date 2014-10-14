#ifndef IMPSPRITE_H
#define IMPSPRITE_H

#include "EnemyEntity.h"

class ImpEntity : public EnemyEntity
{
  public:
    enum impTypeEnum { ImpTypeRed, ImpTypeBlue };
    ImpEntity(float x, float y, impTypeEnum impType);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual int hurt(int damages, enumShotType hurtingType, int level, bool critical, int sourceType) override;

	  virtual void dying();
  private:
    impTypeEnum impType;
    float changingDelay;
    int state;
    void fire();
    void teleport();
};

#endif // BATSPRITE_H
