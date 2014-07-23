#ifndef GIANTSPIDERENTITY_H
#define GIANTSPIDERENTITY_H

#include "EnnemyEntity.h"

class GiantSpiderEntity : public EnnemyEntity
{
  public:
    GiantSpiderEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnnemy(GameEntity* collidingEntity);
  private:
    int state;
    float timer;

    void calculateRotation();
};

#endif // GIANTSPIDERENTITY_H
