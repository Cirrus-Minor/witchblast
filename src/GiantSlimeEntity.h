#ifndef GIANTSLIMESPRITE_H
#define GIANTSLIMESPRITE_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class GiantSlimeEntity : public EnemyEntity
{
  public:
    GiantSlimeEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
    virtual bool canCollide();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();

    void changeToState(int n);

	  virtual void collideWithEnemy(EnemyEntity* entity) override;
	  virtual void dying();

	  virtual enumMovingStyle getMovingStyle();

  private:
    float timer;
    int state, counter;
    float slimeTimer;
    int slimeCounter;
    bool isFirstJumping;
    bool isFalling;
    float missileDelay;

    void fire();
};

#endif // GIANTSLIMESPRITE_H
