#ifndef LITTLESPIDERENTITY_H
#define LITTLESPIDERENTITY_H

#include "EnemyEntity.h"
#include "PlayerEntity.h"

enum EnumSpiderType { SpiderTypeStandard, SpiderTypeTarantula };

class LittleSpiderEntity : public EnemyEntity
{
  public:
    LittleSpiderEntity(float x, float y, EnumSpiderType spideType, bool invocated);
    virtual void animate(float delay);
    virtual void calculateBB();
  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    virtual void collideWithEnemy(EnemyEntity* entity) override;

	  virtual void drop();
  private:
    float timer;
    float webTimer;
    bool roaming;
    EnumSpiderType spideType;
};

#endif // LITTLESPIDERENTITY_H
