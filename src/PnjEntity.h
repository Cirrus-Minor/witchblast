#ifndef PNJENTITY_H
#define PNJENTITY_H

#include "sfml_game/SpriteEntity.h"

class PnjEntity : public SpriteEntity
{
  public:
    PnjEntity(float x, float y, int pnjType );
    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);

  protected:
    float x0, y0;
    float xGoal;
    int direction;
    float pnjVelocity;

    float speechTimer;
    bool isSpeaking;
    std::string speech;

  private:
};
#endif // PNJENTITY_H
