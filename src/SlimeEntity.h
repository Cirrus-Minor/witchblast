#ifndef SLIMESPRITE_H
#define SLIMESPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class SlimeEntity : public EnnemyEntity
{
  public:
    SlimeEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
    virtual void calculateBB();
  protected:
    virtual bool collideWithMap(int direction);
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  virtual void dying();
	  virtual bool canCollide();
	  virtual bool hurt(int damages);
  private:
    float jumpingDelay;
    float h;
    float hVelocity;

    bool isJumping;
    bool isFirstJumping;
};

#endif // SLIMESPRITE_H
