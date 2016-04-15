#ifndef EVILFLOWER_H
#define EVILFLOWER_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

enum flowerTypeEnum { FlowerTypeStandard, FlowerTypeIce, FlowerTypeFire, FlowerTypePet };

class EvilFlowerEntity : public EnemyEntity
{
  public:
    EvilFlowerEntity(float x, float y, flowerTypeEnum flowerType);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    void fire();
    virtual bool canCollide();
    void setFireDelayMax(float fireDelayMax);


  private:
    float fireDelay;
    float fireDelayMax;
    flowerTypeEnum flowerType;
    float ageFromGen;
};

#endif // EVILFLOWER_H
