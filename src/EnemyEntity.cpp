#include "EnemyEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"

EnemyEntity::EnemyEntity(sf::Texture* image, float x, float y)
  : BaseCreatureEntity (image, x, y, 64, 64)
{
  type = ENTITY_ENNEMY;
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
}

enemyTypeEnum EnemyEntity::getEnemyType()
{
  return enemyType;
}

void EnemyEntity::animate(float delay)
{
  if (isAgonising)
  {
    if (h < -0.01f)
    {
      isDying = true;
      SpriteEntity* corpse;
      if (deathFrame >= FRAME_CORPSE_KING_RAT)
      {
        corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES_BIG), x, y, 128, 128);
        corpse->setFrame(deathFrame - FRAME_CORPSE_KING_RAT);
      }
      else
      {
        corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
        corpse->setFrame(deathFrame);
        corpse->setImagesProLine(10);
      }

      corpse->setZ(OFFSET_Y);
      corpse->setType(ENTITY_CORPSE);
      if (dyingSound != SOUND_NONE) SoundManager::getSoundManager()->playSound(dyingSound);
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
        if (playerEntity->hurt(meleeDamages, ShotTypeStandard, 0, false))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);
        }
        inflictsRecoilTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        collideWithBolt(boltEntity);
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENNEMY && entity->getType() <= ENTITY_ENNEMY_MAX)
      {
        if (this != entity)
        {
          EnemyEntity* ennemyEntity = static_cast<EnemyEntity*>(entity);
          if (ennemyEntity->canCollide()) collideWithEnnemy(entity);
        }
      }
    }
  }
}

void EnemyEntity::collideWithBolt(BoltEntity* boltEntity)
{
  float xs = (x + boltEntity->getX()) / 2;
  float ys = (y + boltEntity->getY()) / 2;

  boltEntity->collide();

  hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel(), boltEntity->isCritical());
  if (bloodColor > BloodNone) game().generateBlood(x, y, bloodColor);
  SoundManager::getSoundManager()->playSound(SOUND_IMPACT);

  SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
  star->setFading(true);
  star->setZ(y+ 100);
  star->setLifetime(0.7f);
  star->setType(ENTITY_EFFECT);
  star->setSpin(400.0f);

  if (boltEntity->getBoltType() == ShotTypeStone)
  {
    float recoilVelocity = STONE_DECOIL_VELOCITY[boltEntity->getLevel()];
    float recoilDelay = STONE_DECOIL_DELAY[boltEntity->getLevel()];

    Vector2D recoilVector = Vector2D(0, 0).vectorTo(boltEntity->getVelocity(),
                            recoilVelocity );
    giveRecoil(true, recoilVector, recoilDelay);
  }
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

void EnemyEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  // To implement the behaviour when colliding with another ennemy
}

bool EnemyEntity::hurt(int damages, enumShotType hurtingType, int level, bool critical)
{
  bool hurted = BaseCreatureEntity::hurt(damages, hurtingType, level, critical);
  if (hurted && hurtingSound != SOUND_NONE && hp > 0)
    SoundManager::getSoundManager()->playSound(hurtingSound);
  return hurted;
}

void EnemyEntity::dying()
{
  if (dyingFrame == -1)
  {
    isDying = true;
    SpriteEntity* corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
    corpse->setZ(OFFSET_Y);
    corpse->setImagesProLine(10);
    corpse->setFrame(deathFrame);
    corpse->setType(ENTITY_CORPSE);
    if (dyingSound != SOUND_NONE) SoundManager::getSoundManager()->playSound(dyingSound);
  }
  else
  {
    isAgonising = true;
    hVelocity = 200.0f;
    if (agonizingSound != SOUND_NONE) SoundManager::getSoundManager()->playSound(agonizingSound);
  }
  if (bloodColor != BloodNone) for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);
  drop();
  game().addKilledEnemy(enemyType);
}

void EnemyEntity::drop()
{
  if (rand() % 5 == 0)
  {
    ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
    newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
  }

  if (rand() % 25 == 0)
  {
    ItemEntity* newItem = new ItemEntity(ItemHealthVerySmall, x, y);
    newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
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
      sprite.setPosition(x, y);
      sprite.setTextureRect(sf::IntRect(shadowFrame * width, 0, width, height));
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

void EnemyEntity::displayLifeBar(std::string name, float posY, sf::RenderTarget* app)
{
  float l = hpDisplay * ((MAP_WIDTH - 1) * TILE_WIDTH) / hpMax;

  sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
  rectangle.setFillColor(sf::Color(0, 0, 0,128));
  rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, posY));
  app->draw(rectangle);

  rectangle.setSize(sf::Vector2f(l, 25));
  rectangle.setFillColor(sf::Color(190, 20, 20));
  rectangle.setPosition(sf::Vector2f(OFFSET_X + TILE_WIDTH / 2, posY));
  app->draw(rectangle);

  game().write(          name,
                         18,
                         OFFSET_X + TILE_WIDTH / 2 + 10.0f,
                         posY + 1.0f,
                         ALIGN_LEFT,
                         sf::Color(255, 255, 255),
                         app, 0, 0);
}
