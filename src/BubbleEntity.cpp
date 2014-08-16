#include "BubbleEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BubbleEntity::BubbleEntity(float x, float y, int bubbleSize)
  : EnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_BUBBLE), x, y)
{
  this->bubbleSize = bubbleSize;
  imagesProLine = 8;

  frame = 0;
  enemyType = EnemyTypeBubble;

  if (bubbleSize == 0)
  {
    hp = 20;
    creatureSpeed = 150.0f;
  }
  else if (bubbleSize == 1)
  {
    hp = 18;
    creatureSpeed = 180.0f;
    sprite.setScale(0.7, 0.7);
    age = 0.0f;
  }
  else if (bubbleSize == 2)
  {
    hp = 16;
    creatureSpeed = 210.0f;
    sprite.setScale(0.5, 0.5);
    age = 0.0f;
  }
  else if (bubbleSize == 3)
  {
    hp = 12;
    creatureSpeed = 225.0f;
    sprite.setScale(0.35, 0.35);
    age = 0.0f;
  }
  else if (bubbleSize == 4)
  {
    hp = 8;
    creatureSpeed = 240.0f;
    sprite.setScale(0.2, 0.2);
    age = 0.0f;
  }

  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  meleeDamages = RAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = BloodNone;
  shadowFrame = 1;

  enemyType = EnemyTypeRat;

  width = 128;
  height = 128;
  sprite.setOrigin(64, 64);
}

void BubbleEntity::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - 5);
  sprite.setTextureRect(sf::IntRect(0, 0, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void BubbleEntity::calculateBB()
{
  boundingBox.left = (int)x - 50 * sprite.getScale().x;
  boundingBox.width = 100 * sprite.getScale().x;
  boundingBox.top = (int)y - 50 * sprite.getScale().x;
  boundingBox.height =  100 * sprite.getScale().x;
}

void BubbleEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void BubbleEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void BubbleEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void BubbleEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void BubbleEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  EnemyEntity* entity = static_cast<EnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
    entity->setVelocity(Vector2D(x, y).vectorTo(Vector2D(entity->getX(), entity->getY()), entity->getCreatureSpeed()));
  }
}

void BubbleEntity::dying()
{
  isDying = true;
  if (bubbleSize == 0) game().addKilledEnemy(enemyType);

  if (bubbleSize < 4)
  {
    new BubbleEntity(x - 5 + rand() % 10, y - 5 + rand() % 10, bubbleSize + 1);
    new BubbleEntity(x - 5 + rand() % 10, y - 5 + rand() % 10, bubbleSize + 1);
  }
  else
  {
    // the last one should drop
    if (game().getEnnemyCount() == 1)
    {
      SoundManager::getSoundManager()->playSound(SOUND_GONG);

      ItemEntity* newItem = new ItemEntity(ItemBonusHealth, x, y);
      newItem->setVelocity(Vector2D(100.0f + rand()% 250));
      newItem->setViscosity(0.96f);

      int gold = 2 + rand() % 9;
      for (int i = 0; i < gold; i++)
      {
        ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
        newItem->setVelocity(Vector2D(90.0f + rand()% 150));
        newItem->setViscosity(0.96f);
      }

      float x0 = OFFSET_X + MAP_WIDTH * 0.5f * TILE_WIDTH;
      float y0 = OFFSET_Y + MAP_HEIGHT * 0.5f * TILE_HEIGHT + 40.0f;
      TextEntity* text = new TextEntity("COMPLETE !", 30, x0, y0);
      text->setAlignment(ALIGN_CENTER);
      text->setLifetime(2.5f);
      text->setWeight(-36.0f);
      text->setZ(1000);
      text->setColor(TextEntity::COLOR_FADING_WHITE);
    }
  }

  for (int i = 0; i < 5 - bubbleSize; i++)
  {
    SpriteEntity* blood = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BLOOD), x, y, 16, 16, 6);
    blood->setZ(OFFSET_Y - 1);

    blood->setFrame(24 + rand()%6);
    blood->setType(ENTITY_BLOOD);
    blood->setVelocity(Vector2D(rand()%150));
    blood->setViscosity(0.95f);

    float bloodScale = 1.0f + (rand() % 10) * 0.1f;
    blood->setScale(bloodScale, bloodScale);
  }

  SoundManager::getSoundManager()->playSound(SOUND_BUBBLE_00 + rand() % 2);
}
