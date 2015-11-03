#include "EffectZoneEntity.h"
#include "BaseCreatureEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/SpriteEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"

#include <iostream>

EffectZoneEntity::EffectZoneEntity(float x, float y, bool fromPlayer, float duration,
                     EffectZoneTypeEnum effectZoneType)
  : SpriteEntity(ImageManager::getInstance().getImage(IMAGE_EFFECT_ZONE), x, y, 64, 64)
{
  type = ENTITY_EFFECT_ZONE;
  this->effectZoneType = effectZoneType;
  imagesProLine = 6;
  lifetime = duration;
  this->fromPlayer = fromPlayer;

  switch (effectZoneType)
  {
    case EffectZoneTypeIce: frame = 0; break;
  }
  z = 1500; // for flying zones

  randomAngle = 5 + rand() % 15;
  if (rand() % 2 == 0) randomAngle = -randomAngle;
}

void EffectZoneEntity::animate(float delay)
{
  if (lifetime > 0)
  {
    if (age >= lifetime) isDying = true;
    else if (lifetime - age < 1.0f)
    {
      sprite.setColor(sf::Color(255, 255, 255, 255 * (lifetime - age)));
    }
  }
  age += delay;
  angle += delay * randomAngle;

  testCollisions();
}

void EffectZoneEntity::render(sf::RenderTarget* app)
{
  SpriteEntity::render(app);
}

void EffectZoneEntity::dying()
{
  isDying = true;
}

void EffectZoneEntity::testCollisions()
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

		  if (fromPlayer && entity->getType() == ENTITY_PLAYER) ok = false;
		  if (!fromPlayer && entity->getType() != ENTITY_PLAYER) ok = false;

		  if (ok && entity->getHp() > 0 && entity->canCollide())
      {
        entity->calculateBB();

        sf::IntRect bb;
        bb.left = x - 22;
        bb.width = 44;
        bb.top = y - 22;
        bb.height = 44;

        if (bb.intersects(entity->getBoundingBox()))
        {
          entity->hurt(BaseCreatureEntity::getHurtParams(0, ShotTypeIce, 1, false, SourceTypeExplosion, EnemyTypeNone, false));
        }
      }
		}
	}
}
