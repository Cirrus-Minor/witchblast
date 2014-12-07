#ifndef GIANTSPIDERENTITY_H
#define GIANTSPIDERENTITY_H

#include "EnemyEntity.h"

class GiantSpiderEntity : public EnemyEntity
{
  public:
    GiantSpiderEntity(float x, float y);
    virtual void animate(float delay);
    virtual void calculateBB();
    virtual void render(sf::RenderTarget* app);
    virtual bool canCollide();

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;
    virtual int hurt(StructHurt hurtParam) override;
    virtual void drop();

    int getHealthLevel();
  private:
    int state;
    bool labelHasChanged;
    float timer;
    float fireDelay;
    float walkingSoundDelay;

    int hurtLevel;

    void calculateRotation();
    void fire(int type);

    std::string creatureName;
};

#endif // GIANTSPIDERENTITY_H
