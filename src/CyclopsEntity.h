#ifndef CYCLOPENTITY_H
#define CYCLOPENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class CyclopsEntity : public EnemyEntity
{
  public:
    CyclopsEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRepulsionTo(BaseCreatureEntity* targetEntity) override;
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();
    virtual int hurt(StructHurt hurtParam) override;

	  virtual void collideWithEnemy(EnemyEntity* entity) override;
	  virtual void drop();

	  void computeStates(float delay);
	  int getHealthLevel();
  private:
    float timer;
    int state;
    int counter;
    int destroyLevel;

    int nextRockMissile;
    void computeNextRockMissile();

    void fire();
    void fallRock();
    void initFallingGrid();
    bool fallingGrid[MAP_WIDTH][MAP_HEIGHT];
};

#endif // CYCLOPENTITY_H
