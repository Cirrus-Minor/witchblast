#ifndef KINGRATSPRITE_H
#define KINGRATSPRITE_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

class KingRatEntity : public EnemyEntity
{
  public:
    KingRatEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual void inflictsrepulsionTo(BaseCreatureEntity* targetEntity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();
    virtual int hurt(StructHurt hurtParam) override;

    void generateGreenRats();

	  virtual void collideWithEnemy(EnemyEntity* entity) override;
	  virtual void drop();
  private:
    float timer;
    float berserkDelay;
    int state;
    bool hasBeenBerserk;
};

#endif // KINGRATSPRITE_H
