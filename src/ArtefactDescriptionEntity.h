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
    sf::Sprite rectangle;

    std::string artefactName;
    std::string artefactDescription;
};
#endif // ARTEFACTDESCRIPTIONENTITY_H
