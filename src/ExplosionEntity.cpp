#include "ExplosionEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

ExplosionEntity::ExplosionEntity(float x, float y, int damage)
  : SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_EXPLOSION64), x, y, 100, 100)
{
  type = ENTITY_EXPLOSION;
  frame = 0;
  imagesProLine = 14;
  lifetime = 1.0f;
  spin = 900.0f;

  this->damage = damage;

  sprite.setOrigin(50, 50);
  testCollisions();
}

void ExplosionEntity::animate(float delay)
{
  if (lifetime > 0)
  {
    if (age >= lifetime) isDying = true;
  }
  age += delay;
  //angle += spin * delay;

  z = y + height / 2;

  if (age > lifetime - 0.5f)
  {
    float f = (lifetime - age) * 2.0f;
    sprite.setScale(f, f);
  }

  frame = age / lifetime * 14;
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
  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

    BaseCreatureEntity* entity = dynamic_cast<BaseCreatureEntity*>(e);
    if (entity != NULL)
		{
		  if (entity->getHp() > 0 && entity->canCollide())
      {
        entity->calculateBB();

        sf::IntRect bb;
        bb.left = x - 90;
        bb.width = 180;
        bb.top = y - 90;
        bb.height = 180;

        if (bb.intersects(entity->getBoundingBox()))
        {
          entity->hurt(damage, ShotTypeFire, 0, false);

          Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(entity->getX(), entity->getY()), 800.0f );
          entity->giveRecoil(true, recoilVector, 1.0f);
        }
      }
		}
	}
}
