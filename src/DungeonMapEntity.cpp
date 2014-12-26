#include "WitchBlastGame.h"
#include "DungeonMapEntity.h"
#include "Constants.h"
#include "sfml_game/ImageManager.h"

DungeonMapEntity::DungeonMapEntity() : GameEntity (0.0f, 0.0f)
{
    this->z = -1.0f;

    type = 0;
    hasChanged = true;

    DungeonMapEntityPost* post = new DungeonMapEntityPost(this);
    post->setZ(-0.2f);
    post->setType(0);
}


void DungeonMapEntity::animate(float delay)
{
  age += delay;
  bool needCompute = getChanged() || game().getCurrentMap()->getChanged();
  if (needCompute) computeVertices();

  // blood
  bool moving = false;
  for (unsigned int i = 0; i < blood.size(); i++)
  {
    if (blood[i].moving)
    {
      moving = true;
      animateParticle(blood[i], delay);
    }
  }
  if (moving) computeBloodVertices();

  // corpses
  moving = false;
  for (unsigned int i = 0; i < corpses.size(); i++)
  {
    if (corpses[i].moving)
    {
      moving = true;
      animateParticle(corpses[i], delay);
    }
  }
  for (unsigned int i = 0; i < corpsesLarge.size(); i++)
  {
    if (corpsesLarge[i].moving)
    {
      moving = true;
      animateParticle(corpsesLarge[i], delay);
    }
  }
  if (moving) computeCorpsesVertices();
}

void DungeonMapEntity::animateParticle(displayEntityStruct &particle, float delay)
{
  particle.velocity.x *= 0.95f;
  particle.velocity.y *= 0.95f;

  if (particle.velocity.x < -5 || particle.velocity.x > 5
      || particle.velocity.y < -5 || particle.velocity.y > 5)
  {
    particle.x += delay * particle.velocity.x;
    particle.y += delay * particle.velocity.y;
  }
  else
    particle.moving = false;
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
}

void DungeonMapEntity::renderPost(sf::RenderTarget* app)
{
  displayBlood(app);
  displayCorpses(app);
}

std::vector <displayEntityStruct> DungeonMapEntity::getBlood()
{
  return blood;
}

std::vector <displayEntityStruct> DungeonMapEntity::getCorpses()
{
  auto result = corpses;
  result.insert( result.end(), corpsesLarge.begin(), corpsesLarge.end() );
  return result;
}

void DungeonMapEntity::displayBlood(sf::RenderTarget* app)
{
  app->draw(bloodVertices, ImageManager::getInstance().getImage(IMAGE_BLOOD));
}

void DungeonMapEntity::displayCorpses(sf::RenderTarget* app)
{
  app->draw(corpsesVertices, ImageManager::getInstance().getImage(IMAGE_CORPSES));
  app->draw(corpsesLargeVertices, ImageManager::getInstance().getImage(IMAGE_CORPSES_BIG));
}

void DungeonMapEntity::refreshMap()
{
  hasChanged = true;

  blood.clear();
  corpses.clear();
  corpsesLarge.clear();

  computeBloodVertices();
  computeCorpsesVertices();
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

void DungeonMapEntity::computeBloodVertices()
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
}

void DungeonMapEntity::computeCorpsesVertices()
{
  corpsesVertices.setPrimitiveType(sf::Quads);
  corpsesVertices.resize(corpses.size() * 4);
  for (unsigned int i = 0; i < corpses.size(); i++)
  {
    auto particle = corpses[i];

    sf::Vertex* quad = &corpsesVertices[i * 4];

    float middle = 32;
    int nx = particle.frame % 10;
    int ny = particle.frame / 10;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 64, ny * 64);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 64, ny * 64);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 64, (ny + 1) * 64);
    quad[3].texCoords = sf::Vector2f(nx * 64, (ny + 1) * 64);
  }

  corpsesLargeVertices.setPrimitiveType(sf::Quads);
  corpsesLargeVertices.resize(corpsesLarge.size() * 4);
  for (unsigned int i = 0; i < corpsesLarge.size(); i++)
  {
    auto particle = corpsesLarge[i];

    sf::Vertex* quad = &corpsesLargeVertices[i * 4];

    float middle = 64;
    int nx = (particle.frame) % 6;
    int ny = (particle.frame - FRAME_CORPSE_KING_RAT) / 6;

    quad[0].position = sf::Vector2f(particle.x - middle, particle.y - middle);
    quad[1].position = sf::Vector2f(particle.x + middle, particle.y - middle);
    quad[2].position = sf::Vector2f(particle.x + middle, particle.y + middle);
    quad[3].position = sf::Vector2f(particle.x - middle, particle.y + middle);

    quad[0].texCoords = sf::Vector2f(nx * 128, ny * 128);
    quad[1].texCoords = sf::Vector2f((nx + 1) * 128, ny * 128);
    quad[2].texCoords = sf::Vector2f((nx + 1) * 128, (ny + 1) * 128);
    quad[3].texCoords = sf::Vector2f(nx * 128, (ny + 1) * 128);
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

  bloodEntity.moving = true;

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

  bloodEntity.moving = true;

  blood.push_back(bloodEntity);
}

void DungeonMapEntity::addCorpse(float x, float y, int frame)
{
  displayEntityStruct corpseEntity;

  corpseEntity.frame = frame;
  corpseEntity.x = x;
  corpseEntity.y = y;
  corpseEntity.velocity.x = 0;
  corpseEntity.velocity.y = 0;

  corpseEntity.moving = true;

  if (frame >= FRAME_CORPSE_KING_RAT)
  {
    corpsesLarge.push_back(corpseEntity);
  }
  else
  {
    corpses.push_back(corpseEntity);
  }

}
/////////////////////////////////////////////////////////////////////////


DungeonMapEntityPost::DungeonMapEntityPost(DungeonMapEntity* parent) : GameEntity (0.0f, 0.0f)
{
  this->parent = parent;
}

void DungeonMapEntityPost::animate(float delay)
{
}

void DungeonMapEntityPost::render(sf::RenderTarget* app)
{
  parent->renderPost(app);
}
