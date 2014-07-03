#ifndef SLIMESPRITE_H
#define SLIMESPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

enum slimeTypeEnum { SlimeTypeStandard, SlimeTypeRed, SlimeTypeBlue };

class SlimeEntity : public EnnemyEntity
{
  public:
    SlimeEntity(float x, float y, slimeTypeEnum slimeType, bool invocated);
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

    bool isJumping;
    bool isFirstJumping;
    bool invocated;
    slimeTypeEnum slimeType;

    void fire();
};

#endif // SLIMESPRITE_H
