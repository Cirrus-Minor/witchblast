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

  SoundManager::getInstance().playSound(SOUND_BUBBLE_00 + rand() % 2);
}
