#include "EnemyEntity.h"
#include "PlayerEntity.h"
#include "ExplosionEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

const float FACING_DELAY = 0.17f;

EnemyEntity::EnemyEntity(sf::Texture* image, float x, float y)
  : BaseCreatureEntity (image, x, y, 64, 64)
{
  type = ENTITY_ENEMY;
  bloodColor = BloodRed;

  z = y;
  h = 0;
  age = -0.001f * (rand()%800) - 0.4f;

  deathFrame = -1;
  dyingFrame = -1;
  dyingSound = SOUND_ENNEMY_DYING;
  agonizingSound = SOUND_NONE;
  hurtingSound = SOUND_NONE;
  isAgonising = false;

  enemyType = NB_ENEMY;
  meleeLevel = 0;
  meleeType = ShotTypeStandard;
  canExplode = true;

  label_dy = 0;

  nextFacingDirection = 0;
  facingTimer = -1.0f;
}

enemyTypeEnum EnemyEntity::getEnemyType()
{
  return enemyType;
}

void EnemyEntity::checkNextFacing(float dt)
{
  if (facingTimer > 0.0f) facingTimer -= dt;
  if (facingDirection != nextFacingDirection && facingTimer <= 0.0f)
  {
    facingDirection = nextFacingDirection;
    facingTimer = FACING_DELAY;
  }
}

void EnemyEntity::setLabelDy(float label_dy)
{
  this->label_dy = label_dy;
}

void EnemyEntity::animate(float delay)
{
  if (isAgonising)
  {
    if (hpDisplay > hp) hpDisplay--;

    if (h < -0.01f)
    {
      isAgonising = false;
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

  if (canCollide()) testSpriteCollisions();
  if (age > 0.0f)
    BaseCreatureEntity::animate(delay);
  else
    age += delay;

  // FIX enemy not on map
  if (x < TILE_WIDTH / 2 || x > MAP_WIDTH * TILE_WIDTH || y < TILE_HEIGHT / 2 || y > MAP_HEIGHT * TILE_HEIGHT)
    isDying = true;
}

void EnemyEntity::calculateBB()
{
  boundingBox.left = (int)x - width / 2;
  boundingBox.width = width;
  boundingBox.top = (int)y - height / 2;
  boundingBox.height =  height;
}

void EnemyEntity::collideMapRight()
{
  velocity.x = 0.0f;
}

void EnemyEntity::collideMapLeft()
{
  velocity.x = 0.0f;
}

void EnemyEntity::collideMapTop()
{
  velocity.y = 0.0f;
}

void EnemyEntity::collideMapBottom()
{
  velocity.y = 0.0f;
}

void EnemyEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        if (playerEntity->hurt(getHurtParams(meleeDamages, meleeType, meleeLevel, false, SourceTypeBolt, enemyType, false)) > 0)
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);

          if (playerEntity->isEquiped(EQUIP_ROBE_ADVANCED))
          {
            giveRepulsion(true, Vector2D(playerEntity->getX(), playerEntity->getY()).vectorTo(Vector2D(x, y), 800), 0.8f);
            SoundManager::getInstance().playSound(SOUND_ELECTRIC_CHARGE);

            star->setScale(1.5f, 1.5f);
            star->setColor(sf::Color(220, 180, 255));
            star->setLifetime(1.3f);
            game().makeColorEffect(X_GAME_COLOR_VIOLET, 0.2f);
          }
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
      if (entity->getType() >= ENTITY_ENEMY && entity->getType() <= ENTITY_ENEMY_MAX)
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

void EnemyEntity::collideWithBolt(BoltEntity* boltEntity)
{
  float xs = (x + boltEntity->getX()) / 2;
  float ys = (y + boltEntity->getY()) / 2;

  int maxDamages = hp;
  int boltDamages = hurt(getHurtParams
                           (boltEntity->getDamages(),
                           boltEntity->getBoltType(),
                           boltEntity->getLevel(),
                           boltEntity->isCritical(),
                           SourceTypeBolt,
                           enemyType, boltEntity->getGoThrough()));
  if (hp > 0)
  {
    boltEntity->loseDamages(boltEntity->getDamages());
  }
  else
  {
    boltEntity->loseDamages(maxDamages >= boltDamages ? boltDamages : maxDamages);
  }

  if (bloodColor > BloodNone) game().generateBlood(x, y, bloodColor);
  SoundManager::getInstance().playSound(SOUND_IMPACT);

  SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_HURT_IMPACT), xs, ys);
  star->setFading(true);
  star->setZ(y+ 100);
  star->setLifetime(0.7f);
  star->setType(ENTITY_EFFECT);
  star->setSpin(400.0f);

  if (boltEntity->getBoltType() == ShotTypeStone)
  {
    float factor = (boltEntity->isFromPlayer() && game().getPlayer()->isEquiped(EQUIP_RAPID_SHOT)) ? 0.25f : 1.0f;
    float recoilVelocity = factor * STONE_DECOIL_VELOCITY[boltEntity->getLevel()];
    float recoilDelay = factor * STONE_DECOIL_DELAY[boltEntity->getLevel()];

    Vector2D recoilVector = Vector2D(0, 0).vectorTo(boltEntity->getVelocity(),
                            recoilVelocity );
    giveRepulsion(true, recoilVector, recoilDelay);
  }

  boltEntity->collide();
}

int EnemyEntity::getCollisionDirection(BoltEntity* boltEntity)
{
  int tol = 4;

  float bx = boltEntity->getX();
  float by = boltEntity->getY();

  int alignX, alignY;

  if (bx + tol < x) alignX = 4;
  else if (bx - tol > x) alignX = 6;
  else alignX = 5;

  if (by + tol < y) alignY = 8;
  else if (by - tol > y) alignY = 2;
  else alignY = 5;

  int collisionDir = 5;
  if (alignX == 4)
  {
    if (alignY == 8) collisionDir = 7;
    else if (alignY == 5) collisionDir = 4;
    else if (alignY == 2) collisionDir = 1;
  }
  else if (alignX == 5)
  {
    if (alignY == 8) collisionDir = 8;
    else if (alignY == 5) collisionDir = 5;
    else if (alignY == 2) collisionDir = 2;
  }
  else if (alignX == 6)
  {
    if (alignY == 8) collisionDir = 9;
    else if (alignY == 5) collisionDir = 6;
    else if (alignY == 2) collisionDir = 3;
  }

  return collisionDir;
}

void EnemyEntity::collideWithEnemy(EnemyEntity* entity)
{
  // To implement the behaviour when colliding with another ennemy
}

int EnemyEntity::hurt(StructHurt hurtParam)
{
  int hurtedHp = BaseCreatureEntity::hurt(hurtParam);
  if (hurtedHp > 0 && hurtingSound != SOUND_NONE && hp > 0)
    SoundManager::getInstance().playSound(hurtingSound);
  return hurtedHp;
}

void EnemyEntity::dying()
{
  if (dyingFrame > -1)
  {
    isAgonising = true;
    hVelocity = 200.0f;
    if (agonizingSound != SOUND_NONE) SoundManager::getInstance().playSound(agonizingSound);
  }
  else // dyingFrame == -1
  {
    isDying = true;
    game().addCorpse(x, y, deathFrame);
    if (dyingSound != SOUND_NONE) SoundManager::getInstance().playSound(dyingSound);
  }

  if (bloodColor != BloodNone) for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);
  drop();
  game().addKilledEnemy(enemyType, hurtingType);
}

void EnemyEntity::dropItem(enumItemType item)
{
  ItemEntity* newItem = new ItemEntity(item, x, y);
  newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, 0, 0);
  newItem->setVelocity(Vector2D(50.0f + rand()% 140));
  newItem->setViscosity(0.96f);
  newItem->startsJumping();
}

void EnemyEntity::drop()
{
  if (rand() % 40 == 0)
  {
    if (rand() % 2 == 0)
    {
      dropItem(ItemScrollRevelation);
    }
    else
    {
      dropItem((enumItemType)(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED));
    }
  }
  else
  {
    if (rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }
    if (game().getPlayer()->isEquiped(EQUIP_LUCK) && rand() % 5 == 0)
    {
      dropItem(ItemCopperCoin);
    }

    if (rand() % 25 == 0)
    {
      dropItem(ItemHealthVerySmall);
    }
  }
}

bool EnemyEntity::canCollide()
{
  return (!isAgonising);
}

void EnemyEntity::render(sf::RenderTarget* app)
{
  if (isAgonising || (isDying && dyingFrame > -1))
  {
    if (shadowFrame > -1)
    {
      // shadow
      int nx = shadowFrame;
      int ny = 0;

      if (imagesProLine > 0 && shadowFrame >= imagesProLine)
      {
        nx = shadowFrame % imagesProLine;
        ny = shadowFrame / imagesProLine;
      }
      sprite.setPosition(x, y);
      sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));
      app->draw(sprite);
    }
    int nx = dyingFrame;
    int ny = 0;
    if (imagesProLine > 0)
    {
      nx = dyingFrame % imagesProLine;
      ny = dyingFrame / imagesProLine;
    }
    sprite.setPosition(x, y - h);
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect(nx * width + width, ny * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));

    app->draw(sprite);
  }

  else
    BaseCreatureEntity::render(app);
}

void EnemyEntity::renderLifeBar(sf::RenderTarget* app, std::string label)
{
  game().addLifeBarToDisplay(label, hpDisplay, hpMax);
}

bool EnemyEntity::testEntityInMap()
{
  int collideCounter = 6;

  while (isCollidingWithMap() && collideCounter > 0)
  {
    int movCounter = 100;
    if (collideWithMap(DIRECTION_LEFT) && !collideWithMap(DIRECTION_RIGHT))
    {
      x = (float)((int)x);
      while (collideWithMap(DIRECTION_LEFT) && movCounter > 0)
      {
        x--;
        movCounter--;
      }
    }
    else if (collideWithMap(DIRECTION_RIGHT) && !collideWithMap(DIRECTION_LEFT))
    {
      x = (float)((int)x);
      while (collideWithMap(DIRECTION_RIGHT) && movCounter > 0)
      {
        x++;
        movCounter--;
      }
    }
    else if (collideWithMap(DIRECTION_BOTTOM) && !collideWithMap(DIRECTION_TOP))
    {
      y = (float)((int)y);
      while (collideWithMap(DIRECTION_BOTTOM) && movCounter > 0)
      {
        y--;
        movCounter--;
      }
    }
    else if (collideWithMap(DIRECTION_TOP) && !collideWithMap(DIRECTION_BOTTOM))
    {
      y = (float)((int)y);
      while (collideWithMap(DIRECTION_TOP) && movCounter > 0)
      {
        y++;
        movCounter--;
      }
    }
    collideCounter--;
  }

  return (collideCounter > 0);
}
