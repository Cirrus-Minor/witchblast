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
    int formState;
    sf::Sprite batSprite;
    int bodyFrame;

    int numberOfRays;
    int raySpeedFactor;
    int moveCounter;

    void renderRays(sf::RenderTarget* app, bool isGhost);
    void testRaysCollision();

    void calculatePosition();
    void generateBats(int batFrame);
};

class VampireDeadEntity : public EnemyEntity
{
  public:
    VampireDeadEntity(float x, float y);
    virtual void calculateBB();
    virtual void animate(float delay);
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);

  protected:
	  virtual void drop();
};

#endif // VAMPIREENTITY_H
