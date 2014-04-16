#ifndef MAGICGAME_H
#define MAGICGAME_H

#include "sfml_game/Game.h"
#include "sfml_game/TileMapEntity.h"
#include "PlayerEntity.h"
#include "EnnemyEntity.h"
#include "DoorEntity.h"
#include "GameFloor.h"


class WitchBlastGame : public Game
{
  public:
    WitchBlastGame();
    virtual ~WitchBlastGame();

    virtual void startGame();
    void moveToOtherMap(int direction);
    void closeDoors();
    void openDoors();
    int GetEnnemyCount();
    void generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor);
    void showArtefactDescription(ItemEntity::enumItemType itemType);

    void write(std::string test_str, int size, float x, float y);

  protected:
    virtual void onRender();
    virtual void onUpdate();

  private:
    bool isFiring;

    PlayerEntity* player;
    EnnemyEntity* boss;
    // the doors graphics
    DoorEntity* doorEntity[4];

    GameMap* miniMap;
    DungeonMap* currentMap;
    GameFloor* currentFloor;

    sf::Font font;
    sf::Text myText;
    sf::Sprite keySprite;
    sf::Music music;

    TileMapEntity* currentTileMap;
    int floorX, floorY;

    enum specialStateEnum
    {
      SpecialStateNone,
      SpecialStateFadeIn,
      SpecialStateFadeOut
    };
    specialStateEnum specialState;
    float timer;

    void startNewGame();

    void createFloor();
    void refreshMap();
    void refreshMinimap();
    void generateMap();
    void generateStandardMap();
    void checkEntering();
    void saveMapItems();
    void initMonsterArray();
    void addMonster(monster_type_enum monsterType, float xm, float ym);
    void findPlaceMonsters(monster_type_enum monsterType, int amount);
    int getRandomEquipItem(bool toSale);
    void verifyDoorUnlocking();

    bool roomClosed;
    bool bossRoomOpened;
    enum gameStateEnum { gameStateInit, gameStatePlaying};
    gameStateEnum gameState;

    // use to remember if a case has a monster in monster spawn
    bool monsterArray[MAP_WIDTH][MAP_HEIGHT];
};

#endif // MAGICGAME_H
