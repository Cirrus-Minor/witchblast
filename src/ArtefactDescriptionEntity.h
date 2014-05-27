#ifndef ARTEFACTDESCRIPTIONENTITY_H
#define ARTEFACTDESCRIPTIONENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "ItemEntity.h"

class ArtefactDescriptionEntity : public SpriteEntity
{
  public:
    ArtefactDescriptionEntity(enumItemType);
    ~ArtefactDescriptionEntity();

    virtual void animate(float delay);
    virtual void render(sf::RenderTarget* app);
  private:
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangleBorder;

    std::string artefactName;
    std::string artefactDescription;
};
#endif // ARTEFACTDESCRIPTIONENTITY_H
