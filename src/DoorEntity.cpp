#include "DoorEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

#include "DungeonMap.h"

DoorEntity::DoorEntity(int direction) : SpriteEntity (ImageManager::getInstance().getImage(IMAGE_DOORS))
{
  this->direction = direction;
  isOpen = true;
  removeCenter();
  width = TILE_WIDTH;
  height = TILE_HEIGHT;
  z = -0.5f;
  type = ENTITY_DOOR;
  doorType = DoorStandard_0;
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
  return;
}

void DoorEntity::renderDoors(sf::RenderTarget* app)
{
  if (!isVisible) return;

  float xl, yl, xr, yr, x0, y0;

  if (direction == 8 || direction == 2)
  {
    x0 = 64 + 256 * (doorType / DOORS_PRO_COLUMN);
    y0 = 192 * ((int)doorType % DOORS_PRO_COLUMN);
  }
  else
  {
    x0 = 0 + 256 * (doorType / DOORS_PRO_COLUMN);
    y0 = 192 * ((int)doorType % DOORS_PRO_COLUMN);
  }

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

    // door
    sprite.setTextureRect(sf::IntRect(x0 + 0.5f * width, y0,  width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(x0 + 1.5f * width, y0,  width,  height));
    sprite.setPosition(xr, yr);
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
    // door
    sprite.setTextureRect(sf::IntRect(x0, y0 + 0.5 * height,  width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(x0, y0 + 1.5 * height,  width,  height));
    sprite.setPosition(xr, yr);
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
    // door
    sprite.setTextureRect(sf::IntRect(x0 + 0.5f * width, y0 + height,  width,  -height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(x0 + 1.5f * width,  y0 + height,  width, -height));
    sprite.setPosition(xr, yr);
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
    // door
    sprite.setTextureRect(sf::IntRect(x0 + width, y0 + 0.5 * height,  -width,  height));
    sprite.setPosition(xl, yl);
    app->draw(sprite);

    sprite.setTextureRect(sf::IntRect(x0 + width, y0 + 1.5 * height,  -width,  height));
    sprite.setPosition(xr, yr);
    app->draw(sprite);
  }
}
