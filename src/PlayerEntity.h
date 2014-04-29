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
    void setEquiped(int item, bool eq);
    void setEntering();
    bool canMove();
    virtual void dying();
    virtual bool hurt(int damages); // return true if hurted
    bool isDead();
    float getPercentFireDelay();
    void acquireItem(enumItemType type);
    void loseItem(enumItemType itemType, bool isEquip);
    void acquireStance(enumItemType type);
    void useBossKey();

    int getGold() {return gold; }
    void setGold(int gold) { this->gold = gold; }
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
    int fireDamages;
    float fireVelocity;
    float fireDelay;
    float currentFireDelay;
    float boltLifeTime;
    int gold;

    bool canFirePlayer;
    playerStatusEnum playerStatus;
    float acquireDelay;
    enumItemType acquiredItem;

    bool equip[NUMBER_EQUIP_ITEMS];
    enumBoltType boltType;
    float specialBoltTimer;

    int colliding;

    int facingDirection;
    int firingDirection;

    FairyEntity* fairy;
};

#endif // PLAYERSPRITE_H
