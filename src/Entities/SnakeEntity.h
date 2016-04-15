#ifndef SNAKEENTITY_H
#define SNAKEENTITY_H

#include "EnemyEntity.h"

class SnakeEntity : public EnemyEntity
{
  public:
    enum snakeTypeEnum { SnakeTypeNormal, SnakeTypeBlood };
    SnakeEntity(float x, float y, snakeTypeEnum snakeType, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual void drop();
  private:
    snakeTypeEnum snakeType;
    bool invocated;
    float timer;
};

#endif // SNAKEENTITY_H
