#ifndef ITEMENTITY_H
#define ITEMENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Items.h"

class ItemEntity : public CollidingSpriteEntity
{
  public:
    ItemEntity(enumItemType itemType, float x, float y);
    void setMerchandise(bool isMerchandise);
    bool getMerchandise();
    int getPrice();
    bool canBePickedUp();
    void buy();

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void dying();
    enumItemType getItemType() { return itemType; };

    void startsJumping();

  protected:
    enumItemType itemType;

    bool isMerchandise;

    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    bool isOnMap();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);

  private:
    float timer;
    float jumpTimer;
    bool isBeating;
    bool isFlying;
    bool firstJump;

    float h, hVelocity;
};

#endif // ITEMENTITY_H
