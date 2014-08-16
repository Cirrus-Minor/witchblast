#ifndef BUTCHERENTITY_H
#define BUTCHERENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class ButcherEntity : public EnemyEntity
{
  public:
    ButcherEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    virtual bool hurt(int damages, enumShotType hurtingType, int level, bool critical) override;

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void drop();
  private:
    float timer;
};

#endif // BUTCHERENTITY_H
