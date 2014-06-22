#ifndef BASECREATUREENTITY_H
#define BASECREATUREENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "Constants.h"

const int NB_SPECIAL_STATES = 1;

const int NB_RESISTANCES = 6;

class BaseCreatureEntity : public CollidingSpriteEntity
{
  public:
    BaseCreatureEntity(sf::Texture* image, float x, float y, int spriteWidth, int spriteHeight);
    int getHp();
    int getHpMax();
    void setHp(int hp);
    void setHpMax(int hpMax);
    int getHpDisplay();

    IntCoord getCurrentTile();

    virtual void animate(float delay);
    virtual float animateStates(float delay);
    virtual void animateColors(float delay);
    virtual void animateRecoil(float delay);
    virtual void animatePhysics(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual bool collideWithMap(int direction);
    virtual bool hurt(int damages, enumShotType hurtingType, int level);
    virtual void prepareDying();
    virtual void dying();
    enum enumMovingStyle { movWalking, movFlying};
    virtual enumMovingStyle getMovingStyle();
    enum enumBloodColor { bloodRed, bloodGreen};
    enum enumSpecialState
    {
      SpecialStateIce // = 0
    };
    enum enumStateResistance { ResistanceImmune, ResistanceVeryHigh, ResistanceHigh, ResistanceStandard, ResistanceLow, ResistanceVeryLow};
    struct specialStateStuct
    {
      enumSpecialState type;
      bool active;
      float timer;
      float parameter;
    };
    specialStateStuct specialState[NB_SPECIAL_STATES];

    enum enumResistances
    {
      ResistanceIce, // = 0
      ResistanceFrozen,
      ResistanceStone,
      ResistanceRecoil,
      ResistanceLightning,
      ResistanceIllusion
    };
    enumStateResistance resistance[NB_RESISTANCES];

    virtual void giveRecoil(bool stun, Vector2D velocity, float timer);
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
    virtual void computeFacingDirection();

  protected:
    int hp;
    int hpMax;
    int hpDisplay;
    float creatureSpeed;
    int shadowFrame;
    int facingDirection;

    bool hurting;
    float hurtingDelay;
    enumShotType hurtingType;
    enumBloodColor bloodColor;
    enumMovingStyle movingStyle;

    struct  recoilStruct
    {
      bool active;
      Vector2D velocity;
      bool stun;
      float timer;
    } recoil;
  private:
    bool determineSatusChance(enumStateResistance resistance, int level);
};

#endif // BASECREATUREENTITY_H
