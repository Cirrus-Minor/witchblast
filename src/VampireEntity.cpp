#include "VampireEntity.h"
#include "BatEntity.h"
#include "BoltEntity.h"
#include "PlayerEntity.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>

const int VAMPIRE_HP = 1500;
const int VAMPIRE_DAMAGE = 12;
const float VAMPIRE_FLYING_DELAY = 1.2f;
const float VAMPIRE_BAT_DELAY = 0.225f;
const float VAMPIRE_CONFUSION_DELAY = 2.5f;
const float VAMPIRE_TRANSFORM_DELAY = 0.4f;
const float VAMPIRE_CRY_DELAY = 6.0f;
const float VAMPIRE_MOVE_COUNTER_MAX = 2;
const float VAMPIRE_DYING_TIME = 4.2f;

const int FORM_MAN = 0;
const int FORM_BAT = 1;

VampireEntity::VampireEntity(float myx, float myy)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_VAMPIRE), myx, myy)
{
  width = 96;
  height = 96;
  creatureSpeed = 150;
  hp = VAMPIRE_HP;
  hpDisplay = VAMPIRE_HP;
  hpMax = VAMPIRE_HP;
  meleeDamages = VAMPIRE_DAMAGE;
  shadowFrame = 30;
  bodyFrame = 0;

  type = ENTITY_ENEMY_BOSS;
  //deathFrame = FRAME_CORPSE_CYCLOP;
  batSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_VAMPIRE_BAT));

  frame = 0;
  if (game().getPlayerPosition().x > x) isMirroring = true;
  sprite.setOrigin(48.0f, 74.0f);
  imagesProLine = 6;

  state = 0;
  timer = 2.0f;
  age = -1.0f;
  enemyType = EnemyTypeVampire;
  formState = FORM_MAN;

  resistance[ResistanceFrozen] = ResistanceVeryHigh;
  resistance[ResistanceRepulsion] = ResistanceVeryHigh;
  resistance[ResistancePoison] = ResistanceImmune;
  canExplode = false;

  x = GAME_WIDTH * 0.5f;
  y = GAME_HEIGHT * 0.5f;

  if (game().getPlayer()->getX() < 2 * TILE_WIDTH)
  {
    x = GAME_WIDTH - 2.5f * TILE_WIDTH;
    targetPos = 4;
  }
  else if (game().getPlayer()->getX() > GAME_WIDTH - 2 * TILE_WIDTH)
  {
    x = 2.5f * TILE_WIDTH;
    targetPos = 6;
  }
  else if (game().getPlayer()->getY() < 2 * TILE_HEIGHT)
  {
    y = GAME_HEIGHT - 2.5f * TILE_HEIGHT;
    targetPos = 2;
  }
  else if (game().getPlayer()->getY() > GAME_HEIGHT - 2 * TILE_HEIGHT)
  {
    y = 2.5f * TILE_HEIGHT;
    targetPos = 8;
  }

  moveCounter = 0;
}

int VampireEntity::getHealthLevel()
{
  int healthLevel = 0;
  if (hp <= hpMax * 0.25) healthLevel = 3;
  else if (hp <= hpMax * 0.5) healthLevel = 2;
  else if (hp <= hpMax * 0.75) healthLevel = 1;
  return healthLevel;
}

void VampireEntity::computeStates(float delay)
{
  timer -= delay;

  if (timer <= 0.0f)
  {
    if (state == 0) // waiting bef hypnose
    {
      state = 1; // hypnose
      timer = 1.6f;
      game().getPlayer()->setSpecialState(SpecialStateConfused, true, VAMPIRE_CONFUSION_DELAY, VAMPIRE_CONFUSION_DELAY, 0.0f);
    }
    else if (state == 1) // hypnose
    {
      state = 2;
      timer = 0.5f;
    }
    else if (state == 2) // waiting before laughing
    {
      state = 3; // laughing
      timer = 1.3f; // 3.0f;
      SoundManager::getInstance().playSound(SOUND_VAMPIRE_LAUGHING);
    }
    else if (state == 3) // laughing
    {
      state = 4;
      timer = 0.1f;
    }
    else if (state == 4) // to bat cloud
    {
      if (getHealthLevel() > 1 && moveCounter <= 0)
      {
        state = 8;
        timer = VAMPIRE_TRANSFORM_DELAY;
        formState = FORM_BAT;
        moveCounter = VAMPIRE_MOVE_COUNTER_MAX;
        SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BOLT);
      }
      else
      {
        state = 5;
        timer = VAMPIRE_TRANSFORM_DELAY;
        moveCounter--;
        SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BOLT);
      }
    }
    else if (state == 5) // to bat cloud
    {
      state = 6;
      frame = 10;
      timer = VAMPIRE_FLYING_DELAY;
      batTimer = VAMPIRE_BAT_DELAY;
      xSource = x;
      ySource = y;
      if (targetPos == 4 || targetPos == 6)
      {
        targetPos = rand() % 2 == 0 ? 2 : 8;
      }
      else
      {
        targetPos = rand() % 2 == 0 ? 4 : 6;
      }
    }
    else if (state == 6) // vampire flying in the cloud
    {
      state = 7;
      timer = VAMPIRE_TRANSFORM_DELAY;
      SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BOLT);
    }
    else if (state == 7) // cloud to vampire
    {
      state = 0;
      timer = 0.5f;
    }
    else if (state == 8) // vampire to cloud < 50% HP
    {
      state = 9;  // cloud to center
      frame = 10;
      timer = VAMPIRE_FLYING_DELAY;
      batTimer = VAMPIRE_BAT_DELAY;
      xSource = x;
      ySource = y;
      targetPos = 5;

      if (hp <= hpMax * 0.1f)
      {
        numberOfRays = 6;
        raySpeedFactor = 35.0f;
      }
      else if (hp <= hpMax  * 0.2f)
      {
        numberOfRays = 5;
        raySpeedFactor = 35.0f;
      }
      else if (hp <= hpMax * 0.3f)
      {
        numberOfRays = 5;
        raySpeedFactor = 30.0f;
      }
      else if (hp <= hpMax * 0.4f)
      {
        numberOfRays = 4;
        raySpeedFactor = 30.0f;
      }
      else
      {
        numberOfRays = 4;
        raySpeedFactor = 20.0f;
      }


    }
    else if (state == 9) // cloud to center
    {
      state = 10; // transform to giant bat
      timer = VAMPIRE_TRANSFORM_DELAY;
    }
    else if (state == 10) // transform to giant bat
    {
      state = 11; // giant bat
      for (int i = 0; i < 10; i++)
      {
        game().generateStar(sf::Color(200, 0, 200), x - 60 + rand() % 121, y - 60 + rand() % 121);
        game().generateStar(sf::Color(0, 0, 0), x - 60 + rand() % 121, y - 60 + rand() % 121);
      }
      SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BAT);
      timer = 1.0f;
    }
    else if (state == 11) // giant bat waiting
    {
      state = 12; // to cry
      timer = VAMPIRE_CRY_DELAY;
    }
    else if (state == 12) // cry !
    {
      state = 13; // giant bat waiting
      timer = VAMPIRE_TRANSFORM_DELAY;

      for (int i = 0; i < 10; i++)
      {
        game().generateStar(sf::Color(200, 0, 200), x - 40 + rand() % 81, y - 40 + rand() % 81);
        game().generateStar(sf::Color(0, 0, 0), x - 40 + rand() % 81, y - 40 + rand() % 81);
      }
      SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BAT);
    }
    else if (state == 13) // cry !
    {
      state = 6;
      frame = 10;
      timer = VAMPIRE_FLYING_DELAY;
      batTimer = VAMPIRE_BAT_DELAY;
      xSource = x;
      ySource = y;
      if (targetPos == 4 || targetPos == 6)
      {
        targetPos = rand() % 2 == 0 ? 2 : 8;
      }
      else
      {
        targetPos = rand() % 2 == 0 ? 4 : 6;
      }
    }
    else
    {
      state = 0;
      timer = 5.0f;
    }
  }
}

void VampireEntity::animate(float delay)
{
  if (isAgonising)
  {
    timer += delay;
    if (timer > VAMPIRE_DYING_TIME)
    {
      dying();
      VampireDeadEntity* corpse = new VampireDeadEntity(x, y);
      corpse->setMirroring(isMirroring);
    }

    return;
  }

  EnemyEntity::animate(delay);
  if (isAgonising) return;

  computeStates(delay);
  if (state == 0)
  {
    if (timer < 0.2f) frame = 1;
    else frame = 0;
  }
  else if (state == 1)  // hypnose
  {
    frame = 2;
  }
  else if (state == 2 || state == 4)  // waiting
  {
    frame = 0;
  }
  else if (state == 3)  // laughing
  {
    frame = 3 + ((int)(age * 7.0f)) % 2;
  }
  else if (state == 5 || state == 8)  // to bat cloud
  {
    sprite.setColor(sf::Color(255, 255, 255, 255));
    if (timer > 0.2f)
    {
      frame = 0;
      float fade = (VAMPIRE_TRANSFORM_DELAY - timer) / VAMPIRE_TRANSFORM_DELAY;
      sprite.setColor(sf::Color(255 - fade * 250, 255 - fade * 250, 255 - fade * 250, 255));
      isMirroring = game().getPlayer()->getX() > x;
    }
    else if (timer > 0.15f) frame = 6;
    else if (timer > 0.1f) frame = 7;
    else if (timer > 0.05f) frame = 8;
    else  frame = 9;
  }
  else if (state == 7 || state == 10)  // to bat cloud
  {
    sprite.setColor(sf::Color(255, 255, 255, 255));

    if (timer < 0.2f)
    {
      frame = 0;
      float fade = timer * 4;
      sprite.setColor(sf::Color(255 - fade * 250, 255 - fade * 250, 255 - fade * 250, 255));
      isMirroring = game().getPlayer()->getX() > x;
    }
    else if (timer < VAMPIRE_TRANSFORM_DELAY - 0.15f) frame = 6;
    else if (timer < VAMPIRE_TRANSFORM_DELAY - 0.1f) frame = 7;
    else if (timer < VAMPIRE_TRANSFORM_DELAY - 0.05f) frame = 8;

    else  frame = 9;
  }
  else if (state == 6 || state == 9)
  {
    calculatePosition();
    batTimer -= delay;
    // bat generation
    if (batTimer <= 0.0f)
    {
      batTimer += VAMPIRE_BAT_DELAY;
      isMirroring = !isMirroring;
      BatEntity* bat;
      if (hp <= hpMax  * 0.25f)
        bat = new BatEntity(x, y, BatSkeleton, true);
      else
        bat = new BatEntity(x, y, BatStandard, true);
      bat->setAge(0.0f);
    }
    // particules
    for (int i = 0; i < 2; i++)
    {
      SpriteEntity* particle = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_VAMPIRE), x, y - 16, 96, 96);
      particle->setFading(true);
      particle->setImagesProLine(6);
      particle->setZ(10);
      particle->setLifetime(0.3f + 0.1f * (rand() % 10));
      particle->setVelocity(Vector2D(12.0f));
      particle->setType(ENTITY_EFFECT);
      particle->setFrame(11 + 6 * i);
      //particle->setFading(true);
      particle->setShrinking(true);
      particle->setX(x - 20 + rand()% 41);
      particle->setY(y - 40 + rand()% 41);
    }
    for (int i = 0; i < 2; i++)
    {
      SpriteEntity* particle = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_VAMPIRE), x, y - 16, 96, 96);
      particle->setFading(true);
      particle->setImagesProLine(6);
      particle->setZ(11);
      //particle->setLifetime(0.9f);
      particle->setLifetime(0.3f + 0.1f * (rand() % 10));
      particle->setVelocity(Vector2D(12.0f));
      particle->setType(ENTITY_EFFECT);
      particle->setFrame(23 + 6 * i);
      //particle->setFading(true);
      particle->setShrinking(true);
      particle->setRenderAdd();
      particle->setX(x - 20 + rand()% 41);
      particle->setY(y - 40 + rand()% 41);
    }
  }
  else if (state == 12)
  {
    x = GAME_WIDTH / 2;
    y = GAME_HEIGHT / 2;
    testRaysCollision();
  }

  if (state != 5 && state != 7) isMirroring = game().getPlayer()->getX() > x;

  z = y + 16;
}

void VampireEntity::calculatePosition()
{
  float xTarget, yTarget;

  if (targetPos == 4) xTarget = 2.5f * TILE_WIDTH;
  else if (targetPos == 6) xTarget = GAME_WIDTH - 2.5f * TILE_WIDTH;
  else xTarget = GAME_WIDTH * 0.5f;

  if (targetPos == 8) yTarget = 2.5f * TILE_HEIGHT;
  else if (targetPos == 2) yTarget = GAME_HEIGHT - 2.5f * TILE_HEIGHT;
  else yTarget = GAME_HEIGHT * 0.5f;

  x = xSource + (VAMPIRE_FLYING_DELAY - timer) / VAMPIRE_FLYING_DELAY * (xTarget - xSource);
  y = ySource + (VAMPIRE_FLYING_DELAY - timer) / VAMPIRE_FLYING_DELAY * (yTarget - ySource);
}

int VampireEntity::hurt(StructHurt hurtParam)
{
  if (state == 6 || state == 8 ||state == 9) armor = 1.0f;
  else armor = 0.0f;
  int result = EnemyEntity::hurt(hurtParam);

  return result;
}

void VampireEntity::prepareDying()
{
  // Giant bat ?
  if (state == 11 || state ==12)
  {
    for (int i = 0; i < 10; i++)
    {
      game().generateStar(sf::Color(200, 0, 200), x - 40 + rand() % 81, y - 40 + rand() % 81);
      game().generateStar(sf::Color(0, 0, 0), x - 40 + rand() % 81, y - 40 + rand() % 81);
    }
    SoundManager::getInstance().playSound(SOUND_VAMPIRE_TRANSFORM_BAT);
  }

  timer = 0.0f;
  sprite.setOrigin(0.0f, 0.0f);
  isAgonising = true;
  SoundManager::getInstance().playSound(SOUND_VAMPIRE_DYING);

  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    EnemyEntity* entity = dynamic_cast<EnemyEntity*>(e);
    if (entity != NULL)
    {
      if (entity->getHp() > 0)
      {
        entity->hurt(getHurtParams(entity->getHp(), ShotTypeStandard, 0, false, SourceTypeMelee, enemyType, false));
      }
    }
  }
  type = ENTITY_ENEMY_NC;
}

void VampireEntity::calculateBB()
{
  if (state >= 11)
  {
    boundingBox.left = (int)x - 50;
    boundingBox.width = 100;
    boundingBox.top = (int)y - 50;
    boundingBox.height =  100;
  }
  else
  {
    boundingBox.left = (int)x - 16;
    boundingBox.width = 32;
    boundingBox.top = (int)y - 16;
    boundingBox.height =  32;
  }
}


void VampireEntity::drop()
{
  dropItem(ItemBossHeart);
}

void VampireEntity::render(sf::RenderTarget* app)
{
  if (isAgonising)
  {
    sprite.setPosition(x - 48, y - 74);
    if (isMirroring) sprite.setTextureRect(sf::IntRect(1 * width, 2 * height, -width, height));
    else sprite.setTextureRect(sf::IntRect(0 * width, 2 * height, width, height));
    app->draw(sprite);

    float burnHeight = timer < 4.0f ? height * timer / 4.0f : height;
    if (isMirroring) sprite.setTextureRect(sf::IntRect(2 * width, 2 * height, -width, burnHeight));
    else sprite.setTextureRect(sf::IntRect(width, 2 * height, width, burnHeight));
    app->draw(sprite);

    if (isMirroring) sprite.setTextureRect(sf::IntRect(2 * width, 2 * height, -width, height));
    else sprite.setTextureRect(sf::IntRect(1 * width, 2 * height, width, height));

    // fire
    if (timer > 0.1f && timer < 3.8f)
    {
      sf::Sprite burn;
      burn.setTexture(*ImageManager::getInstance().getImage(IMAGE_VAMPIRE));

      if (isMirroring) burn.setTextureRect(sf::IntRect(5 * width, 2 * height + burnHeight - 4, -width, 6));
      else burn.setTextureRect(sf::IntRect(4 * width, 2 * height + burnHeight - 4, width, 6));
      burn.setPosition(x - 48, y - 74 - 4 + burnHeight);
      app->draw(burn);

      if (isMirroring) burn.setTextureRect(sf::IntRect(4 * width, 2 * height + burnHeight - 2, -width, 4));
      else burn.setTextureRect(sf::IntRect(3 * width, 2 * height + burnHeight - 2, width, 4));
      burn.setPosition(x - 48, y - 74 - 2 + burnHeight);
      app->draw(burn);

      float fade = (cos(12.0f * game().getAbsolutTime()) + 1.0f) * 0.5f;
      burn.setColor(sf::Color(255, 255, 255, 100 * fade));
      app->draw(burn, sf::BlendAdd);
    }

    // cone
    sf::Sprite cone;
    cone.setTexture(*ImageManager::getInstance().getImage(IMAGE_LIGHT_CONE));
    cone.setPosition(x - 68, y - 600);
    int fade = 200;
    if (timer < 0.2f) fade = timer * 1000;
    else if (timer > VAMPIRE_DYING_TIME - 0.5f) fade = (VAMPIRE_DYING_TIME - timer) * 400;
    if (fade < 0) fade = 0;
    cone.setColor(sf::Color(255, 255, 255, fade));
    app->draw(cone, sf::BlendAdd);
  }
  else
  {
    if (state == 11 || state == 12) // giant bat
    {
      int oldBodyFrame = bodyFrame;
      bodyFrame = (int)(age * 18) % 9;
      if (oldBodyFrame == 6 && bodyFrame == 7)
        SoundManager::getInstance().playSound(SOUND_VAMPIRE_FLAP);
      batSprite.setTextureRect(sf::IntRect(418 * (bodyFrame % 3), 342 * (bodyFrame / 3), 418, 342));
      batSprite.setPosition(x - 209, y - 200);
      batSprite.setColor(sprite.getColor());
      app->draw(batSprite);

      if (state == 11)
      {
        frame = (int)(age * 4) % 9;
        if (frame >= 5) frame = 8 - frame;
        sprite.setTextureRect(sf::IntRect(width * frame, 3 * height, width, height));

        if (timer < 1.0f && (int)(timer * 20) % 3 == 0) renderRays(app, false);
      }
      else if (state == 12)
      {
        // rays
        renderRays(app, false);

        // head
        if (timer < 0.05f) frame = 1;
        else if (timer < 0.1f) frame = 2;
        else if (timer < 0.15f) frame = 3;
        else if (timer < VAMPIRE_CRY_DELAY - 0.3f) frame = 4;
        else if (timer < VAMPIRE_CRY_DELAY - 0.2f) frame = 3;
        else if (timer < VAMPIRE_CRY_DELAY - 0.1f) frame = 2;
        else frame = 1;
        sprite.setTextureRect(sf::IntRect(width * frame, 4 * height, width, height));
      }

      sprite.setPosition(x + cosf(age * 2) * 2, y + sinf(age * 2) * 6);
      app->draw(sprite);

      if (game().getShowLogical())
      {
        displayBoundingBox(app);
        displayCenterAndZ(app);
      }
    }
    else
    {
      EnemyEntity::render(app);
      if (state == 1) // hypnose
      {
        sf::Sprite eye;
        eye.setOrigin(6, 6);
        eye.setTexture(*ImageManager::getInstance().getImage(IMAGE_VAMPIRE));
        eye.setTextureRect(sf::IntRect(5 * width, 0, 12, 12));
        eye.setRotation(age * 500);
        if (isMirroring) eye.setPosition(x + 10, y - 44);
        else  eye.setPosition(x - 10, y - 44);
        app->draw(eye);

        float fade = (cos(8.0f * game().getAbsolutTime()) + 1.0f) * 0.5f;
        eye.setColor(sf::Color(255, 255, 255, 255 * fade));
        app->draw(eye, sf::BlendAdd);

        if (isMirroring) eye.setPosition(x - 4, y - 44);
        else  eye.setPosition(x + 4, y - 44);

        eye.setColor(sf::Color(255, 255, 255, 255 ));
        app->draw(eye);
        eye.setColor(sf::Color(255, 255, 255, 255 * fade));
        app->draw(eye, sf::BlendAdd);
      }
    }

    renderLifeBar(app, tools::getLabel("enemy_vampire"));
  }
}

void VampireEntity::renderRays(sf::RenderTarget* app, bool isGhost)
{
  // rays
  sf::RectangleShape ray(sf::Vector2f(500, 4));
  ray.setOrigin(0, 2);
  if (isGhost)
  {
    ray.setFillColor(sf::Color(128, 50, 50, 200));
    ray.setOutlineColor(sf::Color(128, 50, 50, 100));
  }
  else
  {
    ray.setFillColor(sf::Color(255, 50, 50));
    ray.setOutlineColor(sf::Color(255, 50, 50, 128));
  }

  ray.setOutlineThickness(1);
  ray.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);

  sf::RectangleShape rayLittle(sf::Vector2f(500, 2));
  rayLittle.setOrigin(0, 1);
  if (isGhost)
  {
    rayLittle.setFillColor(sf::Color(128, 50, 50, 100));
    rayLittle.setOutlineColor(sf::Color(128, 50, 50, 50));
  }
  else
  {
    rayLittle.setFillColor(sf::Color(255, 50, 50, 150));
    rayLittle.setOutlineColor(sf::Color(255, 50, 50, 75));
  }
  rayLittle.setOutlineThickness(1);
  rayLittle.setPosition(GAME_WIDTH / 2, GAME_HEIGHT / 2);

  float rayAngle = age * raySpeedFactor;
  for (int i = 0; i < numberOfRays; i++)
  {
    ray.setRotation(rayAngle + i * 360 / numberOfRays);
    app->draw(ray);

    rayLittle.setRotation(ray.getRotation() + 3);
    app->draw(rayLittle);
    rayLittle.setRotation(ray.getRotation() - 3);
    app->draw(rayLittle);
  }

  if (!isGhost)
    SoundManager::getInstance().playSound(SOUND_VAMPIRE_SONIC_RAY, false);
}

void VampireEntity::testRaysCollision()
{
  PlayerEntity* player = game().getPlayer();

  if (player->canCollide() && player->getHp() > 0)
  {
    float rayAngle = age * raySpeedFactor;

    Vector2D a1(GAME_WIDTH / 2, GAME_HEIGHT / 2);

    Vector2D b1(player->getBoundingBox().left, player->getBoundingBox().top);
    Vector2D b2(player->getBoundingBox().left + player->getBoundingBox().width, player->getBoundingBox().top);
    Vector2D b3(player->getBoundingBox().left + player->getBoundingBox().width, player->getBoundingBox().top + player->getBoundingBox().height);
    Vector2D b4(player->getBoundingBox().left, player->getBoundingBox().top + player->getBoundingBox().height);

    for (int i = 0; i < numberOfRays; i++)
    {
      float currentAngle = rayAngle + i * 360 / numberOfRays;
      Vector2D a2(GAME_WIDTH / 2 + 500 * cosf(currentAngle / 57.3f),
                  GAME_HEIGHT / 2 + 500 * sinf(currentAngle / 57.3f));

      if (intersectsSegments(a1, a2, b1, b2)
          || intersectsSegments(a1, a2, b2, b3)
          || intersectsSegments(a1, a2, b3, b4)
          || intersectsSegments(a1, a2, b4, b1))
      {
        if (player->hurt(getHurtParams(8, ShotTypeStandard, 0, false, SourceTypeMelee, EnemyTypeVampire, false)) > 0)
        {
          SpriteEntity* star = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_VAMPIRE_PART), player->getX(), player->getY());
          star->setFading(true);
          star->setZ(y+ 100);
          star->setLifetime(0.7f);
          star->setType(ENTITY_EFFECT);
          star->setSpin(400.0f);
        }
      }
    }
  }

}

void VampireEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

  if (playerEntity != NULL && !playerEntity->isDead())
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 450.0f);
    targetEntity->giveRepulsion(true, recoilVector, 0.5f);
  }
}

void VampireEntity::generateBats(int batFrame)
{
  SpriteEntity* spriteStar = new SpriteEntity(
    ImageManager::getInstance().getImage(IMAGE_VAMPIRE),
    x - 50 + rand() % 101, y - 50 + rand() % 101, 96, 96, 6);
  //spriteStar->setScale(0.8f, 0.8f);
  spriteStar->setFrame(batFrame);
  spriteStar->setZ(1000.0f);
  //spriteStar->setSpin(-100 + rand()%200);
  spriteStar->setVelocity(Vector2D(30 + rand()%60));
  spriteStar->setWeight(-150);
  spriteStar->setFading(true);
  spriteStar->setAge(-0.8f);
  spriteStar->setLifetime(0.1f + (rand() % 100) * 0.003f );
  spriteStar->setType(ENTITY_EFFECT);
}


////////////////////// DEAD VAMPIRE /////////////////
VampireDeadEntity::VampireDeadEntity(float myx, float myy)
  : EnemyEntity (ImageManager::getInstance().getImage(IMAGE_VAMPIRE), myx, myy)
{
  width = 96;
  height = 96;
  creatureSpeed = 0;
  hp = 80;
  hpMax = 80;
  shadowFrame = 30;

  type = ENTITY_ENEMY_NC;
  deathFrame = FRAME_CORPSE_VAMPIRE;
  dyingFrame = 14;
  displayDamage = false;

  frame = 13;
  sprite.setOrigin(48.0f, 74.0f);
  imagesProLine = 6;

  bloodColor = BloodNone;
  enemyType = EnemyTypeVampireDead;

  resistance[ResistanceFrozen] = ResistanceImmune;
  resistance[ResistancePoison] = ResistanceImmune;
  canExplode = false;
}

void VampireDeadEntity::calculateBB()
{
  boundingBox.left = (int)x - 16;
  boundingBox.width = 32;
  boundingBox.top = (int)y - 16;
  boundingBox.height =  32;
}

void VampireDeadEntity::drop()
{
  dropItem(ItemGoldCoin);
}

void VampireDeadEntity::animate(float delay)
{
  EnemyEntity::animate(delay);
  z = y + 16;
}

void VampireDeadEntity::inflictsRecoilTo(BaseCreatureEntity* targetEntity)
{
  PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(targetEntity);

  if (playerEntity != NULL && !playerEntity->isDead())
  {
    Vector2D recoilVector = Vector2D(x, y).vectorTo(Vector2D(targetEntity->getX(), targetEntity->getY()), 100.0f);
    targetEntity->giveRepulsion(false, recoilVector, 0.1f);
  }
}

void VampireDeadEntity::readCollidingEntity(CollidingSpriteEntity* entity)
{
  if (!isDying && !isAgonising && collideWithEntity(entity))
  {
    if (entity->getType() == ENTITY_PLAYER || entity->getType() == ENTITY_BOLT )
    {
      PlayerEntity* playerEntity = dynamic_cast<PlayerEntity*>(entity);
      BoltEntity* boltEntity = dynamic_cast<BoltEntity*>(entity);

      if (playerEntity != NULL && !playerEntity->isDead()) inflictsRecoilTo(playerEntity);

      else if (boltEntity != NULL && !boltEntity->getDying() && boltEntity->getAge() > 0.05f)
      {
        collideWithBolt(boltEntity);
      }
    }
  }
}
