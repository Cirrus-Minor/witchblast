#ifndef BASECREATUREENTITY_H
#define BASECREATUREENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"

class WitchBlastGame;

class BaseCreatureEntity : public CollidingSpriteEntity
{
  public:
    BaseCreatureEntity(sf::Texture* image, WitchBlastGame* parent, float x, float y, int spriteWidth, int spriteHeight);
    int getHp();
    int getHpMax();
    void setHp(int hp);
    void setHpMax(int hpMax);
    int getHpDisplay();
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
    virtual void calculateBB();
    virtual bool hurt(int damages, enumBoltType hurtingType);
    virtual void dying();
    enum enumBloodColor { bloodRed, bloodGreen};
  protected:
    int hp;
    int hpMax;
    int hpDisplay;
    float creatureSpeed;
    int shadowFrame;

    bool hurting;
    float hurtingDelay;
    enumBoltType hurtingType;
    WitchBlastGame* parentGame;
    enumBloodColor bloodColor;
  private:
};

#endif // BASECREATUREENTITY_H
