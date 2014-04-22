#include "SlimeEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SlimeEntity::SlimeEntity(float x, float y, WitchBlastGame* parent)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_SLIME), parent, x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = SLIME_HP;
  meleeDamages = SLIME_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodGreen;
  jumpingDelay = 2.0f;
  shadowFrame = 3;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  frame = 0;
  jumpingDelay = 0.6f + 0.1f * (rand() % 20);
}

void SlimeEntity::animate(float delay)
{
  if (isJumping)
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
          jumpingDelay = 0.4f + 0.1f * (rand() % 20);
          isJumping = false;
          SoundManager::getSoundManager()->playSound(SOUND_SLIME_IMAPCT_WEAK);
        }
      }
    }
    if (hVelocity > 0.0f) frame = 2;
    else frame = 0;
  }
  else
  {
    jumpingDelay -= delay;
    if (jumpingDelay < 0.0f)
    {
      SoundManager::getSoundManager()->playSound(SOUND_SLIME_JUMP);
      hVelocity = 350.0f + rand() % 300;
      isJumping = true;
      isFirstJumping = true;

      float randVel = 250.0f + rand() % 250;

      if (rand() % 2 == 0)
      {
        float tan = (parentGame->getPlayer()->getX() - x) / (parentGame->getPlayer()->getY() - y);
        float angle = atan(tan);

        if (parentGame->getPlayer()->getY() > y)
          setVelocity(Vector2D(sin(angle) * randVel,
                                     cos(angle) * randVel));
        else
          setVelocity(Vector2D(-sin(angle) * randVel,
                                     -cos(angle) * randVel));
      }
      else
        velocity = Vector2D(randVel);
    }
    else if (jumpingDelay < 0.25f)
      frame = 1;
    else frame = 0;
  }

  EnnemyEntity::animate(delay);
}

void SlimeEntity::render(sf::RenderWindow* app)
{
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

  #ifdef SHOW_BOUNDING_BOX
  sf::Vertex line[] =
  {
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left + boundingBox.width, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top + boundingBox.height)),
      sf::Vertex(sf::Vector2f(boundingBox.left, boundingBox.top))
  };
  app->draw(line, 8, sf::Lines);
  #endif
}

void SlimeEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + SLIME_BB_LEFT;
    boundingBox.width = width - SLIME_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + SLIME_BB_TOP;
    boundingBox.height =  height - SLIME_BB_HEIGHT_DIFF;
}

void SlimeEntity::collideMapRight()
{
 // if (x > OFFSET_X + MAP_WIDTH * TILE_WIDTH)
    velocity.x = -velocity.x * 0.8f;
}

void SlimeEntity::collideMapLeft()
{
 // if (x < OFFSET_X + MAP_WIDTH )
    velocity.x = -velocity.x * 0.8f;
}

void SlimeEntity::collideMapTop()
{
//  if (y > OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT)
    velocity.y = -velocity.y * 0.8f;
}

void SlimeEntity::collideMapBottom()
{
 // if (y < OFFSET_Y + MAP_HEIGHT )
    velocity.y = -velocity.y * 0.8f;
}

bool SlimeEntity::collideWithMap(int direction)
{
    calculateBB();

    int xTile0 = (boundingBox.left - offsetX) / tileWidth;
    int xTilef = (boundingBox.left + boundingBox.width - offsetX) / tileWidth;
    int yTile0 = (boundingBox.top - offsetY) / tileHeight;
    int yTilef = (boundingBox.top + boundingBox.height - offsetY) / tileHeight;

    if (boundingBox.top < 0) yTile0 = -1;

    for (int xTile = xTile0; xTile <= xTilef; xTile++)
        for (int yTile = yTile0; yTile <= yTilef; yTile++)
        {
          if (xTile == 0 || xTile == MAP_WIDTH - 1 || yTile == 0 || yTile == MAP_HEIGHT - 1)
          {
            switch (direction)
            {
            case DIRECTION_LEFT:
                if (map->isLeftBlocking(xTile, yTile)) return true;
                break;

            case DIRECTION_RIGHT:
                if (map->isRightBlocking(xTile, yTile)) return true;
                break;

            case DIRECTION_TOP:
                if (map->isUpBlocking(xTile, yTile)) return true;
                break;

            case DIRECTION_BOTTOM:
                if (map->isDownBlocking(xTile, yTile)) return true;
                break;
            }
          }
        }

    return false;
}

void SlimeEntity::dying()
{
  isDying = true;
  SpriteEntity* deadSlime = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadSlime->setZ(OFFSET_Y);
  deadSlime->setFrame(FRAME_CORPSE_SLIME);
  deadSlime->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) parentGame->generateBlood(x, y, bloodColor);

  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}

bool SlimeEntity::canCollide()
{
  return h <= 70.0f;
}

bool SlimeEntity::hurt(int damages)
{
  hurting = true;
  hurtingDelay = HURTING_DELAY;

  hp -= damages;
  if (hp <= 0)
  {
    hp = 0;
    if (!isJumping)
      dying();
  }

  return true;
}
