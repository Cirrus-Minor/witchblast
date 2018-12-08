#ifndef LARGESLIMESPRITE_H
#define LARGESLIMESPRITE_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"
#include "SlimeEntity.h"

class LargeSlimeEntity : public EnemyEntity
{
  public:
    LargeSlimeEntity(float x, float y, slimeTypeEnum slimeType);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRepulsionTo(BaseCreatureEntity* targetEntity) override;
    virtual bool canCollide();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();

    void changeToState(int n);

	  virtual void collideWithEnemy(EnemyEntity* entity) override;
	  virtual void dying();

	  virtual enumMovingStyle getMovingStyle();

  private:
    float timer;
    int state, counter;
    bool isFirstJumping;
    bool isFalling;
    float missileDelay;
    slimeTypeEnum slimeType;

    int fireSound;
    int fireDamage;
    enumShotType fireType;
    int baseFrame;
    void fire();
};

#endif // GIANTSLIMESPRITE_H
