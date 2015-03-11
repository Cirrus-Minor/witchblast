#include "VampireEntity.h"
#include "BatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>

const int VAMPIRE_HP = 800;
const int VAMPIRE_DAMAGE = 12;
const float VAMPIRE_FLYING_DELAY = 1.6f;
const float VAMPIRE_BAT_DELAY = 0.3f;
const float VAMPIRE_CONFUSION_DELAY = 5.0f;

VampireEntity::VampireEntity(float myx, float myy)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_VAMPIRE), myx, myy)
{
  width = 96;
  height = 96;
  creatureSpeed = 150;
  hp = VAMPIRE_HP;
  hpDisplay = VAMPIRE_HP;
  hpMax = VAMPIRE_HP;
  meleeDamages = VAMPIRE_DAMAGE;

  type = ENTITY_ENEMY_BOSS;
  dyingFrame = 12;
  //deathFrame = FRAME_CORPSE_CYCLOP;
  //agonizingSound = SOUND_CYCLOP_DIE;
  frame = 0;
  if (game().getPlayerPosition().x > x) isMirroring = true;
  sprite.setOrigin(48.0f, 74.0f);
  imagesProLine = 6;

  state = 0;
  timer = 4.0f;
  age = -1.5f;
  enemyType = EnemyTypeVampire;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRecoil] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceImmune;
  canExplode = false;

  x = GAME_WIDTH * 0.5f;
  y = GAME_HEIGHT * 0.5f;

  if (game().getPlayer()->getX() < 2 * TILE_WIDTH)
  {
    x = GAME_WIDTH - 2.5f * TILE_WIDTH;
    targetPos = 4;
  }
  else if (game().getPlayer()->getX() > GAME_WIDTH - 2 * TILE_WIDTH)
  {
    x = 2.5f * TILE_WIDTH;
    targetPos = 6;
  }
  else if (game().getPlayer()->getY() < 2 * TILE_HEIGHT)
  {
    y = GAME_HEIGHT - 2.5f * TILE_HEIGHT;
    targetPos = 2;
  }
  else if (game().getPlayer()->getY() > GAME_HEIGHT - 2 * TILE_HEIGHT)
  {
    y = 2.5f * TILE_HEIGHT;
    targetPos = 8;
  }
}

int VampireEntity::getHealthLevel()
{
  int healthLevel = 0;
  if (hp <= hpMax * 0.25) healthLevel = 3;
  else if (hp <= hpMax * 0.5) healthLevel = 2;
  else if (hp <= hpMax * 0.75) healthLevel = 1;
  return healthLevel;
}

void VampireEntity::computeStates(float delay)
{
  timer -= delay;

  if (timer <= 0.0f)
  {
    if (state == 0) // waiting bef hypnose
    {
      state = 1; // hypnose
      timer = 2.0f;
      //game().makeColorEffect(X_GAME_COLOR_RED, 0.2f);
      game().getPlayer()->setSpecialState(SpecialStateConfused, true, VAMPIRE_CONFUSION_DELAY, VAMPIRE_CONFUSION_DELAY, 0.0f);
    }
    else if (state == 1) // hypnose
    {
      state = 2;
      timer = 2.0f; //5.0f;
    }
    else if (state == 2) // waiting before laughing
    {
      state = 3; // laughing
      timer = 2.0f; // 3.0f;
    }
    else if (state == 3) // laughing
    {
      state = 4;
      timer = 1.0f;
    }
    else if (state == 4) // to bat cloud
    {
      state = 5;
      timer = 1.0f;
    }
    else if (state == 5) // to bat cloud
    {
      state = 6;
      timer = VAMPIRE_FLYING_DELAY;
      batTimer = VAMPIRE_BAT_DELAY;
      xSource = x;
      ySource = y;
      if (targetPos == 4 || targetPos == 6)
      {
        targetPos = rand() % 2 == 0 ? 2 : 8;
      }
      else
      {
        targetPos = rand() % 2 == 0 ? 4 : 6;
      }
    }
    else if (state == 6) // to bat cloud
    {
      state = 0;
      timer = 2.0f;
    }
    else
    {
      state = 0;
      timer = 5.0f;
    }
  }
}

void VampireEntity::animate(float delay)
{
  computeStates(delay);
  if (state == 0)
  {
    if (timer < 0.2f) frame = 1;
    else frame = 0;
  }
  else if (state == 1)  // hypnose
  {
    frame = 2;
  }
  else if (state == 2 || state == 4)  // waiting
  {
    frame = 0;
  }
  else if (state == 3)  // laughing
  {
    frame = 3 + ((int)(age * 7.0f)) % 2;
  }
  else if (state == 5)  // to bat cloud
  {
    if (timer > 0.8f) frame = 0;
    else if (timer > 0.6f) frame = 6;
    else if (timer > 0.4f) frame = 7;
    else if (timer > 0.2f) frame = 8;
    else  frame = 9;
  }
  else if (state == 6)
  {
    calculatePosition();
    batTimer -= delay;
    if (batTimer <= 0.0f)
    {
      batTimer += VAMPIRE_BAT_DELAY;
      isMirroring = !isMirroring;
      BatEntity* bat = new BatEntity(x, y, true);
      bat->setAge(0.0f);
    }
  }

  EnemyEntity::animate(delay);
  /*if (age <= 0.0f)
  {
    age += delay;
    return;
  }

  if (isAgonising)
  {
    if (h < -0.01f)
    {
      isDying = true;
      game().addCorpse(x, y, deathFrame);
      if (dyingSound != SOUND_NONE) SoundManager::getInstance().playSound(dyingSound);
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
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  // IA
  //computeStates(delay);

  // collisions
  if (canCollide()) testSpriteCollisions();
  BaseCreatureEntity::animate(delay);

  // old frame (for sound)
  int oldFrame = frame;

  // current frame
  if (state == 0)
  {

  }

  // frame's mirroring
  if (velocity.x > 1.0f)
    isMirroring = true;
  else if (velocity.x < -1.0f)
    isMirroring = false;
*/
  z = y + 16;
}

void VampireEntity::calculatePosition()
{
  float xTarget, yTarget;

  if (targetPos == 4) xTarget = 2.5f * TILE_WIDTH;
  else if (targetPos == 6) xTarget = GAME_WIDTH - 2.5f * TILE_WIDTH;
  else xTarget = GAME_WIDTH * 0.5f;

  if (targetPos == 8) yTarget = 2.5f * TILE_HEIGHT;
  else if (targetPos == 2) yTarget = GAME_HEIGHT - 2.5f * TILE_HEIGHT;
  else yTarget = GAME_HEIGHT * 0.5f;

  x = xSource + (VAMPIRE_FLYING_DELAY - timer) / VAMPIRE_FLYING_DELAY * (xTarget - xSource);
  y = ySource + (VAMPIRE_FLYING_DELAY - timer) / VAMPIRE_FLYING_DELAY * (yTarget - ySource);
}

int VampireEntity::hurt(StructHurt hurtParam)
{
  if (state == 6) armor = 1.0f;
  else armor = 0.0f;
  return EnemyEntity::hurt(hurtParam);
}

void VampireEntity::calculateBB()
{
  boundingBox.left = (int)x - 16;
  boundingBox.width = 32;
  boundingBox.top = (int)y - 16;
  boundingBox.height =  32;
}


void VampireEntity::drop()
{
  ItemEntity* newItem = new ItemEntity(ItemBossHeart, x, y);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);
}

void VampireEntity::render(sf::RenderTarget* app)
{
  // shadow
  /*sprite.setPosition(x, y);
  sprite.setTextureRect(sf::IntRect(8 * width, 0, width, height));
  app->draw(sprite);
  sprite.setPosition(x, y);*/

  EnemyEntity::render(app);

  if (state == 1) // hypnose
  {
    sf::Sprite eye;
    eye.setOrigin(6, 6);
    eye.setTexture(*ImageManager::getInstance().getImage(IMAGE_VAMPIRE));
    eye.setTextureRect(sf::IntRect(5 * width, 0, 12, 12));
    eye.setRotation(age * 500);
    if (isMirroring) eye.setPosition(x + 10, y - 44);
    else  eye.setPosition(x - 10, y - 44);
    app->draw(eye);

    if (isMirroring) eye.setPosition(x - 4, y - 44);
    else  eye.setPosition(x + 4, y - 44);
    app->draw(eye);
  }



  renderLifeBar(app, tools::getLabel("enemy_vampire"));
}

void VampireEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

  if (playerEntity != NULL && !playerEntity->isDead())
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 450.0f);
    targetEntity->giveRecoil(true, recoilVector, 0.5f);
  }
}
