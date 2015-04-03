#ifndef BOGEYSPRITE_H
#define BOGEYSPRITE_H

#include "EnemyEntity.h"

class BogeymanEntity : public EnemyEntity
{
  public:
    BogeymanEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    virtual bool canCollide();
    virtual void prepareDying() override;

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual int hurt(StructHurt hurtParam) override;

	  virtual void dying();
  private:
    float changingDelay;
    int state;
    void fire();
    bool hasFired;
    bool isFireType1;

    void teleport();
};

#endif // BOGEYSPRITE_H
