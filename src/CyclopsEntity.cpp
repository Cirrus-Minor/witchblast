#include "CyclopsEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "RockMissileEntity.h"
#include "FallingRockEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

CyclopsEntity::CyclopsEntity(float x, float y)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_CYCLOP), x, y)
{
  width = 128;
  height = 192;
  creatureSpeed = CYCLOP_SPEED[0];
  velocity = Vector2D(creatureSpeed);
  hp = CYCLOP_HP;
  hpDisplay = CYCLOP_HP;
  hpMax = CYCLOP_HP;
  meleeDamages = CYCLOP_DAMAGES;


  type = ENTITY_ENNEMY_BOSS;
  bloodColor = BloodRed;
  shadowFrame = 8;
  dyingFrame = 5;
  deathFrame = FRAME_CORPSE_CYCLOP;
  dyingSound = SOUND_CYCLOP_DIE;
  frame = 0;
  if (game().getPlayerPosition().x > x) isMirroring = true;
  sprite.setOrigin(64.0f, 128.0f);

  nextRockMissile = 0;
  destroyLevel = 0;
  state = 0;
  timer = 2.0f;
  counter = 10;
  age = -1.5f;
  enemyType = EnemyTypeCyclops;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRecoil] = ResistanceVeryHigh;
}

int CyclopsEntity::getHealthLevel()
{
  int healthLevel = 0;
  if (hp <= hpMax * 0.25) healthLevel = 3;
  else if (hp <= hpMax * 0.5) healthLevel = 2;
  else if (hp <= hpMax * 0.75) healthLevel = 1;
  return healthLevel;
}

void CyclopsEntity::fire()
{
  new RockMissileEntity(x, y - 62, nextRockMissile);
  SoundManager::getSoundManager()->playSound(SOUND_THROW);
}

void CyclopsEntity::initFallingGrid()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
      fallingGrid[i][j] = false;
}

void CyclopsEntity::fallRock()
{
  int rx, ry;
  do
  {
    rx = 1 + rand() % (MAP_WIDTH - 2);
    ry = 1 + rand() % (MAP_HEIGHT - 2);
  }
  while (fallingGrid[rx][ry]);

  fallingGrid[rx][ry] = true;
  new FallingRockEntity(rx * TILE_WIDTH + OFFSET_X + TILE_WIDTH / 2,
                        ry * TILE_HEIGHT + OFFSET_Y + TILE_HEIGHT / 2,
                        rand() % 3);
}


void CyclopsEntity::computeNextRockMissile()
{
  if (getHealthLevel() == 0)
    nextRockMissile = rand()%5 == 0 ? 1 : 0;
  else if (getHealthLevel() == 1)
    nextRockMissile = rand()%3 == 0 ? 1 : 0;
  else if (getHealthLevel() == 2)
    nextRockMissile = rand()%2 == 0 ? 0 : 1;
  else
    nextRockMissile = rand()%3 == 0 ? 0 : 1;
}

void CyclopsEntity::computeStates(float delay)
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
        velocity.x = 0.0f;
        velocity.y = 0.0f;
        if (destroyLevel < getHealthLevel())
        {
          state = 3; // charge to destroy
          destroyLevel++;
          counter = destroyLevel + 1;
          timer = 0.9f;
          SoundManager::getSoundManager()->playSound(SOUND_CYCLOP_00);
          initFallingGrid();
        }
        else
        {
          state = 1; // charge to fire
          timer = 0.9f;
          counter = CYCLOP_NUMBER_ROCKS[getHealthLevel()];
          SoundManager::getSoundManager()->playSound(SOUND_CYCLOP_00);

          computeNextRockMissile();
        }
      }
    }
    else if (state == 1) // fire
    {
      state = 2;
      fire();
      timer = CYCLOP_FIRE_DELAY[getHealthLevel()];
    }
    else if (state == 2) // fire end
    {
      if (counter <= 1)
      {
        state = 0;
        timer = 0.2f;
        counter = 10;
      }
      else
      {
        counter--;
        state = 1;
        timer = 0.2f;
        computeNextRockMissile();
      }
    }
    else if (state == 3)
    {
      state = 4; // destroy
      timer = 0.2;
      game().makeShake(0.4f);
      SoundManager::getSoundManager()->playSound(SOUND_WALL_IMPACT);
      for (int i = 0; i < 10 ; i++) fallRock();
    }
    else if (state == 4)
    {
      if (counter <= 1)
      {
        state = 0;
        timer = 0.2f;
        counter = 10;
      }
      else
      {
        counter--;
        state = 3;
        timer = 0.3f;
      }
    }
  }
}

void CyclopsEntity::animate(float delay)
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
      corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), x, y, 128, 128);
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
  else if (state == 3)
  {
    frame = 6;
  }
  else if (state == 4)
  {
    frame = 7;
  }

  // frame's mirroring
  if (velocity.x > 1.0f)
    isMirroring = true;
  else if (velocity.x < -1.0f)
    isMirroring = false;

  z = OFFSET_Y + y + 46;
}

bool CyclopsEntity::hurt(int damages, enumShotType hurtingType, int level, bool critical)
{
  if (destroyLevel < getHealthLevel()) damages /= 3;
  return EnemyEntity::hurt(damages, hurtingType, level, critical);
}

void CyclopsEntity::calculateBB()
{
  boundingBox.left = OFFSET_X + (int)x - 32;
  boundingBox.width = 58;
  boundingBox.top = OFFSET_Y + (int)y - 42;
  boundingBox.height =  90;
}

void CyclopsEntity::afterWallCollide()
{
}
void CyclopsEntity::collideMapRight()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void CyclopsEntity::collideMapLeft()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void CyclopsEntity::collideMapTop()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void CyclopsEntity::collideMapBottom()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void CyclopsEntity::drop()
{
  ItemEntity* newItem = new ItemEntity(ItemBossHeart, x, y);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);
}

void CyclopsEntity::render(sf::RenderTarget* app)
{
  EnemyEntity::render(app);

  // stones
  if (state == 1)
  {
    if (nextRockMissile == 0) // small rock
    {
      sprite.setTextureRect(sf::IntRect(1152, 0,  64,  64));
      if (isMirroring)
        sprite.setPosition(x + 60, y);
      else
        sprite.setPosition(x + 4, y);
    }
    else // medium rock
    {
      sprite.setTextureRect(sf::IntRect(1152, 64,  64,  64));
      if (isMirroring)
        sprite.setPosition(x + 60, y - 12);
      else
        sprite.setPosition(x + 4, y - 12);
    }

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

  game().write(          "Cimmerian Cyclops",
                         18,
                         OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                         OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                         ALIGN_LEFT,
                         sf::Color(255, 255, 255),
                         app, 0 , 0);
}

void CyclopsEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsRecoilTo(entity);
  }
}

void CyclopsEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
}
