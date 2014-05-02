#ifndef BASECREATUREENTITY_H
#define BASECREATUREENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"

const int NB_SPECIAL_STATES = 1;

class BaseCreatureEntity : public CollidingSpriteEntity
{
  public:
    BaseCreatureEntity(sf::Texture* image, float x, float y, int spriteWidth, int spriteHeight);
    int getHp();
    int getHpMax();
    void setHp(int hp);
    void setHpMax(int hpMax);
    int getHpDisplay();
    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
    virtual void calculateBB();
    virtual bool collideWithMap(int direction);
    virtual bool hurt(int damages, enumShotType hurtingType);
    virtual void prepareDying();
    virtual void dying();
    enum enumMovingStyle { movWalking, movFlying};
    enum enumBloodColor { bloodRed, bloodGreen};
    enum enumSpecialState
    {
      SpecialStateIce // = 0
    };
    enum enumStateResistance { ResistanceImmune, ResistanceResistant, ResistanceStandard, ResistanceLow, ResistanceVeryLow};
    struct itemStuct
    {
      enumSpecialState type;
      enumStateResistance resistance;
      bool active;
      float timer;
    };
    itemStuct specialState[NB_SPECIAL_STATES];

  protected:
    int hp;
    int hpMax;
    int hpDisplay;
    float creatureSpeed;
    int shadowFrame;

    bool hurting;
    float hurtingDelay;
    enumShotType hurtingType;
    enumBloodColor bloodColor;
    enumMovingStyle movingStyle;
  private:
};

#endif // BASECREATUREENTITY_H
