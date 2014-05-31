#include "GiantSlimeEntity.h"
#include "BoltEntity.h"
#include "EnnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "SlimeEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

GiantSlimeEntity::GiantSlimeEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_GIANT_SLIME), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = GIANT_SLIME_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = GIANT_SLIME_HP;
  hpDisplay = hp;
  hpMax = GIANT_SLIME_HP;
  meleeDamages = GIANT_SLIME_DAMAGES;

  type = ENTITY_ENNEMY_BOSS;
  bloodColor = bloodGreen;
  shadowFrame = 3;
  frame = 0;
  sprite.setOrigin(64.0f, 64.0f);
  h = 0.0f;

  age = 0.0f;

  changeToState(0);
  slimeCounter = 0;
  slimeTimer =5.0f;

  specialState[SpecialStateIce].resistance = ResistanceImmune;
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
  else if (n == 1 || n == 3) // waiting
  {
    state = n;
    timer = 2.0f;
    setVelocity(Vector2D(0.0f, 0.0f));
  }
  else if (n == 2) // jumping
  {
    state = 2;
    timer = 4.0f;

    viscosity = 0.991f;

    SoundManager::getSoundManager()->playSound(SOUND_SLIME_JUMP);
    hVelocity = 380.0f + rand() % 420;

    isFirstJumping = true;

    float randVel = 250.0f + rand() % 250;

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
    counter = 10;
    timer = GIANT_SLIME_MISSILE_DELAY;
  }
}

void GiantSlimeEntity::animate(float delay)
{
  slimeTimer -= delay;
  if (slimeTimer <= 0.0f)
  {
    switch (slimeCounter)
    {
      case 0: new SlimeEntity(OFFSET_X + TILE_WIDTH * 1.5f, OFFSET_Y + TILE_HEIGHT * 1.5f, true); break;
      case 1: new SlimeEntity(OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 2) + TILE_WIDTH * 0.5f, OFFSET_Y + TILE_HEIGHT * 1.5f, true); break;
      case 2: new SlimeEntity(OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 2) + TILE_WIDTH * 0.5f, OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 2) + TILE_HEIGHT * 0.5f, true); break;
      case 3: new SlimeEntity(OFFSET_X + TILE_WIDTH * 1.5f, OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 2) + TILE_HEIGHT * 0.5f, true); break;
    }
    slimeTimer = 6.0f;
    slimeCounter ++;
    if (slimeCounter == 4) slimeCounter = 0;
  }

  timer -= delay;
  if (timer <= 0.0f)
  {
    if (state == 0) // walking
    {
      counter--;
      if (counter >= 0)
      {
        timer = 0.5f;
        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(),GIANT_SLIME_SPEED ));
      }
      else
      {
        changeToState(1);
      }
    }
    else if (state == 1) // waiting
    {
      changeToState(2);; // jumping
      //counter = 8 + rand() % 7;
    }
    else if (state == 2)
    {
      changeToState(3);
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
        timer = GIANT_SLIME_MISSILE_DELAY;
        fire();
      }
      else
      {
        changeToState(0);
      }
    }
  }


  if (state == 0) // walking
  {
    frame = ((int)(age * 2.0f)) % 2;
  }
  else if (state == 1) // waiting to jump
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
          SoundManager::getSoundManager()->playSound(SOUND_SLIME_IMAPCT);
        }
        else
        {
          //changeToState(0);
          SoundManager::getSoundManager()->playSound(SOUND_SLIME_IMAPCT_WEAK);
        }
      }
    }
    if (hVelocity > 0.0f) frame = 2;
    else frame = 0;
  }

//  else if (state == 0)

  EnnemyEntity::animate(delay);
}

bool GiantSlimeEntity::hurt(int damages)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;

  if (state == 6)
    hp -= damages / 4;
  else
    hp -= damages;

  if (hp <= 0)
  {
    hp = 0;
    dying();
  }
   return true;
}

void GiantSlimeEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + GIANT_SLIME_BB_LEFT;
    boundingBox.width = width - GIANT_SLIME_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + GIANT_SLIME_BB_TOP;
    boundingBox.height =  height - GIANT_SLIME_BB_HEIGHT_DIFF - GIANT_SLIME_BB_TOP;
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
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), x, y, 128, 128);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_KING_RAT - FRAME_CORPSE_KING_RAT);
  deadRat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 10; i++) game().generateBlood(x, y, bloodColor);

  SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_DIE);

  ItemEntity* newItem = new ItemEntity(itemBossHeart, x, y);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);
}

void GiantSlimeEntity::render(sf::RenderTarget* app)
{
    //EnnemyEntity::render(app);
    if (!isDying && shadowFrame > -1)
    {
      // shadow
      sprite.setPosition(x, y);
      sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
      app->draw(sprite);
    }
    sprite.setPosition(x, y - h);
    sprite.setTextureRect(sf::IntRect(frame * width, 0, width, height));
    app->draw(sprite);

    float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / KING_RAT_HP;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 25));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    game().Write(          "Giant Slime",
                            18,
                            OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                            OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255),
                            app);
}

void GiantSlimeEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsRecoilTo(entity);
  }
}

void GiantSlimeEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  if (state == 4 || state == 6)
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), KING_RAT_RUNNING_RECOIL );
    targetEntity->giveRecoil(true, recoilVector, 1.0f);
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
    SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);
    EnnemyBoltEntity* bolt = new EnnemyBoltEntity
          (ImageManager::getImageManager()->getImage(IMAGE_BOLT), x, y + 10);
    bolt->setFrame(1);
    bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

    bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(),GIANT_SLIME_FIRE_VELOCITY ));
}
