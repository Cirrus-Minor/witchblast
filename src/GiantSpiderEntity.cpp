#include "GiantSpiderEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

GiantSpiderEntity::GiantSpiderEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_GIANT_SPIDER), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = 180.0f;
  velocity = Vector2D(0.0f, 0.0f);

  hp = 400;
  hpMax = hp;
  hpDisplay = hp;

  meleeDamages = 12;

  type = ENTITY_ENNEMY;
  bloodColor = bloodGreen;
  shadowFrame = 3;
  dyingFrame = 4;
  deathFrame = FRAME_CORPSE_GIANT_SPIDER;
  //agonizingSound = SOUND_RAT_DYING;
  sprite.setOrigin(64.0f, 64.0f);

  h = 2000;
  state = 0;
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
      }
    }
    else if (state == 1) // wait after falling
    {
      timer -= delay;
      if (timer <= 0.0f)
      {
        state = 2;
        velocity = Vector2D(creatureSpeed);
        timer = 8.0f;
      }
    }
    else if (state == 2) // moving
    {
      timer -= delay;
      if (timer <= 0.0f)
      {
        state = 3;
        velocity = Vector2D(0.0f, 0.0f);
        timer = 1.0f;
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
        state = 0;
      }
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

  EnnemyEntity::animate(delay);
  z = y + 40;
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
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void GiantSpiderEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
  else computeFacingDirection();
}

void GiantSpiderEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void GiantSpiderEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
  else computeFacingDirection();
}

void GiantSpiderEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
    computeFacingDirection();
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

    if (state == 0 || state == 4)
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

  float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / hpMax;

  sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
  rectangle.setFillColor(sf::Color(0, 0, 0,128));
  rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
  app->draw(rectangle);

  rectangle.setSize(sf::Vector2f(l, 25));
  rectangle.setFillColor(sf::Color(190, 20, 20));
  rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
  app->draw(rectangle);

  game().write(          "Giant Spider",
                         18,
                         OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                         OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                         ALIGN_LEFT,
                         sf::Color(255, 255, 255),
                         app, 0, 0);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}
