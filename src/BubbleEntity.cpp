#include "BubbleEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BubbleEntity::BubbleEntity(float x, float y, EnumBubbleType bubbleType, int bubbleSize)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_BUBBLE), x, y)
{
  this->bubbleSize = bubbleSize;
  this->bubbleType = bubbleType;
  imagesProLine = 2;

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
    hp = 5;
    creatureSpeed = 240.0f;
    sprite.setScale(0.2, 0.2);
    age = 0.0f;
  }

  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  meleeDamages = 5;

  bloodColor = BloodNone;
  shadowFrame = 1;

  if (bubbleType == BubbleIce)
  {
    frame = 2;
    shadowFrame = 3;
    meleeType = ShotTypeIce;
    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceVeryHigh;
    resistance[ResistanceFire] = ResistanceVeryLow;
  }
  else
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_EYE));
    imagesProLine = 12;
    shadowFrame = 23;
    spin = 20 + rand() % 20;
  }

  width = 128;
  height = 128;
  sprite.setOrigin(64, 64);
  canExplode = false;
}

int BubbleEntity::getBubbleSize()
{
  return bubbleSize;
}

void BubbleEntity::calculateBB()
{
  boundingBox.left = (int)x - 50 * sprite.getScale().x;
  boundingBox.width = 100 * sprite.getScale().x;
  boundingBox.top = (int)y - 50 * sprite.getScale().x;
  boundingBox.height =  100 * sprite.getScale().x;
}

void BubbleEntity::animate(float delay)
{
  if (bubbleType != BubbleIce)
  {
    if (age > 0)
      frame = ((int)(age * (28.0f + 5.0f * (float)bubbleSize))) % 18;
    else
      frame = 0;
  }
  EnemyEntity::animate(delay);
}

void BubbleEntity::render(sf::RenderTarget* app)
{
  if (bubbleType != BubbleIce)
  {
    // shadow
    sprite.setPosition(x, y + 5);
    sprite.setTextureRect(sf::IntRect(11 * width, 1 * height, width, height));
    app->draw(sprite);

    // eye
    sprite.setRotation(age * spin);
    sprite.setPosition(x, y);
    int nx = frame % imagesProLine;
    int ny = frame / imagesProLine;
    sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));
    app->draw(sprite);

    // lightning
    sprite.setRotation(0);
    sprite.setTextureRect(sf::IntRect(6 * width, height, width, height));
    app->draw(sprite);

    nx = 7 + (int)(age * 5) % 4;
    sprite.setTextureRect(sf::IntRect(nx * width, height, width, height));
    app->draw(sprite);
  }
  else
    EnemyEntity::render(app);
}

void BubbleEntity::collideMapRight()
{
  velocity.x = -velocity.x;
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void BubbleEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
  if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void BubbleEntity::collideMapTop()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void BubbleEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
  if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void BubbleEntity::collideWithEnemy(EnemyEntity* entity)
{
  BubbleEntity* bubbleEntity = dynamic_cast<BubbleEntity*>(entity);

  if (bubbleEntity == NULL)
  {
    if (entity->getMovingStyle() == movWalking)
      setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
  }
  else
  {
    if (bubbleEntity->getBubbleSize() <= bubbleSize)
      setVelocity(Vector2D(bubbleEntity->getX(), bubbleEntity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
  }
}

void BubbleEntity::dying()
{
  isDying = true;
  if (bubbleSize == 0) game().addKilledEnemy(enemyType, hurtingType);

  if (bubbleSize < 4)
  {
    int nbBubbles = 2;
    if (bubbleType == BubbleTriple) nbBubbles = 3;
    for (int i = 0; i < nbBubbles; i++)
    {
      BubbleEntity* b = new BubbleEntity(x - 5 + rand() % 10, y - 5 + rand() % 10, bubbleType, bubbleSize + 1);
      b->setSpecialState(SpecialStateIce,
                          true,
                          specialState[SpecialStateIce].timer,
                          specialState[SpecialStateIce].param1,
                          specialState[SpecialStateIce].param2);
    }
  }

  for (int i = 0; i < 5 - bubbleSize; i++)
    game().generateBlood(x, y, bubbleType == BubbleIce ? BloodBubbleIce : BloodBubble);

  if (bubbleType == BubbleIce)
    SoundManager::getInstance().playSound(SOUND_ICE_BLOCK);
  else
    SoundManager::getInstance().playSound(SOUND_BUBBLE_00 + rand() % 2);
}
