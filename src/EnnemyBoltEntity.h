#ifndef ENNEMYBOLTENTITY_H
#define ENNEMYBOLTENTITY_H

#include "BoltEntity.h"


class EnnemyBoltEntity : public BoltEntity
{
  public:
    EnnemyBoltEntity(float x, float y, enumShotType boltType, int level);
    virtual void animate(float delay);
};

#endif // ENNEMYBOLTENTITY_H
