#ifndef BASECREATUREENTITY_H
#define BASECREATUREENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "TextEntity.h"
#include "Constants.h"

enum enemyTypeEnum
{
  // normal
  EnemyTypeBat,
  EnemyTypeRat,
  EnemyTypeRatBlack,
  EnemyTypeRatHelmet,
  EnemyTypeRatBlackHelmet,
  EnemyTypeEvilFlower,
  EnemyTypeEvilFlowerIce,
  EnemyTypeEvilFlowerFire,
  EnemyTypeSnake,
  EnemyTypeSnakeBlood,
  EnemyTypeSlime,
  EnemyTypeSlimeRed,
  EnemyTypeSlimeBlue,
  EnemyTypeSlimeViolet,
  EnemyTypeImpBlue,
  EnemyTypeImpRed,
  EnemyTypePumpkin,
  EnemyTypeWitch,
  EnemyTypeWitchRed,
  EnemyTypeCauldron,
  EnemyTypeSpiderEgg,
  EnemyTypeSpiderLittle,
  EnemyTypeGhost,
  EnemyTypeZombie,
  EnemyTypeZombieDark,

  // mini boss
  EnemyTypeBubble,
  EnemyTypeBubbleIce,
  EnemyTypeBubbleGreater,

  // boss
  EnemyTypeButcher,
  EnemyTypeSlimeBoss,
  EnemyTypeCyclops,
  EnemyTypeRatKing,
  EnemyTypeSpiderGiant,
  EnemyTypeFrancky,

  // invocated
  EnemyTypeBat_invocated,
  EnemyTypeRat_invocated,
  EnemyTypeRatGreen,
  EnemyTypeRatHelmet_invocated,
  EnemyTypeSnake_invocated,
  EnemyTypeSnakeBlood_invocated,
  EnemyTypeSlime_invocated,
  EnemyTypeSlimeRed_invocated,
  EnemyTypeSlimeBlue_invocated,
  EnemyTypeSlimeViolet_invocated,
  EnemyTypePumpkin_invocated,
  EnemyTypeSpiderEgg_invocated,
  EnemyTypeSpiderLittle_invocated,
  EnemyTypeZombie_invocated,

  EnemyTypeRockFalling,
  EnemyTypeRockMissile,
  EnemyTypeSpiderWeb,

  EnemyTypeFranckyHead,
  EnemyTypeFranckyHand,
  EnemyTypeFranckyFoot,

  EnemyTypeNone,  // player of fairy

  NB_ENEMY // = no enemy
};

enum sourceTypeEnum
{
  SourceTypeMelee,
  SourceTypeBolt,
  SourceTypeExplosion,
  SourceTypePoison
};

struct StructHurt
{
  int damage;
  enumShotType hurtingType;
  int level;
  bool critical;
  sourceTypeEnum sourceType;
  enemyTypeEnum enemyType;
  bool goThrough;
};

class BaseCreatureEntity : public CollidingSpriteEntity
{
  public:
    BaseCreatureEntity(sf::Texture* image, float x, float y, int spriteWidth, int spriteHeight);
    int getHp();
    int getHpMax();
    void setHp(int hp);
    void setHpMax(int hpMax);
    int getHpDisplay();
    float getCreatureSpeed();

    IntCoord getCurrentTile();

    virtual void animate(float delay);
    virtual float animateStates(float delay);
    virtual void animateColors(float delay);
    virtual void animateRecoil(float delay);
    virtual void animatePhysics(float delay);
    virtual void render(sf::RenderTarget* app);
    virtual void calculateBB();
    virtual bool collideWithMap(int direction);
    virtual int hurt(StructHurt hurtParam);
    virtual void prepareDying();
    virtual void dying();
    enum enumMovingStyle { movWalking, movFlying};
    virtual enumMovingStyle getMovingStyle();
    enum enumBloodColor { BloodNone = -1, BloodRed, BloodGreen, BloodRock, BloodEgg, BloodBubble, BloodBubbleIce};
    enum enumSpecialState
    {
      SpecialStateIce, // = 0
      SpecialStateSlow,
      SpecialStatePoison,

      DivineStateProtection,
      DivineStateSpeed,
      DivineStateFireRate,
      DivineStateFireDamage,

      NB_SPECIAL_STATES
    };
    enum enumStateResistance { ResistanceImmune, ResistanceVeryHigh, ResistanceHigh, ResistanceStandard, ResistanceLow, ResistanceVeryLow};
    struct specialStateStuct
    {
      enumSpecialState type;
      bool active;
      float timer;
      float param1;
      float param2;
      float param3;
    };
    specialStateStuct specialState[NB_SPECIAL_STATES];
    virtual void setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2);

    enum enumResistances
    {
      ResistanceIce, // = 0
      ResistanceFire,
      ResistanceStone,
      ResistanceLightning,
      ResistanceIllusion,
      ResistancePoison,

      ResistanceRecoil,
      ResistanceFrozen,

      NB_RESISTANCES
    };
    enumStateResistance resistance[NB_RESISTANCES];

    bool isSpecialStateActive(enumSpecialState state);

    virtual void giveRecoil(bool stun, Vector2D velocity, float timer);
    virtual void inflictsRecoilTo(BaseCreatureEntity* targetEntity);
    virtual void computeFacingDirection();
    virtual void dyingFromAge();
    virtual bool canCollide();
    bool canSee(float xf, float yf);
    bool canWalkTo(float xf, float yf);

    void heal(int healPoints);

    static StructHurt getHurtParams(int damage,
                          enumShotType hurtingType,
                          int level,
                          bool critical,
                          sourceTypeEnum sourceType,
                          enemyTypeEnum enemyType,
                          bool goThrough)
    {
      StructHurt hurtParams;
      hurtParams.damage = damage;
      hurtParams.hurtingType = hurtingType;
      hurtParams.level = level;
      hurtParams.critical = critical;
      hurtParams.sourceType = sourceType;
      hurtParams.enemyType = enemyType;
      hurtParams.goThrough = goThrough;

      return hurtParams;
    }

  protected:
    int hp;
    int hpMax;
    int hpDisplay;
    float creatureSpeed;
    int shadowFrame;
    int facingDirection;
    float armor;

    bool canExplode;  // true if the monster can explode with a fire attack

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

    void generateStar(sf::Color starColor);

  private:
    bool determineSatusChance(enumStateResistance resistance, int level);
    int determineDamageBonus(enumStateResistance resistance, int level);
    bool textTooClose(TextEntity* textEntity, float xDistMin, float yDistMin);
};

#endif // BASECREATUREENTITY_H
