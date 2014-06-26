#include "EnnemyEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include <iostream>
#include "WitchBlastGame.h"

EnnemyEntity::EnnemyEntity(sf::Texture* image, float x, float y)
    : BaseCreatureEntity (image, x, y, 64, 64)
{
  type = ENTITY_ENNEMY;
  bloodColor = bloodRed;

  z = y;
  h = 0;
  age = -0.001f * (rand()%800) - 0.4f;

  deathFrame = -1;
  dyingSound = SOUND_ENNEMY_DYING;
  agonizingSound = SOUND_NONE;
  hurtingSound = SOUND_NONE;
  isAgonising = false;
}

void EnnemyEntity::animate(float delay)
{
  if (isAgonising)
  {
    if (h < -0.01f)
    {
      isDying = true;
      SpriteEntity* corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
      corpse->setZ(OFFSET_Y);
      corpse->setFrame(deathFrame);
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

void EnnemyEntity::calculateBB()
{
    boundingBox.left = (int)x - width / 2;
    boundingBox.width = width;
    boundingBox.top = (int)y - height / 2;
    boundingBox.height =  height;
}

void EnnemyEntity::collideMapRight()
{
    velocity.x = 0.0f;
}

void EnnemyEntity::collideMapLeft()
{
    velocity.x = 0.0f;
}

void EnnemyEntity::collideMapTop()
{
    velocity.y = 0.0f;
}

void EnnemyEntity::collideMapBottom()
{
    velocity.y = 0.0f;
}

void EnnemyEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead())
      {
        if (playerEntity->hurt(meleeDamages, ShotTypeStandard, 0))
        {
          float xs = (x + playerEntity->getX()) / 2;
          float ys = (y + playerEntity->getY()) / 2;
          SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(16);
          star->setSpin(400.0f);
        }
        inflictsRecoilTo(playerEntity);
      }

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        boltEntity->collide();
        hurt(boltEntity->getDamages(), boltEntity->getBoltType(), boltEntity->getLevel());
        game().generateBlood(x, y, bloodColor);
        SoundManager::getSoundManager()->playSound(SOUND_IMPACT);

        float xs = (x + boltEntity->getX()) / 2;
        float ys = (y + boltEntity->getY()) / 2;
        SpriteEntity* star = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_STAR_2), xs, ys);
        star->setFading(true);
        star->setZ(y+ 100);
        star->setLifetime(0.7f);
        star->setType(16);
        star->setSpin(400.0f);

        if (boltEntity->getBoltType() == ShotTypeStone)
        {
          Vector2D recoilVector = Vector2D(boltEntity->getX(),
                                           boltEntity->getY()).vectorTo(Vector2D(x, y),
                                           STONE_DECOIL_VELOCITY[boltEntity->getLevel()] );
          giveRecoil(true, recoilVector, STONE_DECOIL_DELAY[boltEntity->getLevel()]);
        }
      }
    }
    else // collision with other enemy ?
    {
      if (entity->getType() >= ENTITY_ENNEMY && entity->getType() <= ENTITY_ENNEMY_MAX)
      {
        if (this != entity)
          collideWithEnnemy(entity);
      }
    }
  }
}

void EnnemyEntity::collideWithEnnemy(GameEntity* collidingEntity)
{
  // To implement the behaviour when colliding with another ennemy
}

bool EnnemyEntity::hurt(int damages, enumShotType hurtingType, int level)
{
  bool hurted = BaseCreatureEntity::hurt(damages, hurtingType, level);
  if (hurted && hurtingSound != SOUND_NONE && hp > 0)
    SoundManager::getSoundManager()->playSound(hurtingSound);
  return hurted;
}

void EnnemyEntity::dying()
{
  if (deathFrame == -1)
  {
    isDying = true;
    SpriteEntity* corpse = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_CORPSES), x, y, 64, 64);
    corpse->setZ(OFFSET_Y);
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
  for (int i = 0; i < 4; i++) game().generateBlood(x, y, bloodColor);
  drop();
}

void EnnemyEntity::drop()
{
  if (rand() % 5 == 0)
  {
    ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
    newItem->setMap(map, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
  }
}

bool EnnemyEntity::canCollide()
{
  return true;
}

void EnnemyEntity::render(sf::RenderTarget* app)
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
    sprite.setTextureRect(sf::IntRect(nx * width, ny * height, width, height));

    app->draw(sprite);
  }

  else
    BaseCreatureEntity::render(app);
}
