#ifndef PLAYERSPRITE_H
#define PLAYERSPRITE_H

#include "BaseCreatureEntity.h"
#include "ItemEntity.h"
#include "Constants.h"

class FairyEntity;

class PlayerEntity : public BaseCreatureEntity
{
  public:
    PlayerEntity(sf::Texture* image, float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);

    void moveTo(float newX, float newY);

    virtual void calculateBB();
    void move(int direction);
    void fire(int direction);
    bool canFire();
    bool isMoving();
    bool isEquiped(int eq);
    void setEntering();
    bool canMove();
    virtual void dying();
    virtual void hurt(int damages);
    bool isDead();
    float getPercentFireDelay();
    void acquireItem(ItemEntity::enumItemType type);
    void loseItem(ItemEntity::enumItemType itemType, bool isEquip);
    void acquireStance(ItemEntity::enumItemType type);
    void useBossKey();

    int getGold() {return gold; }
    void pay(int price);
    int getColliding();

    enum playerStatusEnum
    {
      playerStatusPlaying,
      playerStatusEntering,
      playerStatusAcquire,
      playerStatusUnlocking,
      playerStatusDead
    };
  protected:
    void computePlayer();
    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    void generateBolt(float velx, float vely);

    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

  private:
    //int hp;
    //int hpMax;
    int fireDamages;
    float fireVelocity;
    float fireDelay;
    float currentFireDelay;
    float boltLifeTime;
    int gold;

 //   float hurtingDelay;
 //   float currentHurtingDelay;

    bool canFirePlayer;
    //float playerSpeed;
    playerStatusEnum playerStatus;
    float acquireDelay;
    ItemEntity::enumItemType acquiredItem;

    bool equip[NUMBER_EQUIP_ITEMS];

    int colliding;

    FairyEntity* fairy;
};

#endif // PLAYERSPRITE_H
