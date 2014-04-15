#include "WitchBlastGame.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/TileMapEntity.h"
#include "DungeonMap.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/EntityManager.h"
#include "Constants.h"
#include "RatEntity.h"
#include "GreenRatEntity.h"
#include "KingRatEntity.h"
#include "BatEntity.h"
#include "ChestEntity.h"
#include "EvilFlowerEntity.h"
#include "ItemEntity.h"
#include "ArtefactDescriptionEntity.h"
#include "StaticTextEntity.h"
#include "PnjEntity.h"

#include <iostream>
#include <sstream>

WitchBlastGame::WitchBlastGame(): Game(SCREEN_WIDTH, SCREEN_HEIGHT)
{
  app->setTitle(APP_NAME + " V" + APP_VERSION);
  ImageManager::getImageManager()->addImage((char*)"media/sprite.png");
  ImageManager::getImageManager()->addImage((char*)"media/bolt.png");
  ImageManager::getImageManager()->addImage((char*)"media/tiles.png");
  ImageManager::getImageManager()->addImage((char*)"media/rat.png");
  ImageManager::getImageManager()->addImage((char*)"media/minimap.png");
  ImageManager::getImageManager()->addImage((char*)"media/doors.png");
  ImageManager::getImageManager()->addImage((char*)"media/items.png");
  ImageManager::getImageManager()->addImage((char*)"media/items_equip.png");
  ImageManager::getImageManager()->addImage((char*)"media/chest.png");
  ImageManager::getImageManager()->addImage((char*)"media/bat.png");
  ImageManager::getImageManager()->addImage((char*)"media/evil_flower.png");
  ImageManager::getImageManager()->addImage((char*)"media/king_rat.png");
  ImageManager::getImageManager()->addImage((char*)"media/blood.png");
  ImageManager::getImageManager()->addImage((char*)"media/corpses.png");
  ImageManager::getImageManager()->addImage((char*)"media/corpses_big.png");
  ImageManager::getImageManager()->addImage((char*)"media/star.png");
  ImageManager::getImageManager()->addImage((char*)"media/star2.png");
  ImageManager::getImageManager()->addImage((char*)"media/interface.png");
  ImageManager::getImageManager()->addImage((char*)"media/pnj.png");
  ImageManager::getImageManager()->addImage((char*)"media/fairy.png");

  SoundManager::getSoundManager()->addSound((char*)"media/sound/step.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/blast00.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/blast01.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/door_closing.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/door_opening.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/chest_opening.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/impact.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/bonus.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/drink.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/player_hit.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/player_die.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/ennemy_dying.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/coin.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/pay.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/wall_impact.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/big_wall_impact.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/king_rat_cry_1.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/king_rat_cry_2.ogg");
  SoundManager::getSoundManager()->addSound((char*)"media/sound/king_rat_die.ogg");


  music.openFromFile("media/sound/track00.ogg");
  music.setVolume(75);
  music.setLoop(true);

  if (!font.loadFromFile("media/DejaVuSans-Bold.ttf"))
  {
      // erreur...
  }
  myText.setFont(font); // font est un sf::Font
}

WitchBlastGame::~WitchBlastGame()
{
  //dtor
}

void WitchBlastGame::startNewGame()
{
  // cleaning all entities
  EntityManager::getEntityManager()->clean();

  gameState = gameStateInit;

  currentFloor = new GameFloor(1);
  floorX = FLOOR_WIDTH / 2;
  floorY = FLOOR_HEIGHT / 2;

  miniMap = new GameMap(FLOOR_WIDTH, FLOOR_HEIGHT);
  refreshMinimap();

  // the interface
  SpriteEntity* interface = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_INTERFACE));
  interface->setZ(10000.0f);
  interface->removeCenter();
  interface->setType(0);

  // key symbol on the interface
  keySprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_ITEMS_EQUIP));
  keySprite.setTextureRect(sf::IntRect(ITEM_WIDTH * EQUIP_BOSS_KEY, 0,  ITEM_WIDTH, ITEM_HEIGHT));
  keySprite.setPosition(326, 616);

  // minimap on the interface
  TileMapEntity* miniMapEntity = new TileMapEntity(ImageManager::getImageManager()->getImage(4), miniMap, 16, 12, 10);
  miniMapEntity->setX(400);
  miniMapEntity->setY(607);
  miniMapEntity->setZ(10001.0f);

  // current map (tiles)
  currentTileMap = new TileMapEntity(ImageManager::getImageManager()->getImage(IMAGE_TILES), currentMap, 64, 64, 10);
  currentTileMap->setX(OFFSET_X);
  currentTileMap->setY(OFFSET_Y);

  // doors
  doorEntity[0] = new DoorEntity(8);
  doorEntity[1] = new DoorEntity(4);
  doorEntity[2] = new DoorEntity(2);
  doorEntity[3] = new DoorEntity(6);

  // the player
  player = new PlayerEntity(ImageManager::getImageManager()->getImage(0),
                              OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                              OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
  player->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  player->setParent(this);

  // generate the map
  refreshMap();

  // first map is open
  roomClosed = false;

  // and the boss room is closed
  bossRoomOpened = false;

  // game time counter an state
  lastTime = getAbsolutTime();
  gameState = gameStatePlaying;
  music.play();
}

void WitchBlastGame::startGame()
{
    startNewGame();

    // Start game loop
    while (app->isOpen())
    {
        // Process events
        sf::Event event;

        while (app->pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app->close();
        }

        if (player->canMove()) player->setVelocity(Vector2D(0.0f, 0.0f));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player->move(7);
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player->move(1);
          else
            player->move(4);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
          if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            player->move(9);
          else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            player->move(3);
          else
            player->move(6);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
          player->move(8);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
          player->move(2);
        }


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          player->fire(4);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          player->fire(6);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          player->fire(8);
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          player->fire(2);

        if (player->isDead() && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
          startNewGame();

        onUpdate();
        EntityManager::getEntityManager()->sortByZ();
        onRender();

        verifyDoorUnlocking();

        if (roomClosed)
        {
          if (GetEnnemyCount() == 0)
          {
            currentMap->setCleared(true);
            openDoors();
          }
        }
    }
    quitGame();
}

void WitchBlastGame::createFloor()
{
}

void WitchBlastGame::closeDoors()
{
  if (!currentMap->isCleared())
  {
    int i;
    for(i = 0; i < MAP_WIDTH; i++)
    {
      if (currentMap->getTile(i, 0) < 4) currentMap->setTile(i, 0, MAP_DOOR);
      if (currentMap->getTile(i, MAP_HEIGHT - 1) < 4) currentMap->setTile(i, MAP_HEIGHT - 1, MAP_DOOR);
    }
    for(i = 0; i < MAP_HEIGHT; i++)
    {
      if (currentMap->getTile(0, i) < 4) currentMap->setTile(0, i, MAP_DOOR);
      if (currentMap->getTile(MAP_WIDTH - 1, i) < 4) currentMap->setTile(MAP_WIDTH - 1, i, MAP_DOOR);
    }
    roomClosed = true;
    /*SoundManager::getSoundManager()->playSound(SOUND_DOOR_CLOSING);
    for (int i=0; i<4; i++)
      doorEntity[i]->closeDoor();*/
  }
}

void WitchBlastGame::openDoors()
{
    int i, j;
    for(i = 0; i < MAP_WIDTH; i++)
      for(j = 0; j < MAP_WIDTH; j++)
        if (currentMap->getTile(i, j) == MAP_DOOR) currentMap->setTile(i, j, 0);
  roomClosed = false;
  SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);

  if (currentMap->hasNeighbourUp() == 2 && !bossRoomOpened)
    currentMap->setTile(MAP_WIDTH/2, 0, MAP_DOOR);
  else
    doorEntity[0]->openDoor();

  if (currentMap->hasNeighbourLeft() == 2 && !bossRoomOpened)
    currentMap->setTile(0, MAP_HEIGHT / 2, MAP_DOOR);
  else
    doorEntity[1]->openDoor();

  if (currentMap->hasNeighbourDown() == 2 && !bossRoomOpened)
    currentMap->setTile(MAP_WIDTH / 2, MAP_HEIGHT - 1, MAP_DOOR);
  else
    doorEntity[2]->openDoor();

  if (currentMap->hasNeighbourRight() == 2 && !bossRoomOpened)
    currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, MAP_DOOR);
  else
    doorEntity[3]->openDoor();
}

int WitchBlastGame::GetEnnemyCount()
{
  int n=0;

  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

		if (e->getType() >= 20)
		{
		  n++;
		} // endif
	} // end for

	return n;
}

void WitchBlastGame::refreshMap()
{
  // clean the sprites from old map
  EntityManager::getEntityManager()->partialClean(10);

  // if new map, it has to be randomized
  bool generateMap = !(currentFloor->getMap(floorX, floorY)->isVisited());
  currentMap = currentFloor->getAndVisitMap(floorX, floorY);

  // load the map
  currentTileMap->setMap(currentMap);
  player->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  refreshMinimap();

  if(generateMap)
    this->generateMap();
  else
  {
    if (currentMap->getRoomType() == roomTypeMerchant)
      new PnjEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                  OFFSET_Y + (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT,
                  0);
  }

  // for testing purpose (new stuff)
  if (player->getAge() <2.0f)
  {
    /*ItemEntity* book = new ItemEntity(ItemEntity::itemHealth, player->getX(), player->getY()- 180);
    book->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    book->setMerchandise(true);*/

    int bonusType = getRandomEquipItem(true);
    ItemEntity* boots = new ItemEntity((ItemEntity::enumItemType)(ItemEntity::itemMagicianHat + bonusType), player->getX(), player->getY()+ 180);
    boots->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);

    //ChestEntity* chest = new ChestEntity(player->getX() + 100, player->getY()+ 150, CHEST_FAIRY, false);
    //chest->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
  }

  // check doors
  doorEntity[0]->setVisible(currentMap->hasNeighbourUp() > 0);
  if (currentMap->hasNeighbourUp() == 1) doorEntity[0]->setDoorType(0);
  if (currentMap->hasNeighbourUp() == 2 || currentMap->getRoomType()==roomTypeBoss) doorEntity[0]->setDoorType(1);
  if (currentMap->hasNeighbourUp() == 2 && !bossRoomOpened)
  {
    doorEntity[0]->setOpen(false);
    currentMap->setTile(MAP_WIDTH/2, 0, MAP_DOOR);
  }
  else
    doorEntity[0]->setOpen(true);

  doorEntity[3]->setVisible(currentMap->hasNeighbourRight() > 0);
  if (currentMap->hasNeighbourRight() == 1) doorEntity[3]->setDoorType(0);
  if (currentMap->hasNeighbourRight() == 2 || currentMap->getRoomType()==roomTypeBoss) doorEntity[3]->setDoorType(1);
  if (currentMap->hasNeighbourRight() == 2 && !bossRoomOpened)
  {
    doorEntity[3]->setOpen(false);
    currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, MAP_DOOR);
  }
  else
    doorEntity[3]->setOpen(true);

  doorEntity[2]->setVisible(currentMap->hasNeighbourDown() > 0);
  if (currentMap->hasNeighbourDown() == 1) doorEntity[2]->setDoorType(0);
  if (currentMap->hasNeighbourDown() == 2 || currentMap->getRoomType()==roomTypeBoss) doorEntity[2]->setDoorType(1);
  if (currentMap->hasNeighbourDown() == 2 && !bossRoomOpened)
  {
    doorEntity[2]->setOpen(false);
    currentMap->setTile(MAP_WIDTH/2, MAP_HEIGHT - 1, MAP_DOOR);
  }
  else
    doorEntity[2]->setOpen(true);

  doorEntity[1]->setVisible(currentMap->hasNeighbourLeft() > 0);
  if (currentMap->hasNeighbourLeft() == 1) doorEntity[1]->setDoorType(0);
  if (currentMap->hasNeighbourLeft() == 2 || currentMap->getRoomType()==roomTypeBoss) doorEntity[1]->setDoorType(1);
  if (currentMap->hasNeighbourLeft() == 2 && !bossRoomOpened)
  {
    doorEntity[1]->setOpen(false);
    currentMap->setTile(0, MAP_HEIGHT / 2, MAP_DOOR);
  }
  else
    doorEntity[1]->setOpen(true);
}

void WitchBlastGame::refreshMinimap()
{
  for (int j=0; j < FLOOR_HEIGHT; j++)
    for (int i=0; i < FLOOR_WIDTH; i++)
    {
      int n = currentFloor->getRoom(i, j);
      if (n > 0 && currentFloor->getMap(i, j)->isVisited())
        miniMap->setTile(i, j, currentFloor->getRoom(i, j));
      else if (n > 0 && currentFloor->getMap(i, j)->isKnown())
        miniMap->setTile(i, j, 9);
      else
        miniMap->setTile(i, j, 0);
    }
  miniMap->setTile(floorX, floorY, 8);
}

void WitchBlastGame::checkEntering()
{
  if (!currentMap->isCleared())
  {
    player->setEntering();
    SoundManager::getSoundManager()->playSound(SOUND_DOOR_CLOSING);
    for (int i=0; i<4; i++)
      doorEntity[i]->closeDoor();
  }
}

void WitchBlastGame::saveMapItems()
{
  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity* e = *it;
		it++;

    ItemEntity* itemEntity = dynamic_cast<ItemEntity*>(e);
    ChestEntity* chestEntity = dynamic_cast<ChestEntity*>(e);

		if (itemEntity != NULL)
		{
      currentMap->addItem(itemEntity->getItemType(), itemEntity->getX(), itemEntity->getY(), itemEntity->getMerchandise());
		} // endif
		else if (chestEntity != NULL)
		{
      currentMap->addChest(chestEntity->getChestType(), chestEntity->getOpened(), chestEntity->getX(), chestEntity->getY());
		} // endif
		else
    {
      SpriteEntity* spriteEntity = dynamic_cast<SpriteEntity*>(e);
      if (spriteEntity != NULL && (e->getType() == TYPE_BLOOD || e->getType() == TYPE_CORPSE ) )
      {
        int spriteFrame = spriteEntity->getFrame();
        if (spriteEntity->getWidth() == 128) spriteFrame += FRAME_CORPSE_KING_RAT;
        currentMap->addSprite(e->getType(), spriteFrame, e->getX(), e->getY(), spriteEntity->getScaleX());
      }

    }
	} // end for
}

void WitchBlastGame::moveToOtherMap(int direction)
{
  saveMapItems();
  switch (direction)
  {
    case (4): floorX--;  player->moveTo((OFFSET_X + MAP_WIDTH * TILE_WIDTH), player->getY()); player->move(4);  break;
    case (6): floorX++;  player->moveTo(OFFSET_X, player->getY()); player->move(6); break;
    case (8): floorY--;  player->moveTo(player->getX(), OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT - 10); player->move(8); break;
    case (2): floorY++;  player->moveTo(player->getX(), OFFSET_Y);  break;
  }
  refreshMap();
  checkEntering();
  currentMap->restoreMapObjects();
}

void WitchBlastGame::onRender()
{
    // clear the view
    app->clear(sf::Color(32, 32, 32));

    // render the game objects
    EntityManager::getEntityManager()->render(app);

    myText.setColor(sf::Color(255, 255, 255, 255));
    myText.setCharacterSize(17);
    myText.setString("WASD or ZQSD to move\nArrows to shoot");
    myText.setPosition(650, 650);
    app->draw(myText);

    myText.setCharacterSize(18);
    std::ostringstream oss;
    oss << player->getGold();
    myText.setString(oss.str());
    myText.setPosition(690, 612);
    app->draw(myText);

    myText.setColor(sf::Color(0, 0, 0, 255));
    myText.setCharacterSize(16);
    myText.setString("Level 1");
    myText.setPosition(410, 692);
    app->draw(myText);

    sf::RectangleShape rectangle(sf::Vector2f(200, 25));
    // life

    if (gameState == gameStatePlaying)
    {
      // life and mana
      rectangle.setFillColor(sf::Color(190, 20, 20));
      rectangle.setPosition(sf::Vector2f(90, 622));
      rectangle.setSize(sf::Vector2f(200.0f * (float)(player->getHpDisplay()) / (float)(player->getHpMax()) , 25));
      app->draw(rectangle);

      rectangle.setFillColor(sf::Color(255, 190, 190));
      rectangle.setPosition(sf::Vector2f(90, 625));
      rectangle.setSize(sf::Vector2f(200.0f * (float)(player->getHpDisplay()) / (float)(player->getHpMax()) , 2));
      app->draw(rectangle);

      rectangle.setFillColor(sf::Color(20, 20, 190));
      rectangle.setPosition(sf::Vector2f(90, 658));
      rectangle.setSize(sf::Vector2f(200.0f * player->getPercentFireDelay() , 25));
      app->draw(rectangle);

      rectangle.setFillColor(sf::Color(190, 190, 255));
      rectangle.setPosition(sf::Vector2f(90, 661));
      rectangle.setSize(sf::Vector2f(200.0f * player->getPercentFireDelay() , 2));
      app->draw(rectangle);

      // drawing the key on the interface
      if (player->isEquiped(EQUIP_BOSS_KEY)) app->draw(keySprite);

      if (player->isDead())
      {
        float x0 = OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;
        int fade = 255 * (1.0f + cos(2.0f * getAbsolutTime())) * 0.5f;

        myText.setColor(sf::Color(255, 255, 255, 255));
        myText.setCharacterSize(25);
        myText.setString("GAME OVER");
        myText.setPosition(x0 - myText.getLocalBounds().width / 2, 400);
        app->draw(myText);

        myText.setColor(sf::Color(255, 255, 255, fade));
        myText.setCharacterSize(20);
        myText.setString("Press [ENTER] to play again !");
        myText.setPosition(x0 - myText.getLocalBounds().width / 2, 440);
        app->draw(myText);
      }
      else if (currentMap->getRoomType() == roomTypeExit)
      {
        float x0 = OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;
        myText.setColor(sf::Color(255, 255, 255, 255));
        myText.setCharacterSize(25);
        myText.setString("CONGRATULATIONS !\nYou've challenged this demo and\nmanaged to kill the boss !\nSee you soon for new adventures !");
        myText.setPosition(x0 - myText.getLocalBounds().width / 2, 220);
        app->draw(myText);
      }
    }

    app->display();
}

void WitchBlastGame::generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor)
{
  SpriteEntity* blood = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BLOOD), x, y, 16, 16, 6);
  //deadRat->setZ(y + height);
  blood->setZ(OFFSET_Y - 1);
  int b0 = 0;
  if (bloodColor == BaseCreatureEntity::bloodGreen) b0 += 6;
  blood->setFrame(b0 + rand()%6);
  blood->setType(TYPE_BLOOD);
  blood->setVelocity(Vector2D(rand()%250));
  blood->setViscosity(0.95f);

  float bloodScale = 1.0f + (rand() % 10) * 0.1f;
  blood->setScale(bloodScale, bloodScale);
}

void WitchBlastGame::showArtefactDescription(ItemEntity::enumItemType itemType)
{
  new ArtefactDescriptionEntity(itemType, this); //, &font);
}

void WitchBlastGame::generateMap()
{
  if (currentMap->getRoomType() == roomTypeStandard)
    generateStandardMap();
  else if (currentMap->getRoomType() == roomTypeBonus)
  {
    currentMap->setCleared(true);
    Vector2D v = currentMap->generateBonusRoom();
    int bonusType = getRandomEquipItem(false);
    if (bonusType == EQUIP_FAIRY)
    {
      ChestEntity* chest = new ChestEntity(v.x, v.y, CHEST_FAIRY, false);
      chest->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    }
    else
    {
      ItemEntity* newItem
        = new ItemEntity( (ItemEntity::enumItemType)(ItemEntity::itemMagicianHat + bonusType), v.x ,v.y);
      newItem->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    }
  }
  else if (currentMap->getRoomType() == roomTypeKey)
  {
    Vector2D v = currentMap->generateKeyRoom();
    ItemEntity* newItem
      = new ItemEntity( (ItemEntity::enumItemType)(ItemEntity::itemBossKey), v.x ,v.y);
    newItem->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    initMonsterArray();
    int x0 = MAP_WIDTH / 2;
    int y0 = MAP_HEIGHT / 2;
    monsterArray[x0][y0] = true;
    findPlaceMonsters(MONSTER_RAT, 5);
    findPlaceMonsters(MONSTER_BAT, 5);
  }
  else if (currentMap->getRoomType() == roomTypeMerchant)
  {
    currentMap->generateMerchantRoom();

    ItemEntity* item1 = new ItemEntity(
      ItemEntity::itemHealth,
      OFFSET_X + (MAP_WIDTH / 2 - 2) * TILE_WIDTH + TILE_WIDTH / 2,
      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item1->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    item1->setMerchandise(true);

    int bonusType = getRandomEquipItem(true);
    ItemEntity* item2 = new ItemEntity(
      (ItemEntity::enumItemType)(ItemEntity::itemMagicianHat + bonusType),
      OFFSET_X + (MAP_WIDTH / 2 + 2) * TILE_WIDTH + TILE_WIDTH / 2,
      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item2->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    item2->setMerchandise(true);

    new PnjEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                  OFFSET_Y + (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT,
                  0);

    currentMap->setCleared(true);
  }
  else if (currentMap->getRoomType() == roomTypeBoss)
  {
    currentMap->generateRoom(0);

    boss = new KingRatEntity(OFFSET_X + (MAP_WIDTH / 2 - 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT + TILE_HEIGHT / 2,
                      currentMap, player);
  }
  else if (currentMap->getRoomType() == roomTypeStarting)
  {
    currentMap->generateRoom(0);
    currentMap->setCleared(true);
  }
  else if (currentMap->getRoomType() == roomTypeExit)
  {
    currentMap->generateRoom(0);
    currentMap->setCleared(true);
  }
  else
    currentMap->randomize(currentMap->getRoomType());
}

void WitchBlastGame::initMonsterArray()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
      monsterArray[i][j] = false;
}

void WitchBlastGame::addMonster(monster_type_enum monsterType, float xm, float ym)
{
  switch (monsterType)
  {
    case MONSTER_RAT: new RatEntity(xm, ym - 2, currentMap); break;
    case MONSTER_BAT: new BatEntity(xm, ym, currentMap); break;
    case MONSTER_EVIL_FLOWER: new EvilFlowerEntity(xm, ym, currentMap, player); break;

    case MONSTER_KING_RAT: new KingRatEntity(xm, ym, currentMap, player); break;
  }
}

void WitchBlastGame::findPlaceMonsters(monster_type_enum monsterType, int amount)
{
  // find a suitable place
  bool isMonsterFlying = monsterType == MONSTER_BAT;

  bool bOk;
  int xm, ym;
  float xMonster, yMonster;

  for (int index = 0; index < amount; index++)
  {
    bOk = false;

    while (!bOk)
    {
      bOk = true;
      xm = 1 +rand() % (MAP_WIDTH - 3);
      ym = 1 +rand() % (MAP_HEIGHT - 3);
      if (monsterArray[xm][ym])
      {
        bOk = false;
      }
      if (bOk && !isMonsterFlying && !currentMap->isWalkable(xm, ym))
      {
        bOk = false;
      }
      if (bOk)
      {
        xMonster = OFFSET_X + xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
        yMonster = OFFSET_Y + ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

        float dist2 = (xMonster - player->getX())*(xMonster - player->getX()) + (yMonster - player->getY())*(yMonster - player->getY());
        if ( dist2 < 75000.0f)
        {
          bOk = false;
        }
        else
        {
          addMonster(monsterType, xMonster, yMonster);
          monsterArray[xm][ym] = true;
        }
      }
    }
  }
}

void WitchBlastGame::generateStandardMap()
{
  initMonsterArray();

  int random = rand() % 100;

  if (random < 20)
  {
    currentMap->generateRoom(rand()%3);
    findPlaceMonsters(MONSTER_RAT,4);
  }
  else if (random < 40)
  {
    currentMap->generateRoom(rand()%4);
    findPlaceMonsters(MONSTER_BAT,4);
  }
  else if (random < 60)
  {
    currentMap->generateRoom(rand()%4);
    findPlaceMonsters(MONSTER_EVIL_FLOWER,4);
  }
  else if (random < 80)
  {
    Vector2D v = currentMap->generateBonusRoom();
    ChestEntity* chest = new ChestEntity(v.x, v.y, CHEST_BASIC, false);
    chest->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, OFFSET_X, OFFSET_Y);
    currentMap->setCleared(true);
  }
  else
  {
    currentMap->generateRoom(rand()%3);
    findPlaceMonsters(MONSTER_RAT,3);
    findPlaceMonsters(MONSTER_BAT,3);
  }
  //currentMap->setCleared(true);
}

int WitchBlastGame::getRandomEquipItem(bool toSale = false)
{
  std::vector<int> bonusSet;
  int setSize = 0;
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    if (!player->isEquiped(i) && i != EQUIP_BOSS_KEY)
    {
      if (!toSale || i!= EQUIP_FAIRY)
      {
        bonusSet.push_back(i);
        setSize++;
      }
    }
  }
  int bonusType = 0;
  if (setSize > 0) bonusType = bonusSet[rand() % setSize];

  return bonusType;
}

void WitchBlastGame::verifyDoorUnlocking()
{
  int colliding = (player->getColliding());

  if (colliding > 0 && currentMap->isCleared() && !bossRoomOpened && player->isEquiped(EQUIP_BOSS_KEY))
  {
    int xt = (player->getX() - OFFSET_X) / TILE_WIDTH;
    int yt = (player->getY() - OFFSET_Y) / TILE_HEIGHT;
    //std::cout << "VERIFY: " << xt << ", " << yt << std::endl;

    if (yt <= 1 && xt >= MAP_WIDTH / 2 - 1 && xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourUp() == 2)
    {
      //std::cout << "touché !";
      doorEntity[0]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, 0, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (yt >= MAP_HEIGHT - 2 && xt >= MAP_WIDTH / 2 - 1 &&xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourDown() == 2)
    {
      //std::cout << "touché !";
      doorEntity[2]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, MAP_HEIGHT - 1, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt <= 1 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourLeft() == 2)
    {
      //std::cout << "touché !";
      doorEntity[1]->openDoor();
      currentMap->setTile(0, MAP_HEIGHT / 2, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt >= MAP_WIDTH - 2 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourRight() == 2)
    {
      //std::cout << "touché !";
      doorEntity[3]->openDoor();
      currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
  }
}
