#ifndef MAGNETENTITY_H
#define MAGNETENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "PlayerEntity.h"

class MagnetEntity : public SpriteEntity
{
  public:
    MagnetEntity(float x, float y, PlayerEntity* parentEntity, enumItemType );
    virtual void animate(float delay);

  protected:

  private:
    PlayerEntity* parentEntity;
};
#endif // MAGNETENTITY_H
