#ifndef EVILFLOWER_H
#define EVILFLOWER_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

enum flowerTypeEnum { FlowerTypeStandard, FlowerTypeIce };

class EvilFlowerEntity : public EnemyEntity
{
  public:
    EvilFlowerEntity(float x, float y, flowerTypeEnum flowerType);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    void fire();

  private:
    float fireDelay;
    flowerTypeEnum flowerType;
};

#endif // EVILFLOWER_H
