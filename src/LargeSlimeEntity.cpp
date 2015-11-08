#include "LargeSlimeEntity.h"
#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>

LargeSlimeEntity::LargeSlimeEntity(float x, float y, slimeTypeEnum slimeType)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_GIANT_SLIME), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = GIANT_SLIME_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = GIANT_SLIME_HP / 2;
  hpDisplay = hp;
  hpMax = hp;
  meleeDamages = GIANT_SLIME_DAMAGES;
  missileDelay = GIANT_SLIME_MISSILE_DELAY;

  type = ENTITY_ENEMY;
  bloodColor = BloodGreen;
  shadowFrame = 3;
  imagesProLine = 5;
  h = 0.0f;

  age = -1.0f;
  this->slimeType = slimeType;

  changeToState(0);

  switch (slimeType)
  {
  case SlimeTypeBlue:
    resistance[ResistanceFrozen] = ResistanceVeryHigh;
    resistance[ResistanceRepulsion] = ResistanceVeryHigh;
    resistance[ResistancePoison] = ResistanceHigh;
    resistance[ResistanceIce] = ResistanceVeryHigh;
    resistance[ResistanceFire] = ResistanceVeryLow;
    enemyType = EnemyTypeSlimeBlueLarge;
    meleeType = ShotTypeIce;
    fireSound = SOUND_BLAST_ICE;
    fireType  = ShotTypeIce;
    fireDamage = 8;
    baseFrame = 10;
    break;

  case SlimeTypeRed:
    resistance[ResistanceFrozen] = ResistanceHigh;
    resistance[ResistanceRepulsion] = ResistanceVeryHigh;
    resistance[ResistancePoison] = ResistanceHigh;
    resistance[ResistanceIce] = ResistanceVeryLow;
    resistance[ResistanceFire] = ResistanceVeryHigh;
    enemyType = EnemyTypeSlimeRedLarge;
    fireSound = SOUND_BLAST_FIRE;
    fireType  = ShotTypeFire;
    fireDamage = 12;
    baseFrame = 5;
    break;

  case SlimeTypeViolet:
    resistance[ResistanceFrozen] = ResistanceHigh;
    resistance[ResistanceRepulsion] = ResistanceVeryHigh;
    resistance[ResistancePoison] = ResistanceHigh;
    enemyType = EnemyTypeSlimeLarge;
    fireSound = SOUND_BLAST_FLOWER;
    fireType  = ShotTypeStandard;
    fireDamage = 8;
    baseFrame = 15;
    break;

  case SlimeTypeStandard:
    resistance[ResistanceFrozen] = ResistanceHigh;
    resistance[ResistanceRepulsion] = ResistanceVeryHigh;
    resistance[ResistancePoison] = ResistanceHigh;
    enemyType = EnemyTypeSlimeLarge;
    fireSound = SOUND_BLAST_FLOWER;
    fireType  = ShotTypeStandard;
    fireDamage = 8;
    baseFrame = 0;
    break;
  }

  frame = baseFrame;
  sprite.setOrigin(64, 84);
  canExplode = false;
}

void LargeSlimeEntity::changeToState(int n)
{
  if (n == 0) // walking
  {
    state = 0;
    counter = 5 + rand() % 7;
    timer = -1.0f;
    viscosity = 1.0f;
  }
  else if (n == 1 || n == 3 || n == 5 || n == 8) // waiting
  {
    state = n;
    timer = 1.2f;
    setVelocity(Vector2D(0.0f, 0.0f));
  }
  else if (n == 2) // jumping
  {
    state = 2;
    timer = 4.0f;

    viscosity = 0.991f;

    SoundManager::getInstance().playSound(SOUND_SLIME_JUMP);
    hVelocity = 420.0f + rand() % 380;

    isFirstJumping = true;

    float randVel = 350.0f + rand() % 200;

    if (rand() % 2 == 0)
    {
      float tan = (game().getPlayer()->getX() - x) / (game().getPlayer()->getY() - y);
      float angle = atan(tan);

      if (game().getPlayer()->getY() > y)
        setVelocity(Vector2D(sin(angle) * randVel,
                             cos(angle) * randVel));
      else
        setVelocity(Vector2D(-sin(angle) * randVel,
                             -cos(angle) * randVel));
    }
    else
      velocity = Vector2D(randVel);
  }
  else if (n == 4) // walking
  {
    state = 4;
    if (hp <= hpMax / 4)
      counter = 10;
    if (hp <= hpMax / 2)
      counter = 8;
    else
      counter = 6;
    timer = GIANT_SLIME_MISSILE_DELAY;
  }
}

void LargeSlimeEntity::animate(float delay)
{
  if (age <= 0.0f)
  {
    age += delay;
    return;
  }
  EnemyEntity::animate(delay);
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  timer -= delay;
  frame = 0;
  if (timer <= 0.0f)
  {
    if (state == 0) // walking
    {
      counter--;
      if (counter >= 0)
      {
        timer = 0.5f;
        if (hp <= hpMax / 4)
          creatureSpeed = GIANT_SLIME_SPEED * 1.4f;
        if (hp <= hpMax / 2)
          creatureSpeed = GIANT_SLIME_SPEED * 1.2f;
        else
          creatureSpeed = GIANT_SLIME_SPEED;

        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), GIANT_SLIME_SPEED ));
      }
      else
      {
        int r = rand() % 2;
        if (r == 0) changeToState(1);
        else changeToState(3);
      }
    }
    else if (state == 1) // waiting for jumping
    {
      changeToState(2);
    }
    else if (state == 2) // jumping
    {
      changeToState(8);
    }
    else if (state == 3)
    {
      changeToState(4);
    }
    else if (state == 4) // walking
    {
      counter--;
      if (counter >= 0)
      {
        if (hp <= hpMax / 4)
          timer = missileDelay * 0.6f;
        if (hp <= hpMax / 2)
          timer = missileDelay * 0.8f;
        else
          timer = missileDelay;
        fire();
      }
      else
      {
        changeToState(8);
      }
    }
    else if (state == 8)  // jump
    {
      changeToState(0); // fall
    }
  }

  if (state == 0) // walking
  {
    frame = ((int)(age * 2.0f)) % 2;
  }
  else if (state == 1 || state == 5) // waiting to jump
  {
    if (timer < 0.25f)
      frame = 1;
    else
      frame = 0;
  }
  else if (state == 2)  // jumping
  {
    hVelocity -= 700.0f * delay;

    h += hVelocity * delay;

    if (h <= 0.0f)
    {
      if (hp <= 0)
        dying();
      else
      {
        h = 0.0f;
        if (isFirstJumping)
        {
          isFirstJumping = false;
          hVelocity = 160.0f;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
        }
        else
        {
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
          viscosity = 0.96f;
          changeToState(0);
        }
      }
    }
    if (hVelocity > 0.0f) frame = 2;
    else frame = 0;
  }

  isMirroring = (frame == 2) && (velocity.x < 0.0f);
  frame += baseFrame;

  z = y + 26;
}

void LargeSlimeEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + GIANT_SLIME_BB_LEFT;
  boundingBox.width = width - GIANT_SLIME_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - height / 2 + 40;
  boundingBox.height =  height - 76;
}


void LargeSlimeEntity::collideMapRight()
{
  velocity.x = -velocity.x;
}

void LargeSlimeEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
}

void LargeSlimeEntity::collideMapTop()
{
  velocity.y = -velocity.y;
}

void LargeSlimeEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
}


void LargeSlimeEntity::dying()
{
  isDying = true;
  game().addKilledEnemy(enemyType, hurtingType);
  if (slimeType == SlimeTypeBlue) game().addCorpse(x, y, FRAME_CORPSE_GIANT_SLIME_BLUE);
  else if (slimeType == SlimeTypeRed) game().addCorpse(x, y, FRAME_CORPSE_GIANT_SLIME_RED);
  else if (slimeType == SlimeTypeStandard) game().addCorpse(x, y, FRAME_CORPSE_GIANT_SLIME);

  float xSlime = x;
  float ySlime = y;

  if (x <= 1.5 * TILE_WIDTH) x = 1.5f * TILE_WIDTH + 2;
  else if (x >= TILE_WIDTH * MAP_WIDTH - 1.5f * TILE_WIDTH) x = TILE_WIDTH * MAP_WIDTH - 1.5f * TILE_WIDTH -3;
  if (y <= 1.5 * TILE_HEIGHT) y = 1.5 * TILE_HEIGHT + 2;
  else if (y >= TILE_HEIGHT * MAP_HEIGHT - 1.5f * TILE_HEIGHT) y = TILE_HEIGHT * MAP_HEIGHT - 1.5f * TILE_HEIGHT -3;

  for (int i = 0; i < 9; i++)
  {
    game().generateBlood(xSlime, ySlime, bloodColor);
    if (i % 2 == 0)
    {
      SlimeEntity* slime;
      switch (slimeType)
      {
        case SlimeTypeBlue: slime = new SlimeEntity(x, y, SlimeTypeBlue, true); break;
        case SlimeTypeRed: slime = new SlimeEntity(x, y, SlimeTypeRed, true); break;
        case SlimeTypeViolet: slime = new SlimeEntity(x, y, SlimeTypeViolet, true); break;
        default: slime = new SlimeEntity(x, y, SlimeTypeStandard, true); break;
      }
      slime->disableCollidingTemporary();
    }
  }

  game().makeShake(1.0f);
  SoundManager::getInstance().playSound(SOUND_SLIME_SMASH);

  ItemEntity* newItem = new ItemEntity(ItemSilverCoin, x, y);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);

  SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_GIANT_SLIME), x, y, 128, 128, 8);
  star->setFrame(4);
  star->setFading(true);
  star->setZ(y+ 100);
  star->setAge(-0.4f);
  star->setLifetime(0.3f);
  star->setType(ENTITY_EFFECT);
  star->setSpin(400.0f);
}

void LargeSlimeEntity::render(sf::RenderTarget* app)
{
  if (!isDying)
  {
    // shadow
    sprite.setPosition(x, y);
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect(shadowFrame * width + width, 0, -width, height));
    else
      sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (frame % 5) * width + width, (frame / 5) * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect((frame % 5) * width, (frame / 5) * height, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void LargeSlimeEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsRepulsionTo(entity);
  }
}

void LargeSlimeEntity::inflictsRepulsionTo(BaseCreatureEntity* targetEntity)
{
  if (state == 7)
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), KING_RAT_RUNNING_RECOIL );
    targetEntity->giveRepulsion(true, recoilVector, 1.0f);
  }
}

bool LargeSlimeEntity::canCollide()
{
  return h <= 70.0f;
}

BaseCreatureEntity::enumMovingStyle LargeSlimeEntity::getMovingStyle()
{
  if (h <= 70.0f)
    return movWalking;
  else
    return movFlying;
}

void LargeSlimeEntity::fire()
{
  SoundManager::getInstance().playSound(fireSound);
  EnemyBoltEntity* bolt = new EnemyBoltEntity(x, y + 10, fireType, 0, enemyType);
  bolt->setDamages(fireDamage);
  bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(),GIANT_SLIME_FIRE_VELOCITY ));
}

