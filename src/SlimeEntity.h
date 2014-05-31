#ifndef SLIMESPRITE_H
#define SLIMESPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class SlimeEntity : public EnnemyEntity
{
  public:
    SlimeEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
  protected:
    virtual bool collideWithMap(int direction);
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);

	  virtual void dying();
	  virtual void prepareDying();
	  virtual bool canCollide();

	  virtual enumMovingStyle getMovingStyle();

  private:
    float jumpingDelay;
    float h;
    float hVelocity;

    bool isJumping;
    bool isFirstJumping;
    bool invocated;
};

#endif // SLIMESPRITE_H
