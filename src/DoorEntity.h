#ifndef DOORENTITY_H
#define DOORENTITY_H

#include "sfml_game/SpriteEntity.h"

class DoorEntity : public SpriteEntity
{
  public:
    DoorEntity(int direction);
    virtual void animate(float delay);
    void render(sf::RenderWindow* app);

    void setOpen(bool open);
    void setDoorType(int doorType);
    void closeDoor();
    void openDoor();

  private:
    int direction;
    bool isOpen;
    float timer;
    int doorType;
};
#endif // DOORENTITY_H
