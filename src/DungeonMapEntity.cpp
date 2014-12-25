#include "WitchBlastGame.h"
#include "DungeonMapEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

DungeonMapEntity::DungeonMapEntity() : GameEntity (0.0f, 0.0f)
{
    this->z = -1.0f;

    type = 0;
    hasChanged = true;
}


void DungeonMapEntity::animate(float delay)
{
  age += delay;
  bool needCompute = getChanged() || game().getCurrentMap()->getChanged();
  if (needCompute) computeVertices();

  for (unsigned int i = 0; i < blood.size(); i++) animateParticle(blood[i], delay);
}

void DungeonMapEntity::animateParticle(displayEntityStruct &particle, float delay)
{
  particle.velocity.x *= 0.95f;
  particle.velocity.y *= 0.95f;

  particle.x += delay * particle.velocity.x;
  particle.y += delay * particle.velocity.y;
}

bool DungeonMapEntity::getChanged()
{
  bool result = hasChanged;
  hasChanged = false;
  return result;
}

void DungeonMapEntity::render(sf::RenderTarget* app)
{
  app->draw(vertices, ImageManager::getInstance().getImage(IMAGE_TILES));

  displayBlood(app);
}

std::vector <displayEntityStruct> DungeonMapEntity::getBlood()
{
  return blood;
}

void DungeonMapEntity::displayBlood(sf::RenderTarget* app)
{
  bloodVertices.setPrimitiveType(sf::Quads);
  bloodVertices.resize(blood.size() * 4);

  for (unsigned int i = 0; i < blood.size(); i++)
  {
    auto particle = blood[i];

    sf::Vertex* quad = &bloodVertices[i * 4];

    float middle = 8.0f * particle.scale;
    int nx = particle.frame % 6;
    int ny = particle.frame / 6;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 16, ny * 16);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 16, ny * 16);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 16, (ny + 1) * 16);
    quad[3].texCoords = sf::Vector2f(nx * 16, (ny + 1) * 16);
  }
  app->draw(bloodVertices, ImageManager::getInstance().getImage(IMAGE_BLOOD));
}

void DungeonMapEntity::refreshMap()
{
  hasChanged = true;

  blood.clear();
}

void DungeonMapEntity::computeVertices()
{
  GameMap* gameMap = game().getCurrentMap();
  int tilesProLine = 10;
  int tileWidth = 64;
  int tileHeight = 64;
  int tileBoxWidth = 64;
  int tileBoxHeight = 64;

  vertices.setPrimitiveType(sf::Quads);
  vertices.resize(gameMap->getWidth() * gameMap->getHeight() * 4);

  for (int i = 0; i < gameMap->getWidth(); i++)
    for (int j = 0; j < gameMap->getHeight(); j++)
    {
      int nx = gameMap->getTile(i, j) % tilesProLine;
      int ny = gameMap->getTile(i, j) / tilesProLine;

      sf::Vertex* quad = &vertices[(i + j * gameMap->getWidth()) * 4];

      quad[0].position = sf::Vector2f(x + i * tileWidth, y + j * tileHeight);
      quad[1].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + j * tileHeight);
      quad[2].position = sf::Vector2f(x + (i + 1) * tileWidth + (tileBoxWidth -tileWidth), y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));
      quad[3].position = sf::Vector2f(x + i * tileWidth, y + (j + 1) * tileHeight + (tileBoxHeight - tileHeight));

      quad[0].texCoords = sf::Vector2f(nx * tileBoxWidth, ny * tileBoxHeight);
      quad[1].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, ny * tileBoxHeight);
      quad[2].texCoords = sf::Vector2f((nx + 1) * tileBoxWidth, (ny + 1) * tileBoxHeight);
      quad[3].texCoords = sf::Vector2f(nx * tileBoxWidth, (ny + 1) * tileBoxHeight);
    }
}

void DungeonMapEntity::generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor)
{
  displayEntityStruct bloodEntity;

  int b0 = 0 + 6 * (int)bloodColor;
  bloodEntity.frame = b0 + rand()%6;
  bloodEntity.velocity = Vector2D(rand()%250);
  bloodEntity.x = x;
  bloodEntity.y = y;
  bloodEntity.scale = 1.0f + (rand() % 10) * 0.1f;

  blood.push_back(bloodEntity);
}

void DungeonMapEntity::addBlood(float x, float y, int frame, float scale)
{
  displayEntityStruct bloodEntity;

  bloodEntity.frame = frame;
  bloodEntity.x = x;
  bloodEntity.y = y;
  bloodEntity.velocity.x = 0;
  bloodEntity.velocity.y = 0;
  bloodEntity.scale = scale;

  blood.push_back(bloodEntity);
}
