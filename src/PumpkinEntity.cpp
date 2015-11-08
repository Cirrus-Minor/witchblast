#include "PumpkinEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

PumpkinEntity::PumpkinEntity(float x, float y, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_PUMPKIN), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = 40;
  meleeDamages = 5;

  this->invocated = invocated;
  if (invocated)
  {
    type = ENTITY_ENEMY_INVOCATED;
    jumpingDelay = 0.1f;
    age = 0.0f;
  }
  else
  {
    jumpingDelay = 0.6f + 0.1f * (rand() % 20);
  }

  enemyType = invocated ? EnemyTypePumpkin_invocated : EnemyTypePumpkin;

  bloodColor = BloodGreen;
  frame = 2;
  shadowFrame = 3;
  dyingFrame = 4;
  deathFrame = FRAME_CORPSE_PUMPKIN;
  agonizingSound = SOUND_PUMPKIN_DIE;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);
}

void PumpkinEntity::animate(float delay)
{
  float slimeDelay = delay;
  if (specialState[SpecialStateIce].active) slimeDelay = delay * specialState[SpecialStateIce].param1;

  if (!isAgonising)
  {
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
            hVelocity = 120.0f;
            SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
          }
          else
          {
            jumpingDelay = 0.05f;
            isJumping = false;
            SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
          }
        }
      }
      if (firstTimeGround) frame = 2;
      else if (hVelocity > -60.0f) frame = 1;
      else frame = 0;
    }
    else
    {
      jumpingDelay -= slimeDelay;
      if (jumpingDelay < 0.0f)
      {
        if (rand() % 2 == 0)
          SoundManager::getInstance().playSound(SOUND_PUMPKIN_01, false);
        else
          SoundManager::getInstance().playSound(SOUND_PUMPKIN_00, false);
        hVelocity = 200.0f;
        isJumping = true;
        isFirstJumping = true;

        float randVel = 280.0f;

        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), randVel ));
      }
      else if (jumpingDelay < 0.05f)
        frame = 1;
      else frame = 2;
    }

    if (age > 0.0f && jumpingDelay < 0.05f)
    {
      frame = (int)(age * 6) % 4;
      if (frame == 3) frame = 1;
    }
    else frame = 2;
  }

  EnemyEntity::animate(delay);
  z = y + 14;
}

void PumpkinEntity::render(sf::RenderTarget* app)
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

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void PumpkinEntity::calculateBB()
{
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 14;
    boundingBox.height =  28;
}

void PumpkinEntity::collideMapRight()
{
  velocity.x = -velocity.x * 0.8f;
}

void PumpkinEntity::collideMapLeft()
{
  velocity.x = -velocity.x * 0.8f;
}

void PumpkinEntity::collideMapTop()
{
  velocity.y = -velocity.y * 0.8f;
}

void PumpkinEntity::collideMapBottom()
{
  velocity.y = -velocity.y * 0.8f;
}

void PumpkinEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking)
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), velocity.norm() ));
}

void PumpkinEntity::dying()
{
  EnemyEntity::dying();
  hVelocity = 320.0f;
}

void PumpkinEntity::prepareDying()
{
  dying();
}
