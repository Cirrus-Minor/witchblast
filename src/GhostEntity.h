#ifndef GHOSTENTITY_H
#define GHOSTENTITY_H

#include "EnemyEntity.h"

class GhostEntity : public EnemyEntity
{
  public:
    GhostEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void collideWithBolt(BoltEntity* boltEntity);
    virtual void drop();
  private:
    float timer;

    int getGhostFade();
};

#endif // GHOSTENTITY_H
