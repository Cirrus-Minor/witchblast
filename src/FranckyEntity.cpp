#include "FranckyEntity.h"
#include "BoltEntity.h"
#include "EnemyBoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>

FranckyEntity::FranckyEntity(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_FRANCKY), x, y)
{
  width = 128;
  height = 192;
  creatureSpeed = 140;
  velocity = Vector2D(creatureSpeed);
  hp = 900;
  hpDisplay = hp;
  hpMax = hp;
  meleeDamages = 16;

  type = ENTITY_ENEMY_BOSS;
  bloodColor = BloodRed;
  deathFrame = FRAME_CORPSE_FRANCKY_TORSO;
  shadowFrame = 5;
  agonizingSound = SOUND_FRANCKY_DYING;
  frame = 0;
  if (game().getPlayerPosition().x > x) isMirroring = true;
  sprite.setOrigin(64.0f, 143.0f);

  state = 0;
  timer = 3.0f;
  followTimer = -1.0f;
  counter = 10;
  age = -1.5f;
  enemyType = EnemyTypeFrancky;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceImmune;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistanceLightning] = ResistanceVeryHigh;
  canExplode = false;
}

void FranckyEntity::animate(float delay)
{
  if (age <= 0.0f)
  {
    age += delay;
    return;
  }

  // special states
  if (specialState[SpecialStateIce].active) delay *= specialState[SpecialStateIce].param1;

  // IA
  timer -= delay;
  if (timer < 0.0f)
  {
    state++;
    if (state == 5)
    {
      state = 0;
      followTimer = 0.25f;
    }

    int health;

    switch (state)
    {
    case 0:
      timer = 6.0f;
      health = ((100 * hp) / hpMax);
      if (health <= 33) timer = 4.5f;
      else if (health <= 66) timer = 5.25f;
      SoundManager::getInstance().playSound(SOUND_FRANCKY_00);
      break;
    case 1:
      timer = 1.0f;
      velocity.x = 0.0f;
      velocity.y = 0.0f;
      SoundManager::getInstance().playSound(SOUND_FRANCKY_01);
      break;
    case 3: timer = 0.2f; break;
    case 2:
    case 4: timer = 0.2f; fire(); break;
    }
  }

  if (state == 0)
  {
    // walking
    followTimer -= delay;
    if (followTimer <= 0.0f)
    {
      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), creatureSpeed ));
      followTimer = 0.25f;
    }
  }
  else
  {
    SoundManager::getInstance().playSound(SOUND_ELECTRICITY, false);
  }

  // collisions
  if (canCollide()) testSpriteCollisions();
  BaseCreatureEntity::animate(delay);

  // current frame
  if (state == 0)
  {
    int r = ((int)(age * 5.0f)) % 4;
    if (r == 2) frame = 0;
    else if (r == 3) frame = 2;
    else frame = r;

    // frame's mirroring
    if (velocity.x > 1.0f)
      isMirroring = true;
    else if (velocity.x < -1.0f)
      isMirroring = false;
  }
  else
  {
    frame =  3 +((int)(age * 7.0f)) % 2;
    isMirroring = game().getPlayer()->getX() > x;
  }
  z = y + 36;
}

void FranckyEntity::calculateBB()
{
  boundingBox.left = (int)x - 32;
  boundingBox.width = 58;
  boundingBox.top = (int)y - 52;
  boundingBox.height =  90;
}


void FranckyEntity::collideMapRight()
{
  velocity.x = -velocity.x;
}

void FranckyEntity::collideMapLeft()
{
  velocity.x = -velocity.x;
}

void FranckyEntity::collideMapTop()
{
  velocity.y = -velocity.y;
}

void FranckyEntity::collideMapBottom()
{
  velocity.y = -velocity.y;
}

void FranckyEntity::drop()
{
  dropItem(ItemBossHeart);
}

void FranckyEntity::render(sf::RenderTarget* app)
{
  EnemyEntity::render(app);
  renderLifeBar(app, tools::getLabel("enemy_francky"));
}

void FranckyEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getMovingStyle() == movWalking)
    inflictsRepulsionTo(entity);
}

void FranckyEntity::inflictsRepulsionTo(BaseCreatureEntity* targetEntity)
{
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

  if (playerEntity != NULL && !playerEntity->isDead())
  {
    Vector2D repulsionVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 450.0f);
    targetEntity->giveRepulsion(true, repulsionVector, 0.5f);
  }
}

void FranckyEntity::dying()
{
  // generates body parts
  new FranckyEntityHand(x + 0.01f * (rand() % 100), y);
  new FranckyEntityHand(x, y + 0.01f * (rand() % 100));
  new FranckyEntityFoot(x + 0.01f * (rand() % 100), y);
  new FranckyEntityFoot(x, y + 0.01f * (rand() % 100));
  new FranckyEntityHead(x, y);

  // FX
  SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_GIANT_SLIME), x, y, 128, 128, 8);
  star->setFrame(4);
  star->setColor(sf::Color(230, 255, 255));
  star->setFading(true);
  star->setZ(y+ 100);
  star->setAge(-0.4f);
  star->setLifetime(0.3f);
  star->setType(ENTITY_EFFECT);
  star->setSpin(400.0f);

  SoundManager::getInstance().playSound(SOUND_SLIME_SMASH);
  EnemyEntity::dying();
}

void FranckyEntity::fire()
{
  SoundManager::getInstance().playSound(SOUND_ELECTRIC_BLAST);

  int health = ((100 * hp) / hpMax);
  float boltLifetime = 1.4f;
  float boltSpeed = 370;

  if (health <= 33)
  {
    boltLifetime = 1.8f;
    boltSpeed = 430;
  }
  else if (health <= 66)
  {
    boltLifetime = 1.6f;
    boltSpeed = 400;
  }

  for (float i = state == 2 ? 0.0f : PI / 8; i < 2 * PI; i +=  PI / 4)
  {
    EnemyBoltEntity* bolt = new EnemyBoltEntity(x, y, ShotTypeLightning, 0, enemyType);
    bolt->setDamages(12);
    float velx = boltSpeed * cos(i);
    float vely = boltSpeed * sin(i);
    bolt->setVelocity(Vector2D(velx, vely));
    bolt->setLifetime(boltLifetime);
  }

  game().makeColorEffect(X_GAME_COLOR_BLUE, 0.3f);
}

///////////////////////////////////////////////////////////////////
//// HAND
///////////////////////////////////////////////////////////////////

FranckyEntityHand::FranckyEntityHand(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_FRANCKY), x, y)
{
  creatureSpeed = 260;
  velocity = Vector2D(creatureSpeed);
  hp = 40;
  meleeDamages = 8;

  imagesProLine = 10;
  bloodColor = BloodRed;
  shadowFrame = 3 * imagesProLine + 5;
  movingStyle = movWalking;

  deathFrame = FRAME_CORPSE_FRANCKY_HAND;
  sprite.setOrigin(32.0f, 46.0f);

  enemyType = EnemyTypeFranckyHand;

  frame = 3 * imagesProLine + 1;
  age = 0.0f;
}

void FranckyEntityHand::animate(float delay)
{
  if (!isAgonising)
  {
    if (age < 0.0f) frame = 3 * imagesProLine + 3;
    else frame = 3 * imagesProLine + 3 + ((int)(age * 6.0f)) % 2;
    isMirroring = velocity.x > 1.0f;
  }
  EnemyEntity::animate(delay);
}

void FranckyEntityHand::calculateBB()
{
    boundingBox.left = (int)x - 20;
    boundingBox.width = 40;
    boundingBox.top = (int)y - 8;
    boundingBox.height =  22;
}

void FranckyEntityHand::collideMapRight()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void FranckyEntityHand::collideMapLeft()
{
    velocity.x = -velocity.x;
    if (repulsion.active) repulsion.velocity.x = -repulsion.velocity.x;
}

void FranckyEntityHand::collideMapTop()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void FranckyEntityHand::collideMapBottom()
{
    velocity.y = -velocity.y;
    if (repulsion.active) repulsion.velocity.y = -repulsion.velocity.y;
}

void FranckyEntityHand::collideWithEnemy(EnemyEntity* entity)
{
  if (entity->getEnemyType() != EnemyTypeSpiderWeb && entity->getMovingStyle() == movWalking)
  {
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), creatureSpeed ));
  }
}

void FranckyEntityHand::drop()
{
  // Nothing
}

///////////////////////////////////////////////////////////////////
//// HEAD
///////////////////////////////////////////////////////////////////

FranckyEntityHead::FranckyEntityHead(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_FRANCKY), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = 150;
  hpMax = hp;
  hpDisplay = hp;
  meleeDamages = 5;

  jumpingDelay = 0.0f;
  enemyType = EnemyTypeFranckyHead;

  bloodColor = BloodRed;
  imagesProLine = 10;
  frame = 3 * imagesProLine;
  shadowFrame = 3 * imagesProLine + 2;

  deathFrame = FRAME_CORPSE_FRANCKY_HEAD;
  agonizingSound = SOUND_FRANCKY_DYING;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);
  resistance[ResistanceLightning] = ResistanceVeryHigh;
  canExplode = false;
  age = 0.0f;
}

void FranckyEntityHead::animate(float delay)
{
  float slimeDelay = delay;
  if (specialState[SpecialStateIce].active) slimeDelay = delay * specialState[SpecialStateIce].param1;

  if (isJumping)
  {
    hVelocity -= 700.0f * slimeDelay;

    h += hVelocity * slimeDelay;

    bool firstTimeGround = false;

    if (h <= 0.0f)
    {
      if (hp <= 0)
        dying();
      else
      {
        h = 0.0f;
        if (isFirstJumping)
        {
          isFirstJumping = false;
          firstTimeGround = true;
          hVelocity = 120.0f;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
          fire();
          game().generateBlood(x, y, BloodRed);
        }
        else
        {
          jumpingDelay = 0.05f;
          isJumping = false;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
          int r = rand() % 4;
          if (r <= 1) SoundManager::getInstance().playSound(SOUND_FRANCKY_01 + r);
          fire();
          game().generateBlood(x, y, BloodRed);
        }
      }
    }
    if (firstTimeGround) frame = 2;
    else if (hVelocity > -60.0f) frame = 1;
    else frame = 0;
  }
  else
  {
    jumpingDelay -= slimeDelay;
    if (jumpingDelay < 0.0f)
    {
      hVelocity = 180.0f;
      isJumping = true;
      isFirstJumping = true;

      float randVel = 280.0f;

      setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), randVel ));
    }
  }

  frame = imagesProLine * 3 + (int)(age * 3) % 2;
  isMirroring = velocity.x > 1.0f;

  EnemyEntity::animate(delay);
  z = y + 14;
}

void FranckyEntityHead::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect((shadowFrame % imagesProLine) * width, (shadowFrame / imagesProLine) * width, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect((frame % imagesProLine) * width + width, (frame / imagesProLine) * width, -width, height));
  else
    sprite.setTextureRect(sf::IntRect((frame % imagesProLine) * width, (frame / imagesProLine) * width, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }

  renderLifeBar(app, tools::getLabel("enemy_francky_head"));
}

void FranckyEntityHead::calculateBB()
{
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 14;
    boundingBox.height =  28;
}

void FranckyEntityHead::collideMapRight()
{
  velocity.x = -velocity.x * 0.8f;
}

void FranckyEntityHead::collideMapLeft()
{
  velocity.x = -velocity.x * 0.8f;
}

void FranckyEntityHead::collideMapTop()
{
  velocity.y = -velocity.y * 0.8f;
}

void FranckyEntityHead::collideMapBottom()
{
  velocity.y = -velocity.y * 0.8f;
}

void FranckyEntityHead::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking)
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), velocity.norm() ));
}

void FranckyEntityHead::fire()
{
  SoundManager::getInstance().playSound(SOUND_ELECTRIC_BLAST);
  EnemyBoltEntity* bolt = new EnemyBoltEntity
            (x, y + 10, ShotTypeLightning, 0, enemyType);
  bolt->setDamages(8);
  bolt->setLifetime(1.5f);
  bolt->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);
  bolt->setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), 400 ));
}



///////////////////////////////////////////////////////////////////
//// FOOT
///////////////////////////////////////////////////////////////////

FranckyEntityFoot::FranckyEntityFoot(float x, float y)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_FRANCKY), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = 40;
  meleeDamages = 5;

  jumpingDelay = 0.0f;

  enemyType = EnemyTypeFranckyFoot;

  bloodColor = BloodRed;
  imagesProLine = 10;
  frame = 3 * imagesProLine + 6;
  shadowFrame = 3 * imagesProLine + 8;

  deathFrame = FRAME_CORPSE_FRANCKY_FOOT;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);
  canExplode = false;
  age = 0.0f;
}

void FranckyEntityFoot::animate(float delay)
{
  float slimeDelay = delay;
  if (specialState[SpecialStateIce].active) slimeDelay = delay * specialState[SpecialStateIce].param1;

  if (isJumping)
  {
    hVelocity -= 700.0f * slimeDelay;

    h += hVelocity * slimeDelay;

    bool firstTimeGround = false;

    if (h <= 0.0f)
    {
      if (hp <= 0)
        dying();
      else
      {
        h = 0.0f;
        if (isFirstJumping)
        {
          isFirstJumping = false;
          firstTimeGround = true;
          hVelocity = 120.0f;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
          game().generateBlood(x, y, BloodRed);
        }
        else
        {
          jumpingDelay = 0.05f;
          isJumping = false;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
          game().generateBlood(x, y, BloodRed);
        }
      }
    }
    if (firstTimeGround) frame = 2;
    else if (hVelocity > -60.0f) frame = 1;
    else frame = 0;
  }
  else
  {
    jumpingDelay -= slimeDelay;
    if (jumpingDelay < 0.0f)
    {
      hVelocity = 180.0f + rand() % 180;
      isJumping = true;
      isFirstJumping = true;

      float randVel = 280.0f + rand() % 250;

      setVelocity(Vector2D(randVel ));
    }
  }

  frame = imagesProLine * 3 + 6 + (int)(age * 3) % 2;
  isMirroring = velocity.x > 1.0f;

  EnemyEntity::animate(delay);
  z = y + 14;
}

void FranckyEntityFoot::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect((shadowFrame % imagesProLine) * width, (shadowFrame / imagesProLine) * width, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect((frame % imagesProLine) * width + width, (frame / imagesProLine) * width, -width, height));
  else
    sprite.setTextureRect(sf::IntRect((frame % imagesProLine) * width, (frame / imagesProLine) * width, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void FranckyEntityFoot::calculateBB()
{
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 14;
    boundingBox.height =  28;
}

void FranckyEntityFoot::collideMapRight()
{
  velocity.x = -velocity.x * 0.8f;
}

void FranckyEntityFoot::collideMapLeft()
{
  velocity.x = -velocity.x * 0.8f;
}

void FranckyEntityFoot::collideMapTop()
{
  velocity.y = -velocity.y * 0.8f;
}

void FranckyEntityFoot::collideMapBottom()
{
  velocity.y = -velocity.y * 0.8f;
}

void FranckyEntityFoot::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking)
    setVelocity(Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), velocity.norm() ));
}
