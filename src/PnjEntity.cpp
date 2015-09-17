#include "PnjEntity.h"
#include "Constants.h"
#include "WitchBlastGame.h"
#include "sfml_game/ImageManager.h"

#include "TextMapper.h"

PnjEntity::PnjEntity(float x, float y, int pnjType) : SpriteEntity (ImageManager::getInstance().getImage(IMAGE_PNJ), x, y, 64, 96)
{
  this->x = x;
  this->y = y;

  x0 = x;
  y0 = y;

  pnjVelocity = 140.0f;

  xGoal = x0 + 180.0f;
  direction = 6;
  velocity.x = pnjVelocity;

  isSpeaking = false;
  speechTimer = 2.5f + 0.1f * (rand() % 50);
  isMirroring = false;

  type = ENTITY_PNJ;
}


void PnjEntity::animate(float delay)
{
  if (direction == 6 && x >= xGoal)
  {
    direction = 4;
    velocity.x = -pnjVelocity;
    xGoal = x0 - 180.0f;
    isMirroring = true;
  }
  else if (direction == 4 && x <= xGoal)
  {
    direction = 6;
    velocity.x = pnjVelocity;
    xGoal = x0 + 180.0f;
    isMirroring = false;
  }

  speechTimer -= delay;
  if (speechTimer <= 0.0f)
  {
    if (isSpeaking)
    {
      isSpeaking = false;
      speechTimer = 3.0f + 0.1f * (rand() % 65);
      velocity.x = (direction == 6) ? pnjVelocity : -pnjVelocity;
    }
    else
    {
      isSpeaking = true;
      speechTimer = 5.0f;
      velocity.x = 0.0f;
      int r = rand() % 3;
      switch (r)
      {
        case 0: speech = tools::getLabel("merchant_speech_0"); break;
        case 1: speech = tools::getLabel("merchant_speech_1"); break;
        case 2: speech = tools::getLabel("merchant_speech_2"); break;
      }
    }
  }

  if (isSpeaking)
  {
    frame = 3 + (int)(4 * age) % 2;
  }
  else
  {
    frame = 0 + (int)(4 * age) % 4;
    if (frame == 3) frame = 1;
  }
  z = y + height / 2 - 3;

  SpriteEntity::animate(delay);
}

void PnjEntity::render(sf::RenderTarget* app)
{
  SpriteEntity::render(app);

  if (isSpeaking)
  {
    game().write(speech, 20, x0, y0 - 72.0f, ALIGN_CENTER, sf::Color(255, 255, 255), app, 0 , 0, 0);
  }

  if (game().getShowLogical())
  {
    displayCenterAndZ(app);
  }
}
