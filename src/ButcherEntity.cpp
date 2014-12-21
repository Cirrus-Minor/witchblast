#include "ButcherEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

ButcherEntity::ButcherEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_BUTCHER), x, y)
{
  width = 80;
  height = 160;
  sprite.setOrigin(40, 115);

  creatureSpeed = BUTCHER_VELOCITY;
  velocity = Vector2D(creatureSpeed);
  computeFacingDirection();
  hp = BUTCHER_HP;
  hpMax = BUTCHER_HP;
  hpDisplay = BUTCHER_HP;
  meleeDamages = BUTCHER_DAMAGES;

  type = ENTITY_ENEMY_BOSS;
  bloodColor = BloodRed;
  shadowFrame = 4;
  dyingFrame = 3;
  deathFrame = FRAME_CORPSE_BUTCHER;
  agonizingSound = SOUND_BUTCHER_DIE;
  hurtingSound = SOUND_BUTCHER_HURT;
  enemyType = EnemyTypeButcher;

  timer = (rand() % 50) / 10.0f;
  age = -1.5f;
  frame = 1;

  resistance[ResistanceFrozen] = ResistanceHigh;
  resistance[ResistanceRecoil] = ResistanceHigh;
  canExplode = false;
}

void ButcherEntity::animate(float delay)
{
  if (age > 0.0f && !isAgonising)
  {
    sprite.setColor(sf::Color(255,255,255,255));

    timer = timer - delay;
    if (timer <= 0.0f)
    {
      creatureSpeed = BUTCHER_VELOCITY + (hpMax - hp) * 0.8f;
      timer = (rand() % 50) / 10.0f;
      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      if (rand()%2 == 0)
        SoundManager::getInstance().playSound(SOUND_BUTCHER_00);
      else
        SoundManager::getInstance().playSound(SOUND_BUTCHER_01);
    }

    frame = ((int)(age * creatureSpeed / 25)) % 4;
    if (frame == 3) frame = 1;

    if (velocity.x > 1.0f) isMirroring = true;
    else if (velocity.x < -1.0f) isMirroring = false;
  }

  EnemyEntity::animate(delay);
  z = y + 30;
}

void ButcherEntity::calculateBB()
{
    boundingBox.left = (int)x - 22;
    boundingBox.width = 44;
    boundingBox.top = (int)y - 18;
    boundingBox.height =  48;
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

void ButcherEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (recoil.active && recoil.stun) return;

  if (entity->getMovingStyle() == movWalking )
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRecoil(false, vel, 0.3f);
  }
}

void ButcherEntity::drop()
{
  ItemEntity* newItem = new ItemEntity(ItemSilverCoin, x, y);
  newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);
  newItem->setVelocity(Vector2D(100.0f + rand()% 250));
  newItem->setViscosity(0.96f);
}

void ButcherEntity::render(sf::RenderTarget* app)
{
    EnemyEntity::render(app);

    renderLifeBar(app, tools::getLabel("enemy_butcher"));
}

int ButcherEntity::hurt(StructHurt hurtParam)
{
  creatureSpeed = BUTCHER_VELOCITY + hpMax - hp;
  setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
  return EnemyEntity::hurt(hurtParam);
}
