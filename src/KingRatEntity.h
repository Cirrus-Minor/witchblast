#ifndef KINGRATSPRITE_H
#define KINGRATSPRITE_H

#include "EnnemyEntity.h"
#include "PlayerEntity.h"

class KingRatEntity : public EnnemyEntity
{
  public:
    KingRatEntity(float x, float y);
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
    virtual void calculateBB();
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();
    void afterWallCollide();
    virtual bool hurt(int damages);

    void generateGreenRats();

	  //virtual void readCollidingEntity(CollidingSpriteEntity* entity);
	  virtual void dying();
  private:
    float timer;
    float berserkDelay;
    int state;
    bool hasBeenBerserk;
};

#endif // KINGRATSPRITE_H
