#include "BatEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BatEntity::BatEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_BAT), x, y)
{
  creatureSpeed = BAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = BAT_HP;
  meleeDamages = BAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
  changingDelay = -0.5f;
  shadowFrame = 3;
  movingStyle = movFlying;
}

void BatEntity::animate(float delay)
{
  changingDelay -= delay;
  if (changingDelay < 0.0f)
  {
    velocity = Vector2D(creatureSpeed);
    changingDelay = 0.5f + (float)(rand() % 2500) / 1000.0f;
  }
  if (age < 0.0f)
    frame = 1;
  else
    frame = ((int)(age * 5.0f)) % 2;

  testSpriteCollisions();
  EnnemyEntity::animate(delay);
}

void BatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + BAT_BB_LEFT;
    boundingBox.width = width - BAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + BAT_BB_TOP;
    boundingBox.height =  height - BAT_BB_HEIGHT_DIFF;
}

void BatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void BatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void BatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void BatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}

void BatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadBat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadBat->setZ(OFFSET_Y);
  deadBat->setFrame(FRAME_CORPSE_BAT);
  deadBat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);

  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}
