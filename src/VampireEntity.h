#ifndef VAMPIREENTITY_H
#define VAMPIREENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class VampireEntity : public EnemyEntity
{
  public:
    VampireEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);

  protected:
	  virtual void drop();
	  virtual int hurt(StructHurt hurtParam) override;

	  void computeStates(float delay);
	  int getHealthLevel();
	  virtual void prepareDying() override;

  private:
    float timer;
    float batTimer;
    int state;
    int targetPos;
    float xSource, ySource;

    void calculatePosition();
};

#endif // VAMPIREENTITY_H
