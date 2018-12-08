#include "GiantSpiderEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "SpiderEggEntity.h"
#include "SpiderWebEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>

GiantSpiderEntity::GiantSpiderEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_GIANT_SPIDER), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = GIANT_SPIDER_SPEED[0];
  velocity = Vector2D(0.0f, 0.0f);

  hp = GIANT_SPIDER_HP;
  hpMax = hp;
  hpDisplay = hp;

  meleeDamages = GIANT_SPIDER_DAMAGE;

  type = ENTITY_ENEMY;
  enemyType = EnemyTypeSpiderGiant;
  bloodColor = BloodGreen;
  shadowFrame = 3;
  dyingFrame = 4;
  deathFrame = FRAME_CORPSE_GIANT_SPIDER;
  agonizingSound = SOUND_SPIDER_DIE;
  sprite.setOrigin(64.0f, 64.0f);
  walkingSoundDelay = -1.0f;

  h = 2000;
  state = 0;
  hurtLevel = 0;
  labelHasChanged = false;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceImmune;
  creatureName = "???";
  canExplode = false;
}

void GiantSpiderEntity::animate(float delay)
{
  if (age <= 0.0f)
  {
    age += delay;
    return;
  }

  if (!isAgonising)
  {
    if (state == 0) // falling
    {
      h -= delay * 300.0f;
      if (h <= 0.0f)
      {
        h = 0;
        state = 1;
        timer = 1.0f;
        if (!labelHasChanged)
        {
          labelHasChanged = true;
          creatureName = tools::getLabel("enemy_giant_spider");
          game().testAndAddMessageToQueue(MsgInfoGiantSpiderAfter);
        }
      }
    }
    else if (state == 1) // wait after falling
    {
      timer -= delay;
      if (timer <= 0.0f)
      {
        state = 2;
        velocity = Vector2D(creatureSpeed);
        timer = 10.0f;
        fireDelay = 0.5f;
      }
    }
    else if (state == 2) // moving
    {
      walkingSoundDelay -= delay;
      if (walkingSoundDelay <= 0.0f)
      {
        walkingSoundDelay = 1.0f;
        SoundManager::getInstance().playSound(SOUND_SPIDER_WALKING);
      }
      fireDelay -= delay;
      if (fireDelay <= 0.0f)
      {
        if (rand() % 12 == 0)
        {
          SoundManager::getInstance().playSound(SOUND_SPIDER_WEB);
          for (int i = 0; i < 3; i++) new SpiderWebEntity(x, y, false);
        }
        else
        {
          for (int i = 0; i < 4; i++) fire(i == 0 ? 1 : 0);
        }

        fireDelay = GIANT_SPIDER_FIRE_DELAY[hurtLevel];
      }

      timer -= delay;

      if (getHealthLevel() > hurtLevel)
      {
        hurtLevel++;
        state = 3;
        velocity = Vector2D(0.0f, 0.0f);
        timer = 1.0f;
        creatureSpeed = GIANT_SPIDER_SPEED[hurtLevel];
        SoundManager::getInstance().playSound(SOUND_SPIDER_HURT);
      }
    }
    else if (state == 3) // wait after falling
    {
      timer -= delay;
      if (timer <= 0.0f) state = 4;
    }
    else if (state == 4) // moving up
    {
      h += delay * 300.0f;
      if (h >= 1500.0f)
      {
        state = 5;
        timer = 6.0f;
        for (int i = 0; i < GIANT_SPIDER_NUMBER_EGGS[hurtLevel]; i++)
        {
          new SpiderEggEntity(TILE_WIDTH * 1.5f + rand() % (TILE_WIDTH * 12),
                              TILE_HEIGHT * 1.5f + rand() % (TILE_HEIGHT * 6), true);
        }
      }
    }
    else if (state == 5) // waiting to fall
    {
      timer -= delay;
      if (timer <= 0.0f)
        state = 0;
    }

    // frame
    frame = 0;
    if (state == 2)
    {
      frame = ((int)(age * 6.0f)) % 4;
      if (frame == 2) frame = 0;
      else if (frame == 3) frame = 2;
    }
  }

  EnemyEntity::animate(delay);
  z = y + 40;
}

int GiantSpiderEntity::hurt(StructHurt hurtParam)
{
  if (hurtLevel < getHealthLevel()) hurtParam.damage /= 5;
  return EnemyEntity::hurt(hurtParam);
}

void GiantSpiderEntity::calculateBB()
{
  boundingBox.left = (int)x - 45;
  boundingBox.width = 90;
  boundingBox.top = (int)y - 25;
  boundingBox.height =  65;
}

void GiantSpiderEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void GiantSpiderEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void GiantSpiderEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void GiantSpiderEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void GiantSpiderEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    Vector2D v = Vector2D(x, y).vectorTo(Vector2D(entity->getX(), entity->getY()), 150.0f);
    entity->giveRepulsion(false, v, 0.5f);
  }
}

void GiantSpiderEntity::calculateRotation()
{
  float h0 = 100;
  float hf = 18;
  if (state == 0)
  {
    if (h > h0) sprite.setRotation(180);
    else if (h < hf) sprite.setRotation(0);
    else
    {
      sprite.setRotation(180 + (h0 - h) * 180 / (h0 - hf));
    }
  }
  if (state == 4)
  {
    if (h > h0) sprite.setRotation(180);
    else if (h < hf) sprite.setRotation(0);
    else
    {
      sprite.setRotation(180 - (h0 - h) * 180 / (h0 - hf));
    }
  }
  else if (state == 2) sprite.setRotation(0);
}

void GiantSpiderEntity::render(sf::RenderTarget* app)
{
  if (!isDying)
  {
    // shadow
    sprite.setRotation(0.0f);

    if (state == 0 || state == 4 || state == 5)
    {
      int h0 = 850;
      int hf = 600;
      int fade = 0;
      if (h < h0)
      {
        if (h < hf) fade = 255;
        else
        {
          fade = (h0 - h) * 255 / (h0 - hf);
          if (fade < 0) fade = 0;
          else if (fade > 255) fade = 255;
        }
        sprite.setColor(sf::Color(255, 255, 255, fade));
        sprite.setPosition(x, y);
        sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
        app->draw(sprite);
        sprite.setColor(sf::Color(255, 255, 255, 255));
      }
    }
    else
    {
      sprite.setPosition(x, y);
      sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
      app->draw(sprite);
    }
  }

  calculateRotation();

  if (state == 0 || state == 1 || state == 3 || state == 4)
  {
    if (y - h > 0)
    {
      int fade = 255;
      if (state == 1) fade = 255 * timer;
      else if (state == 3) fade = 255 * (1.0f - timer);
      sf::RectangleShape line(sf::Vector2f(2, y - h));
      line.setPosition(x - 1, 0);
      line.setFillColor(sf::Color(255, 255, 255, fade));
      app->draw(line);
    }
  }

  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(frame * width, 0, width, height));
  app->draw(sprite);

  renderLifeBar(app, creatureName);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

bool GiantSpiderEntity::canCollide()
{
  return h <= 70.0f;
}

void GiantSpiderEntity::fire(int fireType)
{
  SoundManager::getInstance().playSound(SOUND_BLAST_POISON);

  EnemyBoltEntity* bolt;

  bolt = new EnemyBoltEntity(x, y, ShotTypePoison, 0, enemyType);
  bolt->setDamages(5);

  float fireVelocity = 180.0f;
  if (specialState[SpecialStateIce].active) fireVelocity *= 0.5f;
  bolt->setVelocity(Vector2D(fireVelocity));
}

int GiantSpiderEntity::getHealthLevel()
{
  int healthLevel = 0;
  if (hp <= hpMax * 0.25) healthLevel = 3;
  else if (hp <= hpMax * 0.5) healthLevel = 2;
  else if (hp <= hpMax * 0.75) healthLevel = 1;
  return healthLevel;
}

void GiantSpiderEntity::drop()
{
  dropItem(ItemBossHeart);

  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    EnemyEntity* entity = dynamic_cast<EnemyEntity*>(e);
    if (entity != NULL)
    {
      if (entity->getEnemyType()== EnemyTypeSpiderWeb)
      {
        entity->hurt(getHurtParams(entity->getHp(), ShotTypeStandard, 0, false, SourceTypeMelee, enemyType, false));
      }
    }
  }
}
