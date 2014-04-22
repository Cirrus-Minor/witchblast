#include "BatEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

BatEntity::BatEntity(float x, float y, WitchBlastGame* parent)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_BAT), parent, x, y)
{
  creatureSpeed = BAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = BAT_HP;
  meleeDamages = BAT_DAMAGES;

  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;
  changingDelay = -0.5f;
  shadowFrame = 3;
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
 // if (x > OFFSET_X + MAP_WIDTH * TILE_WIDTH)
    velocity.x = -velocity.x;
}

void BatEntity::collideMapLeft()
{
 // if (x < OFFSET_X + MAP_WIDTH )
    velocity.x = -velocity.x;
}

void BatEntity::collideMapTop()
{
//  if (y > OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT)
    velocity.y = -velocity.y;
}

void BatEntity::collideMapBottom()
{
 // if (y < OFFSET_Y + MAP_HEIGHT )
    velocity.y = -velocity.y;
}

bool BatEntity::collideWithMap(int direction)
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

void BatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadBat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadBat->setZ(OFFSET_Y);
  deadBat->setFrame(FRAME_CORPSE_BAT);
  deadBat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) parentGame->generateBlood(x, y, bloodColor);

  drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}
