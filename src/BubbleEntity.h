#ifndef BUBBLEENTITY_H
#define BUBBLEENTITY_H

#include "EnemyEntity.h"

class BubbleEntity : public EnemyEntity
{
  public:
    BubbleEntity(float x, float y, int bubbleSize);
    virtual void calculateBB();
    int getBubbleSize();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void dying();
    virtual void render(sf::RenderTarget* app);

    virtual void collideWithEnnemy(GameEntity* collidingEntity);
  private:
    int bubbleSize;
};

#endif // BUBBLEENTITY_H
