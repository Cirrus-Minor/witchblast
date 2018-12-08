#include "SlimeEntity.h"
#include "PlayerEntity.h"
#include "EnemyBoltEntity.h"
#include "ExplosionEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

SlimeEntity::SlimeEntity(float x, float y, slimeTypeEnum slimeType, bool invocated)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_SLIME), x, y)
{
  creatureSpeed = 0.0f;
  velocity = Vector2D(0.0f, 0.0f);
  hp = SLIME_HP;
  meleeDamages = SLIME_DAMAGES;
  this->slimeType = slimeType;

  this->invocated = invocated;
  if (invocated)
  {
    type = ENTITY_ENEMY_INVOCATED;
    jumpingDelay = 0.1f;
    age = 0.0f;
  }
  else
  {
    jumpingDelay = 0.6f + 0.1f * (rand() % 20);
  }

  if (slimeType == SlimeTypeBlue)
  {
    resistance[ResistanceFrozen] = ResistanceImmune;
    resistance[ResistanceIce] = ResistanceVeryHigh;
    resistance[ResistanceFire] = ResistanceVeryLow;
    enemyType = invocated ? EnemyTypeSlimeBlue_invocated : EnemyTypeSlimeBlue;
    deathFrame = FRAME_CORPSE_SLIME_BLUE;
  }
  else if (slimeType == SlimeTypeRed)
  {
    resistance[ResistanceIce] = ResistanceVeryLow;
    resistance[ResistanceFire] = ResistanceVeryHigh;
    enemyType = invocated ? EnemyTypeSlimeRed_invocated : EnemyTypeSlimeRed;
    deathFrame = FRAME_CORPSE_SLIME_RED;
  }
  else if (slimeType == SlimeTypeViolet)
  {
    enemyType = invocated ? EnemyTypeSlimeViolet_invocated : EnemyTypeSlimeViolet;
    canExplode = false;
    deathFrame = FRAME_CORPSE_SLIME_VIOLET;
  }
  else
  {
    enemyType = invocated ? EnemyTypeSlime_invocated : EnemyTypeSlime;
    deathFrame = FRAME_CORPSE_SLIME;
  }

  bloodColor = BloodGreen;
  frame = 0;
  shadowFrame = 3;
  imagesProLine = 4;

  isJumping = false;
  h = 0.0f;

  viscosity = 0.98f;
  sprite.setOrigin(32, 44);

  isPet = false;
  willExplode = false;
  noCollisionTimer = -1.0f;
}

void SlimeEntity::setH(float h)
{
  this->h = h;
}

void SlimeEntity::animate(float delay)
{
  if (noCollisionTimer > 0.0f) noCollisionTimer -= delay;
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
          hVelocity = 160.0f;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT);
          if (slimeType == SlimeTypeBlue || slimeType == SlimeTypeRed)
            fire();
          else if (slimeType == SlimeTypeViolet)
          {
            hp = 0;
            dying();
          }
        }
        else
        {
          jumpingDelay = 0.4f + 0.1f * (rand() % 20);
          isJumping = false;
          SoundManager::getInstance().playSound(SOUND_SLIME_IMAPCT_WEAK);
        }
      }
    }
    if (firstTimeGround) frame = 0;
    else if (hVelocity > -190.0f) frame = 2;
    else frame = 1;
  }
  else
  {
    jumpingDelay -= slimeDelay;
    if (jumpingDelay < 0.0f)
    {
      SoundManager::getInstance().playSound(SOUND_SLIME_JUMP);
      hVelocity = 350.0f + rand() % 300;
      isJumping = true;
      isFirstJumping = true;

      float randVel = 250.0f + rand() % 250;

      if (!game().getPlayer()->isEquiped(EQUIP_MANUAL_SLIMES) && rand() % 2 == 0)
      {
        setVelocity(Vector2D(x, y).vectorTo(game().getPlayerPosition(), randVel ));
      }
      else
        velocity = Vector2D(randVel);
    }
    else if (jumpingDelay < 0.1f)
      frame = 1;
    else frame = 0;
  }

  EnemyEntity::animate(delay);
  z = y + 14;
}

void SlimeEntity::makeExplode()
{
  if (isJumping)
    willExplode = true;
  else
    BaseCreatureEntity::makeExplode();
}

void SlimeEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (!isPet && (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT ) )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        if (playerEntity->hurt(getHurtParams(meleeDamages, meleeType, meleeDamages, false, SourceTypeMelee, enemyType, false)))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);
        }
        inflictsRepulsionTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        collideWithBolt(boltEntity);
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX_COUNT)
      {
        if (this != entity)
        {
          EnemyEntity* enemyEntity = static_cast<EnemyEntity*>(entity);
          if (enemyEntity->canCollide()) collideWithEnemy(enemyEntity);
        }
      }
    }
  }
}

void SlimeEntity::render(sf::RenderTarget* app)
{
  if (!isDying && shadowFrame > -1)
  {
    // shadow
    sprite.setPosition(x, y);
    sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
    app->draw(sprite);
  }
  sprite.setPosition(x, y - h);
  sprite.setTextureRect(sf::IntRect(frame * width, slimeType * height, width, height));
  app->draw(sprite);

  if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }
}

void SlimeEntity::calculateBB()
{
  if (isPet)
  {
    boundingBox.left = (int)x - 1;
    boundingBox.width = 2;
    boundingBox.top = (int)y - 1;
    boundingBox.height =  2;
  }
  else
  {
    boundingBox.left = (int)x - width / 2 + SLIME_BB_LEFT;
    boundingBox.width = width - SLIME_BB_WIDTH_DIFF;
    boundingBox.top = (int)y - height / 2 + SLIME_BB_TOP - 15;
    boundingBox.height =  height - SLIME_BB_HEIGHT_DIFF;
  }
}

void SlimeEntity::collideMapRight()
{
 // if (x > OFFSET_X + MAP_WIDTH * TILE_WIDTH)
    velocity.x = -velocity.x * 0.8f;
}

void SlimeEntity::collideMapLeft()
{
 // if (x < OFFSET_X + MAP_WIDTH )
    velocity.x = -velocity.x * 0.8f;
}

void SlimeEntity::collideMapTop()
{
//  if (y > OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT)
    velocity.y = -velocity.y * 0.8f;
}

void SlimeEntity::collideMapBottom()
{
 // if (y < OFFSET_Y + MAP_HEIGHT )
    velocity.y = -velocity.y * 0.8f;
}

void SlimeEntity::collideWithEnemy(EnemyEntity* entity)
{
  if (repulsion.active && repulsion.stun) return;

  if (entity->getMovingStyle() == movWalking)
  {
    Vector2D vel = Vector2D(entity->getX(), entity->getY()).vectorTo(Vector2D(x, y), 100.0f );
    giveRepulsion(false, vel, 0.3f);

    computeFacingDirection();
  }
}

bool SlimeEntity::collideWithMap(int direction)
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
          if (!game().getCurrentMap()->isFlyable(xTile, yTile))
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

void SlimeEntity::dying()
{
  if (slimeType == SlimeTypeViolet) explode();
  else if (willExplode) BaseCreatureEntity::makeExplode();
  EnemyEntity::dying();
}

void SlimeEntity::prepareDying()
{
    if (!isJumping)
      dying();
}

bool SlimeEntity::canCollide()
{
  if (noCollisionTimer > 0.0f) return false;
  return h <= 70.0f && hp > 0;
}

void SlimeEntity::disableCollidingTemporary()
{
  noCollisionTimer = 0.8f;
}

BaseCreatureEntity::enumMovingStyle SlimeEntity::getMovingStyle()
{
  if (h <= 70.0f)
    return movWalking;
  else
    return movFlying;
}

void SlimeEntity::fire()
{
  for (int i = 0; i < 4; i++)
  {
    EnemyBoltEntity* bolt;

    if (slimeType == SlimeTypeBlue)
    {
      bolt = new EnemyBoltEntity(x, y, ShotTypeIce, 0, enemyType);
      bolt->setDamages(5);
    }
    else if (slimeType == SlimeTypeRed)
    {
      bolt = new EnemyBoltEntity(x, y, ShotTypeFire, 0, enemyType);
      bolt->setDamages(8);
    }
    else
      return;

    switch (i)
    {
      case 0: bolt->setVelocity(Vector2D(SLIME_FIRE_VELOCITY, 0)); break;
      case 1: bolt->setVelocity(Vector2D(-SLIME_FIRE_VELOCITY, 0)); break;
      case 2: bolt->setVelocity(Vector2D(0, SLIME_FIRE_VELOCITY)); break;
      case 3: bolt->setVelocity(Vector2D(0, -SLIME_FIRE_VELOCITY)); break;
    }
  }
  if (slimeType == SlimeTypeBlue) SoundManager::getInstance().playSound(SOUND_BLAST_ICE);
  else if (slimeType == SlimeTypeRed) SoundManager::getInstance().playSound(SOUND_BLAST_FIRE);
  else SoundManager::getInstance().playSound(SOUND_BLAST_FLOWER);
}

void SlimeEntity::explode()
{
  int damage = 12;
  if (isPet) damage = game().getPlayer()->isEquiped(EQUIP_BOOK_MAGIC_II) ? 24 : 18;

  new ExplosionEntity(x, y, ExplosionTypeStandard, damage, enemyType, true);
  game().makeShake(1.0f);
  SoundManager::getInstance().playSound(SOUND_BOOM_00);
}

void SlimeEntity::makePet(int direction)
{
  isPet = true;
  hVelocity = 450.0f;
  SoundManager::getInstance().playSound(SOUND_SLIME_JUMP);
  isJumping = true;
  isFirstJumping = true;

  switch (direction)
  {
    case 4: velocity.x = -350.0f; velocity.y = -0; break;
    case 6: velocity.x = 350.0f; velocity.y = -0; break;
    case 2: velocity.y = 350.0f; velocity.x = -0; break;
    default: velocity.y = -350.0f; velocity.x = -0; break;
  }
}

void SlimeEntity::drop()
{
  if (!invocated) EnemyEntity::drop();
}
