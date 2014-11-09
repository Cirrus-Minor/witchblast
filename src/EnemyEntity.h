#ifndef ENNEMYPRITE_H
#define ENNEMYPRITE_H

#include "BaseCreatureEntity.h"
#include "BoltEntity.h"

class EnemyEntity : public BaseCreatureEntity
{
public:
  EnemyEntity(sf::Texture* image, float x, float y);
  virtual void animate(float delay);
  virtual void calculateBB();
  virtual void render(sf::RenderTarget* app);
  virtual int hurt(int damages, enumShotType hurtingType, int level, bool critical, sourceTypeEnum sourceType, enemyTypeEnum enemyType) override;

  virtual bool canCollide();
  enemyTypeEnum getEnemyType();

  void setLabelDy(float label_dy);

protected:
  virtual void collideMapRight();
  virtual void collideMapLeft();
  virtual void collideMapTop();
  virtual void collideMapBottom();

  void renderLifeBar(sf::RenderTarget* app, std::string label);
  void makeExplode();

  virtual void readCollidingEntity(CollidingSpriteEntity* entity);
  virtual void dying();
  virtual void drop();
  virtual void collideWithEnemy(EnemyEntity* entity);
  virtual void collideWithBolt(BoltEntity* boltEntity);
  int getCollisionDirection(BoltEntity* boltEntity);

  int meleeDamages;
  int meleeLevel;
  enumShotType meleeType;

  float h;
  float hVelocity;

  float label_dy;   /* dy of the bar label for bosses */

  int dyingFrame;
  int deathFrame;
  bool isAgonising;
  sound_resources hurtingSound;
  sound_resources dyingSound;
  sound_resources agonizingSound;

  enemyTypeEnum enemyType;

  bool testEntityInMap();

private:
};

#endif // ENNEMYPRITE_H
