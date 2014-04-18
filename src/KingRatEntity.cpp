#include "KingRatEntity.h"
#include "GreenRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "StaticTextEntity.h"

#include <iostream>

KingRatEntity::KingRatEntity(float x, float y, GameMap* map, PlayerEntity* player)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_KING_RAT), x, y, map)
{
  width = 128;
  height = 128;
  creatureSpeed = KING_RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = KING_RAT_HP;
  hpDisplay = hp;
  hpMax = KING_RAT_HP;
  meleeDamages = KING_RAT_DAMAGES;

  type = 21;
  bloodColor = bloodRed;
  shadowFrame = 4;
  frame = 0;
  sprite.setOrigin(64.0f, 64.0f);
  this->player = player;

  state = 0;
  timer = 2.0f + (rand() % 40) / 10.0f;
  age = 0.0f;

  berserkDelay = 1.0f + rand()%5 * 0.1f;

  hasBeenBerserk = false;
}

void KingRatEntity::animate(float delay)
{
  float timerMult = 1.0f;
  if (hp <= hpMax / 4)
  {
    creatureSpeed = KING_RAT_SPEED * 1.4f;
    timerMult = 0.7f;
  }
  else if (hp <= hpMax / 2)
  {
    creatureSpeed = KING_RAT_SPEED * 1.2f;
    timerMult = 0.85f;
  }
  else
  {
    creatureSpeed = KING_RAT_SPEED;
  }

  if (state == 5)
  {
    velocity.x *= 0.965f;
    velocity.y *= 0.965f;
  }

  else
  {
    if (velocity.x > 0.1f) sprite.setScale(-1.0f, 1.0f);
    else if (velocity.x < -0.1f) sprite.setScale(1.0f, 1.0f);
  }


  timer -= delay;

  if (timer <= 0.0f)
  {
    if (state == 0)
    {
      state = 1;
      // generate rats
      generateGreenRats();
      SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_1);
      timer = 2.0f;

      velocity.x = 0.0f;
      velocity.y = 0.0f;
    }

    else if (state == 1) // generate rats
    {
      // to normal or berserk
      if (hp < hpMax / 4 && !hasBeenBerserk)
      {
        hasBeenBerserk = true;
        timer = 12.0f;
        state = 6;
        velocity = Vector2D(KING_RAT_BERSERK_SPEED);
        SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_2);
      }
      else
      {
        timer = 7.0f * timerMult;
        velocity = Vector2D(creatureSpeed);
        state = 2;
      }
    }

    else if (state == 2) // normal -> rush
    {
      state = 3;
      // angry
      timer = 2.0f;

      velocity.x = 0.0f;
      velocity.y = 0.0f;
      SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_2);
    }

    else if (state == 3) // normal -> rush
    {
      state = 4;
      // rush
      timer = 12.0f;

      float tan = (player->getX() - x) / (player->getY() - y);
      float angle = atan(tan);

      if (player->getY() > y)
        setVelocity(Vector2D(sin(angle) * KING_RAT_RUNNING_SPEED,
                                       cos(angle) * KING_RAT_RUNNING_SPEED));
      else
        setVelocity(Vector2D(-sin(angle) * KING_RAT_RUNNING_SPEED,
                                       -cos(angle) * KING_RAT_RUNNING_SPEED));
    }
    else if (state == 5) // wall impact
    {
      // to normal or berserk
      if (hp < hpMax / 4 && !hasBeenBerserk)
      {
        hasBeenBerserk = true;
        timer = 12.0f;
        state = 6;
        velocity = Vector2D(KING_RAT_BERSERK_SPEED);
        SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_2);
      }
      else
      {
        timer = 7.0f * timerMult;
        velocity = Vector2D(creatureSpeed);
        state = 0;
      }
    }

    else if (state == 6)
    {
      timer = 7.0f * timerMult;
      velocity = Vector2D(creatureSpeed);
      state = 0;
    }
  }

  if (state == 6)
  {
    berserkDelay -= delay;

    if (berserkDelay <= 0.0f)
    {
      berserkDelay = 0.8f + (rand()%10) / 10.0f;
      SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_2);

      float tan = (player->getX() - x) / (player->getY() - y);
      float angle = atan(tan);

      if (player->getY() > y)
        setVelocity(Vector2D(sin(angle) * KING_RAT_BERSERK_SPEED,
                                       cos(angle) * KING_RAT_BERSERK_SPEED));
      else
        setVelocity(Vector2D(-sin(angle) * KING_RAT_BERSERK_SPEED,
                                       -cos(angle) * KING_RAT_BERSERK_SPEED));
    }
  }

  frame = 0;

  if (state == 1)
    frame = 3;
  else if (state == 3 || state == 6)
  {
    frame = 3; //0;
    int r = ((int)(age * 10.0f)) % 2;
    if (r == 0)
      sprite.setScale(-1.0f, 1.0f);
    else
      sprite.setScale(1.0f, 1.0f);
  }
  else if (state == 4)
  {
    int r = ((int)(age * 7.5f)) % 4;
    if (r == 1) frame = 1;
    else if (r == 3) frame = 2;
  }
  else if (state == 5)
  {
    frame = 0;
  }
  else
  {
    int r = ((int)(age * 5.0f)) % 4;
    if (r == 1) frame = 1;
    else if (r == 3) frame = 2;
  }

  EnnemyEntity::animate(delay);
}

void KingRatEntity::hurt(int damages)
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
}

void KingRatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + 25;
    boundingBox.width = width - 50;
    boundingBox.top = (int)y - height / 2 + 25;
    boundingBox.height =  height - 35;
}


void KingRatEntity::afterWallCollide()
{
    if (state == 4)
    {
      state = 5;
      timer = 1.4f;
      velocity.x *= 0.75f;
      velocity.y *= 0.75f;
      //velocity = Vector2D(creatureSpeed);
      SoundManager::getSoundManager()->playSound(SOUND_BIG_WALL_IMPACT);
    }
}
void KingRatEntity::collideMapRight()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void KingRatEntity::collideMapLeft()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void KingRatEntity::collideMapTop()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void KingRatEntity::collideMapBottom()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}


void KingRatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), x, y, 128, 128);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_KING_RAT - FRAME_CORPSE_KING_RAT);
  deadRat->setType(TYPE_CORPSE);

  for (int i = 0; i < 10; i++) parentGame->generateBlood(x, y, bloodColor);

  SoundManager::getSoundManager()->playSound(SOUND_KING_RAT_DIE);
}

void KingRatEntity::generateGreenRats()
{
  for (int i = 0; i < 5; i++)
  {
    float xr = x + -100 + rand() % 200;
    float yr = y + -100 + rand() % 200;

    if (xr > OFFSET_X + TILE_WIDTH * 1.5f && xr < OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 2)
        && yr > OFFSET_Y + TILE_HEIGHT * 1.5f && yr < OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 2))
    {
      new GreenRatEntity(xr, yr, map, player);
    }
    else
      i--;
  }
}

void KingRatEntity::render(sf::RenderWindow* app)
{
    EnnemyEntity::render(app);

    if (state == 6)
    {
      int r = ((int)(age *12.0f)) % 2;
      if (r == 0)
        sprite.setTextureRect(sf::IntRect(1 * width, 1 * height,  width,  height));
      else
        sprite.setTextureRect(sf::IntRect(2 * width, 1 * height,  -width,  height));

      sprite.setPosition(x, y);
      sprite.setColor(sf::Color(255, 255, 255, 190));
      app->draw(sprite);
      sprite.setColor(sf::Color(255, 255, 255, 255));
    }

    float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / KING_RAT_HP;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 25));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    StaticTextEntity::Write(app,
                            "Rat King",
                            18,
                            OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                            OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255));
}
