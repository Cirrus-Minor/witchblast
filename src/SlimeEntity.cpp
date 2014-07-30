#include "SlimeEntity.h"
#include "PlayerEntity.h"
#include "EnnemyBoltEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SlimeEntity::SlimeEntity(float x, float y, slimeTypeEnum slimeType, bool invocated)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_SLIME), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = SLIME_HP;
  meleeDamages = SLIME_DAMAGES;
  this->slimeType = slimeType;

  this->invocated = invocated;
  if (invocated)
  {
    type = ENTITY_ENNEMY_INVOCATED;
    jumpingDelay = 0.1f;
    age = 0.0f;
  }
  else
  {
    type = ENTITY_ENNEMY;
    jumpingDelay = 0.6f + 0.1f * (rand() % 20);
  }

  if (slimeType == SlimeTypeBlue)
  {
    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceHigh;
    resistance[ResistanceFire] = ResistanceLow;
    enemyType = invocated ? EnemyTypeSlimeBlue_invocated : EnemyTypeSlimeBlue;
  }
  else if (slimeType == SlimeTypeRed)
  {
    resistance[ResistanceIce] = ResistanceLow;
    resistance[ResistanceFire] = ResistanceHigh;
    enemyType = invocated ? EnemyTypeSlimeRed_invocated : EnemyTypeSlimeRed;
  }
  else
  {
    enemyType = invocated ? EnemyTypeSlime_invocated : EnemyTypeSlime;
  }


  bloodColor = bloodGreen;
  frame = 0;
  shadowFrame = 3;
  imagesProLine = 4;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);
}

void SlimeEntity::animate(float delay)
{
  float slimeDelay = delay;
  if (specialState[SpecialStateIce].active) slimeDelay = delay * specialState[SpecialStateIce].parameter;

  if (isJumping)
  {
    hVelocity -= 700.0f * slimeDelay;

    h += hVelocity * slimeDelay;

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
          if (slimeType == SlimeTypeBlue || slimeType == SlimeTypeRed)
            fire();
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
    jumpingDelay -= slimeDelay;
    if (jumpingDelay < 0.0f)
    {
      SoundManager::getSoundManager()->playSound(SOUND_SLIME_JUMP);
      hVelocity = 350.0f + rand() % 300;
      isJumping = true;
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
    else if (jumpingDelay < 0.25f)
      frame = 1;
    else frame = 0;
  }

  EnemyEntity::animate(delay);
  z = y + 14;
}

void SlimeEntity::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(frame * width, slimeType * height, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void SlimeEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + SLIME_BB_LEFT;
    boundingBox.width = width - SLIME_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + SLIME_BB_TOP - 15;
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

void SlimeEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  if (recoil.active && recoil.stun) return;

  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRecoil(false, vel, 0.3f);

    computeFacingDirection();
  }
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
  game().addKilledEnemy(enemyType);
  SpriteEntity* deadSlime = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadSlime->setZ(OFFSET_Y);
  switch (slimeType)
  {
    case SlimeTypeStandard: deadSlime->setFrame(FRAME_CORPSE_SLIME); break;
    case SlimeTypeRed: deadSlime->setFrame(FRAME_CORPSE_SLIME_RED); break;
    case SlimeTypeBlue: deadSlime->setFrame(FRAME_CORPSE_SLIME_BLUE); break;
  }
  deadSlime->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);

  if (!invocated) drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}

void SlimeEntity::prepareDying()
{
    if (!isJumping)
      dying();
}

bool SlimeEntity::canCollide()
{
  return h <= 70.0f;
}

BaseCreatureEntity::enumMovingStyle SlimeEntity::getMovingStyle()
{
  if (h <= 70.0f)
    return movWalking;
  else
    return movFlying;
}

void SlimeEntity::fire()
{
  for (int i = 0; i < 4; i++)
  {
    EnnemyBoltEntity* bolt;

    if (slimeType == SlimeTypeBlue)
    {
      bolt = new EnnemyBoltEntity(x, y, ShotTypeIce, 0);
      bolt->setDamages(5);
    }
    else if (slimeType == SlimeTypeRed)
    {
      bolt = new EnnemyBoltEntity(x, y, ShotTypeFire, 0);
      bolt->setDamages(8);
    }
    else
      return;

    switch (i)
    {
      case 0: bolt->setVelocity(Vector2D(SLIME_FIRE_VELOCITY, 0)); break;
      case 1: bolt->setVelocity(Vector2D(-SLIME_FIRE_VELOCITY, 0)); break;
      case 2: bolt->setVelocity(Vector2D(0, SLIME_FIRE_VELOCITY)); break;
      case 3: bolt->setVelocity(Vector2D(0, -SLIME_FIRE_VELOCITY)); break;
    }
  }
  SoundManager::getSoundManager()->playSound(SOUND_BLAST_FLOWER);
}
