#include "GiantSlimeEntity.h"
#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "SlimeEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>

GiantSlimeEntity::GiantSlimeEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_GIANT_SLIME), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = GIANT_SLIME_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = GIANT_SLIME_HP;
  hpDisplay = hp;
  hpMax = GIANT_SLIME_HP;
  meleeDamages = GIANT_SLIME_DAMAGES;
  missileDelay = GIANT_SLIME_MISSILE_DELAY;

  type = ENTITY_ENEMY_BOSS;
  enemyType = EnemyTypeSlimeBoss;
  bloodColor = BloodGreen;
  shadowFrame = 3;
  frame = 0;
  sprite.setOrigin(64.0f, 64.0f);
  h = 0.0f;

  age = -2.0f;

  changeToState(0);
  slimeCounter = 0;
  slimeTimer =5.0f;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceVeryHigh;

  sprite.setOrigin(64, 84);
  canExplode = false;
}

void GiantSlimeEntity::changeToState(int n)
{
  if (n == 0) // walking
  {
    state = 0;
    counter = 8 + rand() % 7;
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
      counter = 26;
    if (hp <= hpMax / 2)
      counter = 18;
    else
      counter = 12;
    timer = GIANT_SLIME_MISSILE_DELAY;
  }
  else if (n == 6) // jumping
  {
    state = 6;
    timer = 1.2f;

    viscosity = 1.0f;

    SoundManager::getInstance().playSound(SOUND_SLIME_JUMP);
    hVelocity = GIANT_SLIME_ULTRA_JUMP_VELOCITY;
  }
  else if (n == 7) // falling
  {
    isFalling = false;
    state = 7;
    timer = GIANT_SLIME_ULTRA_JUMP_TIMER;

    hVelocity = GIANT_SLIME_ULTRA_FALL_VELOCITY;
    h = 1500;
  }
}

void GiantSlimeEntity::animate(float delay)
{
  slimeTimer -= delay;
  if (slimeTimer <= 0.0f)
  {
    slimeTypeEnum slimeType = SlimeTypeStandard;
    if (game().isAdvancedLevel())
    {
      slimeType =  (slimeTypeEnum)(slimeType + rand() % 3);
    }
    switch (slimeCounter)
    {
    case 0:
      new SlimeEntity(TILE_WIDTH * 1.5f, TILE_HEIGHT * 1.5f, slimeType, true);
      break;
    case 1:
      new SlimeEntity(TILE_WIDTH * (MAP_WIDTH - 2) + TILE_WIDTH * 0.5f, TILE_HEIGHT * 1.5f, slimeType, true);
      break;
    case 2:
      new SlimeEntity(TILE_WIDTH * (MAP_WIDTH - 2) + TILE_WIDTH * 0.5f, TILE_HEIGHT * (MAP_HEIGHT - 2) + TILE_HEIGHT * 0.5f, slimeType, true);
      break;
    case 3:
      new SlimeEntity(TILE_WIDTH * 1.5f, TILE_HEIGHT * (MAP_HEIGHT - 2) + TILE_HEIGHT * 0.5f, slimeType, true);
      break;
    }
    slimeTimer = 7.0f;
    slimeCounter ++;
    if (slimeCounter == 4) slimeCounter = 0;
  }

  if (age <= 0.0f)
  {
    age += delay;
    return;
  }
  EnemyEntity::animate(delay);
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  timer -= delay;
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
        int r = rand() % 3;
        if (r == 0) changeToState(1);
        else if (r == 1) changeToState(3);
        else changeToState(5);
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
    else if (state == 5)
    {
      changeToState(6);
    }
    else if (state == 6)  // jump
    {
      changeToState(7); // fall
    }
    else if (state == 7)  // jump
    {
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
  else if (state == 6) // ultra jump
  {
    if (h < 2000)
      h += hVelocity * delay;
  }
  else if (state == 7) // ultra jump
  {
    if (!isFalling && timer <= 2.2f)
    {
      isFalling = true;
      x = game().getPlayer()->getX();
      y = game().getPlayer()->getY();
      // to prevent collisions
      float x0 = TILE_WIDTH + 1;
      float xf = TILE_WIDTH * (MAP_WIDTH - 1) - 1;
      float y0 = TILE_HEIGHT + 1;
      float yf = TILE_HEIGHT * (MAP_HEIGHT - 1) - 1;

      calculateBB();

      if (boundingBox.left < x0) x += (x0 - boundingBox.left);
      else if (boundingBox.left + boundingBox.width > xf) x -= (boundingBox.left + boundingBox.width - xf);

      if (boundingBox.top < y0) y += (y0 - boundingBox.top);
      else if (boundingBox.top + boundingBox.height > yf) y -= (boundingBox.top + boundingBox.height - yf);
    }
    if (timer < 2.3f)
    {
      h += hVelocity * delay;
      if (h <= 0)
      {
        h = 0;
        changeToState(8);
        game().makeShake(0.8f);
        SoundManager::getInstance().playSound(SOUND_CYCLOPS_IMPACT);
      }
    }
  }

  if (state == 6 && timer < 0.5f)
  {
    int fade = timer * 512;
    if (fade < 0) fade = 0;
    sprite.setColor(sf::Color(255, 255, 255, fade));
  }
  else if (state == 7 && timer < 1.5f)
    sprite.setColor(sf::Color(255, 255, 255, 255));
  else if (state == 7 && timer < 2.0f)
    sprite.setColor(sf::Color(255, 255, 255, (2.0f - timer) * 512));
  else if (state == 7)
    sprite.setColor(sf::Color(255, 255, 255, 0));

  isMirroring = (frame == 2) && (velocity.x < 0.0f);
  z = y + 26;
}

void GiantSlimeEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2 + GIANT_SLIME_BB_LEFT;
  boundingBox.width = width - GIANT_SLIME_BB_WIDTH_DIFF;
  boundingBox.top = (int)y - height / 2 + 40;
  boundingBox.height =  height - 76;
}


void GiantSlimeEntity::collideMapRight()
{
  velocity.x = -velocity.x;
}

void GiantSlimeEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
}

void GiantSlimeEntity::collideMapTop()
{
  velocity.y = -velocity.y;
}

void GiantSlimeEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
}


void GiantSlimeEntity::dying()
{
  isDying = true;
  game().addKilledEnemy(enemyType, hurtingType);
  game().addCorpse(x, y, FRAME_CORPSE_GIANT_SLIME);

  float xSlime = x;
  float ySlime = y;

  if (x <= 1.5 * TILE_WIDTH) x = 1.5f * TILE_WIDTH + 2;
  else if (x >= TILE_WIDTH * MAP_WIDTH - 1.5f * TILE_WIDTH) x = TILE_WIDTH * MAP_WIDTH - 1.5f * TILE_WIDTH -3;
  if (y <= 1.5 * TILE_HEIGHT) y = 1.5 * TILE_HEIGHT + 2;
  else if (y >= TILE_HEIGHT * MAP_HEIGHT - 1.5f * TILE_HEIGHT) y = TILE_HEIGHT * MAP_HEIGHT - 1.5f * TILE_HEIGHT -3;

  for (int i = 0; i < 9; i++)
  {
    game().generateBlood(xSlime, ySlime, bloodColor);
    SlimeEntity* slime = new SlimeEntity(x, y, SlimeTypeStandard, true);
    slime->disableCollidingTemporary();
  }

  game().makeShake(1.0f);
  SoundManager::getInstance().playSound(SOUND_SLIME_SMASH);

  ItemEntity* newItem = new ItemEntity(ItemBossHeart, x, y);
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

void GiantSlimeEntity::render(sf::RenderTarget* app)
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
      sprite.setTextureRect(sf::IntRect(frame * width + width, 0, -width, height));
    else
      sprite.setTextureRect(sf::IntRect(frame * width, 0, width, height));
  app->draw(sprite);

  renderLifeBar(app, tools::getLabel("enemy_giant_slime"));

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void GiantSlimeEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsRecoilTo(entity);
  }
}

void GiantSlimeEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  if (state == 7)
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), KING_RAT_RUNNING_RECOIL );
    targetEntity->giveRepulsion(true, recoilVector, 1.0f);
  }
}

bool GiantSlimeEntity::canCollide()
{
  return h <= 70.0f;
}

BaseCreatureEntity::enumMovingStyle GiantSlimeEntity::getMovingStyle()
{
  if (h <= 70.0f)
    return movWalking;
  else
    return movFlying;
}

void GiantSlimeEntity::fire()
{
  SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);
  EnemyBoltEntity* bolt = new EnemyBoltEntity
  (x, y + 10, ShotTypeStandard, 0, enemyType);
  bolt->setFrame(1);
  bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);

  bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(),GIANT_SLIME_FIRE_VELOCITY ));
}

