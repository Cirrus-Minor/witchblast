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
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();
    virtual int hurt(int damages, enumShotType hurtingType, int level, bool critical, int sourceType) override;

    void generateGreenRats();

	  virtual void collideWithEnnemy(GameEntity* collidingEntity);
	  virtual void drop();
  private:
    float timer;
    float berserkDelay;
    int state;
    bool hasBeenBerserk;
};

#endif // KINGRATSPRITE_H
