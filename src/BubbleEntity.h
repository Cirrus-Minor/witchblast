#ifndef BUBBLEENTITY_H
#define BUBBLEENTITY_H

#include "EnemyEntity.h"

enum EnumBubbleType { BubbleStandard, BubbleIce, BubbleTriple };

class BubbleEntity : public EnemyEntity
{
  public:
    BubbleEntity(float x, float y, EnumBubbleType bubbleType, int bubbleSize);
    virtual void calculateBB();
    int getBubbleSize();

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void dying();
    virtual void collideWithEnemy(EnemyEntity* entity) override;
  private:
    int bubbleSize;
    EnumBubbleType bubbleType;
};

#endif // BUBBLEENTITY_H
