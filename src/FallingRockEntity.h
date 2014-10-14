#ifndef FALLINGROCKENTITY_H
#define FALLINGROCKENTITY_H

#include "EnemyEntity.h"

class FallingRockEntity : public EnemyEntity
{
  public:
    FallingRockEntity(float x, float y, int rockType, bool hurtAll);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    virtual bool canCollide();

  protected:
	  virtual void dying();
	  virtual void collideWithEnemy(EnemyEntity* entity) override;

  private:
    int rockType;
    int h;
    bool jumping;
    float hVelocity;
    bool hurtAll;
    bool hasHurted;
};

#endif // FALLINGROCKENTITY_H
