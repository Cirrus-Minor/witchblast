#include "RatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

RatEntity::RatEntity(float x, float y, GameMap* map)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y, map)
{
  creatureSpeed = RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = RAT_HP;
  meleeDamages = RAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
  shadowFrame = 3;
}

void RatEntity::animate(float delay)
{
  if (age > 0.0f)
    frame = ((int)(age * 5.0f)) % 2;

  EnnemyEntity::animate(delay);
}

void RatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
    boundingBox.width = width - RAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + RAT_BB_TOP;
    boundingBox.height =  height - RAT_BB_HEIGHT_DIFF;
}

void RatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void RatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void RatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void RatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}


void RatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_RAT);
  deadRat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) parentGame->generateBlood(x, y, bloodColor);
  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}
