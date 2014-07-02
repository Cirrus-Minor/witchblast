#include "CyclopEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "RockMissileEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

CyclopEntity::CyclopEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_CYCLOP), x, y)
{
  width = 128;
  height = 192;
  creatureSpeed = CYCLOP_SPEED[0];
  velocity = Vector2D(creatureSpeed);
  hp = 300;
  hpDisplay = hp;
  hpMax = 300;
  meleeDamages = 10;


  type = ENTITY_ENNEMY_BOSS;
  bloodColor = bloodRed;
  shadowFrame = 8;
  dyingFrame = 5;
  deathFrame = FRAME_CORPSE_CYCLOP;
  frame = 0;
  if (game().getPlayerPosition().x > x) isMirroring = true;
  sprite.setOrigin(64.0f, 64.0f);

  state = 0;
  timer = 2.0f;
  counter = 10;
  age = -1.5f;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
}

int CyclopEntity::getHealthLevel()
{
  int healthLevel = 0;
  if (hp <= hpMax * 0.25) healthLevel = 3;
  else if (hp <= hpMax * 0.5) healthLevel = 2;
  else if (hp <= hpMax * 0.75) healthLevel = 1;
  return healthLevel;
}

void CyclopEntity::fire()
{
  new RockMissileEntity(x, y+30);
  SoundManager::getSoundManager()->playSound(SOUND_THROW);
}

void CyclopEntity::computeStates(float delay)
{
  timer -= delay;

  if (timer <= 0.0f)
  {
    if (state == 0) // walking
    {
      if (counter > 0)
      {
        counter--;
        timer = 0.5f;
        creatureSpeed = CYCLOP_SPEED[getHealthLevel()];
        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      }
      else
      {
        state = 1; // charge
        timer = 0.9f;
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        counter = CYCLOP_NUMBER_ROCKS[getHealthLevel()];
        SoundManager::getSoundManager()->playSound(SOUND_CYCLOP00);
      }
    }
    else if (state == 1) // fire
    {
      state = 2;
      fire();
      timer = 0.2;
    }
    else if (state == 2) // fire end
    {
      if (counter <= 1)
      {
        state = 0;
        timer = CYCLOP_FIRE_DELAY[getHealthLevel()];
        counter = 10;
      }
      else
      {
        counter--;
        state = 1;
        timer = 0.2f;
      }
    }
  }
}

void CyclopEntity::animate(float delay)
{
  if (age <= 0.0f)
  {
    age += delay;
    return;
  }

  if (isAgonising)
  {
    if (h < -0.01f)
    {
      isDying = true;
      SpriteEntity* corpse;
      corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), x, y + 48, 128, 128);
      corpse->setFrame(deathFrame - FRAME_CORPSE_KING_RAT);

      corpse->setZ(OFFSET_Y);
      corpse->setType(ENTITY_CORPSE);
      if (dyingSound != SOUND_NONE) SoundManager::getSoundManager()->playSound(dyingSound);
    }
    else
    {
      frame = dyingFrame;
      hVelocity -= 700.0f * delay;
      h += hVelocity * delay;
    }

    return;
  }

  // special states
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].parameter;

  // IA
  computeStates(delay);

  // collisions
  if (canCollide()) testSpriteCollisions();
  BaseCreatureEntity::animate(delay);

  // current frame
  if (state == 0)
  {
    int r = ((int)(age * 5.0f)) % 4;
    if (r == 2) frame = 0;
    else if (r == 3) frame = 2;
    else frame = r;
  }
  else if (state == 1)
  {
    isMirroring = game().getPlayer()->getX() > x;
    frame = 3;
  }
  else if (state == 2)
  {
    frame = 4;
  }


  // frame's mirroring
  if (velocity.x > 1.0f)
    isMirroring = true;
  else if (velocity.x < -1.0f)
    isMirroring = false;
}

bool CyclopEntity::hurt(int damages, enumShotType hurtingType, int level)
{
   return EnnemyEntity::hurt(damages, hurtingType, level);
}

void CyclopEntity::calculateBB()
{
    boundingBox.left = OFFSET_X + (int)x - width / 2 + 32;
    boundingBox.width = 58;
    boundingBox.top = OFFSET_Y + (int)y - height / 2 + 120;
    boundingBox.height =  90;
}

void CyclopEntity::afterWallCollide()
{
}
void CyclopEntity::collideMapRight()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void CyclopEntity::collideMapLeft()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void CyclopEntity::collideMapTop()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void CyclopEntity::collideMapBottom()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}


void CyclopEntity::dying()
{
  ItemEntity* newItem = new ItemEntity(itemBossHeart, x, y);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);
  EnnemyEntity::dying();
}


void CyclopEntity::render(sf::RenderTarget* app)
{
    EnnemyEntity::render(app);

    // stones
    if (state == 1)
    {
      sprite.setTextureRect(sf::IntRect(1152, 0,  64,  64));  // small
      //sprite.setTextureRect(sf::IntRect(1152, 64,  64,  64));  // medium
      if (isMirroring)
        sprite.setPosition(x + 60, y);
      else
        sprite.setPosition(x + 4, y);

      // medium
      /*if (isMirroring)
        sprite.setPosition(x + 60, y - 12);
      else
        sprite.setPosition(x + 4, y - 12);*/

      app->draw(sprite);

      sprite.setPosition(x, y);
    }

    float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / hpMax;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 25));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    game().write(          "Cimmerian Cyclop",
                            18,
                            OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                            OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255),
                            app, 0 , 0);
}

void CyclopEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsRecoilTo(entity);
  }
}

void CyclopEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
}
