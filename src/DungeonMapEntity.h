#ifndef DUNGEONMAPENTITY_H
#define DUNGEONMAPENTITY_H

#include "sfml_game/GameEntity.h"
#include "sfml_game/TileMapEntity.h"

struct displayEntityStruct
{
  bool moving;
  int frame;
  float x;
  float y;
  Vector2D velocity;
  float scale;
};

class DungeonMapEntity : public GameEntity
{
public:
  DungeonMapEntity();
  virtual void animate(float delay);
  virtual void render(sf::RenderTarget* app);

  void refreshMap();

  /*!
   *  \brief Generates blood particle
   *
   *  \param x : x position of the blood
   *  \param y : y position of the blood
   *  \param bloodColor : color of the blood (red; green, ...)
   */
  void generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor);

  void addBlood(float x, float y, int frame, float scale);

  std::vector <displayEntityStruct> getBlood();

protected:

private:
  sf::VertexArray vertices;
  bool hasChanged;

  sf::VertexArray bloodVertices;

  bool getChanged();
  void computeVertices();
  void computeBloodVertices();

  void animateParticle(displayEntityStruct &particle, float delay);

  void displayBlood(sf::RenderTarget* app);

  std::vector<displayEntityStruct> blood;
};
#endif // DUNGEONMAPENTITY_H
