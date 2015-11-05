#ifndef SLIMESPRITE_H
#define SLIMESPRITE_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

enum slimeTypeEnum { SlimeTypeStandard, SlimeTypeRed, SlimeTypeBlue, SlimeTypeViolet };

class SlimeEntity : public EnemyEntity
{
  public:
    SlimeEntity(float x, float y, slimeTypeEnum slimeType, bool invocated);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
	  virtual bool canCollide();
	  void makePet(int direction);
	  void setH(float h);
	  void disableCollidingTemporary();

  protected:
    virtual bool collideWithMap(int direction);
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void readCollidingEntity(CollidingSpriteEntity* entity);
    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void dying();
	  virtual void prepareDying();
	  virtual void drop();
	  virtual enumMovingStyle getMovingStyle();
	  virtual void makeExplode();

  private:
    float jumpingDelay;
    float noCollisionTimer;

    bool isJumping;
    bool isPet;
    bool isFirstJumping;
    bool invocated;
    slimeTypeEnum slimeType;

    void fire();
    void explode();
    bool willExplode;
};

#endif // SLIMESPRITE_H
