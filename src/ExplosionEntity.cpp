#include "ExplosionEntity.h"
#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

ExplosionEntity::ExplosionEntity(float x, float y, explosionTypeEnum explosionType, int damage, enemyTypeEnum enemyType, bool canHurtPlayer)
  : SpriteEntity(ImageManager::getInstance().getImage(IMAGE_EXPLOSION), x, y, 100, 100)
{
  type = ENTITY_EXPLOSION;
  this->explosionType = explosionType;
  imagesProLine = 6;
  lifetime = 0.6f;
  this->canHurtPlayer = canHurtPlayer;

  this->damage = damage;

  switch (explosionType)
  {
    case ExplosionTypeStandard: frame = 0; break;
    case ExplosionTypeViolet: frame = 6; break;
  }

  sprite.setOrigin(50, 50);
  testCollisions();
}

void ExplosionEntity::setCanHurtPlayer(bool can)
{
  canHurtPlayer = can;
}

void ExplosionEntity::animate(float delay)
{
  if (lifetime > 0)
  {
    if (age >= lifetime) isDying = true;
  }
  age += delay;

  z = y + height / 2;

  frame = age / lifetime * 6;
  if (frame > 5) frame = 5;
  switch (explosionType)
  {
    case ExplosionTypeStandard: break;
    case ExplosionTypeViolet: frame += 6; break;
  }
}

void ExplosionEntity::render(sf::RenderTarget* app)
{
  SpriteEntity::render(app);
}

void ExplosionEntity::dying()
{
  isDying = true;
}

void ExplosionEntity::testCollisions()
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

    BaseCreatureEntity* entity = dynamic_cast<BaseCreatureEntity*>(e);
    if (entity != NULL)
		{
		  bool ok = true;
		  if (!canHurtPlayer && entity->getType() == ENTITY_PLAYER) ok = false;
		  if (ok && entity->getHp() > 0 && entity->canCollide())
      {
        entity->calculateBB();

        sf::IntRect bb;
        bb.left = x - 90;
        bb.width = 180;
        bb.top = y - 90;
        bb.height = 180;

        if (bb.intersects(entity->getBoundingBox()))
        {
          // TODO explosion type
          if (damage > 0) entity->hurt(BaseCreatureEntity::getHurtParams(damage, ShotTypeFire, 0, false, SourceTypeExplosion, enemyType, false));

          Vector2D repulsionVector = Vector2D(x, y).vectorTo(Vector2D(entity->getX(), entity->getY()), 800.0f );
          entity->giveRepulsion(true, repulsionVector, 1.0f);
        }
      }
		}
	}
}
