#ifndef DUNGEONMAPENTITY_H
#define DUNGEONMAPENTITY_H

#include "sfml_game/GameEntity.h"
#include "sfml_game/TileMapEntity.h"
#include "DungeonMap.h"

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
  void renderDoors(sf::RenderTarget* app);
  void renderPost(sf::RenderTarget* app);
  void renderOverlay(sf::RenderTarget* app);

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

  void activateKeyRoomEffect();

protected:

private:
  sf::VertexArray vertices;
  sf::VertexArray overVertices;
  sf::VertexArray shadowVertices;
  bool hasChanged;

  sf::VertexArray bloodVertices;
  sf::VertexArray corpsesVertices;
  sf::VertexArray corpsesLargeVertices;

  bool getChanged();
  void computeVertices();
  void computeOverVertices();
  void computeShadowVertices();
  void computeBloodVertices();
  void computeCorpsesVertices();

  bool shouldBeTransformed(int part);

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

  sf::Sprite overlaySprite;
  sf::Sprite randomSprite;
  sf::Sprite doorSprite;
  roomTypeEnum roomType;

  struct structKeyRoomFX
  {
    float delay;
    int amplitude;
    bool isBlinking;
  } keyRoomEffect;
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

class DungeonMapEntityOverlay: public GameEntity
{
public:
  DungeonMapEntityOverlay(DungeonMapEntity* parent);
  virtual void animate(float delay);
  virtual void render(sf::RenderTarget* app);
private:
  DungeonMapEntity* parent;
};
#endif // DUNGEONMAPENTITY_H
