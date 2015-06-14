#ifndef SAUSAGEENTITY_H
#define SAUSAGEENTITY_H

#include "EnemyEntity.h"

class SausageEntity : public EnemyEntity
{
  public:
    SausageEntity(float x, float y, bool invocated);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity) override;
    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void dying();
	  virtual void drop();

  private:
    bool invocated;
    float timer;
    int state;
};

#endif // SAUSAGEENTITY_H
