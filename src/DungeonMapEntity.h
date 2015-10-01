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
  float initialScale;
  float age;
  float lifetime;
  sf::Color color;
};

class DungeonMapEntity : public GameEntity
{
public:
  DungeonMapEntity();
  virtual void animate(float delay);
  virtual void render(sf::RenderTarget* app);
  void renderDoors(sf::RenderTarget* app);
  void renderKeyStone(sf::RenderTarget* app);
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

  displayEntityStruct& generateBoltParticle(float x, float y, Vector2D velocity, bool back, int frame, float scale, float lifetime);

  std::vector <displayEntityStruct> getBlood();
  std::vector <displayEntityStruct> getCorpses();
  void computeBoltParticulesVertices();

  void activateKeyRoomEffect();

private:
  sf::VertexArray vertices;
  sf::VertexArray overVertices;
  sf::VertexArray shadowVertices;
  bool hasChanged;
  enum EnumShadowType { ShadowTypeStandard, ShadowTypeCorner, ShadowTypeMedium, ShadowTypeSmall };
  EnumShadowType shadowType;

  sf::VertexArray bloodVertices;
  sf::VertexArray corpsesVertices;
  sf::VertexArray corpsesLargeVertices;
  sf::VertexArray boltParticlesVertices;
  sf::VertexArray backBoltParticlesVertices;

  bool getChanged();
  void computeVertices();
  void computeOverVertices();
  void computeShadowVertices();
  void computeBloodVertices();
  void computeCorpsesVertices();

  void computeDoors();

  bool shouldBeTransformed(int part);

  void animateParticle(displayEntityStruct &particle, float delay, float viscosity);
  bool collideWithWall(displayEntityStruct &particle, int boxWidth, int boxHeight, bool canGoThroughObstacle = false);
  void autoSpeed(displayEntityStruct &particle, float speed);
  bool checkFalling(displayEntityStruct &particle, int boxWidth, int boxHeight);

  void displayBlood(sf::RenderTarget* app);
  void displayCorpses(sf::RenderTarget* app);
  void displayBoltParticles(sf::RenderTarget* app);

  std::vector<displayEntityStruct> blood;
  std::vector<displayEntityStruct> boltParticles;
  std::vector<displayEntityStruct> backBoltParticles;
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
  sf::Sprite randomSprite[NB_RANDOM_TILES_IN_ROOM];
  roomTypeEnum roomType;

  sf::Sprite doorShadow[4];
  bool isDoorShadow[4];
  sf::Sprite doorWall[4];
  bool isDoorWall[4];
  sf::Sprite doorFrame[4];
  bool isDoorFrame[4];
  sf::Sprite doorKeyStone[4];
  bool isDoorKeyStone[4];
  sf::Sprite doorSpecial;
  bool isDoorSpecial;

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
