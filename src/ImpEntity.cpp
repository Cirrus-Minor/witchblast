#include "ImpEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "EnnemyBoltEntity.h"

ImpEntity::ImpEntity(float x, float y, impTypeEnum impType)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_IMP), x, y)
{
  // general
  creatureSpeed = IMP_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = IMP_HP;
  meleeDamages = IMP_MELEE_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
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
    resistance[ResistanceIce] = ResistanceHigh;
    resistance[ResistanceFire] = ResistanceLow;
  }
  else // ImpTypeRed
  {
    dyingFrame = 3;
    deathFrame = FRAME_CORPSE_IMP_RED;
    resistance[ResistanceIce] = ResistanceLow;
    resistance[ResistanceFire] = ResistanceHigh;
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

      switch (((int)(age * 5.0f)) % 4)
      {
        case 0:
        case 2: frame = 0; break;
        case 1: frame = 1; break;
        case 3: frame = 2; break;
      }
    }
    if (impType == ImpTypeBlue) frame += 5;
  }

  EnnemyEntity::animate(delay);
}

void ImpEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + IMP_BB_LEFT;
    boundingBox.width = width - IMP_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + IMP_BB_TOP;
    boundingBox.height =  height - IMP_BB_HEIGHT_DIFF;
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

void ImpEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movFlying)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
  }
}

bool ImpEntity::hurt(int damages, enumShotType hurtingType, int level)
{
  bool result = EnnemyEntity::hurt(damages, hurtingType, level);

  if (!isDying && !isAgonising)
  {
    teleport();
    state = 3;
    changingDelay = 0.4f;
  }

  return result;
}

void ImpEntity::dying()
{
  EnnemyEntity::dying();
  h = 25.0f;
}

void ImpEntity::fire()
{
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);

    EnnemyBoltEntity* bolt;
    if (impType == ImpTypeBlue)
    {
      bolt = new EnnemyBoltEntity(x, y, ShotTypeIce, 0);
      bolt->setDamages(5);
    }
    else
    {
      bolt = new EnnemyBoltEntity(x, y, ShotTypeFire, 0);
      bolt->setDamages(8);
    }
    bolt->setFlying(true);

    float fireVelocity = IMP_FIRE_VELOCITY;
    if (specialState[SpecialStateIce].active) fireVelocity *= 0.5f;
    bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), fireVelocity ));
}

void ImpEntity::generateStar(sf::Color starColor)
{
  SpriteEntity* spriteStar = new SpriteEntity(
                           ImageManager::getImageManager()->getImage(IMAGE_STAR_2),
                            x, y);
  spriteStar->setScale(0.8f, 0.8f);
  spriteStar->setZ(z-1.0f);
  spriteStar->setLifetime(0.8f);
  spriteStar->setSpin(-100 + rand()%200);
  spriteStar->setVelocity(Vector2D(10 + rand()%40));
  spriteStar->setWeight(-150);
  spriteStar->setFading(true);
  spriteStar->setColor(starColor);
}

void ImpEntity::teleport()
{
  bool ok = false;
  int xm, ym;
  float xMonster, yMonster;

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

    xMonster = OFFSET_X + xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
    yMonster = OFFSET_Y + ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

    ok = (game().getPlayerPosition().distance2(Vector2D(xMonster, yMonster)) > 40000);
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
