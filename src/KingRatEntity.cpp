#include "KingRatEntity.h"
#include "GreenRatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>

KingRatEntity::KingRatEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_KING_RAT), x, y)
{
  width = 128;
  height = 128;
  creatureSpeed = KING_RAT_SPEED;
  velocity = Vector2D(creatureSpeed);
  hp = KING_RAT_HP;
  hpDisplay = hp;
  hpMax = KING_RAT_HP;
  meleeDamages = KING_RAT_DAMAGES;
  imagesProLine = 5;

  type = ENTITY_ENEMY_BOSS;
  enemyType = EnemyTypeRatKing;
  bloodColor = BloodRed;
  shadowFrame = 4;
  dyingFrame = 5;
  deathFrame = FRAME_CORPSE_KING_RAT;
  dyingSound = SOUND_KING_RAT_DIE;
  frame = 0;
  if (game().getPlayerPosition().x > x) sprite.setScale(-1.0f, 1.0f);
  sprite.setOrigin(64.0f, 70.0f);

  state = 0;
  timer = 2.0f + (rand() % 40) / 10.0f;
  age = -2.0f;

  berserkDelay = 1.0f + rand()%5 * 0.1f;

  hasBeenBerserk = false;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceVeryHigh;
  canExplode = false;
}

void KingRatEntity::animate(float delay)
{
  if (age <= 0.0f)
  {
    age += delay;
    return;
  }

  if (isAgonising)
  {
    if (h < -0.01f)
    {
      isDying = true;
      game().addCorpse(x, y, deathFrame);
      if (dyingSound != SOUND_NONE) SoundManager::getInstance().playSound(dyingSound);
    }
    else
    {
      frame = dyingFrame;
      hVelocity -= 700.0f * delay;
      h += hVelocity * delay;
    }

    return;
  }

  EnemyEntity::animate(delay);
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  float timerMult = 1.0f;
  if (hp <= hpMax / 4)
  {
    creatureSpeed = KING_RAT_SPEED * 1.4f;
    timerMult = 0.7f;
  }
  else if (hp <= hpMax / 2)
  {
    creatureSpeed = KING_RAT_SPEED * 1.2f;
    timerMult = 0.85f;
  }
  else
  {
    creatureSpeed = KING_RAT_SPEED;
  }

  if (state == 5)
  {
    velocity.x *= 0.965f;
    velocity.y *= 0.965f;
  }

  else
  {
    if (velocity.x > 0.1f) sprite.setScale(-1.0f, 1.0f);
    else if (velocity.x < -0.1f) sprite.setScale(1.0f, 1.0f);
  }


  timer -= delay;

  if (timer <= 0.0f)
  {
    if (state == 0)
    {
      state = 1;
      // generate rats
      generateGreenRats();
      SoundManager::getInstance().playSound(SOUND_KING_RAT_1);
      timer = 2.0f;

      velocity.x = 0.0f;
      velocity.y = 0.0f;
    }

    else if (state == 1) // generate rats
    {
      // to normal or berserk
      if (hp < hpMax / 4 && !hasBeenBerserk)
      {
        hasBeenBerserk = true;
        timer = 12.0f;
        state = 6;
        velocity = Vector2D(KING_RAT_BERSERK_SPEED);
        SoundManager::getInstance().playSound(SOUND_KING_RAT_2);
      }
      else
      {
        timer = 7.0f * timerMult;
        velocity = Vector2D(creatureSpeed);
        state = 2;
      }
    }

    else if (state == 2) // normal -> rush
    {
      state = 3;
      // angry
      timer = 1.0f;

      velocity.x = 0.0f;
      velocity.y = 0.0f;
      SoundManager::getInstance().playSound(SOUND_KING_RAT_2);
    }

    else if (state == 3) // normal -> rush
    {
      state = 4;
      // rush
      timer = 12.0f;

      float tan = (game().getPlayer()->getX() - x) / (game().getPlayer()->getY() - y);
      float angle = atan(tan);

      if (game().getPlayer()->getY() > y)
        setVelocity(Vector2D(sin(angle) * KING_RAT_RUNNING_SPEED,
                             cos(angle) * KING_RAT_RUNNING_SPEED));
      else
        setVelocity(Vector2D(-sin(angle) * KING_RAT_RUNNING_SPEED,
                             -cos(angle) * KING_RAT_RUNNING_SPEED));
    }
    else if (state == 5) // wall impact
    {
      // to normal or berserk
      if (hp < hpMax / 4 && !hasBeenBerserk)
      {
        hasBeenBerserk = true;
        timer = 12.0f;
        state = 6;
        velocity = Vector2D(KING_RAT_BERSERK_SPEED);
        SoundManager::getInstance().playSound(SOUND_KING_RAT_2);
      }
      else
      {
        timer = 7.0f * timerMult;
        velocity = Vector2D(creatureSpeed);
        state = 0;
      }
    }

    else if (state == 6)
    {
      timer = 7.0f * timerMult;
      velocity = Vector2D(creatureSpeed);
      state = 0;
    }
  }

  if (state == 6)
  {
    berserkDelay -= delay;

    if (berserkDelay <= 0.0f)
    {
      berserkDelay = 0.6f + (rand()%10) / 20.0f;
      SoundManager::getInstance().playSound(SOUND_KING_RAT_2);

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(),KING_RAT_BERSERK_SPEED ));
    }
  }

  frame = 0;

  if (state == 1)
    frame = 3;
  else if (state == 3 || state == 6)
  {
    frame = 3; //0;
    int r = ((int)(age * 10.0f)) % 2;
    if (r == 0)
      sprite.setScale(-1.0f, 1.0f);
    else
      sprite.setScale(1.0f, 1.0f);
  }
  else if (state == 4)
  {
    int r = ((int)(age * 7.5f)) % 4;
    if (r == 1) frame = 1;
    else if (r == 3) frame = 2;
  }
  else if (state == 5)
  {
    frame = 0;
  }
  else
  {
    int r = ((int)(age * 5.0f)) % 4;
    if (r == 1) frame = 1;
    else if (r == 3) frame = 2;
  }

  z = y + 48;
}

int KingRatEntity::hurt(StructHurt hurtParam)
{
  // berserk state protects
  if (state == 6)
    hurtParam.damage /= 4;

  return EnemyEntity::hurt(hurtParam);
}

void KingRatEntity::calculateBB()
{
  boundingBox.left = (int)x - 35;
  boundingBox.width = 70;
  boundingBox.top = (int)y - 15;
  boundingBox.height =  63;
}


void KingRatEntity::afterWallCollide()
{
  if (state == 4)
  {
    state = 5;
    timer = 1.4f;
    velocity.x *= 0.75f;
    velocity.y *= 0.75f;

    SoundManager::getInstance().playSound(SOUND_BIG_WALL_IMPACT);
    game().makeShake(0.75f);
  }
}
void KingRatEntity::collideMapRight()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void KingRatEntity::collideMapLeft()
{
  velocity.x = -velocity.x;

  afterWallCollide();
}

void KingRatEntity::collideMapTop()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void KingRatEntity::collideMapBottom()
{
  velocity.y = -velocity.y;

  afterWallCollide();
}

void KingRatEntity::drop()
{
  dropItem(ItemBossHeart);
}

void KingRatEntity::generateGreenRats()
{
  for (int i = 0; i < 5; i++)
  {
    float xr = x + -100 + rand() % 200;
    float yr = y + -100 + rand() % 200;

    if (xr > TILE_WIDTH * 1.5f && xr < TILE_WIDTH * (MAP_WIDTH - 2)
        && yr > TILE_HEIGHT * 1.5f && yr < TILE_HEIGHT * (MAP_HEIGHT - 2))
    {
      new GreenRatEntity(xr, yr);
    }
    else
      i--;
  }
}

void KingRatEntity::render(sf::RenderTarget* app)
{
  EnemyEntity::render(app);

  if (state == 6)
  {
    int r = ((int)(age *12.0f)) % 2;
    if (r == 0)
      sprite.setTextureRect(sf::IntRect(1 * width, 1 * height,  width,  height));
    else
      sprite.setTextureRect(sf::IntRect(2 * width, 1 * height,  -width,  height));

    sprite.setPosition(x, y);
    sprite.setColor(sf::Color(255, 255, 255, 190));
    app->draw(sprite);
    sprite.setColor(sf::Color(255, 255, 255, 255));
  }

  renderLifeBar(app, tools::getLabel("enemy_rat_king"));
}

void KingRatEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
  {
    inflictsrepulsionTo(entity);
  }
}

void KingRatEntity::inflictsrepulsionTo(BaseCreatureEntity* targetEntity)
{
  if (state == 4 || state == 6)
  {
    Vector2D repulsionVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), KING_RAT_RUNNING_RECOIL );
    targetEntity->giveRepulsion(true, repulsionVector, 1.0f);
  }

  if (state == 6)
  {
    PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

    if (playerEntity != NULL && !playerEntity->isDead())
    {
      Vector2D repulsionVector = Vector2D(targetEntity->getX(), targetEntity->getY()).vectorTo(Vector2D(x, y), KING_RAT_RUNNING_RECOIL);
      giveRepulsion(true, repulsionVector, 1.0f);
    }
  }
}
