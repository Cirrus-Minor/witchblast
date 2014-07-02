#ifndef CYCLOPENTITY_H
#define CYCLOPENTITY_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class CyclopEntity : public EnnemyEntity
{
  public:
    CyclopEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();
    virtual bool hurt(int damages, enumShotType hurtingType, int level);

	  virtual void collideWithEnnemy(GameEntity* collidingEntity);
	  virtual void dying();

	  void computeStates(float delay);
	  int getHealthLevel();
  private:
    float timer;
    int state;
    int counter;

    void fire();
};

#endif // CYCLOPENTITY_H
