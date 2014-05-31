#ifndef GIANTSLIMESPRITE_H
#define GIANTSLIMESPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class GiantSlimeEntity : public EnnemyEntity
{
  public:
    GiantSlimeEntity(float x, float y);
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
    virtual bool hurt(int damages);

    void changeToState(int n);

	  virtual void collideWithEnnemy(GameEntity* collidingEntity);
	  virtual void dying();

	  virtual bool canCollide();
	  virtual enumMovingStyle getMovingStyle();

  private:
    float timer;
    int state, counter;
    float slimeTimer;
    int slimeCounter;
    float h;
    float hVelocity;
    bool isFirstJumping;

    void fire();
};

#endif // GIANTSLIMESPRITE_H
