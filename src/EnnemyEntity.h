#ifndef ENNEMYPRITE_H
#define ENNEMYPRITE_H

#include "BaseCreatureEntity.h"

enum enemyTypeEnum
{
  // normal
  EnemyTypeBat,
  EnemyTypeRat,
  EnemyTypeRatBlack,
  EnemyTypeEvilFlower,
  EnemyTypeSlime,
  EnemyTypeSlimeRed,
  EnemyTypeSlimeBlue,
  EnemyTypeImpBlue,
  EnemyTypeImpRed,

  // boss
  EnemyTypeButcher,
  EnemyTypeSlimeBoss,
  EnemyTypeRatKing,
  EnemyTypeCyclops,
  EnemyTypeSpiderGiant,

  // invocated
  EnemyTypeRatGreen,
  EnemyTypeRockFalling,
  EnemyTypeRockMissile,
  EnemyTypeSlime_invocated,
  EnemyTypeSlimeRed_invocated,
  EnemyTypeSlimeBlue_invocated,
  EnemyTypeSpiderEgg_invocated,
  EnemyTypeSpiderLittle_invocated,
  EnemyTypeSpiderWeb,

  NB_ENEMY // = no enemy
};

class EnnemyEntity : public BaseCreatureEntity
{
public:
  EnnemyEntity(sf::Texture* image, float x, float y);
  virtual void animate(float delay);
  virtual void calculateBB();
  virtual void render(sf::RenderTarget* app);
  void displayLifeBar(std::string name, float posY, sf::RenderTarget* app);

protected:
  virtual void collideMapRight();
  virtual void collideMapLeft();
  virtual void collideMapTop();
  virtual void collideMapBottom();

  virtual void readCollidingEntity(CollidingSpriteEntity* entity);
  virtual void dying();
  virtual void drop();
  virtual void collideWithEnnemy(GameEntity* collidingEntity);
  virtual bool hurt(int damages, enumShotType hurtingType, int level);

  virtual bool canCollide();

  int meleeDamages;

  float h;
  float hVelocity;

  int dyingFrame;
  int deathFrame;
  bool isAgonising;
  sound_resources hurtingSound;
  sound_resources dyingSound;
  sound_resources agonizingSound;

  enemyTypeEnum enemyType;

private:
};

#endif // ENNEMYPRITE_H
