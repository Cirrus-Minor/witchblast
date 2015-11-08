#ifndef FRANCKYENTITY_H
#define FRANCKYENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class FranckyEntity : public EnemyEntity
{
  public:
    FranckyEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsRepulsionTo(BaseCreatureEntity* targetEntity) override;
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

	  virtual void collideWithEnemy(EnemyEntity* entity) override;
	  virtual void dying();
	  virtual void drop();

  private:
    float timer;
    float followTimer;
    int state;
    int counter;

    void fire();
};

///////////////////////////////////////////////////////////////////

class FranckyEntityHand : public EnemyEntity
{
  public:
    FranckyEntityHand(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void drop();
};

///////////////////////////////////////////////////////////////////

class FranckyEntityHead : public EnemyEntity
{
  public:
    FranckyEntityHead(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    virtual void collideWithEnemy(EnemyEntity* entity) override;

  private:
    float jumpingDelay;
    bool isJumping;
    bool isFirstJumping;

    void fire();
};

///////////////////////////////////////////////////////////////////

class FranckyEntityFoot : public EnemyEntity
{
  public:
    FranckyEntityFoot(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

  private:
    float jumpingDelay;

    bool isJumping;
    bool isFirstJumping;
};

#endif // FRANCKYENTITY_H
