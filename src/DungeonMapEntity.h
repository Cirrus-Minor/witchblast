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
  void renderPost(sf::RenderTarget* app);

  void refreshMap();

  /*!
   *  \brief Generates blood particle
   *
   *  \param x : x position of the blood
   *  \param y : y position of the blood
   *  \param bloodColor : color of the blood (red; green, ...)
   */
  displayEntityStruct& generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor);

  void addBlood(float x, float y, int frame, float scale);
  void addCorpse(float x, float y, int frame);

  std::vector <displayEntityStruct> getBlood();
  std::vector <displayEntityStruct> getCorpses();

protected:

private:
  sf::VertexArray vertices;
  bool hasChanged;

  sf::VertexArray bloodVertices;
  sf::VertexArray corpsesVertices;
  sf::VertexArray corpsesLargeVertices;

  bool getChanged();
  void computeVertices();
  void computeBloodVertices();
  void computeCorpsesVertices();

  void animateParticle(displayEntityStruct &particle, float delay, float viscosity);
  bool collideWithWall(displayEntityStruct &particle, int boxWidth, int boxHeight);
  void autoSpeed(displayEntityStruct &particle, float speed);
  bool checkFalling(displayEntityStruct &particle, int boxWidth, int boxHeight);

  void displayBlood(sf::RenderTarget* app);
  void displayCorpses(sf::RenderTarget* app);

  std::vector<displayEntityStruct> blood;
  std::vector<displayEntityStruct> corpses;
  std::vector<displayEntityStruct> corpsesLarge;

  bool collide[4];
  enum enumCollisionDirection
  {
    NordWest,
    NordEast,
    SudWest,
    SudEast
  };
};

class DungeonMapEntityPost : public GameEntity
{
public:
  DungeonMapEntityPost(DungeonMapEntity* parent);
  virtual void animate(float delay);
  virtual void render(sf::RenderTarget* app);
private:
  DungeonMapEntity* parent;
};
#endif // DUNGEONMAPENTITY_H
