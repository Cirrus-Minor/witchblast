#include "ExplosionEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

ExplosionEntity::ExplosionEntity(float x, float y)
  : SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_EXPLOSION64), x, y, 64, 64)
{
  type = ENTITY_EXPLOSION;
  frame = 0;
  imagesProLine = 10;
  lifetime = 1.0f;
  spin = 900.0f;

  testCollisions();
}

void ExplosionEntity::animate(float delay)
{
  if (lifetime > 0)
  {
    if (age >= lifetime) isDying = true;
  }
  age += delay;
  angle += spin * delay;

  z = y + height / 2;

  if (age > lifetime - 0.5f)
  {
    float f = (lifetime - age) * 2.0f;
    sprite.setScale(f, f);
  }
}

void ExplosionEntity::render(sf::RenderTarget* app)
{
  SpriteEntity::render(app);
  /*if (game().getShowLogical())
  {
    displayBoundingBox(app);
    displayCenterAndZ(app);
  }*/
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

		if ( (e->getType() >= ENTITY_ENNEMY && e->getType() <= ENTITY_ENNEMY_MAX)
        || e->getType() == ENTITY_PLAYER)
		{
		  BaseCreatureEntity* entity = static_cast<BaseCreatureEntity*>(e);

		  if (entity->getHp() > 0 && entity->canCollide())
      {
        entity->calculateBB();

        sf::IntRect bb;
        bb.left = x - 75;
        bb.width = 150;
        bb.top = y - 75;
        bb.height = 150;

        if (bb.intersects(entity->getBoundingBox()))
        {
          entity->hurt(12, ShotTypeFire, 0);

          Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(entity->getX(), entity->getY()), 800.0f );
          entity->giveRecoil(true, recoilVector, 1.0f);
        }
      }

		}
	}
}
