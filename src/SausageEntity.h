#ifndef SAUSAGEENTITY_H
#define SAUSAGEENTITY_H

#include "EnemyEntity.h"

class SausageEntity : public EnemyEntity
{
  public:
    SausageEntity(float x, float y, bool invocated);
    virtual void animate(float delay) override;
    virtual void render(sf::RenderTarget* app) override;
    virtual void calculateBB() override;
    virtual bool canCollide() override;

  protected:
    virtual void collideMapRight() override;
    virtual void collideMapLeft() override;
    virtual void collideMapTop() override;
    virtual void collideMapBottom() override;

    virtual void readCollidingEntity(CollidingSpriteEntity* entity) override;
    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void dying() override;
	  virtual void drop() override;

  private:
    bool invocated;
    bool isNew;
    float timer;
    int state;
};

#endif // SAUSAGEENTITY_H
