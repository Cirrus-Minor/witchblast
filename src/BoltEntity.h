#ifndef BOLTENTITY_H
#define BOLTENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"

class BoltEntity : public CollidingSpriteEntity
{
  public:
    BoltEntity(sf::Texture* image, float x, float y, float boltLifeTime);
    virtual void animate(float delay);
    void collide();
    void generateParticule(Vector2D vel);

    int getDamages();
    void setDamages(int damages);

  protected:
    virtual void collideMapRight();
    virtual void collideMapLeft();
    virtual void collideMapTop();
    virtual void collideMapBottom();

    int damages;
    float renderScale;
  private:
};

#endif // BOLTENTITY_H
