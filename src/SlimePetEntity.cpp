#include "SlimePetEntity.h"
#include "PlayerEntity.h"
#include "EnemyBoltEntity.h"
#include "ExplosionEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SlimePetEntity::SlimePetEntity()
  : BaseCreatureEntity (ImageManager::getInstance().getImage(IMAGE_SLIME), 200, 200, 64, 64)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = SLIME_HP;

  jumpingDelay = 0.6f + 0.1f * (rand() % 20);

  type = ENTITY_FAMILIAR_LOCAL;

  frame = 0;
  shadowFrame = 3;
  imagesProLine = 4;

  isJumping = false;
  h = 0.0f;
  attackDelay = -1.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);

  h = 1500;
  hVelocity = 0.0f;
  isJumping = true;
  isFirstJumping = true;

  x = GAME_WIDTH * 0.5f;
  y = GAME_HEIGHT * 0.5f;

  if (game().getPlayer()->getX() < 2 * TILE_WIDTH) x = 2.5f * TILE_WIDTH;
  else if (game().getPlayer()->getX() > GAME_WIDTH - 2 * TILE_WIDTH) x = GAME_WIDTH - 2.5f * TILE_WIDTH;
  else if (game().getPlayer()->getY() < 2 * TILE_HEIGHT) y = 2.5f * TILE_HEIGHT;
  else if (game().getPlayer()->getY() > GAME_HEIGHT - 2 * TILE_HEIGHT) y = GAME_HEIGHT - 2.5f * TILE_HEIGHT;
}

void SlimePetEntity::animate(float delay)
{
  float slimeDelay = delay;
  if (specialState[SpecialStateIce].active) slimeDelay = delay * specialState[SpecialStateIce].param1;

  attackDelay -= delay;

  if (isJumping)
  {
    hVelocity -= 700.0f * slimeDelay;

    h += hVelocity * slimeDelay;

    bool firstTimeGround = false;

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
          firstTimeGround = true;
          hVelocity = 160.0f;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
        }
        else
        {
          jumpingDelay = 0.3f + 0.1f * (rand() % 15);
          isJumping = false;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
        }
      }
    }
    if (firstTimeGround) frame = 0;
    else if (hVelocity > -190.0f) frame = 2;
    else frame = 1;
  }
  else
  {
    jumpingDelay -= slimeDelay;
    if (jumpingDelay < 0.0f)
    {
      SoundManager::getInstance().playSound(SOUND_SLIME_JUMP);
      hVelocity = 350.0f + rand() % 300;
      isJumping = true;
      isFirstJumping = true;

      float randVel = 250.0f + rand() % 250;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), randVel ));
    }
    else if (jumpingDelay < 0.1f)
      frame = 1;
    else frame = 0;
  }

  BaseCreatureEntity::animate(delay);
  if (canCollide()) testSpriteCollisions();
  z = y + 14;
}

void SlimePetEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (canCollide() && collideWithEntity(entity))
  {
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX_COUNT)
      {
          EnemyEntity* enemyEntity = static_cast<EnemyEntity*>(entity);
          if (enemyEntity->canCollide())
          {
            if (attackDelay <= 0.0f)
            {
              enemyEntity->hurt(getHurtParams(8, ShotTypeStandard,0, false, SourceTypeMelee, EnemyTypeNone,false));
              attackDelay = 0.65f;
            }

            if (enemyEntity->getMovingStyle() == movWalking)
            {
              Vector2D vel = Vector2D(enemyEntity->getX(), enemyEntity->getY()).vectorTo(Vector2D(x, y), 100.0f );
              giveRecoil(false, vel, 0.3f);
            }
          }
      }
  }
}

void SlimePetEntity::render(sf::RenderTarget* app)
{
  // shadow
  if (h < 1055)
  {
    int fade = 255;
    if (h > 800)
      fade = - (h - 1055);
    sprite.setColor(sf::Color(255, 255, 255, fade));
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
    sprite.setColor(sf::Color(255, 255, 255, 255));
  }

  // sprite
  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(frame * width, 4 * height, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void SlimePetEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + SLIME_BB_LEFT;
    boundingBox.width = width - SLIME_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + SLIME_BB_TOP - 15;
    boundingBox.height =  height - SLIME_BB_HEIGHT_DIFF;
}

void SlimePetEntity::collideMapRight()
{
    velocity.x = -velocity.x * 0.8f;
}

void SlimePetEntity::collideMapLeft()
{
    velocity.x = -velocity.x * 0.8f;
}

void SlimePetEntity::collideMapTop()
{
    velocity.y = -velocity.y * 0.8f;
}

void SlimePetEntity::collideMapBottom()
{
    velocity.y = -velocity.y * 0.8f;
}

void SlimePetEntity::changeRoom()
{
  h = 1500;
  hVelocity = 0.0f;
  isJumping = true;
  isFirstJumping = true;
  x = 300;
  y = 250;
}

bool SlimePetEntity::collideWithMap(int direction)
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
          if (!game().getCurrentMap()->isFlyable(xTile, yTile))
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

bool SlimePetEntity::canCollide()
{
  return h <= 70.0f;
}

BaseCreatureEntity::enumMovingStyle SlimePetEntity::getMovingStyle()
{
  if (h <= 70.0f)
    return movWalking;
  else
    return movFlying;
}
