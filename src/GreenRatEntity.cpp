#include "GreenRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

GreenRatEntity::GreenRatEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_RAT), x, y)
{
  imagesProLine = 6;
  creatureSpeed = GREEN_RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  hp = GREEN_RAT_HP;
  meleeDamages = GREEN_RAT_DAMAGES;

  type = ENTITY_ENNEMY_INVOCATED;
  bloodColor = bloodRed;
  shadowFrame = 6;

  timer = (rand() % 50) / 10.0f;
  age = -GREEN_RAT_FADE;
  frame = 6;
}

void GreenRatEntity::animate(float delay)
{
  z = y + boundingBox.top + boundingBox.height;

  if (age > 0.0f)
  {
    sprite.setColor(sf::Color(255,255,255,255));

    timer = timer - delay;
    if (timer <= 0.0f)
    {
      timer = (rand() % 50) / 10.0f;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), GREEN_RAT_SPEED ));
      computeFacingDirection();
    }

    frame = 6 + ((int)(age * 5.0f)) % 2;
    if (facingDirection == 4 || facingDirection == 6) frame += 2;
    isMirroring = (facingDirection == 4 );
    if (facingDirection == 8) frame += 4;
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
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    computeFacingDirection();
}

void GreenRatEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
    computeFacingDirection();
}

void GreenRatEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    computeFacingDirection();
}

void GreenRatEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
    computeFacingDirection();
}

void GreenRatEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  if (recoil.active && recoil.stun) return;

  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRecoil(false, vel, 0.3f);

    computeFacingDirection();
  }
}

void GreenRatEntity::dying()
{
  isDying = true;
  SpriteEntity* deadRat = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
  deadRat->setZ(OFFSET_Y);
  deadRat->setFrame(FRAME_CORPSE_GREEN_RAT);
  deadRat->setType(ENTITY_CORPSE);

  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);

  SoundManager::getSoundManager()->playSound(SOUND_ENNEMY_DYING);
}
