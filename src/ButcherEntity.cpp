#include "ButcherEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "SausageEntity.h"
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
  sausages = 0;

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
  resistance[ResistanceRepulsion] = ResistanceHigh;
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
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void ButcherEntity::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void ButcherEntity::collideMapTop()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void ButcherEntity::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void ButcherEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking && entity->getEnemyType() != EnemyTypeSausage_invocated)
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRepulsion(false, vel, 0.3f);
  }
}

void ButcherEntity::drop()
{
  dropItem(ItemScrollRevelation);

  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    EnemyEntity* entity = dynamic_cast<EnemyEntity*>(e);
    if (entity != NULL)
    {
      if (entity->getEnemyType()== EnemyTypeSausage_invocated)
      {
        entity->hurt(getHurtParams(entity->getHp(), ShotTypeStandard, 0, false, SourceTypeMelee, enemyType, false));
      }
    }
  }
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

  int result = EnemyEntity::hurt(hurtParam);

  int totalDamages = hpMax - hp;
  if (hp > 0 && totalDamages / 8 > sausages)
  {
    sausages++;
    new SausageEntity(x, y, true);
  }

  return result;
}

bool ButcherEntity::isAttacking()
{
  return true;
}
