#include "BogeymanEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "EnemyBoltEntity.h"

const float BOGEY_APPARITION_TIME = 1.0f;
const float BOGEY_DISAPPEARING_TIME = 1.0f;
const float BOGEY_ANIM_SPEED = 0.4f;
const float BOGEY_MOVING_DELAY = 1.0f;
const float BOGEY_INVISIBLE_DELAY = 1.0f;
const float BOGEY_VELOCITY = 155;
const float BOGEY_FIRE_VELOCITY = 320.0f;
const int BOGEY_HP = 100;

BogeymanEntity::BogeymanEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_BOGEY), x, y)
{
  // general
  creatureSpeed = BOGEY_VELOCITY;
  velocity = Vector2D(0.0f);
  hp = BOGEY_HP;
  meleeDamages = IMP_MELEE_DAMAGES;

  bloodColor = BloodBlack;
  changingDelay = BOGEY_APPARITION_TIME;
  shadowFrame = -1;
  movingStyle = movFlying;
  imagesProLine = 8;
  height = 88;
  sprite.setOrigin(32,80);

  state = 0;

  resistance[ResistanceFrozen] = ResistanceImmune;
  resistance[ResistancePoison] = ResistanceImmune;
  resistance[ResistanceIce] = ResistanceHigh;
  resistance[ResistanceIllusion] = ResistanceHigh;
  resistance[ResistanceLightning] = ResistanceLow;
  enemyType = EnemyTypeBogeyman;
  hasFired = false;
  isFireType1 = rand() % 2 == 0;
  age = -0.001f * (rand()%4000);
}

void BogeymanEntity::animate(float delay)
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
        if (state == 0) // appear -> attack
        {
          state = 1;
          changingDelay = 2 * BOGEY_ANIM_SPEED;
          hasFired = false;
          SoundManager::getInstance().playSound(SOUND_BOGEYMAN_ATTACK);
        }
        else if (state == 1) // attack -> move
        {
          changingDelay = BOGEY_MOVING_DELAY;
          state = 2;
        }
        else if (state == 2) // move -> disappear
        {
          state = 3;
          changingDelay = BOGEY_DISAPPEARING_TIME;
          setVelocity(Vector2D(0.0f, 0.0f));
          SoundManager::getInstance().playSound(SOUND_BOGEYMAN_VORTEX_01);
        }
        else if (state == 3)
        {
          state = 4;
          changingDelay = BOGEY_INVISIBLE_DELAY;
        }
        else if (state == 4) // invisible -> apparition
        {
          state = 0;
          changingDelay = BOGEY_APPARITION_TIME;
          teleport();
          SoundManager::getInstance().playSound(SOUND_BOGEYMAN_VORTEX_00);
        }
        else if (state == -1) // dying
        {
          isDying = true;
        }
      }

      if (state == 1) // fira anim
      {
        frame = changingDelay / BOGEY_ANIM_SPEED * 8;
        if (!hasFired && frame < 8)
        {
          hasFired = true;
          fire();
        }
        frame = 32 - frame;
      }

      else if (state == 2) // normal
      {
        frame = (int)((BOGEY_MOVING_DELAY - changingDelay) / BOGEY_ANIM_SPEED * 8) % 16;
      }

      else if (state == -1)
      {
        frame = 64 - (8 * (changingDelay / BOGEY_ANIM_SPEED));
      }
    }
  }

  EnemyEntity::animate(delay);
  if (age > 0 && delay >= age) SoundManager::getInstance().playSound(SOUND_BOGEYMAN_VORTEX_00);
  z = y + 8;
}

void BogeymanEntity::calculateBB()
{
    boundingBox.left = (int)x - 14;
    boundingBox.width = 28;
    boundingBox.top = (int)y - 55;
    boundingBox.height =  58;
}

void BogeymanEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void BogeymanEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void BogeymanEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void BogeymanEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}

void BogeymanEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getEnemyType() != EnemyTypeSpiderWeb && entity->getMovingStyle() == movFlying)
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 50.0f );
    giveRepulsion(false, vel, 0.3f);
  }
}

bool BogeymanEntity::canCollide()
{
  return (state == 1 || state == 2);
}

int BogeymanEntity::hurt(StructHurt hurtParam)
{
  int result = EnemyEntity::hurt(hurtParam);

  return result;
}

void BogeymanEntity::dying()
{
  EnemyEntity::dying();
  for (int i = 0; i < 4; i++) game().generateBlood(x, y, BloodBlack);
  h = 25.0f;
}

void BogeymanEntity::fire()
{
  for (int i = 0; i < 4; i++)
  {
    EnemyBoltEntity* bolt;

    bolt = new EnemyBoltEntity(x, y - 40, ShotTypeIllusion, 0, enemyType);
    bolt->setDamages(8);

    if (isFireType1)
    {
      switch (i)
      {
        case 0: bolt->setVelocity(Vector2D(BOGEY_FIRE_VELOCITY * 0.7f, BOGEY_FIRE_VELOCITY * 0.7f)); break;
        case 1: bolt->setVelocity(Vector2D(BOGEY_FIRE_VELOCITY * -0.7f, BOGEY_FIRE_VELOCITY * 0.7f)); break;
        case 2: bolt->setVelocity(Vector2D(BOGEY_FIRE_VELOCITY * -0.7f, BOGEY_FIRE_VELOCITY * -0.7f)); break;
        case 3: bolt->setVelocity(Vector2D(BOGEY_FIRE_VELOCITY * 0.7f, BOGEY_FIRE_VELOCITY * -0.7f)); break;
      }
    }
    else
    {
      switch (i)
      {
        case 0: bolt->setVelocity(Vector2D(BOGEY_FIRE_VELOCITY, 0)); break;
        case 1: bolt->setVelocity(Vector2D(-BOGEY_FIRE_VELOCITY, 0)); break;
        case 2: bolt->setVelocity(Vector2D(0, BOGEY_FIRE_VELOCITY)); break;
        case 3: bolt->setVelocity(Vector2D(0, -BOGEY_FIRE_VELOCITY)); break;
      }
    }
  }
  SoundManager::getInstance().playSound(SOUND_BLAST_ILLUSION);
  isFireType1 = !isFireType1;
}

void BogeymanEntity::render(sf::RenderTarget* app)
{
  if (hurting && hp > 0)
  {
    if (hurtingDelay > 0.0f)
    {
      int fadeColor = (sf::Uint8)((HURTING_DELAY - hurtingDelay) * 127);
      if (hurtingDelay > HURTING_DELAY) fadeColor = 0;
      sprite.setColor(sf::Color(255, 255, 255, 128 + fadeColor )); // fade
    }
  }

  if (state == 0)
  {
    if (changingDelay > BOGEY_APPARITION_TIME - 0.5f)
    {
      float scale = 2 * (BOGEY_APPARITION_TIME - changingDelay);
      sprite.setScale(scale, scale);
    }
    else if (changingDelay < 0.5f)
    {
      float scale = changingDelay * 2;
      sprite.setScale(scale, scale);
    }
    else
      sprite.setScale(1.0f, 1.0f);

    int holeFrame = ((int)(age * 20.0f)) % 8;
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(holeFrame * width, 5 * height, width, height));
    app->draw(sprite);

    sprite.setScale(1.0f, 1.0f);

    if (changingDelay <= BOGEY_ANIM_SPEED)
    {
      frame = 32 + (8 * (changingDelay / BOGEY_ANIM_SPEED));
      EnemyEntity::render(app);
    }
  }
  else if (state == 3)
  {
    if (changingDelay > BOGEY_DISAPPEARING_TIME - 0.5f)
    {
      float scale = 2 * (BOGEY_DISAPPEARING_TIME - changingDelay);
      sprite.setScale(scale, scale);
    }
    else if (changingDelay < 0.5f)
    {
      float scale = changingDelay * 2;
      sprite.setScale(scale, scale);
    }
    else
      sprite.setScale(1.0f, 1.0f);

    int holeFrame = ((int)(age * 12.0f)) % 8;
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(holeFrame * width, 5 * height, width, height));
    app->draw(sprite);

    sprite.setScale(1.0f, 1.0f);

    if (changingDelay >= (BOGEY_DISAPPEARING_TIME - BOGEY_ANIM_SPEED))
    {
      frame = 32 + (8 * ((BOGEY_DISAPPEARING_TIME - changingDelay) / BOGEY_ANIM_SPEED));
      EnemyEntity::render(app);
    }
  }

  else if (state != 4)
    EnemyEntity::render(app);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void BogeymanEntity::teleport()
{
  x = ((int)(game().getPlayer()->getX()) / TILE_WIDTH) * TILE_WIDTH  + TILE_WIDTH * 0.5f;
  y = ((int)(game().getPlayer()->getY()) / TILE_HEIGHT) * TILE_HEIGHT + TILE_HEIGHT * 0.5f;
}

void BogeymanEntity::prepareDying()
{
  state = -1; // dying
  changingDelay = BOGEY_ANIM_SPEED * 2;
  velocity.x = 0.0f;
  velocity.y = 0.0f;

  SoundManager::getInstance().playSound(SOUND_BOGEYMAN_DIE);

  type = ENTITY_ENEMY_NC;
}
