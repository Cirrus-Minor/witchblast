#ifndef PUMPKINENTITY_H
#define PUMPKINENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class PumpkinEntity : public EnemyEntity
{
  public:
    PumpkinEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void dying();
	  virtual void prepareDying();

  private:
    float jumpingDelay;

    bool isJumping;
    bool isPet;
    bool isFirstJumping;
    bool invocated;
};

#endif // SLIMESPRITE_H
