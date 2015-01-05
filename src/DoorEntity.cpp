#include "DoorEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

DoorEntity::DoorEntity(int direction) : SpriteEntity (ImageManager::getInstance().getImage(IMAGE_TILES))
{
  this->direction = direction;
  isOpen = true;
  removeCenter();
  width = TILE_WIDTH;
  height = TILE_HEIGHT;
  z = -0.5f;
  type = ENTITY_DOOR;
  doorType = DoorStandard;
  timer = 0.0f;
}


void DoorEntity::animate(float delay)
{
  age += delay;
  if (timer > 0.0f) timer -= delay;
}

void DoorEntity::setOpen(bool open)
{
  isOpen = open;
  timer = 0.0f;
}

void DoorEntity::setDoorType(doorEnum doorType)
{
  this->doorType = doorType;
}

void DoorEntity::closeDoor()
{
  isOpen = false;
  timer = DOOR_CLOSE_TIME;
}

void DoorEntity::openDoor()
{
  isOpen = true;
  timer = DOOR_OPEN_TIME;
}

void DoorEntity::render(sf::RenderTarget* app)
{
  if (!isVisible) return;

  float xl, yl, xr, yr;
  float yTranslate = 13 * TILE_HEIGHT;

  if (direction == 8)
  {
    yl = 0;
    yr = 0;
    if (isOpen)
    {
      if (timer > 0.0f)
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
             - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
             + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
      }
      else
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH * 1.2f;
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH * 1.2f;
      }
    }
    else
    {
      if (timer > 0.0f)
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
             - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
             + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
      }
      else
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH /2;;
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH /2;
      }
    }

    // back
    sprite.setTextureRect(sf::IntRect(3 * width, yTranslate + doorType * height, width * 3, height));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    app->draw(sprite);

    // door
    sprite.setTextureRect(sf::IntRect(0.5f * width, yTranslate + doorType * height,  width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(1.5f * width, yTranslate + doorType * height,  width,  height));
    sprite.setPosition(xr, yr);
    app->draw(sprite);

    // front
    sprite.setTextureRect(sf::IntRect(6 * width, yTranslate + doorType * height, width * 3, height));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    app->draw(sprite);
  }

  if (direction == 4)
  {
    xl = 0;
    xr = 0;
    if (isOpen)
    {
      if (timer > 0.0f)
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
             - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
             + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
      }
      else
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT * 1.2f;
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT * 1.2f;
      }
    }
    else
    {

      if (timer > 0.0f)
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
             - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
             + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
      }
      else
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT /2;
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT /2;
      }
    }

    // back
    sprite.setTextureRect(sf::IntRect(width + (3 * width * doorType), yTranslate + 3 * height,  width , height* 3));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    sprite.setPosition(0,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT);
    app->draw(sprite);


    // door
    sprite.setTextureRect(sf::IntRect( 3 * width * doorType, yTranslate + 3.5 * height,  width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect( 3 * width * doorType, yTranslate + 4.5 * height,  width,  height));
    sprite.setPosition(xr, yr);
    app->draw(sprite);

    // front
    sprite.setTextureRect(sf::IntRect(2 * width + (3 * width * doorType), yTranslate + 3 * height,  width , height* 3));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    sprite.setPosition(0,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT);
    app->draw(sprite);
  }

  if (direction == 2)
  {
    sprite.setRotation(0.0f);

    yl = TILE_HEIGHT * (MAP_HEIGHT - 1);
    yr = TILE_HEIGHT * (MAP_HEIGHT - 1);
    if (isOpen)
    {
      if (timer > 0.0f)
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
             - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
             + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
      }
      else
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH * 1.2f;
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH * 1.2f;
      }
    }
    else
    {
      if (timer > 0.0f)
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
             - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
             + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
      }
      else
      {
        xl = (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH /2;;
        xr = (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH /2;
      }
    }

    // back
    sprite.setTextureRect(sf::IntRect(3 * width, yTranslate + (1 + doorType) * height,  width * 3,  -height));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, yl);
    app->draw(sprite);

    // door
    sprite.setTextureRect(sf::IntRect(0.5f * width, yTranslate + (1 + doorType) * height,  width,  -height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(1.5f * width,  yTranslate + (1 + doorType) * height,  width, -height));
    sprite.setPosition(xr, yr);
    app->draw(sprite);

    // front
    sprite.setTextureRect(sf::IntRect(6 * width, yTranslate + (1 + doorType) * height, width * 3,  -height));
    sprite.setPosition((MAP_WIDTH / 2 - 1) * TILE_WIDTH, yl);
    app->draw(sprite);
  }

  if (direction == 6)
  {
    xl = TILE_WIDTH * (MAP_WIDTH - 1);
    xr = TILE_WIDTH * (MAP_WIDTH - 1);
    if (isOpen)
    {
      if (timer > 0.0f)
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
             - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
             + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
      }
      else
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT * 1.2f;
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT * 1.2f;
      }
    }
    else
    {
      if (timer > 0.0f)
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
             - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
             + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
      }
      else
      {
        yl = (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT /2;
        yr = (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT /2;
      }
    }

    // back
    sprite.setTextureRect(sf::IntRect(width * 2  + (3 * width * doorType), yTranslate + 3 * height,  -width , height* 3));
    sprite.setPosition(xl + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    sprite.setPosition(xl,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT);
    app->draw(sprite);


    // door
    sprite.setTextureRect(sf::IntRect(1 * width + (3 * width * doorType), yTranslate + 3.5 * height,  -width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(1* width + (3 * width * doorType), yTranslate + 4.5 * height,  -width,  height));
    sprite.setPosition(xr, yr);
    app->draw(sprite);

    // front
    sprite.setTextureRect(sf::IntRect(3 * width + (3 * width * doorType), yTranslate + 3 * height,  -width , height* 3));
    sprite.setPosition(xl + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, 0);
    sprite.setPosition(xl,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT);
    app->draw(sprite);
  }
}
