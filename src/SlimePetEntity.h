#ifndef SLIMEPETSPRITE_H
#define SLIMEPETSPRITE_H

#include "EnemyEntity.h"


class SlimePetEntity : public BaseCreatureEntity
{
  public:
    SlimePetEntity();
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
	  virtual bool canCollide();

	  void changeRoom();

  protected:
    virtual bool collideWithMap(int direction);
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

	  virtual enumMovingStyle getMovingStyle();

  private:
    float jumpingDelay;
    float h, hVelocity;
    float attackDelay;

    bool isJumping;
    bool isFirstJumping;
    bool invocated;

    bool isFalling();
    void fall();
};

#endif // SLIMESPRITE_H
