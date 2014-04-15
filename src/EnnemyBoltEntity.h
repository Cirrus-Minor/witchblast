#ifndef ENNEMYBOLTENTITY_H
#define ENNEMYBOLTENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"

class EnnemyBoltEntity : public CollidingSpriteEntity
{
  public:
    EnnemyBoltEntity(sf::Texture* image, float x, float y);
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
  private:
};

#endif // ENNEMYBOLTENTITY_H
