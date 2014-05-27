#include "DoorEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

DoorEntity::DoorEntity(int direction) : SpriteEntity (ImageManager::getImageManager()->getImage(IMAGE_DOOR))
{
  this->direction = direction;
  isOpen = true;
  removeCenter();
  width = TILE_WIDTH;
  height = TILE_HEIGHT;
  z = -0.5f;
  type = ENTITY_DOOR;
  doorType = 0;
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

void DoorEntity::setDoorType(int doorType)
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

    if (direction == 8)
    {
      yl = OFFSET_Y;
      yr = OFFSET_Y;
      if (isOpen)
      {
        if (timer > 0.0f)
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
            - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
            + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
        }
        else
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH * 1.2f;
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH * 1.2f;
        }
      }
      else
      {
          if (timer > 0.0f)
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
            - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
            + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
        }
        else
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH /2;;
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH /2;
        }
      }

      // back
      sprite.setTextureRect(sf::IntRect(3 * width, doorType * height, width * 3, height));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      app->draw(sprite);

      // door
      sprite.setTextureRect(sf::IntRect(0.5f * width, doorType * height,  width,  height));
      sprite.setPosition(xl, yl);
      app->draw(sprite);

      sprite.setTextureRect(sf::IntRect(1.5f * width, doorType * height,  width,  height));
      sprite.setPosition(xr, yr);
      app->draw(sprite);

      // front
      sprite.setTextureRect(sf::IntRect(6 * width, doorType * height, width * 3, height));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      app->draw(sprite);
    }

    if (direction == 4)
    {
      xl = OFFSET_X;
      xr = OFFSET_X;
      if (isOpen)
      {
        if (timer > 0.0f)
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
            - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
            + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
        }
        else
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT * 1.2f;
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT * 1.2f;
        }
      }
      else
      {

        if (timer > 0.0f)
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
            - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
            + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
        }
        else
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT /2;
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT /2;
        }
      }

      // back
      sprite.setTextureRect(sf::IntRect(width + (3 * width * doorType), 2 * height,  width , height* 3));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      sprite.setPosition(OFFSET_X,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + OFFSET_Y);
      app->draw(sprite);


      // door
      sprite.setTextureRect(sf::IntRect( 3 * width * doorType, 2.5 * height,  width,  height));
      sprite.setPosition(xl, yl);
      app->draw(sprite);

      sprite.setTextureRect(sf::IntRect( 3 * width * doorType, 3.5 * height,  width,  height));
      sprite.setPosition(xr, yr);
      app->draw(sprite);

      // front
      sprite.setTextureRect(sf::IntRect(2 * width + (3 * width * doorType), 2 * height,  width , height* 3));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      sprite.setPosition(OFFSET_X,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + OFFSET_Y);
      app->draw(sprite);
    }

    if (direction == 2)
    {
      sprite.setRotation(0.0f);

      yl = OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1);
      yr = OFFSET_Y + TILE_HEIGHT * (MAP_HEIGHT - 1);
      if (isOpen)
      {
        if (timer > 0.0f)
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
            - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
            + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_WIDTH / 2);
        }
        else
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH * 1.2f;
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH * 1.2f;
        }
      }
      else
      {
          if (timer > 0.0f)
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH / 2
            - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH / 2
            + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_WIDTH / 2);
        }
        else
        {
          xl = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH - TILE_WIDTH /2;;
          xr = OFFSET_X + (MAP_WIDTH / 2 ) * TILE_WIDTH + TILE_WIDTH /2;
        }
      }

      // back
      sprite.setTextureRect(sf::IntRect(3 * width, (1 + doorType) * height,  width * 3,  -height));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, yl);
      app->draw(sprite);

      // door
      sprite.setTextureRect(sf::IntRect(0.5f * width, (1 + doorType) * height,  width,  -height));
      sprite.setPosition(xl, yl);
      app->draw(sprite);

      sprite.setTextureRect(sf::IntRect(1.5f * width,  (1 + doorType) * height,  width, -height));
      sprite.setPosition(xr, yr);
      app->draw(sprite);

      // front
      sprite.setTextureRect(sf::IntRect(6 * width, (1 + doorType) * height, width * 3,  -height));
      sprite.setPosition(OFFSET_X + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, yl);
    //sprite.setRotation(angle);
      app->draw(sprite);
    }

    if (direction == 6)
    {
      xl = OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1);
      xr = OFFSET_X + TILE_WIDTH * (MAP_WIDTH - 1);
      if (isOpen)
      {
        if (timer > 0.0f)
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
            - 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
            + 1.2f * ((1.0f - (timer / DOOR_OPEN_TIME)) * TILE_HEIGHT / 2);
        }
        else
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT * 1.2f;
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT * 1.2f;
        }
      }
      else
      {
        if (timer > 0.0f)
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT / 2
            - 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT / 2
            + 1.2f * ((timer / DOOR_OPEN_TIME) * TILE_HEIGHT / 2);
        }
        else
        {
          yl = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT - TILE_HEIGHT /2;
          yr = OFFSET_Y + (MAP_HEIGHT / 2 ) * TILE_HEIGHT + TILE_HEIGHT /2;
        }
      }

      // back
      sprite.setTextureRect(sf::IntRect(width * 2  + (3 * width * doorType), 2 * height,  -width , height* 3));
      sprite.setPosition(xl + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      sprite.setPosition(xl,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + OFFSET_Y);
      app->draw(sprite);


      // door
      sprite.setTextureRect(sf::IntRect(1 * width + (3 * width * doorType), 2.5 * height,  -width,  height));
      sprite.setPosition(xl, yl);
      app->draw(sprite);

      sprite.setTextureRect(sf::IntRect(1* width + (3 * width * doorType), 3.5 * height,  -width,  height));
      sprite.setPosition(xr, yr);
      app->draw(sprite);

      // front
      sprite.setTextureRect(sf::IntRect(3 * width + (3 * width * doorType), 2 * height,  -width , height* 3));
      sprite.setPosition(xl + (MAP_WIDTH / 2 - 1) * TILE_WIDTH, OFFSET_Y);
      sprite.setPosition(xl,  + (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT + OFFSET_Y);
      app->draw(sprite);
    }
}
