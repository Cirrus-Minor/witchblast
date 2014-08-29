/**  This file is part of Witch Blast.
  *
  *  Witch Blast is free software: you can redistribute it and/or modify
  *  it under the terms of the GNU General Public License as published by
  *  the Free Software Foundation, either version 3 of the License, or
  *  (at your option) any later version.
  *
  *  Witch Blast is distributed in the hope that it will be useful,
  *  but WITHOUT ANY WARRANTY; without even the implied warranty of
  *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  *  GNU General Public License for more details.
  *
  *  You should have received a copy of the GNU General Public License
  *  along with Witch Blast.  If not, see <http://www.gnu.org/licenses/>.
  */

#include "WitchBlastGame.h"
#include "sfml_game/SpriteEntity.h"
#include "sfml_game/TileMapEntity.h"
#include "DungeonMap.h"
#include "sfml_game/ImageManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/EntityManager.h"
#include "Constants.h"
#include "RatEntity.h"
#include "BlackRatEntity.h"
#include "GreenRatEntity.h"
#include "KingRatEntity.h"
#include "CyclopsEntity.h"
#include "GiantSpiderEntity.h"
#include "GiantSlimeEntity.h"
#include "ButcherEntity.h"
#include "BatEntity.h"
#include "ImpEntity.h"
#include "SlimeEntity.h"
#include "ChestEntity.h"
#include "EvilFlowerEntity.h"
#include "BubbleEntity.h"
#include "ItemEntity.h"
#include "WitchEntity.h"
#include "CauldronEntity.h"
#include "ArtefactDescriptionEntity.h"
#include "PnjEntity.h"
#include "TextEntity.h"
#include "StandardRoomGenerator.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

static std::string intToString(int n)
{
  std::ostringstream oss;
  oss << n;
  return oss.str();
}

namespace {
WitchBlastGame* gameptr;
}

WitchBlastGame::WitchBlastGame():
  Game(SCREEN_WIDTH,
       SCREEN_HEIGHT,
       APP_NAME + " V" + APP_VERSION)
{
  gameptr = this;

  // loading resources
  ImageManager::getImageManager()->addImage("media/player_base.png");
  ImageManager::getImageManager()->addImage("media/player_equip.png");
  ImageManager::getImageManager()->addImage("media/player_collar.png");
  ImageManager::getImageManager()->addImage("media/bolt.png");
  ImageManager::getImageManager()->addImage("media/tiles.png");
  ImageManager::getImageManager()->addImage("media/rat.png");
  ImageManager::getImageManager()->addImage("media/minimap.png");
  ImageManager::getImageManager()->addImage("media/doors.png");
  ImageManager::getImageManager()->addImage("media/items.png");
  ImageManager::getImageManager()->addImage("media/items_equip.png");
  ImageManager::getImageManager()->addImage("media/chest.png");
  ImageManager::getImageManager()->addImage("media/bat.png");
  ImageManager::getImageManager()->addImage("media/evil_flower.png");
  ImageManager::getImageManager()->addImage("media/slime.png");
  ImageManager::getImageManager()->addImage("media/imp.png");
  ImageManager::getImageManager()->addImage("media/spider_egg.png");
  ImageManager::getImageManager()->addImage("media/spider_web.png");
  ImageManager::getImageManager()->addImage("media/little_spider.png");
  ImageManager::getImageManager()->addImage("media/bubble.png");
  ImageManager::getImageManager()->addImage("media/witch.png");
  ImageManager::getImageManager()->addImage("media/cauldron.png");

  ImageManager::getImageManager()->addImage("media/butcher.png");
  ImageManager::getImageManager()->addImage("media/giant_slime.png");
  ImageManager::getImageManager()->addImage("media/king_rat.png");
  ImageManager::getImageManager()->addImage("media/cyclop.png");
  ImageManager::getImageManager()->addImage("media/giant_spider.png");

  ImageManager::getImageManager()->addImage("media/blood.png");
  ImageManager::getImageManager()->addImage("media/corpses.png");
  ImageManager::getImageManager()->addImage("media/corpses_big.png");
  ImageManager::getImageManager()->addImage("media/star.png");
  ImageManager::getImageManager()->addImage("media/star2.png");
  ImageManager::getImageManager()->addImage("media/interface.png");
  ImageManager::getImageManager()->addImage("media/hud_shots.png");
  ImageManager::getImageManager()->addImage("media/boom64.png");

  ImageManager::getImageManager()->addImage("media/pnj.png");
  ImageManager::getImageManager()->addImage("media/fairy.png");

  SoundManager::getSoundManager()->addSound("media/sound/blast00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/blast01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/door_closing.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/door_opening.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/chest_opening.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/impact.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/bonus.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/drink.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/eat.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/player_hit.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/player_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/ennemy_dying.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/coin.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/pay.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/wall_impact.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/big_wall_impact.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/king_rat_cry_1.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/king_rat_cry_2.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/king_rat_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/slime_jump.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/slime_impact.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/slime_impact_weak.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/slime_smash.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/ice_charge.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/electric.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/select.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/heart.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/rat_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/bat_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/imp_hurt.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/imp_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/rock_impact.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/throw.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/cyclop00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/cyclop_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/butcher_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/butcher_01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/butcher_hurt.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/butcher_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/vib.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/boom_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/clang_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/bubble_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/bubble_01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/trap.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/egg_smash_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/egg_smash_01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/spider_walking.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/spider_web.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/spider_hurt.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/spider_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/little_spider_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/witch_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/witch_01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/witch_die_00.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/witch_die_01.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/witch_02.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/invoke.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/cauldron_die.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/critical.ogg");
  SoundManager::getSoundManager()->addSound("media/sound/gong.ogg");

  if (font.loadFromFile("media/DejaVuSans-Bold.ttf"))
  {
    myText.setFont(font);
  }

  miniMap = NULL;
  menuMap = NULL;
  currentMap = NULL;
  currentFloor = NULL;
  xGameState = xGameStateNone;
  isPausing = false;
  showLogical = false;

  shotsSprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_HUD_SHOTS));

  configureFromFile();
  srand(time(NULL));

  //tools::setLanguage("french");
}

WitchBlastGame::~WitchBlastGame()
{
  // cleaning all entities
  EntityManager::getEntityManager()->clean();

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (menuMap != NULL) delete (menuMap);
  if (currentFloor != NULL) delete (currentFloor);
}

DungeonMap* WitchBlastGame::getCurrentMap()
{
  return currentMap;
}

PlayerEntity* WitchBlastGame::getPlayer()
{
  return player;
}

Vector2D WitchBlastGame::getPlayerPosition()
{
  return Vector2D(player->getX(), player->getY());
}

int WitchBlastGame::getLevel()
{
  return level;
}

bool WitchBlastGame::getShowLogical()
{
  return showLogical;
}

void WitchBlastGame::onUpdate()
{
  if (!isPausing)
  {
    checkFallingEntities();
    EntityManager::getEntityManager()->animate(deltaTime);
    if (sf::Keyboard::isKeyPressed(input[KeyTimeControl]))
    {
      EntityManager::getEntityManager()->animate(deltaTime);
      SoundManager::getSoundManager()->playSound(SOUND_VIB);
    }
    else
      SoundManager::getSoundManager()->stopSound(SOUND_VIB);

    if (xGameState != xGameStateNone)
    {
      xGameTimer -= deltaTime;
      if (xGameTimer <= 0.0f)
      {
        if (xGameState == xGameStateFadeOut)
        {
          if (player->getPlayerStatus() == PlayerEntity::playerStatusGoingUp)
          {
            level++;
            startNewLevel();
          }
          else
            startNewGame(false);
        }

        else
          xGameState = xGameStateNone;
      }
    }

    if (isPlayerAlive)
    {
      if (player->getHp() <= 0)
      {
        isPlayerAlive = false;
        playMusic(MusicEnding);
      }
    }
  }
}

void WitchBlastGame::startNewGame(bool fromSaveFile)
{
  gameState = gameStateInit;
  level = 1;

  // cleaning all entities
  EntityManager::getEntityManager()->clean();

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (currentFloor != NULL) delete (currentFloor);
  miniMap = NULL;
  currentFloor = NULL;

  // current map (tiles)
  currentTileMap = new TileMapEntity(ImageManager::getImageManager()->getImage(IMAGE_TILES), currentMap, 64, 64, 10);
  currentTileMap->setX(OFFSET_X);
  currentTileMap->setY(OFFSET_Y);

  // the interface
  SpriteEntity* interface = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_INTERFACE));
  interface->setZ(10000.0f);
  interface->removeCenter();
  interface->setType(0);

  // key symbol on the interface
  keySprite.setTexture(*ImageManager::getImageManager()->getImage(IMAGE_ITEMS_EQUIP));
  keySprite.setTextureRect(sf::IntRect(ITEM_WIDTH * EQUIP_BOSS_KEY, 0,  ITEM_WIDTH, ITEM_HEIGHT));
  keySprite.setPosition(326, 616);

  miniMap = new GameMap(FLOOR_WIDTH, FLOOR_HEIGHT);
  // minimap on the interface
  TileMapEntity* miniMapEntity = new TileMapEntity(ImageManager::getImageManager()->getImage(IMAGE_MINIMAP), miniMap, 15, 11, 12);
  miniMapEntity->setTileBox(16, 12);
  miniMapEntity->setX(407);
  miniMapEntity->setY(614);
  miniMapEntity->setZ(10001.0f);

  // doors
  doorEntity[0] = new DoorEntity(8);
  doorEntity[1] = new DoorEntity(4);
  doorEntity[2] = new DoorEntity(2);
  doorEntity[3] = new DoorEntity(6);

  if (fromSaveFile)
  {
    if (!loadGame())
      fromSaveFile = false;
    else
      playLevel();
  }
  if (!fromSaveFile)
  {
    // the player
    player = new PlayerEntity(OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                              OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
    resetKilledEnemies();
    startNewLevel();
  }
}

void WitchBlastGame::startNewLevel()
{
  bool needShop = false;
  // create the new level
  if (currentFloor != NULL)
  {
    if (level > 1)
    {
      if (!currentFloor->hasRoomOfType(roomTypeMerchant)) needShop = true;
    }
    delete currentFloor;
  }
  currentFloor = new GameFloor(level);
  if (needShop) currentFloor->setForceShop();
  currentFloor->createFloor();

  // center it
  floorX = FLOOR_WIDTH / 2;
  floorY = FLOOR_HEIGHT / 2;

  // move the player
  if (level == 1)
    player->moveTo(OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                              OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
  else
    player->moveTo(OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                              OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT - 3 * TILE_HEIGHT));

  // the boss room is closed
  bossRoomOpened = false;
  // to test
  displayKilledEnemies();

  playLevel();
}

void WitchBlastGame::playLevel()
{
  isPlayerAlive = true;
  player->setVelocity(Vector2D(0.0f, 0.0f));
  player->setPlayerStatus(PlayerEntity::playerStatusPlaying);

  // generate the map
  refreshMap();
  // items from save
  currentMap->restoreMapObjects();

  // first map is open
  roomClosed = false;

  // game time counter an state
  lastTime = getAbsolutTime();
  gameState = gameStatePlaying;

  playMusic(MusicDungeon);

  // fade in
  xGameState = xGameStateFadeIn;
  xGameTimer = FADE_IN_DELAY;

  float x0 = OFFSET_X + MAP_WIDTH * 0.5f * TILE_WIDTH;
  float y0 = OFFSET_Y + MAP_HEIGHT * 0.5f * TILE_HEIGHT + 40.0f;

  std::ostringstream oss;
  oss << "Level " << level;

  TextEntity* text = new TextEntity(oss.str(), 30, x0, y0);
  text->setAlignment(ALIGN_CENTER);
  text->setLifetime(2.5f);
  text->setWeight(-36.0f);
  text->setZ(1000);
  text->setColor(TextEntity::COLOR_FADING_WHITE);
}

void WitchBlastGame::updateRunningGame()
{
  bool backToMenu = false;
  // Process events
  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
      if (gameState == gameStatePlaying && !player->isDead() && currentMap->isCleared()) saveGame();
      app->close();
    }

    if (event.type == sf::Event::MouseWheelMoved)
    {
      if (gameState == gameStatePlaying && !isPausing) player->selectNextShotType();
    }

    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
      {
        if (player->isDead()) backToMenu = true;
        else if (gameState == gameStatePlaying && !isPausing) isPausing = true;
        else if (gameState == gameStatePlaying && isPausing) isPausing = false;
      }

      if (event.key.code == input[KeyFireSelect])
      {
        if (gameState == gameStatePlaying && !isPausing) player->selectNextShotType();
      }

      if (event.key.code == input[KeyFire])
      {
        if (gameState == gameStatePlaying && !isPausing) firingDirection = player->getFacingDirection();
      }

      if (event.key.code == sf::Keyboard::X)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) startNewGame(false);
      }

      if (event.key.code == sf::Keyboard::F2)
      {
        showLogical = !showLogical;
      }
    }

    if (event.type == sf::Event::LostFocus && !player->isDead())
      isPausing = true;
  }

  if (gameState == gameStatePlaying && !isPausing)
  {
    if (player->canMove()) player->setVelocity(Vector2D(0.0f, 0.0f));

    if (sf::Keyboard::isKeyPressed(input[KeyLeft]))
    {
      if (sf::Keyboard::isKeyPressed(input[KeyUp]))
        player->move(7);
      else if (sf::Keyboard::isKeyPressed(input[KeyDown]))
        player->move(1);
      else
        player->move(4);
    }
    else if (sf::Keyboard::isKeyPressed(input[KeyRight]))
    {
      if (sf::Keyboard::isKeyPressed(input[KeyUp]))
        player->move(9);
      else if (sf::Keyboard::isKeyPressed(input[KeyDown]))
        player->move(3);
      else
        player->move(6);
    }
    else if (sf::Keyboard::isKeyPressed(input[KeyUp]))
    {
      player->move(8);
    }
    else if (sf::Keyboard::isKeyPressed(input[KeyDown]))
    {
      player->move(2);
    }

    player->resestFireDirection();

    if (sf::Keyboard::isKeyPressed(input[KeyFireLeft]))
      player->fire(4);
    else if (sf::Keyboard::isKeyPressed(input[KeyFireRight]))
      player->fire(6);
    else if (sf::Keyboard::isKeyPressed(input[KeyFireUp]))
      player->fire(8);
    else if (sf::Keyboard::isKeyPressed(input[KeyFireDown]))
      player->fire(2);
    // alternative "one button" gameplay
    else if (sf::Keyboard::isKeyPressed(input[KeyFire]))
    {
      player->fire(firingDirection);
    }
    // alternative "firing with the mouse" gameplay
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(*app);
      int xm = mousePosition.x - player->getX();
      int ym = mousePosition.y - player->getY();

      if (abs(xm) >= abs(ym))
      {
        if (xm > 0) player->fire(6);
        else player->fire(4);
      }
      else
      {
        if (ym > 0) player->fire(2);
        else player->fire(8);
      }
    }

    if (player->isDead() && xGameState == xGameStateNone && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
      xGameState = xGameStateFadeOut;
      xGameTimer = FADE_OUT_DELAY;
    }
  }

  onUpdate();

  verifyDoorUnlocking();
  if (roomClosed)
  {
    if (getEnnemyCount() == 0)
    {
      currentMap->setCleared(true);
      openDoors();
      if (currentMap->getRoomType() == roomTypeBoss)
        playMusic(MusicDungeon);
    }
  }

  if (backToMenu) switchToMenu();
}

void WitchBlastGame::renderRunningGame()
{
  EntityManager::getEntityManager()->sortByZ();
  if (xGameState == xGameStateShake)
  {
    sf::View view = app->getDefaultView();
    sf::View viewSave = app->getDefaultView();
    view.move(-4 + rand() % 9, -4 + rand() % 9);
    app->setView(view);

    EntityManager::getEntityManager()->renderUnder(app, 5000);

    app->setView(viewSave);
    EntityManager::getEntityManager()->renderAfter(app, 5000);
  }
  else
  {
    // render the game objects
    EntityManager::getEntityManager()->render(app);
  }

  myText.setColor(sf::Color(255, 255, 255, 255));

  myText.setCharacterSize(18);
  std::ostringstream oss;
  oss << player->getGold();
  myText.setString(oss.str());
  myText.setPosition(690, 612);
  app->draw(myText);

  myText.setColor(sf::Color(0, 0, 0, 255));
  myText.setCharacterSize(16);

  oss.str("");
  oss << "Level " << level;
  myText.setString(oss.str());
  myText.setPosition(410, 692);
  app->draw(myText);

  sf::RectangleShape rectangle(sf::Vector2f(200, 25));

  if (gameState == gameStatePlaying)
  {
    // life
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setPosition(sf::Vector2f(90, 622));
    rectangle.setSize(sf::Vector2f(200.0f * (float)(player->getHpDisplay()) / (float)(player->getHpMax()) , 25));
    app->draw(rectangle);

    rectangle.setFillColor(sf::Color(255, 190, 190));
    rectangle.setPosition(sf::Vector2f(90, 625));
    rectangle.setSize(sf::Vector2f(200.0f * (float)(player->getHpDisplay()) / (float)(player->getHpMax()) , 2));
    app->draw(rectangle);

    oss.str("");
    oss << player->getHp() << "/" << player->getHpMax();
    myText.setString(oss.str());
    myText.setPosition(95, 624);
    myText.setColor(sf::Color(255, 255,255, 255));
    app->draw(myText);

    // mana
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

    // render the shots
    renderHudShots(app);

    if (isPausing)
    {
      rectangle.setFillColor(sf::Color(0, 0, 0, 160));
      rectangle.setPosition(sf::Vector2f(OFFSET_X, OFFSET_Y));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH, MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);

      float x0 = OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;
      int fade = 50 + 205 * (1.0f + cos(3.0f * getAbsolutTime())) * 0.5f;
      myText.setColor(sf::Color(255, 255, 255, fade));
      myText.setCharacterSize(40);
      myText.setString("PAUSE");
      myText.setPosition(x0 - myText.getLocalBounds().width / 2, 300);
      app->draw(myText);
    }

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
      myText.setString("Press [ENTER] to play again / [ESC] to go back to the menu");
      myText.setPosition(x0 - myText.getLocalBounds().width / 2, 440);
      app->draw(myText);
    }
    else if (currentMap->getRoomType() == roomTypeExit && level >= LAST_LEVEL)
    {
      float x0 = OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;
      myText.setColor(sf::Color(255, 255, 255, 255));
      myText.setCharacterSize(25);
      myText.setString("CONGRATULATIONS !\nYou've challenged this demo and\nmanaged to kill the bosses !\nSee you soon for new adventures !");
      myText.setPosition(x0 - myText.getLocalBounds().width / 2, 220);
      app->draw(myText);
    }

    if (sf::Keyboard::isKeyPressed(input[KeyTimeControl]))
    {
      // effect
      //int effectFade = 10 + rand() % 20;
      int effectFade = 10 + 20 * (1.0f + cos(12.0f * getAbsolutTime())) * 0.5f;
      rectangle.setFillColor(sf::Color(0, 255, 255, effectFade));
      rectangle.setPosition(sf::Vector2f(OFFSET_X, OFFSET_Y));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      sf::RenderStates r;
      r.blendMode = sf::BlendAlpha ;
      app->draw(rectangle, r);
    }

    if (xGameState == xGameStateFadeIn)
    {
      // fade in
      rectangle.setFillColor(sf::Color(0, 0, 0, 255 - ((FADE_IN_DELAY - xGameTimer) / FADE_IN_DELAY) * 255));
      rectangle.setPosition(sf::Vector2f(OFFSET_X, OFFSET_Y));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);
    }
    else if (xGameState == xGameStateFadeOut)
    {
      // fade out
      rectangle.setFillColor(sf::Color(0, 0, 0, ((FADE_IN_DELAY - xGameTimer) / FADE_IN_DELAY) * 255));
      rectangle.setPosition(sf::Vector2f(OFFSET_X, OFFSET_Y));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);
    }
  }
}

void WitchBlastGame::switchToMenu()
{
  EntityManager::getEntityManager()->clean();

  if (menuMap != NULL) delete menuMap;
  menuMap = new GameMap(MENU_MAP_WIDTH, MENU_MAP_HEIGHT);
  for (int i = 0; i < MENU_MAP_WIDTH; i++)
    for (int j = 0; j < MENU_MAP_HEIGHT; j++)
    {
      if (rand() % 6 == 0)
        menuMap->setTile(i, j, rand() %7 + 1);
    }
  menuTileMap = new TileMapEntity(ImageManager::getImageManager()->getImage(IMAGE_TILES), menuMap, 64, 64, 10);
  menuTileMap->setX(-30.0f);
  menuTileMap->setY(-20.0f);

  gameState = gameStateMenu;
  buildMenu();
  if (!config.configFileExists())
  {
    menu.redefineKey = true;
    menu.keyIndex = 0;
  }
  playMusic(MusicIntro);
}

void WitchBlastGame::updateMenu()
{
  EntityManager::getEntityManager()->animate(deltaTime);
  menu.age += deltaTime;
  float mapY = menuTileMap->getY();
  mapY -= 30.0f * deltaTime;
  if (mapY < -64.0f)
  {
    mapY += 64.0f;
    for (int i = 0; i < MENU_MAP_WIDTH; i++)
    {
      for (int j = 0; j < MENU_MAP_HEIGHT - 1; j++)
      {
         menuMap->setTile(i, j, menuMap->getTile(i, j+1));
      }
      if (rand() % 6 == 0)
        menuMap->setTile(i, MENU_MAP_HEIGHT - 1, rand() %7 + 1);
      else
        menuMap->setTile(i, MENU_MAP_HEIGHT - 1, 0);
    }
  }
  menuTileMap->setY(mapY);

  // Process events
  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
       app->close();
    }

    if (event.type == sf::Event::KeyPressed && menu.redefineKey)
    {
      bool alreadyUsed = false;
      if (event.key.code == sf::Keyboard::Escape) alreadyUsed = true;
      for (unsigned int i = 0; i < menu.keyIndex; i++)
        if (input[i] == event.key.code) alreadyUsed = true;

      // TODO more tests
      if (!alreadyUsed)
      {
        input[menu.keyIndex] = event.key.code;
        menu.keyIndex++;
        if (menu.keyIndex == NumberKeys)
        {
          menu.redefineKey = false;
          saveConfigurationToFile();
        }
      }
    }
    else if (event.type == sf::Event::KeyPressed && !menu.redefineKey)
    {
      if (event.key.code == sf::Keyboard::Escape)
      {
        app->close();
      }
      else if (event.key.code == input[KeyDown] || event.key.code == sf::Keyboard::Down)
      {
        menu.index++;
        if (menu.index == menu.items.size()) menu.index = 0;
        SoundManager::getSoundManager()->playSound(SOUND_SHOT_SELECT);
      }

      else if (event.key.code == input[KeyUp] || event.key.code == sf::Keyboard::Up)
      {
        if (menu.index == 0) menu.index = menu.items.size() - 1;
        else menu.index--;

        SoundManager::getSoundManager()->playSound(SOUND_SHOT_SELECT);
      }
      else if (event.key.code == sf::Keyboard::Return)
      {
       switch (menu.items[menu.index].id)
       {
         case MenuStartNew: startNewGame(false); remove(SAVE_FILE.c_str()); break;
         case MenuStartOld: startNewGame(true); break;
         case MenuKeys: menu.redefineKey = true; menu.keyIndex = 0; break;
         case MenuExit: app->close(); break;
       }
      }
    }
  }
}

void WitchBlastGame::renderMenu()
{
  // rendering tiles
  EntityManager::getEntityManager()->render(app);

  // title
  write("Witch Blast", 70, 485, 120, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 3, 3);

  if (menu.redefineKey)
  {
    // menu background
    sf::RectangleShape rectangle(sf::Vector2f(470 , 400));
    rectangle.setFillColor(sf::Color(50, 50, 50, 160));
    rectangle.setPosition(sf::Vector2f(250, 240));
    app->draw(rectangle);

    // menu keys
    if (config.configFileExists())
      write("Key configuration", 18, 300, 250, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
    else
      write("Please configure the keys", 18, 300, 250, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      sf::Color itemColor;
      if (menu.keyIndex == i) itemColor = sf::Color(255, 255, 255, 255);
      else itemColor = sf::Color(180, 180, 180, 255);
      std::ostringstream oss;
      oss << inputKeyString[i] << ": ";
      if (menu.keyIndex == i) oss << "[insert Key]";
      else if (menu.keyIndex > i) oss << "DONE";
      write(oss.str(), 16, 300, 285 + i * 32, ALIGN_LEFT, itemColor, app, 1, 1);
    }
  }
  else
  {
    // menu background
    sf::RectangleShape rectangle(sf::Vector2f(470 , 390));
    rectangle.setFillColor(sf::Color(50, 50, 50, 160));
    rectangle.setPosition(sf::Vector2f(250, 240));
    if (menu.items.size() == 3) rectangle.setSize(sf::Vector2f(470 , 310));
    app->draw(rectangle);

    // menu
    for (unsigned int i = 0; i < menu.items.size(); i++)
    {
      sf::Color itemColor;
      if (menu.index == i) itemColor = sf::Color(255, 255, 255, 255);
      else itemColor = sf::Color(180, 180, 180, 255);
      write(menu.items[i].label, 24, 300, 260 + i * 90, ALIGN_LEFT, itemColor, app, 1, 1);
      write(menu.items[i].description, 15, 300, 260 + i * 90 + 40, ALIGN_LEFT, itemColor, app, 0, 0);
    }
  }

  std::ostringstream oss;
  oss << APP_NAME << " v" << APP_VERSION << " by Seby 2014";
  write(oss.str(), 17, 5, 680, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
}

void WitchBlastGame::startGame()
{
  lastTime = getAbsolutTime();
  switchToMenu();

  // Start game loop
  while (app->isOpen())
  {
    deltaTime = getAbsolutTime() - lastTime;
    lastTime = getAbsolutTime();
    if (deltaTime > 0.05f) deltaTime = 0.05f;
    switch (gameState)
    {
      case gameStateInit:
      case gameStateKeyConfig:
      case gameStateMenu: updateMenu(); break;
      case gameStatePlaying: updateRunningGame(); break;
    }
    onRender();
  }
  quitGame();
}

void WitchBlastGame::createFloor()
{
  // TODO : extracts from createNewGame
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
  }
}

void WitchBlastGame::openDoors()
{
    int i, j;
    for(i = 0; i < MAP_WIDTH; i++)
      for(j = 0; j < MAP_WIDTH; j++)
        if (currentMap->getTile(i, j) == MAP_DOOR) currentMap->setTile(i, j, MAP_DOOR_OPEN);
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

int WitchBlastGame::getEnnemyCount()
{
  int n=0;

  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

		if (e->getType() >= ENTITY_ENNEMY && e->getType() <= ENTITY_ENNEMY_MAX) n++;
	}

	return n;
}

Vector2D WitchBlastGame::getNearestEnnemy(float x, float y)
{
  Vector2D target(-100.0f, -100.0f);
  float distanceMin = -1.0f;

  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

		if (e->getType() >= ENTITY_ENNEMY && e->getType() <= ENTITY_ENNEMY_MAX)
		{
		  // enemy
		  EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(e);
		  if (enemy->canCollide())
      {
        float d2 = (x - enemy->getX()) * (x - enemy->getX()) + (y - enemy->getY()) * (y - enemy->getY());

        if (target.x < -1.0f || d2 < distanceMin)
        {
          distanceMin = d2;
          target.x = enemy->getX();
          target.y = enemy->getY();
        }
      }
		}
	}

  return target;
}

void WitchBlastGame::checkDoor(int doorId, roomTypeEnum roomCurrent, roomTypeEnum roomNeighbour)
{
  if (roomNeighbour == roomTypeNULL)
  {
    doorEntity[doorId]->setVisible(false);
    return;
  }
  doorEntity[doorId]->setVisible(true);
  doorEntity[doorId]->setDoorType(DoorStandard);
  if (roomNeighbour == roomTypeBoss || roomCurrent == roomTypeBoss)
    doorEntity[doorId]->setDoorType(DoorBoss);
  if (roomNeighbour == roomTypeChallenge || roomCurrent == roomTypeChallenge)
    doorEntity[doorId]->setDoorType(DoorChallenge);
  if (roomNeighbour == roomTypeBoss && !bossRoomOpened)
  {
    doorEntity[doorId]->setOpen(false);
    switch (doorId)
    {
      case 0: currentMap->setTile(MAP_WIDTH/2, 0, MAP_DOOR); break;
      case 1: currentMap->setTile(0, MAP_HEIGHT / 2, MAP_DOOR); break;
      case 2: currentMap->setTile(MAP_WIDTH/2, MAP_HEIGHT - 1, MAP_DOOR); break;
      case 3: currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, MAP_DOOR); break;
    }

  }
  else
    doorEntity[doorId]->setOpen(true);
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

  // check doors
  checkDoor(0, currentMap->getRoomType(), currentMap->getNeighbourUp());
  checkDoor(1, currentMap->getRoomType(), currentMap->getNeighbourLeft());
  checkDoor(2, currentMap->getRoomType(), currentMap->getNeighbourDown());
  checkDoor(3, currentMap->getRoomType(), currentMap->getNeighbourRight());
}

void WitchBlastGame::refreshMinimap()
{
  for (int j=0; j < FLOOR_HEIGHT; j++)
    for (int i=0; i < FLOOR_WIDTH; i++)
    {
      int n = currentFloor->getRoom(i, j);

      if (n > 0 && currentFloor->getMap(i, j)->isVisited())
      {
        if (currentFloor->getRoom(i, j) == roomTypeStarting
            || currentFloor->getRoom(i, j) == roomTypeChallenge
            || currentFloor->getRoom(i, j) == roomTypeBonus
            || currentFloor->getRoom(i, j) == roomTypeKey
            || currentFloor->getRoom(i, j) == roomTypeStandard)
            {
              if ( currentFloor->getMap(i, j)->containsHealth())
                miniMap->setTile(i, j, 5);
              else
                miniMap->setTile(i, j, roomTypeStandard);
            }

        else
          miniMap->setTile(i, j, currentFloor->getRoom(i, j));
      }
      else if (n > 0 && currentFloor->getMap(i, j)->isKnown())
      {
        if (currentFloor->getRoom(i, j) == roomTypeBoss)
          miniMap->setTile(i, j, 7);
        else if (currentFloor->getRoom(i, j) == roomTypeChallenge)
          miniMap->setTile(i, j, 10);
        else
          miniMap->setTile(i, j, 9);
      }
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
  EntityManager::EntityList* entityList = EntityManager::getEntityManager()->getList();
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
      if (spriteEntity != NULL && (e->getType() == ENTITY_BLOOD || e->getType() == ENTITY_CORPSE ) )
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
  // stairs to next level
  if (direction == 8 && currentMap->getRoomType() == roomTypeExit)
  {
    if (player->getPlayerStatus() != PlayerEntity::playerStatusGoingUp)
    {
      player->setLeavingLevel();
      xGameState = xGameStateFadeOut;
      xGameTimer = FADE_OUT_DELAY;
      player->setVelocity(Vector2D(0.0f, - INITIAL_PLAYER_SPEED / 2));
    }
  }
  // go to another room
  else
  {
    saveMapItems();
    switch (direction)
    {
      case (4): floorX--;  player->moveTo((OFFSET_X + MAP_WIDTH * TILE_WIDTH), player->getY()); player->move(4);  break;
      case (6): floorX++;  player->moveTo(OFFSET_X, player->getY()); player->move(6); break;
      case (8): floorY--;  player->moveTo(player->getX(), OFFSET_Y + MAP_HEIGHT * TILE_HEIGHT/* - 20*/); player->move(8); break;
      case (2): floorY++;  player->moveTo(player->getX(), OFFSET_Y);  break;
    }
    refreshMap();
    checkEntering();
    currentMap->restoreMapObjects();
  }
}

void WitchBlastGame::onRender()
{
    // clear the view
    app->clear(sf::Color(32, 32, 32));

    switch (gameState)
    {
      case gameStateInit:
      case gameStateKeyConfig:
      case gameStateMenu: renderMenu(); break;
      case gameStatePlaying: renderRunningGame(); break;
    }

    app->display();
}

void WitchBlastGame::renderHudShots(sf::RenderTarget* app)
{
  int xHud = 640;
  int yHud = 650;
  int index = 0;

  for (int i = 0; i < SPECIAL_SHOT_SLOTS; i++)
  {
    if (i == 0 || player->getShotType(i) != ShotTypeStandard)
    {
      int type_shot = player->getShotType(i);
      shotsSprite.setPosition(xHud + 48 * index, yHud);
      if (index == player->getShotIndex())
      {
        shotsSprite.setTextureRect(sf::IntRect(0, 0,  48, 48));
        app->draw(shotsSprite);
      }
      shotsSprite.setTextureRect(sf::IntRect(48 * ( 1 + type_shot), 0,  48, 48));
      app->draw(shotsSprite);

      // level
      if (i > 0)
      {
        std::ostringstream oss;
        oss << "lvl " << player->getShotLevel(i) + 1;
        write(oss.str(), 10, xHud + 48 * index + 10, yHud + 48, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0);
      }

      index++;
    }
  }
}

void WitchBlastGame::generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor)
{
  // double blood if the "Blood Snake3 object is equipped
  int nbIt;
  if (player->isEquiped(EQUIP_BLOOD_SNAKE))
    nbIt = 2;
  else
    nbIt = 1;

  for (int i=0; i < nbIt; i++)
  {
    SpriteEntity* blood = new SpriteEntity(ImageManager::getImageManager()->getImage(IMAGE_BLOOD), x, y, 16, 16, 6);
    blood->setZ(OFFSET_Y - 1);
    int b0 = 0;
    if (bloodColor == BaseCreatureEntity::BloodGreen) b0 += 6;
    blood->setFrame(b0 + rand()%6);
    blood->setType(ENTITY_BLOOD);
    blood->setVelocity(Vector2D(rand()%250));
    blood->setViscosity(0.95f);

    float bloodScale = 1.0f + (rand() % 10) * 0.1f;
    blood->setScale(bloodScale, bloodScale);
  }
}

void WitchBlastGame::showArtefactDescription(enumItemType itemType)
{
  new ArtefactDescriptionEntity(itemType);
}

void WitchBlastGame::generateMap()
{
  if (currentMap->getRoomType() == roomTypeStandard)
    generateStandardMap();
  else if (currentMap->getRoomType() == roomTypeBonus)
  {
    currentMap->setCleared(true);
    Vector2D v = currentMap->generateBonusRoom();
    int bonusType = getRandomEquipItem(false, false);
    //if (bonusType == EQUIP_FAIRY)
    if (items[FirstEquipItem + bonusType].familiar > FamiliarNone)
    {
      new ChestEntity(v.x, v.y, CHEST_FAIRY + items[FirstEquipItem + bonusType].familiar, false);
    }
    else
    {
      new ItemEntity( (enumItemType)(FirstEquipItem + bonusType), v.x ,v.y);
    }
  }
  else if (currentMap->getRoomType() == roomTypeKey)
  {
    Vector2D v = currentMap->generateKeyRoom();
    new ItemEntity( (enumItemType)(ItemBossKey), v.x ,v.y);
    initMonsterArray();
    int x0 = MAP_WIDTH / 2;
    int y0 = MAP_HEIGHT / 2;
    monsterArray[x0][y0] = true;
    if (level == 1)
    {
      findPlaceMonsters(EnemyTypeRat, 2);
      findPlaceMonsters(EnemyTypeBat, 2);
    }
    else
    {
      findPlaceMonsters(EnemyTypeRat, 5);
      findPlaceMonsters(EnemyTypeBat, 5);
      for (int i = 2; i < level; i++)
      {
        if (rand()%2 == 0)findPlaceMonsters(EnemyTypeImpBlue, 1);
        else findPlaceMonsters(EnemyTypeImpRed, 1);
      }
    }
  }
  else if (currentMap->getRoomType() == roomTypeMerchant)
  {
    currentMap->generateMerchantRoom();

    ItemEntity* item1 = new ItemEntity(
      ItemHealth,
      OFFSET_X + (MAP_WIDTH / 2 - 3) * TILE_WIDTH + TILE_WIDTH / 2,
      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item1->setMerchandise(true);

    ItemEntity* item3 = new ItemEntity(
      ItemHealthSmall,
      OFFSET_X + (MAP_WIDTH / 2 + 3) * TILE_WIDTH + TILE_WIDTH / 2,
      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item3->setMerchandise(true);

    int bonusType = getRandomEquipItem(true, true);
    ItemEntity* item2 = new ItemEntity(
      (enumItemType)(FirstEquipItem + bonusType),
      OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item2->setMerchandise(true);

    new PnjEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                  OFFSET_Y + (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT,
                  0);

    currentMap->setCleared(true);
  }
  else if (currentMap->getRoomType() == roomTypeChallenge)
  {
    currentMap->generateRoomWithoutHoles(0);

    if (level < 3)
    {
      new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                        OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 0);
    }
    else
    {
      new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 80,
                        OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 0);
      new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 80,
                        OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 0);
    }
  }
  else if (currentMap->getRoomType() == roomTypeBoss)
  {
    currentMap->generateRoomWithoutHoles(0);

    if (level == 1)
      new ButcherEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    else if (level == 2)
      new GiantSlimeEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    else if (level == 3)
      new KingRatEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    else if (level == 4)
      new CyclopsEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    else //if (level == 5)
      new GiantSpiderEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                      OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);

    playMusic(MusicBoss);
  }
  else if (currentMap->getRoomType() == roomTypeStarting)
  {
    currentMap->generateRoomWithoutHoles(0);
    currentMap->setCleared(true);
  }
  else if (currentMap->getRoomType() == roomTypeExit)
  {
    currentMap->generateExitRoom();
    currentMap->setCleared(true);

    new ChestEntity(OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                    OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT * 0.5f),
                    CHEST_EXIT, false);
  }
  else
    currentMap->randomize(currentMap->getRoomType());
}

void WitchBlastGame::write(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow = 0, int yShadow = 0)
{
  myText.setString(str);
  myText.setCharacterSize(size);

  float xFont = x;

  if (align == ALIGN_CENTER)
    xFont = x - myText.getLocalBounds().width / 2;

  if (xShadow != 0 && yShadow != 0)
  {
    myText.setPosition(xFont + xShadow, y + yShadow);
    myText.setColor(sf::Color(0, 0, 0, 255));
    app->draw(myText);
  }

  myText.setPosition(xFont, y);
  myText.setColor(color);
  app->draw(myText);
}

void WitchBlastGame::initMonsterArray()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
      monsterArray[i][j] = false;
}

void WitchBlastGame::addMonster(enemyTypeEnum monsterType, float xm, float ym)
{
  switch (monsterType)
  {
    case EnemyTypeRat: new RatEntity(xm, ym - 2, RatEntity::RatTypeNormal, false); break;
    case EnemyTypeRatBlack: new BlackRatEntity(xm, ym - 5, BlackRatEntity::RatBlackTypeNormal); break;
    case EnemyTypeRatHelmet: new RatEntity(xm, ym - 2, RatEntity::RatTypeHelmet, false); break;
    case EnemyTypeRatBlackHelmet: new BlackRatEntity(xm, ym - 5, BlackRatEntity::RatBlackTypeHelmet); break;
    case EnemyTypeBat: new BatEntity(xm, ym, false); break;
    case EnemyTypeEvilFlower: new EvilFlowerEntity(xm, ym - 2); break;
    case EnemyTypeSlime: new SlimeEntity(xm, ym, SlimeTypeStandard, false); break;
    case EnemyTypeImpRed: new ImpEntity(xm, ym, ImpEntity::ImpTypeRed); break;
    case EnemyTypeImpBlue: new ImpEntity(xm, ym, ImpEntity::ImpTypeBlue); break;
    case EnemyTypeSlimeRed: new SlimeEntity(xm, ym, SlimeTypeRed, false); break;
    case EnemyTypeSlimeBlue: new SlimeEntity(xm, ym, SlimeTypeBlue, false); break;
    case EnemyTypeSlimeViolet: new SlimeEntity(xm, ym, SlimeTypeViolet, false); break;
    case EnemyTypeWitch: new WitchEntity(xm, ym, WitchEntity::WitchTypeNormal); break;
    case EnemyTypeWitchRed: new WitchEntity(xm, ym, WitchEntity::WitchTypeRed); break;
    case EnemyTypeCauldron: new CauldronEntity(xm, ym); break;

    default: std::cout << "[WARNING] Enemy (" << monsterType << ") not handled in switch.\n";
  }
}

void WitchBlastGame::findPlaceMonsters(enemyTypeEnum monsterType, int amount)
{
  // find a suitable place
  bool isMonsterFlying = monsterType == EnemyTypeBat
      || monsterType == EnemyTypeImpBlue
      || monsterType == EnemyTypeImpRed;

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
  generateStandardRoom(level);
}

item_equip_enum WitchBlastGame::getRandomEquipItem(bool toSale = false, bool noFairy = false)
{
  std::vector<int> bonusSet;
  int setSize = 0;
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    bool itemOk = true;
    int eq = i + FirstEquipItem;

    if (player->isEquiped(i)) itemOk = false;
    // TODO item already in floor
    if (itemOk && toSale && !items[eq].canBeSold) itemOk = false;
    if (itemOk && !toSale && !items[eq].canBeFound) itemOk = false;
    if (itemOk && items[eq].level > level) itemOk = false;
    if (itemOk && items[eq].requirement >= FirstEquipItem
        && !player->isEquiped(items[eq].requirement - FirstEquipItem)) itemOk = false;

    if (itemOk && (items[eq].specialShot != ShotTypeStandard && items[eq].level < 3) && player->canGetNewShot(false))
      itemOk = false;

    if (itemOk && (items[eq].specialShot != ShotTypeStandard && items[eq].level >= 3) && player->canGetNewShot(true))
      itemOk = false;


    if (itemOk && noFairy && items[eq].familiar != FamiliarNone) itemOk = false;

    if (itemOk)
    {
      int n = 0;
      switch (items[eq].rarity)
      {
        case RarityCommon:  n = 4; break;
        case RarityUncommon: n = 2; break;
        case RarityRare:    n = 1; break;
      }
      for (int j = 0; j < n; j++)
      {
        bonusSet.push_back(i);
        setSize++;
      }
    }
  }
  int bonusType = 0;
  if (setSize > 0) bonusType = bonusSet[rand() % setSize];

  return (item_equip_enum) bonusType;
}

void WitchBlastGame::verifyDoorUnlocking()
{
  int collidingDirection = (player->getCollidingDirection());

  if (collidingDirection > 0 && currentMap->isCleared() && !bossRoomOpened && player->isEquiped(EQUIP_BOSS_KEY))
  {
    int xt = (player->getX() - OFFSET_X) / TILE_WIDTH;
    int yt = (player->getY() - OFFSET_Y) / TILE_HEIGHT;

    if (yt <= 1 && xt >= MAP_WIDTH / 2 - 1 && xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourUp() == 2)
    {
      doorEntity[0]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, 0, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (yt >= MAP_HEIGHT - 2 && xt >= MAP_WIDTH / 2 - 1 &&xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourDown() == 2)
    {
      doorEntity[2]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, MAP_HEIGHT - 1, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt <= 1 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourLeft() == 2)
    {

      doorEntity[1]->openDoor();
      currentMap->setTile(0, MAP_HEIGHT / 2, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt >= MAP_WIDTH - 2 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourRight() == 2)
    {
      doorEntity[3]->openDoor();
      currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, 0);
      SoundManager::getSoundManager()->playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
  }
}

void WitchBlastGame::playMusic(musicEnum musicChoice)
{
  music.stop();
  music.setLoop(true);
  bool ok = false;

  switch (musicChoice)
  {
  case MusicDungeon:
    ok = music.openFromFile("media/sound/track00.ogg");
    music.setVolume(75);
    break;

  case MusicEnding:
    ok = music.openFromFile("media/sound/track_ending.ogg");
    music.setVolume(35);
    break;

  case MusicBoss:
    ok = music.openFromFile("media/sound/track_boss.ogg");
    music.setVolume(90);
    break;

  case MusicIntro:
    ok = music.openFromFile("media/sound/track_intro.ogg");
    music.setVolume(90);
    break;
  }

  if (ok)
    music.play();
}

void WitchBlastGame::makeShake(float duration)
{
  xGameState = xGameStateShake;
  xGameTimer = duration;
}

void WitchBlastGame::saveGame()
{
  if (player->getPlayerStatus() == PlayerEntity::playerStatusAcquire)
    player->acquireItemAfterStance();
  ofstream file(SAVE_FILE.c_str(), ios::out | ios::trunc);

  int i, j, k, l;

  if (file)
  {
    // version (for compatibility check)
    file << SAVE_VERSION << std::endl;

    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    file << (now->tm_year + 1900) << '-';

    if (now->tm_mon < 9) file << "0";

    file << (now->tm_mon + 1) << '-';

    if (now->tm_mday < 9) file << "0";

    file <<  now->tm_mday
         << endl;

    if (now->tm_hour <= 9) file << "0";

    file << (now->tm_hour) << ':';

    if (now->tm_min <= 9) file << "0";
    file << (now->tm_min) << endl;

    // floor
    file << level << std::endl;

    int nbRooms = 0;
    for (j = 0; j < FLOOR_HEIGHT; j++)
    {
      for (i = 0; i < FLOOR_WIDTH; i++)
      {
        file << currentFloor->getRoom(i,j) << " ";
        if (currentFloor->getRoom(i,j) > 0) nbRooms++;
      }
      file << std::endl;
    }
    // kill stats
    for (i = 0; i < NB_ENEMY; i++) file << killedEnemies[i] << " ";
    file << std::endl;

    // maps
    saveMapItems();

    file << nbRooms << std::endl;
    for (j = 0; j < FLOOR_HEIGHT; j++)
    {
      for (i = 0; i < FLOOR_WIDTH; i++)
      {
        if (currentFloor->getRoom(i,j) > 0)
        {
          file << i << " " << j << " "
          << currentFloor->getMap(i, j)->getRoomType() << " "
          << currentFloor->getMap(i, j)->isKnown() << " "
          << currentFloor->getMap(i, j)->isVisited() << " "
          << currentFloor->getMap(i, j)->isCleared() << std::endl;
          if (currentFloor->getMap(i, j)->isVisited())
          {
            for (l = 0; l < MAP_HEIGHT; l++)
            {
              for (k = 0; k < MAP_WIDTH; k++)
              {
                file << currentFloor->getMap(i, j)->getTile(k, l) << " ";
              }
              file << std::endl;
            }
            // items, etc...
            std::list<DungeonMap::itemListElement> itemList = currentFloor->getMap(i, j)->getItemList();
            file << itemList.size() << std::endl;
            std::list<DungeonMap::itemListElement>::iterator it;
            for (it = itemList.begin (); it != itemList.end ();)
            {
              DungeonMap::itemListElement ilm = *it;
              it++;

              file << ilm.type << " " << ilm.x << " " << ilm.y << " " << ilm.merch << std::endl;
            }

            // chests
            std::list<DungeonMap::chestListElement> chestList = currentFloor->getMap(i, j)->getChestList();
            file << chestList.size() << std::endl;
            std::list<DungeonMap::chestListElement>::iterator itc;
            for (itc = chestList.begin (); itc != chestList.end ();)
            {
              DungeonMap::chestListElement ilm = *itc;
              itc++;

              file << ilm.type << " " << ilm.x << " " << ilm.y << " " << ilm.state << std::endl;
            }

            // sprites
            std::list<DungeonMap::spriteListElement> spriteList = currentFloor->getMap(i, j)->getSpriteList();
            file << spriteList.size() << std::endl;
            std::list<DungeonMap::spriteListElement>::iterator its;
            for (its = spriteList.begin (); its != spriteList.end ();)
            {
              DungeonMap::spriteListElement ilm = *its;
              its++;

              file << ilm.type << " " << ilm.frame << " " << ilm.x << " " << ilm.y << " " << ilm.scale << std::endl;
            }
          }
        }
      }
      file << std::endl;
    }

    // game
    file << floorX << " " << floorY << std::endl;
    file << bossRoomOpened << std::endl;
    // boss door !

    // player
    file << player->getHp() << " " << player->getHpMax() << " " << player->getGold() << std::endl;
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++) file << player->isEquiped(i) << " ";
    file << std::endl;
    file << player->getX() << " " << player->getY() << std::endl;
    file << player->getShotIndex();
    for (i = 0; i < SPECIAL_SHOT_SLOTS; i++) file << " " << player->getShotType(i);

    file.close();
  }
  else
  {
    cerr << "[ERROR] Saving the game..." << endl;
  }
}

bool WitchBlastGame::loadGame()
{
  ifstream file(SAVE_FILE.c_str(), ios::in);

  if (file)
  {
    int i, j, k, n;

    // version
    std::string v;
    file >> v;

    if (v != SAVE_VERSION)
    {
      file.close();
      remove(SAVE_FILE.c_str());
      return false;
    }

    // date an time
    file >> v;
    file >> v;

    // floor
    file >> level;
    currentFloor = new GameFloor(level);
    for (j = 0; j < FLOOR_HEIGHT; j++)
    {
      for (i = 0; i < FLOOR_WIDTH; i++)
      {
        int n;
        file >> n;
        currentFloor->setRoom(i, j, (roomTypeEnum)n);
      }
    }

    // kill stats
    for (int i = 0; i < NB_ENEMY; i++) file >> killedEnemies[i];

    // maps
    int nbRooms;
    file >> nbRooms;

    for (k = 0; k < nbRooms; k++)
    {
      file >> i;
      file >> j;
      file >> n;
      DungeonMap* iMap = new DungeonMap(currentFloor, i, j);
      currentFloor->setMap(i, j, iMap);
      iMap->setRoomType((roomTypeEnum)n);
      bool flag;
      file >> flag;
      iMap->setKnown(flag);
      file >> flag;
      iMap->setVisited(flag);
      file >> flag;
      iMap->setCleared(flag);

      if (iMap->isVisited())
      {
        for (j = 0; j < MAP_HEIGHT; j++)
        {
          for (i = 0; i < MAP_WIDTH; i++)
          {
            file >> n;
            iMap->setTile(i, j, n);
          }
        }
        // items int the map
        file >> n;
        for (i = 0; i < n; i++)
        {
          int t;
          float x, y;
          bool merc;
          file >> t >> x >> y >> merc;
          iMap->addItem(t, x, y, merc);
        }
        // chests in the map
        file >> n;
        for (i = 0; i < n; i++)
        {
          int t;
          float x, y;
          bool state;
          file >> t >> x >> y >> state;
          iMap->addChest(t, state, x, y);
        }
        // sprites in the map
        file >> n;
        for (i = 0; i < n; i++)
        {
          int t, f;
          float x, y, scale;
          file >> t >> f >> x >> y >> scale;
          iMap->addSprite(t, f, x, y, scale);
        }
      }
    }

    // game
    file >> floorX >> floorY;
    currentMap = currentFloor->getMap(floorX, floorY);
    file >> bossRoomOpened;

    // player
    int hp, hpMax, gold;
    file >> hp >> hpMax >> gold;
    player = new PlayerEntity(OFFSET_X + (TILE_WIDTH * MAP_WIDTH * 0.5f),
                              OFFSET_Y + (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
    player->setHp(hp);
    player->setHpMax(hpMax);
    player->setGold(gold);
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    {
      bool eq;
      file >> eq;
      player->setEquiped(i, eq);
    }
    float x, y;
    file >> x >> y;

    player->moveTo(x, y);


    file >> n;
    player->setShotIndex(n);

    for (i = 0; i < SPECIAL_SHOT_SLOTS; i++)
    {
      file >> n;
      player->setShotType(i, (enumShotType)n);
    }

    player->computePlayer();
    file.close();
    remove(SAVE_FILE.c_str());
  }
  else
  {
    return false;
  }

  return true;
}

WitchBlastGame::saveHeaderStruct WitchBlastGame::loadGameHeader()
{
  saveHeaderStruct saveHeader;
  saveHeader.ok = true;

  ifstream file(SAVE_FILE.c_str(), ios::in);

  if (file)
  {
    // version
    std::string v;
    file >> v;

    if (v != SAVE_VERSION)
    {
      file.close();
      remove(SAVE_FILE.c_str());
      saveHeader.ok = false;
    }
    else
    {
      // date an time
      file >> saveHeader.date;
      file >> saveHeader.time;

      // floor
      file >> saveHeader.level;
    }
  }
  else
  {
    saveHeader.ok = false;
  }
  return saveHeader;
}

void WitchBlastGame::addKey(int logicInput, std::string key)
{
  int iKey = config.findInt(key);
  if (iKey >= 0)
  {
    sf::Keyboard::Key k = (sf::Keyboard::Key)iKey;
    input[logicInput] = k;
  }
}

void WitchBlastGame::saveConfigurationToFile()
{
  std::map<std::string, std::string> newMap;

  // Keys
  newMap["keyboard_move_up"] = intToString(input[KeyUp]);
  newMap["keyboard_move_down"] = intToString(input[KeyDown]);
  newMap["keyboard_move_left"] = intToString(input[KeyLeft]);
  newMap["keyboard_move_right"] = intToString(input[KeyRight]);
  newMap["keyboard_fire_up"] = intToString(input[KeyFireUp]);
  newMap["keyboard_fire_down"] = intToString(input[KeyFireDown]);
  newMap["keyboard_fire_left"] = intToString(input[KeyFireLeft]);
  newMap["keyboard_fire_right"] = intToString(input[KeyFireRight]);
  newMap["keyboard_fire"] = intToString(input[KeyFire]);
  newMap["keyboard_time_control"] = intToString(input[KeyTimeControl]);
  newMap["keyboard_fire_select"] = intToString(input[KeyFireSelect]);

  config.saveToFile(CONFIG_FILE, newMap);
}

void WitchBlastGame::configureFromFile()
{
  // default
  input[KeyUp]    = sf::Keyboard::Z;
  input[KeyDown]  = sf::Keyboard::S;
  input[KeyLeft]  = sf::Keyboard::Q;
  input[KeyRight] = sf::Keyboard::D;
  input[KeyFireUp]    = sf::Keyboard::Up;
  input[KeyFireDown]  = sf::Keyboard::Down;
  input[KeyFireLeft]  = sf::Keyboard::Left;
  input[KeyFireRight] = sf::Keyboard::Right;
  input[KeyFire] = sf::Keyboard::Space;
  input[KeyFireSelect] = sf::Keyboard::Tab;
  input[KeyTimeControl] = sf::Keyboard::RShift;

  // from file
  addKey(KeyUp, "keyboard_move_up");
  addKey(KeyDown, "keyboard_move_down");
  addKey(KeyLeft, "keyboard_move_left");
  addKey(KeyRight, "keyboard_move_right");
  addKey(KeyFireUp, "keyboard_fire_up");
  addKey(KeyFireDown, "keyboard_fire_down");
  addKey(KeyFireLeft, "keyboard_fire_left");
  addKey(KeyFireRight, "keyboard_fire_right");
  addKey(KeyFire, "keyboard_fire");
  addKey(KeyTimeControl, "keyboard_time_control");
  addKey(KeyFireSelect, "keyboard_fire_select");
}

void WitchBlastGame::buildMenu()
{
  menu.items.clear();
  menu.redefineKey = false;
  menu.age = 0.0f;

  WitchBlastGame::saveHeaderStruct saveHeader = loadGameHeader();

  if (saveHeader.ok)
  {
    menuItemStuct itemStart;
	itemStart.label = tools::getLabel("start_new_game");
	itemStart.description = tools::getLabel("start_desc");

    itemStart.id = MenuStartNew;
    menu.items.push_back(itemStart);

    menuItemStuct itemLoad;
	itemStart.label = tools::getLabel("restore");

    std::ostringstream oss;
	oss << saveHeader.date << " " << tools::getLabel("at") << " " << saveHeader.time << " - " << tools::getLabel("level") << " " << saveHeader.level;

    itemStart.description = oss.str();
    itemStart.id = MenuStartOld;
    menu.items.push_back(itemStart);

    menu.index = 1;
  }
  else
  {
    menuItemStuct itemStart;
	itemStart.label = tools::getLabel("start_new_game");
	itemStart.description = tools::getLabel("begin_journey");

    itemStart.id = MenuStartNew;
    menu.items.push_back(itemStart);

    menu.index = 0;
  }

  menuItemStuct itemKeys;
  itemKeys.label = tools::getLabel("config_keys");
  itemKeys.description = tools::getLabel("redef_input");

  itemKeys.id = MenuKeys;
  menu.items.push_back(itemKeys);

  menuItemStuct itemExit;
  itemExit.label = tools::getLabel("exit_game");
  itemExit.description = tools::getLabel("return_to_desktop");

  itemExit.id = MenuExit;
  menu.items.push_back(itemExit);
}

void WitchBlastGame::checkFallingEntities()
{
  EntityManager::EntityList* entityList =EntityManager::getEntityManager()->getList();
  EntityManager::EntityList::iterator it;

	for (it = entityList->begin (); it != entityList->end ();)
	{
		GameEntity *e = *it;
		it++;

		if (e->getLifetime() < 0.8f && (e->getType() == ENTITY_BLOOD || e->getType() == ENTITY_CORPSE))
		{
		  int tilex = (e->getX() - OFFSET_X) / TILE_WIDTH;
		  int tiley = (e->getY() - OFFSET_Y) / TILE_HEIGHT;

		  if (currentMap->getTile(tilex, tiley) >= MAP_HOLE)
      {
        SpriteEntity* spriteEntity = dynamic_cast<SpriteEntity*>(e);
        spriteEntity->setAge(0.0f);
        spriteEntity->setLifetime(3.0f);
        spriteEntity->setShrinking(true);
        spriteEntity->setFading(true);
      }
		}
	}
}

void WitchBlastGame::resetKilledEnemies()
{
  for (int i = 0; i < NB_ENEMY; i++) killedEnemies[i] = 0;
}

void WitchBlastGame::addKilledEnemy(enemyTypeEnum enemyType)
{
  if (enemyType == NB_ENEMY)
    std::cout << "[ERROR] No enemy type";
  else
    killedEnemies[enemyType]++;
}

void WitchBlastGame::displayKilledEnemies()
{
  std::cout<<"KILLED: ";
  for (int i = 0; i < NB_ENEMY; i++) if (killedEnemies[i] > 0) std::cout << i << "x" << killedEnemies[i] << " ";
  std::cout << std::endl;
}

WitchBlastGame &game()
{
    return *gameptr;
}
