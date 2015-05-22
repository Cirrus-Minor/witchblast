#ifndef OBSTACLEENTITY_H
#define OBSTACLEENTITY_H

#include "EnemyEntity.h"

class ObstacleEntity : public EnemyEntity
{
  public:
    ObstacleEntity(float x, float y, int objectFrame);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

    virtual void render(sf::RenderTarget* app);
    virtual void dying();

  protected:
	  virtual void drop();

  private:
    int xGrid;
    int yGrid;
    int initialFrame;

    void correctFrame();
};

#endif // OBSTACLEENTITY_H
