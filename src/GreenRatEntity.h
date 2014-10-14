#ifndef GREENRATSPRITE_H
#define GREENRATSPRITE_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class GreenRatEntity : public EnemyEntity
{
  public:
    GreenRatEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    virtual bool canCollide();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void drop();
  private:
    float timer;
};

#endif // GREENRATSPRITE_H
