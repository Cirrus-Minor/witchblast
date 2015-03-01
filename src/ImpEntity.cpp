#include "ImpEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "EnemyBoltEntity.h"

ImpEntity::ImpEntity(float x, float y, impTypeEnum impType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_IMP), x, y)
{
  // general
  creatureSpeed = IMP_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = IMP_HP;
  meleeDamages = IMP_MELEE_DAMAGES;

  bloodColor = BloodNone;
  changingDelay = 1.5f + (float)(rand() % 2500) / 1000.0f;
  shadowFrame = 4;
  movingStyle = movFlying;
  imagesProLine = 5;

  hurtingSound = SOUND_IMP_HURT;
  agonizingSound = SOUND_IMP_DYING;
  this->impType = impType;

  state = 0;

  // Imp-specific
  if (impType == ImpTypeBlue)
  {
    dyingFrame = 8;
    deathFrame = FRAME_CORPSE_IMP_BLUE;
    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceVeryHigh;
    resistance[ResistanceFire] = ResistanceVeryLow;
    enemyType = EnemyTypeImpBlue;
  }
  else // ImpTypeRed
  {
    dyingFrame = 3;
    deathFrame = FRAME_CORPSE_IMP_RED;
    resistance[ResistanceIce] = ResistanceVeryLow;
    resistance[ResistanceFire] = ResistanceVeryHigh;
    enemyType = EnemyTypeImpRed;
  }
}

void ImpEntity::animate(float delay)
{
  if (!isDying && !isAgonising)
  {
    if (age < 0.0f)
      frame = 1;
    else
    {
      changingDelay -= delay;
      if (changingDelay < 0.0f)
      {
        if (state == 0) // flying -> pause
        {
          viscosity = 0.965f;
          state = 1;
          changingDelay = 0.5f;
        }
        else if (state == 1) // pause -> fire -> pause
        {
          changingDelay = 0.4f;
          fire();
          state = 2;
        }
        else if (state == 2) // pause -> teleport -> pause
        {
          teleport();
          state = 3;
          changingDelay = 0.4f;
        }
        else if (state == 3)
        {
          velocity = Vector2D(creatureSpeed);
          changingDelay = 1.5f + (float)(rand() % 2500) / 1000.0f;
          viscosity = 1.0f;
          state = 0;
        }

      }

      frame = ((int)(age * 5.0f)) % 4;
      if (frame == 3) frame = 1;
    }
    if (impType == ImpTypeBlue) frame += 5;
  }

  EnemyEntity::animate(delay);
}

void ImpEntity::calculateBB()
{
    boundingBox.left = (int)x - 14;
    boundingBox.width = 28;
    boundingBox.top = (int)y - 14;
    boundingBox.height =  28;
}

void ImpEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void ImpEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void ImpEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void ImpEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}

void ImpEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getEnemyType() != EnemyTypeSpiderWeb && entity->getMovingStyle() == movFlying)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
  }
}

int ImpEntity::hurt(StructHurt hurtParam)
{
  int result = EnemyEntity::hurt(hurtParam);

  if (!isDying && !isAgonising)
  {
    game().generateBlood(x, y, BloodRed);

    if (result > 1 && (state != 2 || changingDelay > 0.4f))
    {
      state = 2;
      changingDelay = 0.15f;
    }
  }

  return result;
}

void ImpEntity::dying()
{
  EnemyEntity::dying();
  for (int i = 0; i < 4; i++) game().generateBlood(x, y, BloodRed);
  h = 25.0f;
}

void ImpEntity::fire()
{
    EnemyBoltEntity* bolt;
    if (impType == ImpTypeBlue)
    {
      bolt = new EnemyBoltEntity(x, y, ShotTypeIce, 0, enemyType);
      bolt->setDamages(5);
      SoundManager::getInstance().playSound(SOUND_BLAST_ICE);
    }
    else
    {
      bolt = new EnemyBoltEntity(x, y, ShotTypeFire, 0, enemyType);
      bolt->setDamages(8);
      SoundManager::getInstance().playSound(SOUND_BLAST_FIRE);
    }
    bolt->setFlying(true);

    float fireVelocity = IMP_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) fireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), fireVelocity ));
}

void ImpEntity::teleport()
{
  bool ok = false;
  int xm, ym;
  float xMonster, yMonster;

  SoundManager::getInstance().playSound(SOUND_TELEPORT);

  for(int i=0; i < 6; i++)
  {
    if (impType == ImpTypeBlue)
    {
      generateStar(sf::Color(50, 50, 255, 255));
      generateStar(sf::Color(200, 200, 255, 255));
    }
    else
    {
      generateStar(sf::Color(255, 50, 50, 255));
      generateStar(sf::Color(255, 200, 200, 255));
    }
  }

  int counter = 50;
  while (!ok && counter > 0)
  {
    counter--;
    xm = 1 +rand() % (MAP_WIDTH - 3);
    ym = 1 +rand() % (MAP_HEIGHT - 3);

    xMonster = xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
    yMonster = ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

    ok = game().getCurrentMap()->isFlyable(xm, ym) && (game().getPlayerPosition().distance2(Vector2D(xMonster, yMonster)) > 40000);
  }
  x = xMonster;
  y = yMonster;

  for(int i=0; i < 6; i++)
  {
    if (impType == ImpTypeBlue)
    {
      generateStar(sf::Color(50, 50, 255, 255));
      generateStar(sf::Color(200, 200, 255, 255));
    }
    else
    {
      generateStar(sf::Color(255, 50, 50, 255));
      generateStar(sf::Color(255, 200, 200, 255));
    }
  }
}
