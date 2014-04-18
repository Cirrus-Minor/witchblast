#include "GreenRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

GreenRatEntity::GreenRatEntity(float x, float y, GameMap* map, PlayerEntity* player)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y, map)
{
  imagesProLine = 4;
  creatureSpeed = GREEN_RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = GREEN_RAT_HP;
  meleeDamages = GREEN_RAT_DAMAGES;

  type = 21;
  bloodColor = bloodRed;
  shadowFrame = 3;

  timer = (rand() % 50) / 10.0f;
  age = -GREEN_RAT_FADE;
  frame = 4;
  this->player = player;
}

void GreenRatEntity::animate(float delay)
{
  z = y + boundingBox.top + boundingBox.height;

  if (age > 0.0f)
  {
    sprite.setColor(sf::Color(255,255,255,255));
        frame = 4 + ((int)(age * 5.0f)) % 2;
        timer = timer - delay;
        if (timer <= 0.0f)
        {
          timer = (rand() % 50) / 10.0f;
          float tan = (player->getX() - x) / (player->getY() - y);
          float angle = atan(tan);

          if (player->getY() > y)
            setVelocity(Vector2D(sin(angle) * RAT_SPEED,
                                       cos(angle) * RAT_SPEED));
          else
            setVelocity(Vector2D(-sin(angle) * RAT_SPEED,
                                       -cos(angle) * RAT_SPEED));
        }
  }
  else
  {
    sprite.setColor(sf::Color(255,255,255,255 * (1.0 + age)));
  }

  EnnemyEntity::animate(delay);
}

void GreenRatEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + RAT_BB_LEFT;
    boundingBox.width = width - RAT_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + RAT_BB_TOP;
    boundingBox.height =  height - RAT_BB_HEIGHT_DIFF;
}

void GreenRatEntity::collideMapRight()
{
    velocity.x = -velocity.x;
}

void GreenRatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
}

void GreenRatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
}

void GreenRatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
}


void GreenRatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_GREEN_RAT);
  deadRat->setType(TYPE_CORPSE);

  for (int i = 0; i < 4; i++) parentGame->generateBlood(x, y, bloodColor);
  //drop();
  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}
