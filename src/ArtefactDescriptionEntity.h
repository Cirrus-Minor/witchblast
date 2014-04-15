#ifndef ARTEFACTDESCRIPTIONENTITY_H
#define ARTEFACTDESCRIPTIONENTITY_H

#include "sfml_game/SpriteEntity.h"
#include "ItemEntity.h"

class WitchBlastGame;

class ArtefactDescriptionEntity : public SpriteEntity
{
  public:
    ArtefactDescriptionEntity(ItemEntity::enumItemType, WitchBlastGame* parent);
    ~ArtefactDescriptionEntity();

    virtual void animate(float delay);
    virtual void render(sf::RenderWindow* app);
  private:
    sf::RectangleShape rectangle;
    sf::RectangleShape rectangleBorder;
    WitchBlastGame* parent;

    std::string artefactName;
    std::string artefactDescription;
};
#endif // ARTEFACTDESCRIPTIONENTITY_H
