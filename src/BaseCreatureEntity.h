#ifndef BASECREATUREENTITY_H
#define BASECREATUREENTITY_H

#include "sfml_game/CollidingSpriteEntity.h"
#include "TextEntity.h"
#include "Constants.h"

enum enemyTypeEnum
{
  // normal
  EnemyTypeBat,
  EnemyTypeBatSkeleton,
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
  EnemyTypeCauldronElemental,
  EnemyTypeSpiderEgg,
  EnemyTypeSpiderLittle,
  EnemyTypeSpiderTarantula,
  EnemyTypeGhost,
  EnemyTypeZombie,
  EnemyTypeZombieDark,
  EnemyTypeBogeyman,
  EnemyTypeSlimeLarge,
  EnemyTypeSlimeRedLarge,
  EnemyTypeSlimeBlueLarge,
  EnemyTypeSlimeVioletLarge,
  EnemyTypeSausage,

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
  EnemyTypeVampire,

  // invocated
  EnemyTypeBat_invocated,
  EnemyTypeBatSkeleton_invocated,
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
  EnemyTypeSpiderTarantula_invocated,
  EnemyTypeZombie_invocated,
  EnemyTypeSausage_invocated,

  EnemyTypeRockFalling,
  EnemyTypeRockMissile,
  EnemyTypeSpiderWeb,

  EnemyTypeFranckyHead,
  EnemyTypeFranckyHand,
  EnemyTypeFranckyFoot,

  EnemyTypeVampireDead,

  EnemyTypeNone,  // player of fairy
  EnemyTypeDestroyable,

  NB_ENEMY // = no enemy
};

const std::vector<std::string> enemyString =
{
  // normal
  "EnemyTypeBat",
  "EnemyTypeBatSkeleton",
  "EnemyTypeRat",
  "EnemyTypeRatBlack",
  "EnemyTypeRatHelmet",
  "EnemyTypeRatBlackHelmet",
  "EnemyTypeEvilFlower",
  "EnemyTypeEvilFlowerIce",
  "EnemyTypeEvilFlowerFire",
  "EnemyTypeSnake",
  "EnemyTypeSnakeBlood",
  "EnemyTypeSlime",
  "EnemyTypeSlimeRed",
  "EnemyTypeSlimeBlue",
  "EnemyTypeSlimeViolet",
  "EnemyTypeImpBlue",
  "EnemyTypeImpRed",
  "EnemyTypePumpkin",
  "EnemyTypeWitch",
  "EnemyTypeWitchRed",
  "EnemyTypeCauldron",
  "EnemyTypeCauldronElemental",
  "EnemyTypeSpiderEgg",
  "EnemyTypeSpiderLittle",
  "EnemyTypeSpiderTarantula",
  "EnemyTypeGhost",
  "EnemyTypeZombie",
  "EnemyTypeZombieDark",
  "EnemyTypeBogeyman",
  "EnemyTypeSlimeLarge",
  "EnemyTypeSlimeRedLarge",
  "EnemyTypeSlimeBlueLarge",
  "EnemyTypeSlimeVioletLarge",
  "EnemyTypeSausage",

  // mini boss
  "EnemyTypeBubble",
  "EnemyTypeBubbleIce",
  "EnemyTypeBubbleGreater",

  // boss
  "EnemyTypeButcher",
  "EnemyTypeSlimeBoss",
  "EnemyTypeCyclops",
  "EnemyTypeRatKing",
  "EnemyTypeSpiderGiant",
  "EnemyTypeFrancky",
  "EnemyTypeVampire",

  // invocated
  "EnemyTypeBat_invocated",
  "EnemyTypeBatSkeleton_invocated",
  "EnemyTypeRat_invocated",
  "EnemyTypeRatGreen",
  "EnemyTypeRatHelmet_invocated",
  "EnemyTypeSnake_invocated",
  "EnemyTypeSnakeBlood_invocated",
  "EnemyTypeSlime_invocated",
  "EnemyTypeSlimeRed_invocated",
  "EnemyTypeSlimeBlue_invocated",
  "EnemyTypeSlimeViolet_invocated",
  "EnemyTypePumpkin_invocated",
  "EnemyTypeSpiderEgg_invocated",
  "EnemyTypeSpiderLittle_invocated",
  "EnemyTypeSpiderTarantula_invocated",
  "EnemyTypeZombie_invocated",
  "EnemyTypeSausage_invocated",

  "EnemyTypeRockFalling",
  "EnemyTypeRockMissile",
  "EnemyTypeSpiderWeb",

  "EnemyTypeFranckyHead",
  "EnemyTypeFranckyHand",
  "EnemyTypeFranckyFoot",

  "EnemyTypeVampireDead",

  "EnemyTypeNone",  // player of fairy
  "EnemyTypeDestroyable",
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

enum enumSpecialState
{
  SpecialStateIce, // = 0
  SpecialStateSlow,   // param 1 = multiplier
  SpecialStateSpeed,  // param 1 = multiplier
  SpecialStatePoison,
  SpecialStateWeakness,
  SpecialStateStrength,
  SpecialStateRage,
  SpecialStateConfused,
  SpecialStateTime,

  DivineStateProtection,
  DivineStateSpeed,
  DivineStateFireRate,
  DivineStateFireDamage,

  NB_SPECIAL_STATES
};

enum enumStateResistance { ResistanceImmune, ResistanceVeryHigh, ResistanceHigh, ResistanceStandard, ResistanceLow, ResistanceVeryLow};

enum enumResistances
{
  ResistanceIce, // = 0
  ResistanceFire,
  ResistanceStone,
  ResistanceLightning,
  ResistanceIllusion,
  ResistancePoison,

  ResistanceRepulsion,
  ResistanceFrozen,

  NB_RESISTANCES
};

struct specialStateStuct
{
  enumSpecialState type;
  bool active;
  float timer;
  float param1;
  float param2;
  float param3;
  bool waitUnclear;
};

const std::string specialStateToLabel[NB_SPECIAL_STATES] =
{
  "state_frozen",
  "state_slow",
  "state_speed",
  "state_poison",
  "state_weakness",
  "state_strength",
  "state_rage",
  "state_confusion",
  "state_time",

  "state_div_protection",
  "state_div_speed",
  "state_div_firerate",
  "state_div_damage"
};

const sf::Color specialStateToColor[NB_SPECIAL_STATES] =
{
  sf::Color(100, 200, 255),
  sf::Color::Red,
  sf::Color::White,
  sf::Color::Green,
  sf::Color::Red,
  sf::Color::White,
  sf::Color::White,
  sf::Color::White,
  sf::Color::Red,

  sf::Color::Yellow,
  sf::Color::Yellow,
  sf::Color::Yellow,
  sf::Color::Yellow
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
  void displayFlyingText(float xText, float yText, int sizeText, std::string text, TextEntity::colorTypeEnum color);
  virtual int hurt(StructHurt hurtParam);
  virtual void prepareDying();
  virtual void dying();
  enum enumMovingStyle { movWalking, movFlying};
  virtual enumMovingStyle getMovingStyle();
  enum enumBloodColor
  {
    BloodNone = -1,
    BloodRed,
    BloodGreen,
    BloodRock,
    BloodEgg,
    BloodBubble,
    BloodBubbleIce,
    BloodBlack,
    BloodBarrel,
    BloodBarrelPowder,
    BloodSkull
  };

  specialStateStuct specialState[NB_SPECIAL_STATES];
  virtual void setSpecialState(enumSpecialState state, bool active, float timer, float param1, float param2, bool waitUnclear = false);

  enumStateResistance resistance[NB_RESISTANCES];

  bool isSpecialStateActive(enumSpecialState state);
  specialStateStuct getSpecialState(enumSpecialState state);

  virtual void giveRepulsion(bool stun, Vector2D velocity, float timer);
  virtual void inflictsRepulsionTo(BaseCreatureEntity* targetEntity);
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

  bool intersectsSegments(Vector2D a1, Vector2D a2, Vector2D b1, Vector2D b2);
  bool intersectsTile(Vector2D a1, Vector2D a2, int xTile, int yTile);

  virtual bool isAttacking(); // true when the monster is performing a melee attack

protected:
  int hp;
  int hpMax;
  int hpDisplay;
  float creatureSpeed;
  bool doesAccelerate;
  int shadowFrame;
  int facingDirection;
  float armor;

  bool canExplode;  // true if the monster can explode with a fire attack
  bool displayDamage;

  bool hurting;
  float hurtingDelay;
  enumShotType hurtingType;
  enumBloodColor bloodColor;
  enumMovingStyle movingStyle;

  struct  RepulseStruct
  {
    bool active;
    Vector2D velocity;
    bool stun;
    bool propage;
    float timer;
  } repulsion;

  void generateStar(sf::Color starColor);
  virtual void makeExplode();

private:
  bool determineSatusChance(enumStateResistance resistance, int level);
  int determineDamageBonus(enumStateResistance resistance, int level);
  bool textTooClose(TextEntity* textEntity, float xDistMin, float yDistMin);
};

#endif // BASECREATUREENTITY_H
