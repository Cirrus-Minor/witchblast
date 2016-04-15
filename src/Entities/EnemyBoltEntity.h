#ifndef ENNEMYBOLTENTITY_H
#define ENNEMYBOLTENTITY_H

#include "BoltEntity.h"


class EnemyBoltEntity : public BoltEntity
{
  public:
    EnemyBoltEntity(float x, float y, enumShotType boltType, int level, enemyTypeEnum enemyType);
    virtual void animate(float delay);
};

#endif // ENNEMYBOLTENTITY_H
