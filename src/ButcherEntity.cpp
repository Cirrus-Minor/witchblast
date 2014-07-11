#include "ButcherEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

ButcherEntity::ButcherEntity(float x, float y)
  : EnnemyEntity (ImageManager::getImageManager()->getImage(IMAGE_BUTCHER), x, y)
{
  width = 128;
  height = 128;
  sprite.setOrigin((float)(this->width / 2), (float)(this->height / 2));

  creatureSpeed = BUTCHER_VELOCITY;
  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  hp = BUTCHER_HP;
  hpMax = BUTCHER_HP;
  hpDisplay = BUTCHER_HP;
  meleeDamages = BUTCHER_DAMAGES;

  type = ENTITY_ENNEMY_BOSS;
  bloodColor = bloodRed;
  shadowFrame = 5;
  dyingFrame = 3;
  deathFrame = FRAME_CORPSE_BUTCHER;
  agonizingSound = SOUND_BUTCHER_DIE;
  hurtingSound = SOUND_BUTCHER_HURT;

  timer = (rand() % 50) / 10.0f;
  age = -1.5f;
  frame = 0;
}

void ButcherEntity::animate(float delay)
{
  z = y + boundingBox.top + boundingBox.height;

  if (age > 0.0f && !isAgonising)
  {
    sprite.setColor(sf::Color(255,255,255,255));

    timer = timer - delay;
    if (timer <= 0.0f)
    {
      creatureSpeed = BUTCHER_VELOCITY + hpMax - hp;
      timer = (rand() % 50) / 10.0f;
      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      if (rand()%2 == 0)
        SoundManager::getSoundManager()->playSound(SOUND_BUTCHER_00);
      else
        SoundManager::getSoundManager()->playSound(SOUND_BUTCHER_01);
    }

    frame = ((int)(age * creatureSpeed / 25)) % 4;
    if (frame == 2) frame = 0;
    else if (frame == 3) frame = 2;

    if (velocity.x > 1.0f) isMirroring = true;
    else if (velocity.x < -1.0f) isMirroring = false;
  }

  EnnemyEntity::animate(delay);
}

void ButcherEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2 + 45;
    boundingBox.width = 44;
    boundingBox.top = (int)y - height / 2 + 48;
    boundingBox.height =  72;
}

void ButcherEntity::collideMapRight()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void ButcherEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (recoil.active) recoil.velocity.x = -recoil.velocity.x;
}

void ButcherEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void ButcherEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (recoil.active) recoil.velocity.y = -recoil.velocity.y;
}

void ButcherEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  if (recoil.active && recoil.stun) return;

  EnnemyEntity* entity = static_cast<EnnemyEntity*>(collidingEntity);
  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRecoil(false, vel, 0.3f);
  }
}

void ButcherEntity::drop()
{
  // no drop
}

void ButcherEntity::render(sf::RenderTarget* app)
{
    EnnemyEntity::render(app);

    float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / hpMax;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 25));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    game().write(          "Pigman Butcher",
                            18,
                            OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                            OFFSET_Y + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255),
                            app, 0 , 0);
}

bool ButcherEntity::hurt(int damages, enumShotType hurtingType, int level)
{
  creatureSpeed = BUTCHER_VELOCITY + hpMax - hp;
  setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
  return EnnemyEntity::hurt(damages, hurtingType, level);
}
