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
#include "PumpkinEntity.h"
#include "ChestEntity.h"
#include "EvilFlowerEntity.h"
#include "BubbleEntity.h"
#include "ItemEntity.h"
#include "WitchEntity.h"
#include "CauldronEntity.h"
#include "SnakeEntity.h"
#include "GhostEntity.h"
#include "ZombieEntity.h"
#include "ZombieDarkEntity.h"
#include "LittleSpiderEntity.h"
#include "SpiderEggEntity.h"
#include "FranckyEntity.h"
#include "ArtefactDescriptionEntity.h"
#include "PnjEntity.h"
#include "TextEntity.h"
#include "StandardRoomGenerator.h"
#include "Scoring.h"
#include "MessageGenerator.h"
#include "TextMapper.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>

#include <algorithm>

#include <extlib/utf8/utf8.h>

//#define START_LEVEL 3

static std::string intToString(int n)
{
  std::ostringstream oss;
  oss << n;
  return oss.str();
}

static std::string keyToString(sf::Keyboard::Key key)
{
  std::string s = "Unknown";

  if (key >= sf::Keyboard::A && key <= sf::Keyboard::Z)
  {
    char c = 'A' + key - sf::Keyboard::A;
    std::ostringstream oss;
    oss << c;
    s = oss.str();
  }
  else if (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9)
  {
    char c = '0' + key - sf::Keyboard::Num0;
    std::ostringstream oss;
    oss << c;
    s = oss.str();
  }
  else if (key >= sf::Keyboard::Numpad0 && key <= sf::Keyboard::Numpad9)
  {
    char c = '0' + key - sf::Keyboard::Num0;
    std::ostringstream oss;
    oss << c << " (numpad)";
    s = oss.str();
  }
  else if (key == sf::Keyboard::LControl) s = "Left Control";
  else if (key == sf::Keyboard::LShift) s = "Left Shift";
  else if (key == sf::Keyboard::LAlt) s = "Left Alt";
  else if (key == sf::Keyboard::RControl) s = "Right Control";
  else if (key == sf::Keyboard::RShift) s = "Right Shift";
  else if (key == sf::Keyboard::RAlt) s = "Right Alt";
  else if (key == sf::Keyboard::Space) s = "Space";
  else if (key == sf::Keyboard::Tab) s = "Tab";
  else if (key == sf::Keyboard::Left) s = "Left arrow";
  else if (key == sf::Keyboard::Right) s = "Right arrow";
  else if (key == sf::Keyboard::Up) s = "Up arrow";
  else if (key == sf::Keyboard::Down) s = "Down arrow";

  return s;
}

std::map<EnumWorldEvents, EnumMessages> eventToMessage =
{
  { EventMeetRatsOrBats,      MsgInfoRatsBats },
  { EventMeetSnakes,          MsgInfoSnakes },
  { EventMeetWitches,         MsgInfoWitches },
  { EventGetCoin,             MsgInfoGold },
  { EventGetFamiliar,         MsgInfoFamiliar },
  { EventBeingHurted,         MsgTutoHeal },
  { EventFindShop,            MsgTutoShops },
  { EventFindBossDoor,        MsgTutoBossDoor },
  { EventFindChallengeDoor,   MsgTutoChallengeDoor },
  { EventGetItem,             MsgTutoItems },
  { EventGetSpecialShot,      MsgTutoShots },
  { EventGetSpell,            MsgTutoSpell },
};

// author: AFS
// source: https://github.com/LaurentGomila/SFML/wiki/Source:-Letterbox-effect-using-a-view
static sf::View getLetterboxView(sf::View view, int windowWidth, int windowHeight)
{

  // Compares the aspect ratio of the window to the aspect ratio of the view,
  // and sets the view's viewport accordingly in order to archieve a letterbox effect.
  // A new view (with a new viewport set) is returned.

  float windowRatio = windowWidth / (float) windowHeight;
  float viewRatio = view.getSize().x / (float) view.getSize().y;
  float sizeX = 1;
  float sizeY = 1;
  float posX = 0;
  float posY = 0;

  bool horizontalSpacing = true;
  if (windowRatio < viewRatio)
    horizontalSpacing = false;

  // If horizontalSpacing is true, the black bars will appear on the left and right side.
  // Otherwise, the black bars will appear on the top and bottom.

  if (horizontalSpacing)
  {
    sizeX = viewRatio / windowRatio;
    posX = (1 - sizeX) / 2.0;
  }

  else
  {
    sizeY = windowRatio / viewRatio;
    posY = (1 - sizeY) / 2.0;
  }

  view.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

  return view;
}

namespace
{
WitchBlastGame* gameptr;
}

WitchBlastGame::WitchBlastGame():
  Game(SCREEN_WIDTH,
       SCREEN_HEIGHT,
       APP_NAME + " V" + APP_VERSION)
{
  gameptr = this;

  configureFromFile();

  if (parameters.vsync == false)
  {
    app->setVerticalSyncEnabled(false);
    app->setFramerateLimit(60);
  }

  // loading resources
  const char *const images[] =
  {
    "media/player_base.png",   "media/player_faces.png",
    "media/bolt.png",          "media/tiles01.png",
    "media/rat.png",           "media/minimap.png",
    "media/items.png",         "media/items_equip.png",
    "media/chest.png",         "media/bat.png",
    "media/evil_flower.png",   "media/slime.png",
    "media/imp.png",           "media/spider_egg.png",
    "media/spider_web.png",    "media/little_spider.png",
    "media/bubble.png",        "media/witch.png",
    "media/cauldron.png",      "media/snake.png",
    "media/pumpkin.png",       "media/ghost.png",
    "media/zombie.png",
    "media/butcher.png",       "media/giant_slime.png",
    "media/king_rat.png",      "media/cyclops.png",
    "media/giant_spider.png",  "media/francky.png",
    "media/blood.png",
    "media/corpses.png",       "media/corpses_big.png",
    "media/star.png",          "media/star2.png",
    "media/interface.png",     "media/hud_shots.png",
    "media/explosion.png",     "media/keys_qwer.png",
    "media/keys_azer.png",     "media/message_icons.png",
    "media/night.png",         "media/title.png",
    "media/overlay_00.png",    "media/light_cone.png",
    "media/divinity.png",
    "media/pnj.png",           "media/fairy.png",
  };

  for (const char *const filename : images)
  {
    ImageManager::getInstance().addImage(filename);
  }

  const char *const sounds[] =
  {
    "media/sound/blast00.ogg",        "media/sound/blast01.ogg",
    "media/sound/door_closing.ogg",   "media/sound/door_opening.ogg",
    "media/sound/chest_opening.ogg",  "media/sound/impact.ogg",
    "media/sound/bonus.ogg",          "media/sound/drink.ogg",
    "media/sound/eat.ogg",            "media/sound/yawn.ogg",
    "media/sound/player_hit.ogg",
    "media/sound/player_die.ogg",     "media/sound/ennemy_dying.ogg",
    "media/sound/coin.ogg",           "media/sound/pay.ogg",
    "media/sound/wall_impact.ogg",    "media/sound/big_wall_impact.ogg",
    "media/sound/king_rat_cry_1.ogg", "media/sound/king_rat_cry_2.ogg",
    "media/sound/king_rat_die.ogg",   "media/sound/slime_jump.ogg",
    "media/sound/slime_impact.ogg",   "media/sound/slime_impact_weak.ogg",
    "media/sound/slime_smash.ogg",    "media/sound/ice_charge.ogg",
    "media/sound/electric.ogg",       "media/sound/select.ogg",
    "media/sound/heart.ogg",          "media/sound/rat_die.ogg",
    "media/sound/bat_die.ogg",        "media/sound/imp_hurt.ogg",
    "media/sound/imp_die.ogg",        "media/sound/rock_impact.ogg",
    "media/sound/throw.ogg",          "media/sound/cyclop00.ogg",
    "media/sound/cyclop_die.ogg",     "media/sound/butcher_00.ogg",
    "media/sound/butcher_01.ogg",     "media/sound/butcher_hurt.ogg",
    "media/sound/butcher_die.ogg",    "media/sound/vib.ogg",
    "media/sound/boom_00.ogg",        "media/sound/clang_00.ogg",
    "media/sound/bubble_00.ogg",      "media/sound/bubble_01.ogg",
    "media/sound/trap.ogg",           "media/sound/egg_smash_00.ogg",
    "media/sound/egg_smash_01.ogg",   "media/sound/spider_walking.ogg",
    "media/sound/spider_web.ogg",     "media/sound/spider_hurt.ogg",
    "media/sound/spider_die.ogg",     "media/sound/little_spider_die.ogg",
    "media/sound/witch_00.ogg",       "media/sound/witch_01.ogg",
    "media/sound/witch_die_00.ogg",   "media/sound/witch_die_01.ogg",
    "media/sound/witch_02.ogg",       "media/sound/invoke.ogg",
    "media/sound/cauldron.ogg",       "media/sound/cauldron_die.ogg",
    "media/sound/snake_die.ogg",      "media/sound/pumpkin_00.ogg",
    "media/sound/pumpkin_01.ogg",     "media/sound/pumpkin_die.ogg",
    "media/sound/critical.ogg",
    "media/sound/gong.ogg",           "media/sound/teleport.ogg",
    "media/sound/spell_charge.ogg",   "media/sound/fireball.ogg",
    "media/sound/message.ogg",        "media/sound/earthquake.ogg",
    "media/sound/spell_freeze.ogg",   "media/sound/spell_shield.ogg",
    "media/sound/heavy_step_00.ogg",  "media/sound/heavy_step_01.ogg",
    "media/sound/night.ogg",          "media/sound/grumble.ogg",
    "media/sound/zombie_00.ogg",      "media/sound/zombie_01.ogg",
    "media/sound/zombie_attack.ogg",  "media/sound/zombie_die.ogg",
    "media/sound/ghost.ogg",          "media/sound/ghost_die.ogg",
    "media/sound/fuse.ogg",           "media/sound/electricity.ogg",
    "media/sound/electric_blast.ogg", "media/sound/francky_00.ogg",
    "media/sound/francky_01.ogg",     "media/sound/francky_02.ogg",
    "media/sound/francky_die.ogg",    "media/sound/om.ogg",
  };

  // game main client position in the UI
  xOffset = OFFSET_X;
  yOffset = OFFSET_Y;

  SoundManager::getInstance().setVolume(parameters.soundVolume);
  for (const char *const filename : sounds)
  {
    SoundManager::getInstance().addSound(filename);
  }

  if (font.loadFromFile("media/DejaVuSans-Bold.ttf"))
  {
    myText.setFont(font);
  }

  miniMap = NULL;
  currentMap = NULL;
  currentFloor = NULL;

  lastScore.score = 0;
  lastScore.name = "";
  lastScore.level = 0;

  int i;
  for (i = 0; i < NB_X_GAME; i++) xGame[i].active = false;
  for (i = 0; i < NUMBER_EQUIP_ITEMS; i++) equipNudeToDisplay[i] = false;
  for (i = 0; i < NB_MESSAGES; i++) gameMessagesToSkip[i] = false;
  saveInFight.monsters.clear();

  isPausing = false;
  showLogical = false;
  showGameTime = false;

  shotsSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_HUD_SHOTS));
  introScreenSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_INTRO));
  titleSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_TITLE));
  titleSprite.setOrigin(titleSprite.getTextureRect().width / 2, titleSprite.getTextureRect().height / 2);

  loadGameData();
  loadHiScores();
  srand(time(NULL));
}

WitchBlastGame::~WitchBlastGame()
{
  // cleaning all entities
  EntityManager::getInstance().clean();

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (currentFloor != NULL) delete (currentFloor);
}

DungeonMap* WitchBlastGame::getCurrentMap()
{
  return currentMap;
}

DungeonMapEntity* WitchBlastGame::getCurrentMapEntity()
{
  return dungeonEntity;
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

int WitchBlastGame::getChallengeLevel()
{
  return challengeLevel;
}

bool WitchBlastGame::getShowLogical()
{
  return showLogical;
}

void WitchBlastGame::onUpdate()
{
  if (!isPausing)
  {
    EntityManager::getInstance().animate(deltaTime);
    if (sf::Keyboard::isKeyPressed(input[KeyTimeControl]))
    {
      EntityManager::getInstance().animate(deltaTime);
      SoundManager::getInstance().playSound(SOUND_VIB);
    }
    else
      SoundManager::getInstance().stopSound(SOUND_VIB);

    if (isPlayerAlive) gameTime += deltaTime;

    for (int i = 0; i < NB_X_GAME; i++)
    {
      if (xGame[i].active)
      {
        xGame[i].timer -= deltaTime;
        if (xGame[i].timer <= 0.0f)
        {
          xGame[i].active = false;
          if (i == (int)xGameTypeFade && xGame[i].param == X_GAME_FADE_OUT)
          {
            if (player->getPlayerStatus() == PlayerEntity::playerStatusGoingNext)
            {
              level++;
              startNewLevel();
            }
            else
              startNewGame(false);
          }
        }
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
  isPausing = false;
  level = 1;
  challengeLevel = 1;
  gameTime = 0.0f;
  initEvents();
  scoreSaveFile = "";
  interaction.active = false;

  // cleaning all entities
  EntityManager::getInstance().clean();

  // cleaning the message queue
  std::queue<messageStruct> empty;
  std::swap( messagesQueue, empty );

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (currentFloor != NULL) delete (currentFloor);
  miniMap = NULL;
  currentFloor = NULL;

  // init in game menu
  buildInGameMenu();
  dungeonEntity = new DungeonMapEntity();

  // the interface
  SpriteEntity* interface = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_INTERFACE));
  interface->setZ(10000.0f);
  interface->removeCenter();
  interface->setType(0);

  // key symbol on the interface
  keySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
  keySprite.setTextureRect(sf::IntRect(ITEM_WIDTH * EQUIP_BOSS_KEY, 0,  ITEM_WIDTH, ITEM_HEIGHT));
  keySprite.setPosition(326, 616);

  miniMap = new GameMap(FLOOR_WIDTH, FLOOR_HEIGHT);
  // minimap on the interface
  TileMapEntity* miniMapEntity = new TileMapEntity(ImageManager::getInstance().getImage(IMAGE_MINIMAP), miniMap, 15, 11, 12);
  miniMapEntity->setTileBox(16, 12);
  miniMapEntity->setX(407);
  miniMapEntity->setY(614);
  miniMapEntity->setZ(10001.0f);

  // doors
  doorEntity[0] = new DoorEntity(8);
  doorEntity[1] = new DoorEntity(4);
  doorEntity[2] = new DoorEntity(2);
  doorEntity[3] = new DoorEntity(6);

  saveInFight.isFight = false;
  if (fromSaveFile)
  {
    if (!loadGame())
      fromSaveFile = false;
    else
      playLevel(saveInFight.isFight);
  }
  if (!fromSaveFile)
  {
    // the player
    player = new PlayerEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                              (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
    resetKilledEnemies();

#ifdef START_LEVEL

    for (int i = 1; i < START_LEVEL; i++)
    {
      level = i;

      if (level == 3)
      {
        player->acquireItem(getItemSpell());
        player->acquireItemAfterStance();
      }

      player->setHpMax(player->getHpMax() + 2 + rand() % 4);
      item_equip_enum item = getRandomEquipItem(false, false);
      player->acquireItem((enumItemType)(item + FirstEquipItem));
      player->acquireItemAfterStance();

      item = getRandomEquipItem(true, true);
      player->acquireItem((enumItemType)(item + FirstEquipItem));
      player->acquireItemAfterStance();

      if (i % 2 == 0)
      {
        item = getRandomEquipItem(false, false);
        player->acquireItem((enumItemType)(item + FirstEquipItem));
        player->acquireItemAfterStance();
      }
    }
    level++;

    player->setHp(player->getHpMax());
    player->setGold(8 + rand() % 45);

#endif // START_LEVEL

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
    player->moveTo((TILE_WIDTH * MAP_WIDTH * 0.5f),
                   (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
  else
    player->moveTo((TILE_WIDTH * MAP_WIDTH * 0.5f),
                   (TILE_HEIGHT * MAP_HEIGHT - 2 * TILE_HEIGHT));

  // the boss room is closed
  bossRoomOpened = false;

  if (level <= 6) testAndAddMessageToQueue((EnumMessages)(MsgInfoLevel1 + level - 1));
  if (level == 1)
  {
    testAndAddMessageToQueue(MsgTutoBasics);
    testAndAddMessageToQueue(MsgTutoTips);
  }

  playLevel(false);
}

void WitchBlastGame::playLevel(bool isFight)
{
  /*
  if (level % 3 == 1)
    ImageManager::getInstance().getImage(IMAGE_TILES)->loadFromFile("media/tiles01.png");
  else if (level % 3 == 2)
    ImageManager::getInstance().getImage(IMAGE_TILES)->loadFromFile("media/tiles02.png");
  else
    ImageManager::getInstance().getImage(IMAGE_TILES)->loadFromFile("media/tiles03.png");
    */

  isPlayerAlive = true;

  if (!isFight)
  {
    player->setVelocity(Vector2D(0.0f, 0.0f));
    player->setPlayerStatus(PlayerEntity::playerStatusPlaying);
  }
  // first map is open
  roomClosed = false;

  // generate the map
  refreshMap();

  // items from save
  currentMap->restoreMapObjects();

  if (isFight)
  {
    checkEntering();
    saveMapItems();
  }

  // populate with monsters
  if (isFight)
  {
    auto monsters_save = saveInFight.monsters;
    saveInFight.monsters.clear();

    for (auto monster: monsters_save)
      addMonster(monster.id, monster.x, monster.y);

    if (currentMap->getRoomType() == roomTypeBoss) playMusic(MusicBoss);
    else if (currentMap->getRoomType() == roomTypeChallenge) playMusic(MusicChallenge);
    else playMusic(MusicDungeon);
  }
  else
    playMusic(MusicDungeon);

  // game time counter an state
  lastTime = getAbsolutTime();
  gameState = gameStatePlaying;

  // fade in
  xGame[xGameTypeFade].active = true;
  xGame[xGameTypeFade].param = X_GAME_FADE_IN;
  xGame[xGameTypeFade].timer = FADE_IN_DELAY;

  float x0 = MAP_WIDTH * 0.5f * TILE_WIDTH;
  float y0 = MAP_HEIGHT * 0.5f * TILE_HEIGHT + 40.0f;

  std::ostringstream oss;
  oss << tools::getLabel("level") << " " << level;

  TextEntity* text = new TextEntity(oss.str(), 30, x0, y0);
  text->setAlignment(ALIGN_CENTER);
  text->setLifetime(2.5f);
  text->setWeight(-36.0f);
  text->setZ(1000);
  text->setColor(TextEntity::COLOR_FADING_WHITE);
}

void WitchBlastGame::prepareIntro()
{
  EntityManager::getInstance().clean();

  introSprites[1] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_WITCH), 1100, 565, 64, 96, 3);
  introSprites[1]->setScale(2, 2);
  introSprites[1]->setFrame(4);

  introSprites[7] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_CAULDRON), 1480, 600, 64, 64, 0);
  introSprites[7]->setScale(2, 2);

  introSprites[3] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_WITCH), 1400, 580, 64, 96, 0);
  introSprites[3]->setScale(2, 2);

  introSprites[6] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_WITCH), 1550, 580, 64, 96, 0);
  introSprites[6]->setScale(2, 2);

  introSprites[0] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_WITCH), 1000, 580, 64, 96, 0);
  introSprites[0]->setScale(2, 2);

  introSprites[2] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_CYCLOP), 1200, 560, 128, 192, 3);
  introSprites[2]->setScale(2, 2);
  introSprites[2]->setFrame(0);

  introSprites[4] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_PLAYER_BASE), 1900, 580, 42, 80, 6);
  introSprites[4]->setScale(2, 2);
  introSprites[4]->setMirroring(true);
  introSprites[5] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_PLAYER_BASE), 1900, 580, 42, 80, 6);
  introSprites[5]->setScale(2, 2);
  introSprites[5]->setMirroring(true);

  gameState = gameStateIntro;
  gameTime = 0.0f;
  introSoundState = 0;

  playMusic(MusicIntro);
}

void WitchBlastGame::updateIntro()
{
  gameTime += deltaTime;

  introSprites[0]->setX(introSprites[0]->getX() - deltaTime * 60);
  int frame = (int)(gameTime * 4) % 4;
  if (frame == 2) frame = 0;
  else if (frame == 3) frame = 2;
  introSprites[0]->setFrame(frame);

  introSprites[3]->setX(introSprites[3]->getX() - deltaTime * 60);
  introSprites[3]->setFrame(frame);
  introSprites[6]->setX(introSprites[6]->getX() - deltaTime * 60);
  introSprites[6]->setFrame(frame);
  introSprites[7]->setX(introSprites[7]->getX() - deltaTime * 60);
  introSprites[7]->setAngle(6.0f * cos(2 * getAbsolutTime()));

  introSprites[1]->setX(introSprites[1]->getX() - deltaTime * 60);
  introSprites[1]->setFrame(frame + 3);

  int oldFrame = introSprites[2]->getFrame();
  introSprites[2]->setX(introSprites[2]->getX() - deltaTime * 50);
  frame = (int)(gameTime * 2) % 4;
  if (frame == 2) frame = 0;
  else if (frame == 3) frame = 2;
  introSprites[2]->setFrame(frame);

  if (introSprites[2]->getX() > -200)
  {
    if (oldFrame == 1 && frame == 0) SoundManager::getInstance().playSound(SOUND_HEAVY_STEP_00);
    else if (oldFrame == 2 && frame == 0) SoundManager::getInstance().playSound(SOUND_HEAVY_STEP_01);
  }

  bool grumble = false;
  if (introSprites[4]->getX() > 485 || gameTime > 23.5f)
  {
    introSprites[4]->setX(introSprites[4]->getX() - deltaTime * 68);
    frame = (int)(gameTime * 7) % 4;
    if (frame == 3) frame = 1;
    introSprites[4]->setFrame(6 + frame);
    introSprites[5]->setX(introSprites[4]->getX() - deltaTime * 68);
    introSprites[5]->setFrame(9 + frame);
  }
  else
  {
    frame = (int)(gameTime * 8) % 4;
    if (frame == 3) frame = 1;
    introSprites[4]->setFrame(42 + frame);
    introSprites[5]->setFrame(45 + frame);
    grumble = true;
  }

  // SOUND
  SoundManager::getInstance().playSound(SOUND_NIGHT);
  if (introSoundState == 0 && gameTime > 3.0f)
  {
    SoundManager::getInstance().playSound(SOUND_WITCH_00);
    introSoundState++;
  }
  else if (introSoundState == 1 && gameTime > 5.0f)
  {
    SoundManager::getInstance().playSound(SOUND_WITCH_01);
    introSoundState++;
  }
  else if (introSoundState == 2 && gameTime > 8.5f)
  {
    SoundManager::getInstance().playSound(SOUND_CYCLOP_00);
    introSoundState++;
  }
  else if (introSoundState == 3 && gameTime > 10.0f)
  {
    SoundManager::getInstance().playSound(SOUND_WITCH_00);
    introSoundState++;
  }
  else if (introSoundState == 4 && gameTime > 14.0f)
  {
    SoundManager::getInstance().playSound(SOUND_CYCLOP_00);
    introSoundState++;
  }
  else if (introSoundState == 5 && gameTime > 17.0f)
  {
    SoundManager::getInstance().playSound(SOUND_WITCH_02);
    introSoundState++;
  }
  else if (introSoundState == 6 && grumble)
  {
    SoundManager::getInstance().playSound(SOUND_GRUMBLE);
    introSoundState++;
  }


  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed) app->close();

    if (event.type == sf::Event::Resized)
    {
      sf::View view = app->getDefaultView();
      view = getLetterboxView( view, event.size.width, event.size.height );
      app->setView(view);
    }


    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Return
          || event.key.code == sf::Keyboard::Space
          || event.key.code == sf::Keyboard::Escape)
      {
        for (int i = 0; i < 8; i++)
          introSprites[i]->setWeight(500);
        switchToMenu();
      }
    }
  }
}

void WitchBlastGame::renderIntro()
{
  app->draw(introScreenSprite);
  titleSprite.setPosition(SCREEN_WIDTH / 2, 280);
  app->draw(titleSprite);
  EntityManager::getInstance().render(app);
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
      if (gameState == gameStatePlaying && !player->isDead()) saveGame();
      app->close();
    }

    if (event.type == sf::Event::Resized)
    {
      sf::View view = app->getDefaultView();
      view = getLetterboxView( view, event.size.width, event.size.height );
      app->setView(view);
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

      if (event.key.code == input[KeySpell])
      {
        if (gameState == gameStatePlaying && !isPausing) player->castSpell();
      }

      if (event.key.code == input[KeyDown] || event.key.code == sf::Keyboard::Down)
      {
        // in game menu ?
        if (gameState == gameStatePlaying && isPausing)
        {
          menuInGame.index++;
          if (menuInGame.index == menuInGame.items.size()) menuInGame.index = 0;
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
      }
      else if (event.key.code == input[KeyUp] || event.key.code == sf::Keyboard::Up)
      {
        // in game menu ?
        if (gameState == gameStatePlaying && isPausing)
        {
          if (menuInGame.index == 0) menuInGame.index = menuInGame.items.size() - 1;
          else menuInGame.index--;
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
      }
      else if (event.key.code == sf::Keyboard::Return)
      {
        switch (menuInGame.items[menuInGame.index].id)
        {
        case MenuStartNew:
        case MenuStartOld:
        case MenuKeys:
        case MenuConfig:
        case MenuTutoReset:
        case MenuConfigBack:
        case MenuLanguage:
        case MenuCredits:
        case MenuHiScores:
        case MenuPlayerName:
        case MenuVolumeMusic:
        case MenuVolumeSound:
          std::cout << "[ERROR] Bad Menu ID\n";
          break;

        case MenuExit:
          backToMenu = true;
          remove(SAVE_FILE.c_str());
          break;

        case MenuContinue:
          isPausing = false;
          break;

        case MenuSaveAndQuit:
          saveGame();
          backToMenu = true;
          break;
        }
      }

      if (event.key.code == input[KeyFire])
      {
        if (gameState == gameStatePlaying && !isPausing) firingDirection = player->getFacingDirection();
      }

      if (event.key.code == sf::Keyboard::Return)
      {
        if (player->isDead() && !xGame[xGameTypeFade].active && sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
        {
          if (player->getDeathAge() < DEATH_CERTIFICATE_DELAY)
            player->setDeathAge(DEATH_CERTIFICATE_DELAY);
          else
            backToMenu = true;
        }
        else if (!messagesQueue.empty())
        {
          if (messagesQueue.front().timer > 0.5f)
            messagesQueue.front().timer = 0.5f;
        }
      }

      if (event.key.code == input[KeyInteract])
      {
        if (!player->isDead() && interaction.active)
          player->interact(interaction.type, interaction.id);
      }

      if (event.key.code == sf::Keyboard::X)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) startNewGame(false);
      }

      if (event.key.code == sf::Keyboard::F1)
      {
        if (!isPlayerAlive && player->getDeathAge() > 3.5f)
        {
          if (scoreSaveFile.compare("") == 0) saveDeathScreen();
        }
      }

      if (event.key.code == sf::Keyboard::F2)
      {
        showLogical = !showLogical;
      }
      if (event.key.code == sf::Keyboard::F3)
      {
        showGameTime = !showGameTime;
      }

      // DEBUG
#ifdef TEST_MODE
      if (event.key.code == sf::Keyboard::F5)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                           OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 0);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeRat, 1);
          findPlaceMonsters(EnemyTypeRatBlack, 1);
          findPlaceMonsters(EnemyTypeRatGreen, 1);
          findPlaceMonsters(EnemyTypeRatHelmet, 1);
          findPlaceMonsters(EnemyTypeRatBlackHelmet, 1);
        }
      }
      if (event.key.code == sf::Keyboard::F6)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new ButcherEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                            OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeBat, 2);
          findPlaceMonsters(EnemyTypeImpBlue, 1);
          findPlaceMonsters(EnemyTypeImpRed, 1);
        }
      }
      if (event.key.code == sf::Keyboard::F7)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new GiantSlimeEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                               OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeEvilFlower, 1);
          findPlaceMonsters(EnemyTypeEvilFlowerIce, 1);
          findPlaceMonsters(EnemyTypePumpkin, 1);
        }
      }
      if (event.key.code == sf::Keyboard::F8)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new CyclopsEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                            OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeSlime, 1);
          findPlaceMonsters(EnemyTypeSlimeBlue, 1);
          findPlaceMonsters(EnemyTypeSlimeRed, 1);
          findPlaceMonsters(EnemyTypeSlimeViolet, 1);
        }
      }
      if (event.key.code == sf::Keyboard::F9)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new KingRatEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                            OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeSnake, 2);
          findPlaceMonsters(EnemyTypeSnakeBlood, 2);
        }
      }
      if (event.key.code == sf::Keyboard::F10)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new GiantSpiderEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                                OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeWitch, 1);
          findPlaceMonsters(EnemyTypeWitchRed, 1);
          findPlaceMonsters(EnemyTypeCauldron, 1);
        }
      }
      if (event.key.code == sf::Keyboard::F11)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new FranckyEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                            OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeSpiderEgg_invocated, 2);
          findPlaceMonsters(EnemyTypeSpiderLittle_invocated, 2);
        }
      }
      if (event.key.code == sf::Keyboard::F12)
      {
        initMonsterArray();
        findPlaceMonsters(EnemyTypeZombie, 2);
        findPlaceMonsters(EnemyTypeZombieDark, 2);
        findPlaceMonsters(EnemyTypeGhost, 2);
      }
      if (event.key.code == sf::Keyboard::F4)
      {
        for (int i = 0; i < NUMBER_ITEMS; i++)
          new ItemEntity((enumItemType)i, 100 + (i % 14) * 58, 100 + (i / 14) * 60);
      }
#endif // TEST_MODE
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

    // spell (right click)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (gameState == gameStatePlaying && !isPausing))
    {
      sf::Vector2i mousePosition = sf::Mouse::getPosition(*app);
      int xm = mousePosition.x - player->getX();
      int ym = mousePosition.y - player->getY();

      if (abs(xm) >= abs(ym))
      {
        if (xm > 0) player->setFacingDirection(6);
        else player->setFacingDirection(4);
      }
      else
      {
        if (ym > 0) player->setFacingDirection(2);
        else player->setFacingDirection(8);
      }
      player->castSpell();
    }

    // message queue
    if (!messagesQueue.empty())
    {
      messagesQueue.front().timer -= deltaTime;
      if (messagesQueue.front().timer < 0.0f)
      {
        messagesQueue.pop();
        if (!messagesQueue.empty()) SoundManager::getInstance().playSound(SOUND_MESSAGE);
      }
    }
  }

  onUpdate();

  verifyDoorUnlocking();
  checkInteraction();

  if (roomClosed)
  {
    if (getEnemyCount() == 0)
    {
      currentMap->setCleared(true);
      openDoors();
      remove(SAVE_FILE.c_str());
      if (currentMap->getRoomType() == roomTypeBoss)
      {
        playMusic(MusicDungeon);
      }
      else if (currentMap->getRoomType() == roomTypeChallenge && !player->isDead())
      {
        ChestEntity* chest = new ChestEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                                             (TILE_HEIGHT * MAP_HEIGHT * 0.5f),
                                             ChestChallenge, false);
        chest->makeAppear();

        // sound
        SoundManager::getInstance().playSound(SOUND_GONG);
        playMusic(MusicDungeon);

        // text
        float x0 = MAP_WIDTH * 0.5f * TILE_WIDTH;
        float y0 = MAP_HEIGHT * 0.5f * TILE_HEIGHT + 40.0f;
        TextEntity* text = new TextEntity("COMPLETE !", 30, x0, y0);
        text->setAlignment(ALIGN_CENTER);
        text->setLifetime(2.5f);
        text->setWeight(-36.0f);
        text->setZ(1200);
        text->setColor(TextEntity::COLOR_FADING_WHITE);

        challengeLevel++;
      }
    }
  }

  if (backToMenu)
  {
    if (player->isDead())
    {
      EntityManager::getInstance().clean();
      switchToMenu();
      menuState = MenuStateHiScores;
    }
    else
    {
      prepareIntro(); // switchToMenu();
    }
  }
}

void WitchBlastGame::addLifeBarToDisplay(std::string label, int hp, int hpMax)
{
  lifeBar.toDisplay = true;
  lifeBar.label = label;
  lifeBar.hp = hp;
  lifeBar.hpMax = hpMax;
}

void WitchBlastGame::renderGame()
{
  lifeBar.toDisplay = false;
  EntityManager::getInstance().renderUnder(app, 5000);
}

void WitchBlastGame::renderHud()
{
  // boss life bar ?
  if (lifeBar.toDisplay) renderLifeBar();

  // interaction text ?
  if (interaction.active)
  {
    if (interaction.type == InteractionTypeTemple)
      write(interaction.label, 20, GAME_WIDTH / 2, 480, ALIGN_CENTER,sf::Color::White, app, 2, 2);
  }

  // light cone ?
  float fade = player->getLightCone();
  if (fade > 0.0f)
  {
    sf::Sprite cone;
    cone.setTexture(*ImageManager::getInstance().getImage(IMAGE_LIGHT_CONE));
    cone.setPosition(player->getX() - 64, player->getY() - 580);
    cone.setColor(sf::Color(255, 255, 255, 255 * fade));
    app->draw(cone);
  }

  EntityManager::getInstance().renderAfter(app, 5000);
}

void WitchBlastGame::renderLifeBar()
{
  if (lifeBar.toDisplay)
  {
    float l = lifeBar.hp * ((MAP_WIDTH - 1) * TILE_WIDTH) / lifeBar.hpMax;
    int label_dy = 0;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH, 25));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(TILE_WIDTH / 2, label_dy + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(200, 200, 200, 200));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 25));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setOutlineThickness(0);
    rectangle.setPosition(sf::Vector2f(TILE_WIDTH / 2, label_dy + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT));
    app->draw(rectangle);

    game().write(           lifeBar.label,
                            18,
                            TILE_WIDTH / 2 + 10.0f,
                            label_dy + 25 + (MAP_HEIGHT - 1) * TILE_HEIGHT + 1.0f,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255),
                            app, 0 , 0);
  }
}

void WitchBlastGame::renderRunningGame()
{
  EntityManager::getInstance().sortByZ();

  if (!isPlayerAlive)
  {
    sf::View view = app->getView();
    sf::View viewSave = app->getView();

    if (!parameters.zoom || player->getDeathAge() > 4.0f)
    {
      // do nothing
    }
    else if (player->getDeathAge() > 3.0f)
    {
      view.zoom(1.0f - 0.75f * (4.0f - player->getDeathAge()));
      float xDiff = view.getCenter().x - player->getX();
      float yDiff = view.getCenter().y - player->getY();
      view.setCenter(view.getCenter().x - xDiff * (4.0f - player->getDeathAge()),
                     view.getCenter().y - yDiff * (4.0f - player->getDeathAge()));
    }
    else if (player->getDeathAge() > 1.0f)
    {
      view.zoom(0.25f);
      view.setCenter(player->getX(), player->getY());
    }
    else
    {
      view.zoom(1.0f - 0.75f * (player->getDeathAge()));
      float xDiff = view.getCenter().x - player->getX();
      float yDiff = view.getCenter().y - player->getY();
      view.setCenter(view.getCenter().x - xDiff * player->getDeathAge(),
                     view.getCenter().y - yDiff * player->getDeathAge());
    }

    view.move(-5, -5);
    app->setView(view);

    renderGame();

    app->setView(viewSave);
    renderHud();
  }
  else if (parameters.zoom && gameTime < 1.0f)
  {
    sf::View view = app->getView();
    sf::View viewSave = app->getView();

    view.zoom(0.25f + 0.75f * (gameTime));

    view.move(-5, -5);
    app->setView(view);

    renderGame();

    app->setView(viewSave);
    renderHud();
  }
  else if (xGame[xGameTypeShake].active)
  {
    sf::View view = app->getView();
    sf::View viewSave = app->getView();

    view.move(-4 + rand() % 9, -4 + rand() % 9);
    app->setView(view);

    renderGame();

    app->setView(viewSave);
    renderHud();
  }
  else
  {
    sf::View view = app->getView();
    sf::View viewSave = app->getView();

    view.move(-OFFSET_X, -OFFSET_Y);
    app->setView(view);

    renderGame();

    app->setView(viewSave);
    renderHud();
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
  oss << tools::getLabel("level") << " " << level;
  write(oss.str(), 16, 410, 692, ALIGN_LEFT, sf::Color::Black, app, 0, 0);

  sf::RectangleShape rectangle(sf::Vector2f(200, 25));

  if (gameState == gameStatePlaying)
  {
    // life
    if (player->isPoisoned())
      rectangle.setFillColor(sf::Color(20, 190, 20));
    else
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
    write(oss.str(), 16, 95, 624, ALIGN_LEFT, sf::Color::White, app, 0, 0);

    // mana
    rectangle.setFillColor(sf::Color(20, 20, 190));
    rectangle.setPosition(sf::Vector2f(90, 658));
    rectangle.setSize(sf::Vector2f(200.0f * player->getPercentSpellDelay() , 25));
    app->draw(rectangle);

    rectangle.setFillColor(sf::Color(190, 190, 255));
    rectangle.setPosition(sf::Vector2f(90, 661));
    rectangle.setSize(sf::Vector2f(200.0f * player->getPercentSpellDelay() , 2));
    app->draw(rectangle);

    if (player->getActiveSpell().spell != SpellNone)
    {
      oss.str("");
      oss << tools::getLabel(spellLabel[player->getActiveSpell().spell]);
      if (player->isEquiped(EQUIP_BOOK_MAGIC_II)) oss << "+";
      write(oss.str(), 14, 95, 663, ALIGN_LEFT, sf::Color::White, app, 0, 0);
      // TODO
      sf::Sprite itemSprite;
      itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      itemSprite.setPosition(10, 620);
      int frame = player->getActiveSpell().frame;
      itemSprite.setTextureRect(sf::IntRect((frame % 10) * 32, (frame / 10) * 32, 32, 32));
      itemSprite.scale(2.0f, 2.0f);
      app->draw(itemSprite);

      if (player->canCastSpell())
      {
        float fade = (cos(8.0f * getAbsolutTime()) + 1.0f) * 0.5f;
        itemSprite.setColor(sf::Color(255, 255, 255, 255 * fade));
        app->draw(itemSprite, sf::BlendAdd);
      }
    }

    // drawing the key on the interface
    if (player->isEquiped(EQUIP_BOSS_KEY)) app->draw(keySprite);

    // drawing the divinity
    if (player->getDivinity().divinity >= 0)
    {
      sf::Sprite divSprite;
      divSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_DIVINITY));
      divSprite.setPosition(895, 605);
      divSprite.setTextureRect(sf::IntRect(player->getDivinity().divinity * 64, 0, 64, 64));
      app->draw(divSprite);

      rectangle.setFillColor(sf::Color(0, 0, 0, 200));
      rectangle.setPosition(sf::Vector2f(896, 670));
      rectangle.setSize(sf::Vector2f(62, 12));
      rectangle.setOutlineColor(sf::Color::White);
      rectangle.setOutlineThickness(1);
      app->draw(rectangle);

      rectangle.setOutlineThickness(0);
      rectangle.setFillColor(sf::Color(100, 100, 200, 255));
      rectangle.setSize(sf::Vector2f(62 * player->getDivinity().percentsToNextLevels, 12));
      app->draw(rectangle);

      write(intToString(player->getDivinity().level), 10, 928, 671, ALIGN_CENTER, sf::Color::White, app, 0, 0);
    }

    // render the shots
    renderHudShots(app);

    if (isPausing)
    {
      rectangle.setFillColor(sf::Color(0, 0, 0, 200));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH, MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);

      float x0 = (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;
      int fade = 50 + 205 * (1.0f + cos(3.0f * getAbsolutTime())) * 0.5f;
      myText.setColor(sf::Color(255, 255, 255, fade));
      myText.setCharacterSize(40);
      myText.setString("PAUSE");
      myText.setPosition(x0 - myText.getLocalBounds().width / 2, 100);
      app->draw(myText);

      float x = 200;
      float y = 480;
      // items
      write(tools::getLabel("inventory"), 16, x, y, ALIGN_LEFT, sf::Color::White, app, 0, 0);
      int n = 0;

      for (auto i: sortedEquipement)
      {
        if (i != EQUIP_BOSS_KEY && player->isEquiped(i))
        {
          sf::Sprite itemSprite;
          itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
          itemSprite.setPosition(x + n * 32, y + 20);
          itemSprite.setTextureRect(sf::IntRect((i % 10) * 32, (i / 10) * 32, 32, 32));
          app->draw(itemSprite);
          n++;
        }
      }

      renderInGameMenu();
    }

    if (player->isDead())
    {
      float deathAge = player->getDeathAge();

      if (deathAge > DEATH_CERTIFICATE_DELAY)
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, 180));
        rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
        rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
        app->draw(rectangle);

        renderDeathScreen(80, 110);

        if (scoreSaveFile.compare("") == 0)
        {
          write(tools::getLabel("certificate_capture"), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0);
        }
        else
        {
          std::stringstream ss;
          ss << tools::getLabel("certificate_saved") << " " << scoreSaveFile;
          write(ss.str(), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0);
        }
      }
      else if (deathAge > DEATH_CERTIFICATE_DELAY - 1.0f)
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, 180 * (deathAge - 2.5f)));
        rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
        rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
        app->draw(rectangle);

        renderDeathScreen(80 + (DEATH_CERTIFICATE_DELAY - deathAge) * 1000, 110);
      }

    }
    else if (currentMap->getRoomType() == roomTypeExit && level >= LAST_LEVEL)
    {
      float x0 = (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;

      write(tools::getLabel("congratulations_1"), 25, x0, 220, ALIGN_CENTER, sf::Color::White, app, 2, 2);
      write(tools::getLabel("congratulations_2"), 23, x0, 250, ALIGN_CENTER, sf::Color::White, app, 2, 2);
      write(tools::getLabel("congratulations_3"), 23, x0, 280, ALIGN_CENTER, sf::Color::White, app, 2, 2);
    }

    if (sf::Keyboard::isKeyPressed(input[KeyTimeControl]))
    {
      // effect
      int effectFade = 10 + 20 * (1.0f + cos(12.0f * getAbsolutTime())) * 0.5f;
      rectangle.setFillColor(sf::Color(0, 255, 255, effectFade));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      sf::RenderStates r;
      r.blendMode = sf::BlendAlpha ;
      app->draw(rectangle, r);
    }

    if (xGame[xGameTypeFade].active && xGame[xGameTypeFade].param == X_GAME_FADE_IN)
    {
      // fade in
      rectangle.setFillColor(sf::Color(0, 0, 0, 255 - ((FADE_IN_DELAY - xGame[xGameTypeFade].timer) / FADE_IN_DELAY) * 255));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);
    }
    else if (xGame[xGameTypeFade].active && xGame[xGameTypeFade].param == X_GAME_FADE_OUT)
    {
      // fade out
      rectangle.setFillColor(sf::Color(0, 0, 0, ((FADE_IN_DELAY - xGame[xGameTypeFade].timer) / FADE_IN_DELAY) * 255));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);
    }

    if (xGame[xGameTypeFadeColor].active)
    {
      // color fade
      unsigned int r = 0, g = 0, b = 0;
      switch (xGame[xGameTypeFadeColor].param)
      {
      case X_GAME_COLOR_RED:
        r = 255;
        g = 100;
        break;
      case X_GAME_COLOR_GREEN:
        g = 255;
        break;
      case X_GAME_COLOR_BLUE:
        b = 255;
        break;
      case X_GAME_COLOR_VIOLET:
        r = 255;
        b = 200;
        break;
      case X_GAME_COLOR_BROWN:
        r = 200;
        b = 100;
        g = 150;
        break;
      case X_GAME_COLOR_WHITE:
        r = 255;
        b = 255;
        g = 255;
        break;
      }
      int alpha = xGame[xGameTypeFadeColor].timer * 200.0f /  xGame[xGameTypeFadeColor].duration;

      rectangle.setFillColor(sf::Color(r, g, b, alpha));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle, sf::BlendAdd);
    }
  }

  // message queue
  if (!messagesQueue.empty())
  {

    int xf = 10;
    int yf = MAP_HEIGHT * TILE_HEIGHT + 10;
    int ySize = SCREEN_HEIGHT - (MAP_HEIGHT * TILE_HEIGHT) - 10;
    int xm = xf;
    int ym = yf;

    if (messagesQueue.front().timer < 0.5f)
    {
      ym += (1.0f - 2 * messagesQueue.front().timer) * ySize;
    }
    else if (messagesQueue.front().timerMax - messagesQueue.front().timer < 0.5f)
    {
      ym += (1.0f - 2 * (messagesQueue.front().timerMax - messagesQueue.front().timer)) * ySize;
    }

    rectangle.setFillColor(sf::Color(236, 222, 194, 255));
    rectangle.setOutlineThickness(2.0f);
    rectangle.setOutlineColor(sf::Color(201, 145, 95,255));
    rectangle.setPosition(sf::Vector2f(xm, ym));
    rectangle.setSize(sf::Vector2f(SCREEN_WIDTH - 20, ySize));
    app->draw(rectangle);

    sf::Sprite iconSprite;
    iconSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_MESSAGE_ICONS));
    iconSprite.setPosition(xm + 12, ym + 32);
    iconSprite.setTextureRect(sf::IntRect((messagesQueue.front().icon % 10) * 64, (messagesQueue.front().icon / 10) * 64, 64, 64));
    app->draw(iconSprite);

    for (int i = 0; i < 3; i++)
    {
      int fontSize = i == 0 ? 18 : 16;
      if (!messagesQueue.front().message[i].empty())
      {
        write(messagesQueue.front().message[i], fontSize, xm + 100, ym + ((i == 0 ) ? 15 : 20) + i * 30, ALIGN_LEFT, sf::Color::Black, app, 0,0);
      }
    }
  }

  // show game time
  if (showGameTime)
  {
    int minutes   = (int)gameTime / 60;
    int secondes  = (int)gameTime % 60;
    std::stringstream ss;
    if (minutes < 10) ss << "0";
    ss << minutes;
    ss << ":";
    if (secondes < 10) ss << "0";
    ss << secondes;
    write(ss.str(), 14, 4, 4, ALIGN_LEFT, sf::Color::Green, app, 0,0);
  }

}

void WitchBlastGame::saveDeathScreen()
{
  std::stringstream ss;
  ss << "rip_";
  time_t t = time(0);   // get time now
  struct tm * now = localtime( & t );

  ss << (now->tm_year + 1900);
  if (now->tm_mon < 9) ss << "0";
  ss << (now->tm_mon + 1);
  if (now->tm_mday < 9) ss << "0";
  ss << now->tm_mday;
  if (now->tm_hour <= 9) ss << "0";
  ss << (now->tm_hour);
  if (now->tm_min <= 9) ss << "0";
  ss << (now->tm_min);
  if (now->tm_sec <= 9) ss << "0";
  ss << (now->tm_sec);

  ss << ".png";

  scoreSaveFile = ss.str();
  int width = 810, height = 300, border = 4;
  int x = 80, y = 110;
  sf::Image screenShot(app->capture());
  sf::Image savedImage;
  savedImage.create(width + border * 2, height + border * 2);
  savedImage.copy(screenShot,0 , 0, sf::IntRect( x - border, y - border, width + border * 2, height + border * 2));
  savedImage.saveToFile(scoreSaveFile);
}

void WitchBlastGame::renderDeathScreen(float x, float y)
{
  int xRect = 810;
  sf::RectangleShape rectangle(sf::Vector2f(810 , 300));
  rectangle.setFillColor(sf::Color(255, 255, 240));
  rectangle.setPosition(sf::Vector2f(x, y));
  rectangle.setOutlineThickness(4);
  rectangle.setOutlineColor(sf::Color(125, 100, 170));
  app->draw(rectangle);

  std::stringstream ss;
  ss << parameters.playerName << " - " << tools::getLabel("dc_certificate");
  write(ss.str(), 18, x + xRect / 2, y + 5, ALIGN_CENTER, sf::Color::Black, app, 0, 0);

  ss.str(std::string());
  ss.clear();
  ss << tools::getLabel("dc_killed_by") << " " << sourceToString(player->getLastHurtingSource(), player->getLastHurtingEnemy()) << "." << std::endl;
  int minutes = (int)gameTime / 60;
  if (minutes < 1) minutes = 1;
  ss << tools::getLabel("dc_died_level") << " " << level << " " << tools::getLabel("dc_after") << " " << minutes << " " << tools::getLabel("dc_minutes") << "." << std::endl;

  ss << tools::getLabel("dc_killed_monsters") << ": " << bodyCount << std::endl;
  ss << tools::getLabel("dc_gold") << ": " << player->getGold() << std::endl;
  ss << tools::getLabel("dc_challenges") << ": " << challengeLevel - 1 << std::endl;

  write(ss.str(), 16, x + 112, y + 50, ALIGN_LEFT, sf::Color::Black, app, 0, 0);

  // player
  renderPlayer(x + 40, y + 48, player->getEquipment(), player->getShotType(), 1, 0);

  // items
  write(tools::getLabel("inventory"), 16, x + 14, y + 165, ALIGN_LEFT, sf::Color::Black, app, 0, 0);
  int n = 0;
  for (auto i: sortedEquipement)
  {
    if (i != EQUIP_BOSS_KEY && player->isEquiped(i))
    {
      sf::Sprite itemSprite;
      itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      itemSprite.setPosition(x + 14 + n * 32, y + 183);
      itemSprite.setTextureRect(sf::IntRect((i % 10) * 32, (i / 10) * 32, 32, 32));
      app->draw(itemSprite);
      n++;
    }
  }

  ss.str("");
  ss.clear();
  ss << "Score: " << score;
  write(ss.str(), 24, x + xRect / 2, y + 240, ALIGN_CENTER, sf::Color::Black, app, 0, 0);

}

bool compareScores(WitchBlastGame::StructScore s1, WitchBlastGame::StructScore s2)
{
  return s1.score > s2.score;
}

void WitchBlastGame::calculateScore()
{
  score = 0;
  bodyCount = 0;

  for (int enemyType = EnemyTypeBat; enemyType < EnemyTypeRockFalling; enemyType++)
  {
    bodyCount += killedEnemies[enemyType];
    score += killedEnemies[enemyType] * getMonsterScore((enemyTypeEnum)enemyType);
  }

  score += getChallengeScore(challengeLevel);
  score += getGoldScore(player->getGold());

  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    if (player->isEquiped(i)) score += getItemScore((item_equip_enum)i);
    lastScore.equip[i] = player->isEquiped(i);
  }

  // to save
  lastScore.name = parameters.playerName;
  lastScore.score = score;
  lastScore.level = level;
  lastScore.shotType = player->getShotType();

  scores.push_back(lastScore);

  std::sort (scores.begin(), scores.end(), compareScores);

  saveHiScores();
}

void WitchBlastGame::switchToMenu()
{
  SoundManager::getInstance().stopSound(SOUND_NIGHT);

  gameState = gameStateMenu;
  buildMenu(false);

  if (!config.configFileExists())
  {
    menuState = MenuStateFirst;
    menuFirst.index = 0;
    menuKeyIndex = 0;
  }
}

void WitchBlastGame::updateMenu()
{
  menuStuct* menu = NULL;
  if (menuState == MenuStateMain)
    menu = &menuMain;
  else if (menuState == MenuStateConfig)
    menu = &menuConfig;
  else if (menuState == MenuStateFirst)
    menu = &menuFirst;

  EntityManager::getInstance().animate(deltaTime);

  // Process events
  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
      app->close();
    }

    if (event.type == sf::Event::Resized)
    {
      sf::View view = app->getDefaultView();
      view = getLetterboxView( view, event.size.width, event.size.height );
      app->setView(view);
    }

    if (event.type == sf::Event::TextEntered)
    {
      if (menuState == MenuStateChangeName)
      {
        if (event.text.unicode < 128)
        {
          char c = static_cast<char>(event.text.unicode);

          if ( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
            parameters.playerName += static_cast<char>(event.text.unicode);
        }
      }
    }

    if (event.type == sf::Event::KeyPressed && menuState == MenuStateKeys)
    {
      bool alreadyUsed = false;
      if (event.key.code == sf::Keyboard::Escape) alreadyUsed = true;
      for (unsigned int i = 0; i < menuKeyIndex; i++)
        if (input[i] == event.key.code) alreadyUsed = true;

      // TODO more tests
      if (!alreadyUsed)
      {
        input[menuKeyIndex] = event.key.code;
        menuKeyIndex++;
        if (menuKeyIndex == NumberKeys)
        {
          menuState = MenuStateConfig;
          saveConfigurationToFile();
        }
      }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      if (menuState == MenuStateCredits)
      {
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return)
          menuState = MenuStateMain;
      }
      else if (menuState == MenuStateHiScores)
      {
        if (parameters.playerName.size() > 0
            && (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return))
        {
          menuState = MenuStateMain;
          if (lastScore.level > 0)
          {
            lastScore.level = 0;
            lastScore.score = 0;
            playMusic(MusicIntro);
          }
        }
      }
      else if (menuState == MenuStateChangeName)
      {
        if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return)
        {
          saveConfigurationToFile();
          menuState = MenuStateMain;
        }
        else if (event.key.code == sf::Keyboard::BackSpace)
        {
          if (parameters.playerName.size() > 0)
            parameters.playerName.erase(parameters.playerName.size() - 1);
        }
      }
      else if (event.key.code == sf::Keyboard::Escape)
      {
        if (menuState == MenuStateConfig) menuState = MenuStateMain;
        else app->close();
      }
      else if (event.key.code == input[KeyDown] || event.key.code == sf::Keyboard::Down)
      {
        menu->index++;
        if (menu->index == menu->items.size()) menu->index = 0;
        SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
      }

      else if (event.key.code == input[KeyUp] || event.key.code == sf::Keyboard::Up)
      {
        if (menu->index == 0) menu->index = menu->items.size() - 1;
        else menu->index--;

        SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
      }
      else if (event.key.code == input[KeyRight] || event.key.code == sf::Keyboard::Right)
      {
        if (menu->items[menu->index].id == MenuLanguage)
        {
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
          parameters.language++;
          if (parameters.language >= NB_LANGUAGES) parameters.language = 0;
          if (menuState == MenuStateConfig) saveConfigurationToFile();
          tools::setLanguage(languageString[parameters.language]);
          buildMenu(true);
        }
        else if (menu->items[menu->index].id == MenuVolumeSound)
        {
          parameters.soundVolume = (parameters.soundVolume / 10) * 10 + 10;
          if (parameters.soundVolume > 100) parameters.soundVolume = 100;
          saveConfigurationToFile();
          SoundManager::getInstance().setVolume(parameters.soundVolume);
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
        else if (menu->items[menu->index].id == MenuVolumeMusic)
        {
          parameters.musicVolume = (parameters.musicVolume / 10) * 10 + 10;
          if (parameters.musicVolume > 100) parameters.musicVolume = 100;
          saveConfigurationToFile();
          updateMusicVolume();
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
      }
      else if (event.key.code == input[KeyLeft] || event.key.code == sf::Keyboard::Left)
      {
        if (menu->items[menu->index].id == MenuLanguage)
        {
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
          parameters.language--;
          if (parameters.language < 0) parameters.language = NB_LANGUAGES - 1;
          if (menuState == MenuStateConfig) saveConfigurationToFile();
          tools::setLanguage(languageString[parameters.language]);
          buildMenu(true);
        }
        else if (menu->items[menu->index].id == MenuVolumeSound)
        {
          parameters.soundVolume = (parameters.soundVolume / 10) * 10 - 10;
          if (parameters.soundVolume < 0) parameters.soundVolume = 0;
          saveConfigurationToFile();
          SoundManager::getInstance().setVolume(parameters.soundVolume);
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
        else if (menu->items[menu->index].id == MenuVolumeMusic)
        {
          parameters.musicVolume = (parameters.musicVolume / 10) * 10 - 10;
          if (parameters.musicVolume < 0) parameters.musicVolume = 0;
          saveConfigurationToFile();
          updateMusicVolume();
          SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        }
      }
      else if (event.key.code == sf::Keyboard::Return)
      {

        switch (menu->items[menu->index].id)
        {
        case MenuStartNew:
          startNewGame(false);
          remove(SAVE_FILE.c_str());
          break;
        case MenuStartOld:
          startNewGame(true);
          break;
        case MenuKeys:
          menuState = MenuStateKeys;
          menuKeyIndex = 0;
          break;
        case MenuCredits:
          menuState = MenuStateCredits;
          break;
        case MenuHiScores:
          menuState = MenuStateHiScores;
          break;
        case MenuPlayerName:
          menuState = MenuStateChangeName;
          break;
        case MenuConfig:
          menuState = MenuStateConfig;
          break;
        case MenuTutoReset:
          for (int i = 0; i < NB_MESSAGES; i++) gameMessagesToSkip[i] = false;
          SoundManager::getInstance().playSound(SOUND_SPELL_FREEZE);
          break;
        case MenuConfigBack:
          menuState = MenuStateMain;
          break;
        case MenuLanguage:
          if (menuState == MenuStateFirst)
          {
            registerLanguage();
            if (parameters.playerName.compare("") == 0 )
            {
              menuMain.index = 0;
              menuState = MenuStateChangeName;
            }
            else
              menuState = MenuStateMain;
          }
          break;
        case MenuExit:
          app->close();
          break;
        case MenuVolumeSound:
        case MenuVolumeMusic:
          break;
        case MenuContinue:
        case MenuSaveAndQuit:
          std::cout << "[ERROR] Bad Menu ID\n";
          break;
        }

      }
    }
  }
}

void WitchBlastGame::renderMenu()
{
  if (menuState == MenuStateCredits)
  {
    renderCredits();
    return;
  }
  else if (menuState == MenuStateHiScores)
  {
    renderHiScores();
    return;
  }

  app->draw(introScreenSprite);
  if (titleSprite.getPosition().y > 160) titleSprite.move(0, -8);
  else if (titleSprite.getPosition().y < 160) titleSprite.setPosition(SCREEN_WIDTH / 2, 180);
  app->draw(titleSprite);

  EntityManager::getInstance().render(app);

  menuStuct* menu = nullptr;
  if (menuState == MenuStateMain || menuState == MenuStateChangeName)
    menu = &menuMain;
  else if (menuState == MenuStateConfig)
    menu = &menuConfig;
  else if (menuState == MenuStateFirst)
    menu = &menuFirst;

  int xAlign = 290;
  int yStep = 50;

  if (menuState == MenuStateKeys)
  {
    // menu keys
    if (config.configFileExists())
      write(tools::getLabel("key_configuration"), 18, xAlign, 250, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
    else
      write(tools::getLabel("key_configuration_desc"), 18, xAlign, 250, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      sf::Color itemColor;
      if (menuKeyIndex == i) itemColor = sf::Color(255, 255, 255, 255);
      else itemColor = sf::Color(180, 180, 180, 255);
      std::ostringstream oss;
      oss << tools::getLabel(inputKeyString[i]) << ": ";
      if (menuKeyIndex == i) oss << tools::getLabel("key_configuration_insert");
      else if (menuKeyIndex > i) oss << keyToString(input[i]);
      write(oss.str(), 16, xAlign, 285 + i * 28, ALIGN_LEFT, itemColor, app, 1, 1);
    }
  }
  else
  {
    // menu
    for (unsigned int i = 0; i < menu->items.size(); i++)
    {
      sf::Color itemColor;
      if (menu->index == i)
      {
        itemColor = sf::Color(255, 255, 255, 255);

        sf::Sprite fairySprite;
        fairySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
        fairySprite.setTextureRect(sf::IntRect( 48 * ((int)(8 *getAbsolutTime()) % 2), 0, 48, 48));
        fairySprite.setPosition(xAlign - 60, 250 + i * yStep + 5 * cos( 6 * getAbsolutTime()));
        app->draw(fairySprite);
      }
      else itemColor = sf::Color(120, 120, 120, 255);

      std::string label = menu->items[i].label;
      if (menu->items[i].id == MenuLanguage)
      {
        std::ostringstream oss;
        oss << label << " : " << tools::getLabel(languageString[parameters.language]);
        label = oss.str();
      }
      else if (menu->items[i].id == MenuPlayerName)
      {
        std::ostringstream oss;
        oss << label << " : " << parameters.playerName;
        if (menuState == MenuStateChangeName && (int)(getAbsolutTime() * 3) % 2 == 0) oss << "_";
        label = oss.str();
      }
      else if (menu->items[i].id == MenuVolumeSound)
      {
        std::ostringstream oss;
        oss << label << " : ";
        if (parameters.soundVolume == 0) oss << "OFF";
        else oss << parameters.soundVolume;
        label = oss.str();
      }
      else if (menu->items[i].id == MenuVolumeMusic)
      {
        std::ostringstream oss;
        oss << label << " : ";
        if (parameters.musicVolume == 0) oss << "OFF";
        else oss << parameters.musicVolume;
        label = oss.str();
      }

      write(label, 21, xAlign, 260 + i * yStep, ALIGN_LEFT, itemColor, app, 1, 1);
    }
    write(menu->items[menu->index].description, 18, xAlign,
          260 + menu->items.size() * yStep + 8, ALIGN_LEFT, sf::Color(60, 80, 220), app, 0, 0);

    // Keys
    if (menuState == MenuStateFirst)
    {
      // displaying the standard key configuration
      int xKeys = 270;
      int yKeys = 380;
      sf::Sprite keysSprite;
      if (parameters.language == 1) // french
        keysSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_KEYS_AZER));
      else
        keysSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_KEYS_QWER));
      keysSprite.setPosition(xKeys, yKeys);
      app->draw(keysSprite);

      // legend
      write(tools::getLabel("keys_move"), 16, xKeys + 190, yKeys + 10, ALIGN_LEFT, sf::Color::White, app, 1, 1);
      write(tools::getLabel("keys_time"), 16, xKeys + 295, yKeys + 14, ALIGN_LEFT, sf::Color::White, app, 1, 1);
      write(tools::getLabel("keys_fire"), 16, xKeys + 360, yKeys + 54, ALIGN_LEFT, sf::Color::White, app, 1, 1);
      write(tools::getLabel("key_spell"), 16, xKeys + 148, yKeys + 184, ALIGN_CENTER, sf::Color::White, app, 1, 1);
      // TODO key interact
      std::ostringstream oss;
      oss << tools::getLabel("keys_select_1") << std::endl << tools::getLabel("keys_select_2");
      write(oss.str(), 16, xKeys + 4, yKeys + 100, ALIGN_LEFT, sf::Color::White, app, 1, 1);
    }
  }

  std::ostringstream oss;
  oss << APP_NAME << " v" << APP_VERSION << "  - 2014 - " << " Seby (code), Vetea (2D art)";
  write(oss.str(), 17, 5, 680, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1);
}

void WitchBlastGame::renderCredits()
{
  app->draw(introScreenSprite);
  if (titleSprite.getPosition().y > 160) titleSprite.move(0, -8);
  else if (titleSprite.getPosition().y < 160) titleSprite.setPosition(SCREEN_WIDTH / 2, 180);
  app->draw(titleSprite);

  // credits
  write(tools::getLabel("credits"), 30, 485, 230, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 1, 1);

  int yCursorInit = 340;
  int yStep = 30;
  int xLeft = 30;
  int xRight = 470;
  int xMarging = 20;

  int yCursor = yCursorInit;
  write("Code", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0,0);
  yCursor += yStep;
  int i = 0;
  while (creditsCode[i] != "END")
  {
    write(creditsCode[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0,0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("2D Art", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0,0);
  yCursor += yStep;
  i = 0;
  while (credits2D[i] != "END")
  {
    write(credits2D[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0,0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("Sound", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0,0);
  yCursor += yStep;
  i = 0;
  while (creditsSound[i] != "END")
  {
    write(creditsSound[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0,0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;


  ////// RIGHT

  yCursor = yCursorInit - yStep;
  write("Music", 22, xRight, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0,0);
  yCursor += yStep;
  i = 0;
  while (creditsMusic[i] != "END")
  {
    write(creditsMusic[i], 19, xRight + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0,0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("Translation", 22, xRight, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0,0);
  yCursor += yStep;
  i = 0;
  while (creditsTranslate[i] != "END")
  {
    write(creditsTranslate[i], 19, xRight + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0,0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;
}

void WitchBlastGame::renderHiScores()
{
  sf::Sprite bgSprite;
  bgSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_INTRO));
  app->draw(bgSprite);

  // hi-scores-title
  write(tools::getLabel("hi_scores"), 30, 485, 20, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 1, 1);

  int x0 = 25;
  int x1 = 70;
  int x2 = 125;
  int x3 = 430;
  int y0 = 130;
  int yStep = 100;
  int xRight = SCREEN_WIDTH / 2;

  sf::RectangleShape line(sf::Vector2f(2, 600));
  line.setPosition(SCREEN_WIDTH / 2, 80);
  app->draw(line);

  for (unsigned int i = 0; i < scores.size() && i < SCORES_MAX; i++)
  {
    int index = i < SCORES_MAX / 2 ? i : i - SCORES_MAX / 2;
    sf::Color color = sf::Color( 220, 220, 220);
    if (scores[i].score == lastScore.score && scores[i].level == lastScore.level && scores[i].name == lastScore.name)
    {
      int fade = 1 + cosf(getAbsolutTime() * 8) * 63;
      color = sf::Color(255, 128 + fade, 255);
    }

    renderPlayer(x1 + (i / 5) * xRight, y0 + yStep * index, scores[i].equip, scores[i].shotType, 1, 0);

    write(intToString(i + 1), 24, x0 + (i / 5) * xRight, y0 + 30 + yStep * index, ALIGN_CENTER, color, app, 1, 1);
    std::stringstream ss;
    ss << scores[i].name << " (" << scores[i].level << ")";
    write(ss.str(), 17, x2 + (i / 5) * xRight, y0 + 30 + yStep * index, ALIGN_LEFT, color, app, 1, 1);
    write(intToString(scores[i].score), 17, x3 + (i / 5) * xRight, y0 + 30 + yStep * index, ALIGN_RIGHT, color, app, 1, 1);
  }
}
void WitchBlastGame::renderInGameMenu()
{
  menuStuct* menu = &menuInGame;

  int xAlign = 290;
  int yAlign = 200;

  {
    // menu
    for (unsigned int i = 0; i < menu->items.size(); i++)
    {
      sf::Color itemColor;
      if (menu->index == i)
      {
        itemColor = sf::Color(255, 255, 255, 255);

        sf::Sprite fairySprite;
        fairySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
        fairySprite.setTextureRect(sf::IntRect( 48 * ((int)(8 *getAbsolutTime()) % 2), 0, 48, 48));
        fairySprite.setPosition(xAlign - 60, yAlign + i * 90 + 5 * cos( 6 * getAbsolutTime()));
        app->draw(fairySprite);
      }
      else itemColor = sf::Color(120, 120, 120, 255);

      std::string label = menu->items[i].label;
      write(label, 23, xAlign, yAlign + 10 + i * 90, ALIGN_LEFT, itemColor, app, 1, 1);
      write(menu->items[i].description, 15, xAlign, yAlign + i * 90 + 50, ALIGN_LEFT, itemColor, app, 0, 0);
    }
  }
}

void WitchBlastGame::startGame()
{
  lastTime = getAbsolutTime();

  prepareIntro();

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
    case gameStateMenu:
      updateMenu();
      break;
    case gameStateIntro:
      updateIntro();
      break;
    case gameStatePlaying:
      updateRunningGame();
      break;
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
  SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);

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

int WitchBlastGame::getEnemyCount()
{
  int n=0;

  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX_COUNT) n++;
  }

  return n;
}

Vector2D WitchBlastGame::getNearestEnemy(float x, float y)
{
  Vector2D target(-100.0f, -100.0f);
  float distanceMin = -1.0f;

  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX_COUNT)
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
    case 0:
      currentMap->setTile(MAP_WIDTH/2, 0, MAP_DOOR);
      break;
    case 1:
      currentMap->setTile(0, MAP_HEIGHT / 2, MAP_DOOR);
      break;
    case 2:
      currentMap->setTile(MAP_WIDTH/2, MAP_HEIGHT - 1, MAP_DOOR);
      break;
    case 3:
      currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, MAP_DOOR);
      break;
    }

  }
  else
    doorEntity[doorId]->setOpen(true);
}

void WitchBlastGame::refreshMap()
{
  // clean the sprites from old map
  EntityManager::getInstance().partialClean(10);

  // if new map, it has to be randomized
  bool generateMap = !(currentFloor->getMap(floorX, floorY)->isVisited());
  currentMap = currentFloor->getAndVisitMap(floorX, floorY);

  // load the map
  dungeonEntity->refreshMap();

  player->setMap(currentMap, TILE_WIDTH, TILE_HEIGHT, 0, 0);
  refreshMinimap();

  if(generateMap)
    this->generateMap();
  else
  {
    if (currentMap->getRoomType() == roomTypeMerchant)
    {
      new PnjEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                    (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT,
                    0);
    }
  }

  // check doors
  checkDoor(0, currentMap->getRoomType(), currentMap->getNeighbourUp());
  checkDoor(1, currentMap->getRoomType(), currentMap->getNeighbourLeft());
  checkDoor(2, currentMap->getRoomType(), currentMap->getNeighbourDown());
  checkDoor(3, currentMap->getRoomType(), currentMap->getNeighbourRight());

  // keystones
  if (currentMap->getNeighbourUp() || currentMap->getRoomType() == roomTypeExit)
  {
    SpriteEntity* keystoneEntity = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TILES),
        (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
        TILE_HEIGHT / 2, 192, 64, 1);
    keystoneEntity->setZ(1000);
    keystoneEntity->setFrame(24);
    keystoneEntity->setType(ENTITY_EFFECT);
  }
  if (currentMap->getNeighbourDown())
  {
    SpriteEntity* keystoneEntity = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TILES),
        (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
        MAP_HEIGHT * TILE_WIDTH - TILE_HEIGHT / 2, 192, 64, 1);
    keystoneEntity->setZ(1000);
    keystoneEntity->setAngle(180);
    keystoneEntity->setFrame(24);
    keystoneEntity->setType(ENTITY_EFFECT);
  }
  if (currentMap->getNeighbourLeft())
  {
    SpriteEntity* keystoneEntity = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TILES),
        TILE_WIDTH / 2,
        (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 192, 64, 1);
    keystoneEntity->setZ(1000);
    keystoneEntity->setAngle(270);
    keystoneEntity->setFrame(24);
    keystoneEntity->setType(ENTITY_EFFECT);
  }
  if (currentMap->getNeighbourRight())
  {
    SpriteEntity* keystoneEntity = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TILES),
        MAP_WIDTH * TILE_WIDTH - TILE_WIDTH / 2,
        (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, 192, 64, 1);
    keystoneEntity->setZ(1000);
    keystoneEntity->setAngle(90);
    keystoneEntity->setFrame(24);
    keystoneEntity->setType(ENTITY_EFFECT);
  }
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
            || currentFloor->getRoom(i, j) == roomTypeTemple
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
        {
          miniMap->setTile(i, j, 7);
          proceedEvent(EventFindBossDoor);
        }
        else if (currentFloor->getRoom(i, j) == roomTypeChallenge)
        {
          miniMap->setTile(i, j, 10);
          proceedEvent(EventFindChallengeDoor);
        }
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
    SoundManager::getInstance().playSound(SOUND_DOOR_CLOSING);
    for (int i=0; i<4; i++)
      doorEntity[i]->closeDoor();
  }
}

void WitchBlastGame::saveMapItems()
{
  currentMap->cleanMapObjects();

  // blood
  std::vector <displayEntityStruct> blood = dungeonEntity->getBlood();
  for (auto particle: blood)
    currentMap->addSprite(ENTITY_BLOOD, particle.frame, particle.x, particle.y, particle.scale);

  // corpses
  std::vector <displayEntityStruct> corpses = dungeonEntity->getCorpses();
  for (auto particle: corpses)
    currentMap->addSprite(ENTITY_CORPSE, particle.frame, particle.x, particle.y, particle.scale);

  EntityManager::EntityList* entityList = EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity* e = *it;
    it++;

    ItemEntity* itemEntity = dynamic_cast<ItemEntity*>(e);
    ChestEntity* chestEntity = dynamic_cast<ChestEntity*>(e);

    if (itemEntity != NULL && !itemEntity->getDying())
    {
      currentMap->addItem(itemEntity->getItemType(), itemEntity->getX(), itemEntity->getY(), itemEntity->getMerchandise());
    } // endif
    else if (chestEntity != NULL && !chestEntity->getDying())
    {
      currentMap->addChest(chestEntity->getChestType(), chestEntity->getOpened(), chestEntity->getX(), chestEntity->getY());
    } // endif
  } // end for
}

void WitchBlastGame::moveToOtherMap(int direction)
{
  // stairs to next level
  if (direction == 8 && currentMap->getRoomType() == roomTypeExit)
  {
    if (player->getPlayerStatus() != PlayerEntity::playerStatusGoingNext)
    {
      player->setLeavingLevel();
      xGame[xGameTypeFade].active = true;
      xGame[xGameTypeFade].param = X_GAME_FADE_OUT;
      xGame[xGameTypeFade].timer = FADE_OUT_DELAY;
      player->setVelocity(Vector2D(0.0f, - INITIAL_PLAYER_SPEED / 2));
    }
  }
  // go to another room
  else
  {
    saveMapItems();
    switch (direction)
    {
    case (4):
      floorX--;
      player->moveTo((MAP_WIDTH * TILE_WIDTH), player->getY());
      player->move(4);
      break;
    case (6):
      floorX++;
      player->moveTo(0, player->getY());
      player->move(6);
      break;
    case (8):
      floorY--;
      player->moveTo(player->getX(), MAP_HEIGHT * TILE_HEIGHT);
      player->move(8);
      break;
    case (2):
      floorY++;
      player->moveTo(player->getX(), 0);
      break;
    }

    refreshMap();
    checkEntering();
    currentMap->restoreMapObjects();

    saveInFight.x = player->getX();
    saveInFight.y = player->getY();
    saveInFight.direction = direction;
    saveMapItems();

    if (!currentMap->isCleared()) saveGame();
  }
}

void WitchBlastGame::onRender()
{
  // clear the view
  app->clear(sf::Color::Black);

  switch (gameState)
  {
  case gameStateInit:
  case gameStateKeyConfig:
  case gameStateMenu:
    renderMenu();
    break;
  case gameStatePlaying:
    renderRunningGame();
    break;
  case gameStateIntro:
    renderIntro();
    break;
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
  dungeonEntity->generateBlood(x, y, bloodColor);
  // double blood if the "Blood Snake3 object is equipped
  if (player->isEquiped(EQUIP_BLOOD_SNAKE)) dungeonEntity->generateBlood(x, y, bloodColor);
}

void WitchBlastGame::addCorpse(float x, float y, int frame)
{
  dungeonEntity->addCorpse(x, y, frame);
}

void WitchBlastGame::showArtefactDescription(enumItemType itemType)
{
  new ArtefactDescriptionEntity(itemType);
}

void WitchBlastGame::generateMap()
{
  saveInFight.monsters.clear();

  if (currentMap->getRoomType() == roomTypeStandard)
    generateStandardMap();
  else if (currentMap->getRoomType() == roomTypeBonus)
  {
    currentMap->setCleared(true);
    Vector2D v = currentMap->generateBonusRoom();
    int bonusType = getRandomEquipItem(false, false);

    if (items[FirstEquipItem + bonusType].familiar > FamiliarNone)
    {
      new ChestEntity(v.x, v.y, ChestFairy + items[FirstEquipItem + bonusType].familiar, false);
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
    else if (level < 6)
    {
      findPlaceMonsters(EnemyTypeRat, 5);
      findPlaceMonsters(EnemyTypeBat, 5);
      for (int i = 2; i < level; i++)
      {
        if (rand()%2 == 0)findPlaceMonsters(EnemyTypeImpBlue, 1);
        else findPlaceMonsters(EnemyTypeImpRed, 1);
      }
    }
    else
    {
      findPlaceMonsters(EnemyTypeZombie, 5);
      findPlaceMonsters(EnemyTypeBat, 5);
      for (int i = 5; i < level; i++)
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
      (MAP_WIDTH / 2 - 3) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item1->setMerchandise(true);

    ItemEntity* item3 = new ItemEntity(
      ItemHealthSmall,
      (MAP_WIDTH / 2 + 3) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item3->setMerchandise(true);

    int bonusType = getRandomEquipItem(true, true);
    ItemEntity* item2 = new ItemEntity(
      (enumItemType)(FirstEquipItem + bonusType),
      (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT);
    item2->setMerchandise(true);

    new PnjEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                  (MAP_HEIGHT / 2 - 2) * TILE_HEIGHT,
                  0);

    currentMap->setCleared(true);
    proceedEvent(EventFindShop);
  }
  else if (currentMap->getRoomType() == roomTypeChallenge)
  {
    currentMap->generateRoomWithoutHoles(0);

    if (challengeLevel == 1)
    {
      addMonster(EnemyTypeBubble,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    }
    else
    {
      addMonster(EnemyTypeBubble,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 80,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      addMonster(EnemyTypeBubble,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 80,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
    }
    playMusic(MusicChallenge);
  }
  else if (currentMap->getRoomType() == roomTypeBoss)
  {
    currentMap->generateRoomWithoutHoles(0);

    if (level == 1)
    {
      findPlaceMonsters(EnemyTypeRat, 2);
      addMonster(EnemyTypeButcher,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoButcher);
    }

    else if (level == 2)
    {
      addMonster(EnemyTypeSlimeBoss,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoGiantSlime);
    }

    else if (level == 3)
    {
      addMonster(EnemyTypeCyclops,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoCyclops);
    }

    else if (level == 4)
    {
      addMonster(EnemyTypeRatKing,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoWererat);
    }

    else if (level == 5)
    {
      addMonster(EnemyTypeSpiderGiant,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoGiantSpiderBefore);
    }
    else if (level == 6)
    {
      addMonster(EnemyTypeFrancky,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoFranky);
    }
    else if (level == 7)
    {
      // TODO
      GiantSpiderEntity* b1 = new GiantSpiderEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 100,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b1->setLabelDy(10);

      FranckyEntity* b2 = new FranckyEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b2->setLabelDy(-530);

      KingRatEntity* b3 = new KingRatEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 100,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b3->setLabelDy(-20);
    }

    else // level > 6
    {
      GiantSpiderEntity* b1 = new GiantSpiderEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 100,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b1->setLabelDy(10);

      GiantSlimeEntity* b2 = new GiantSlimeEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b2->setLabelDy(-510);

      KingRatEntity* b3 = new KingRatEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 100,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b3->setLabelDy(-20);

      CyclopsEntity* b4 = new CyclopsEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b4->setLabelDy(-540);

      findPlaceMonsters(EnemyTypeCauldron, 2);
      findPlaceMonsters(EnemyTypeImpBlue, 4);
    }

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

    new ChestEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                    (TILE_HEIGHT * MAP_HEIGHT * 0.5f),
                    ChestExit, false);
  }
  else if (currentMap->getRoomType() == roomTypeTemple)
  {
    currentMap->generateTempleRoom();
    currentMap->setCleared(true);
  }
  else  // "normal" room
    currentMap->randomize(currentMap->getRoomType());
}

void WitchBlastGame::write(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow = 0, int yShadow = 0)
{
  std::basic_string<sf::Uint32> utf32String;
  utf8::utf8to32(str.begin(), str.end(), std::back_inserter(utf32String));
  myText.setString(utf32String);
  myText.setCharacterSize(size);

  float xFont = x;

  if (align == ALIGN_CENTER)
    xFont = x - myText.getLocalBounds().width / 2;
  else if (align == ALIGN_RIGHT)
    xFont = x - myText.getLocalBounds().width;

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
  StructMonster monster;
  monster.id = monsterType;
  monster.x = xm;
  monster.y = ym;
  saveInFight.monsters.push_back(monster);
  switch (monsterType)
  {
  case EnemyTypeRat:
    new RatEntity(xm, ym - 2, RatEntity::RatTypeNormal, false);
    proceedEvent(EventMeetRatsOrBats);
    break;
  case EnemyTypeRatGreen:
    new GreenRatEntity(xm, ym - 2);
    proceedEvent(EventMeetRatsOrBats);
    break;
  case EnemyTypeRatBlack:
    new BlackRatEntity(xm, ym - 5, BlackRatEntity::RatBlackTypeNormal);
    break;
  case EnemyTypeRatHelmet:
    new RatEntity(xm, ym - 2, RatEntity::RatTypeHelmet, false);
    break;
  case EnemyTypeRatBlackHelmet:
    new BlackRatEntity(xm, ym - 5, BlackRatEntity::RatBlackTypeHelmet);
    break;
  case EnemyTypeBat:
    new BatEntity(xm, ym, false);
    proceedEvent(EventMeetRatsOrBats);
    break;
  case EnemyTypeSnake:
    new SnakeEntity(xm, ym, SnakeEntity::SnakeTypeNormal, false);
    proceedEvent(EventMeetSnakes);
    break;
  case EnemyTypeSnakeBlood:
    new SnakeEntity(xm, ym, SnakeEntity::SnakeTypeBlood, false);
    proceedEvent(EventMeetSnakes);
    break;
  case EnemyTypeEvilFlower:
    new EvilFlowerEntity(xm, ym - 2, FlowerTypeStandard);
    break;
  case EnemyTypeEvilFlowerIce:
    new EvilFlowerEntity(xm, ym - 2, FlowerTypeIce);
    break;
  case EnemyTypeSlime:
    new SlimeEntity(xm, ym, SlimeTypeStandard, false);
    break;
  case EnemyTypeImpRed:
    new ImpEntity(xm, ym, ImpEntity::ImpTypeRed);
    break;
  case EnemyTypeImpBlue:
    new ImpEntity(xm, ym, ImpEntity::ImpTypeBlue);
    break;
  case EnemyTypeSlimeRed:
    new SlimeEntity(xm, ym, SlimeTypeRed, false);
    break;
  case EnemyTypeSlimeBlue:
    new SlimeEntity(xm, ym, SlimeTypeBlue, false);
    break;
  case EnemyTypeSlimeViolet:
    new SlimeEntity(xm, ym, SlimeTypeViolet, false);
    break;
  case EnemyTypePumpkin:
    new PumpkinEntity(xm, ym, false);
    break;
  case EnemyTypeWitch:
    new WitchEntity(xm, ym, WitchEntity::WitchTypeNormal);
    proceedEvent(EventMeetWitches);
    break;
  case EnemyTypeWitchRed:
    new WitchEntity(xm, ym, WitchEntity::WitchTypeRed);
    proceedEvent(EventMeetWitches);
    break;
  case EnemyTypeCauldron:
    new CauldronEntity(xm, ym);
    break;
  case EnemyTypeSpiderEgg:
    new SpiderEggEntity(xm, ym, false);
    break;
  case EnemyTypeSpiderLittle:
    new LittleSpiderEntity(xm, ym, false);
    break;
  case EnemyTypeGhost:
    new GhostEntity(xm, ym);
    break;
  case EnemyTypeZombie:
    new ZombieEntity(xm, ym, false);
    break;
  case EnemyTypeZombieDark:
    new ZombieDarkEntity(xm, ym);
    break;

  case EnemyTypeSpiderEgg_invocated:
    new SpiderEggEntity(xm, ym, true);
    break;
  case EnemyTypeSpiderLittle_invocated:
    new LittleSpiderEntity(xm, ym, true);
    break;

  case EnemyTypeBubble:
    new BubbleEntity(xm, ym, 0);
    break;
  case EnemyTypeButcher:
    new ButcherEntity(xm, ym);
    break;
  case EnemyTypeSlimeBoss:
    new GiantSlimeEntity(xm, ym);
    break;
  case EnemyTypeCyclops:
    new CyclopsEntity(xm, ym);
    break;
  case EnemyTypeRatKing:
    new KingRatEntity(xm, ym);
    break;
  case EnemyTypeSpiderGiant:
    new GiantSpiderEntity(xm, ym);
    break;
  case EnemyTypeFrancky:
    new FranckyEntity(xm, ym);
    break;

  default:
    std::cout << "[WARNING] Enemy (" << monsterType << ") not handled in switch.\n";
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

    int watchdog = 200;

    while (!bOk && watchdog > 0)
    {
      bOk = true;
      watchdog--;
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
      if (bOk && isMonsterFlying && !currentMap->isFlyable(xm, ym))
      {
        bOk = false;
      }
      if (bOk)
      {
        xMonster = xm * TILE_WIDTH + TILE_WIDTH * 0.5f;
        yMonster = ym * TILE_HEIGHT+ TILE_HEIGHT * 0.5f;

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
  saveInFight.monsters.clear();
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

    if (itemOk && i == EQUIP_BOOK_DUAL && player->isEquiped(EQUIP_BOOK_DUAL_QUICK))
      itemOk = false;

    if (itemOk && i == EQUIP_BOOK_DUAL_QUICK && player->isEquiped(EQUIP_BOOK_DUAL))
      itemOk = false;

    if (itemOk && noFairy && items[eq].familiar != FamiliarNone) itemOk = false;

    if (itemOk)
    {
      int n = 0;
      switch (items[eq].rarity)
      {
      case RarityCommon:
        n = 4;
        break;
      case RarityUncommon:
        n = 2;
        break;
      case RarityRare:
        n = 1;
        break;
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

enumItemType WitchBlastGame::getItemSpell()
{
  enumCastSpell n = (enumCastSpell)(rand() % SPELL_MAX);
  while (player->getActiveSpell().spell == n) n = (enumCastSpell)(rand() % SPELL_MAX);

  enumItemType item = ItemMagicianHat;
  switch (n)
  {
  case SpellNone:
  case SpellTeleport:
    item = ItemSpellTeleport;
    break;
  case SpellSlimeExplode:
    item = ItemSpellSlimeExplode;
    break;
  case SpellFireball:
    item = ItemSpellFireball;
    break;
  case SpellFreeze:
    item = ItemSpellFreeze;
    break;
  case SpellEarthquake:
    item = ItemSpellEarthquake;
    break;
  case SpellProtection:
    item = ItemSpellProtection;
    break;
  case SpellWeb:
    item = ItemSpellWeb;
    break;
  }
  return item;
}

void WitchBlastGame::generateChallengeBonus(float x, float y)
{
  // loot
  if (player->getActiveSpell().spell == SpellNone || rand() % 2 == 0)
  {
    ItemEntity* newItem = new ItemEntity(getItemSpell(), x, y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);

    ItemEntity* healthItem1 = new ItemEntity(ItemHealthVerySmall, x, y);
    healthItem1->setVelocity(Vector2D(100.0f + rand()% 250));
    healthItem1->setViscosity(0.96f);

    ItemEntity* healthItem2 = new ItemEntity(ItemHealthVerySmallPoison, x, y);
    healthItem2->setVelocity(Vector2D(100.0f + rand()% 250));
    healthItem2->setViscosity(0.96f);
  }
  else
  {
    ItemEntity* newItem = new ItemEntity(ItemBonusHealth, x, y);
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);

    int gold = 2 + rand() % 9;
    for (int i = 0; i < gold; i++)
    {
      ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
      newItem->setVelocity(Vector2D(90.0f + rand()% 150));
      newItem->setViscosity(0.96f);
    }
  }
}

void WitchBlastGame::checkInteraction()
{
  interaction.active = false;

  if (player->getPlayerStatus() != PlayerEntity::playerStatusPlaying) return;

  if (currentMap->getRoomType() == roomTypeTemple)
  {
    int divinity = currentMap->getDivinity(player->getX() / TILE_WIDTH, player->getZ() / TILE_HEIGHT);
    if (divinity > -1)
    {
      interaction.active = true;
      interaction.type = InteractionTypeTemple;
      interaction.id = divinity;
      std::stringstream ss;
      if (player->getDivinity().divinity == divinity)
      {
        ss << tools::getLabel("interact_donate");
        if (player->getGold() < 10)
        {
          ss << " ";
          ss << tools::getLabel("interact_donate_fail");
        }
      }
      else
      {
        ss << tools::getLabel("interact_worship");
        ss << " ";
        ss << tools::getLabel(divinityLabel[divinity]);
      }

      interaction.label = ss.str();
    }
  }
}

void WitchBlastGame::verifyDoorUnlocking()
{
  int collidingDirection = (player->getCollidingDirection());

  if (collidingDirection > 0 && currentMap->isCleared() && !bossRoomOpened && player->isEquiped(EQUIP_BOSS_KEY))
  {
    int xt = (player->getX()) / TILE_WIDTH;
    int yt = (player->getY()) / TILE_HEIGHT;

    if (yt <= 1 && xt >= MAP_WIDTH / 2 - 1 && xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourUp() == 2)
    {
      doorEntity[0]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, 0, 0);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (yt >= MAP_HEIGHT - 2 && xt >= MAP_WIDTH / 2 - 1 &&xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourDown() == 2)
    {
      doorEntity[2]->openDoor();
      currentMap->setTile(MAP_WIDTH / 2, MAP_HEIGHT - 1, 0);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt <= 1 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourLeft() == 2)
    {

      doorEntity[1]->openDoor();
      currentMap->setTile(0, MAP_HEIGHT / 2, 0);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt >= MAP_WIDTH - 2 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourRight() == 2)
    {
      doorEntity[3]->openDoor();
      currentMap->setTile(MAP_WIDTH - 1, MAP_HEIGHT / 2, 0);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
  }
}

void WitchBlastGame::playMusic(musicEnum musicChoice)
{
  music.stop();

  if (parameters.musicVolume <= 0) return;

  music.setLoop(true);
  music.setVolume(parameters.musicVolume);
  bool ok = false;

  switch (musicChoice)
  {
  case MusicDungeon:

    if (rand() % 2 == 0)
    {
      ok = music.openFromFile("media/sound/game_Fantasy_Theme_Of_Elvarim.ogg");
    }
    else
    {
      ok = music.openFromFile("media/sound/game_Marching_United.ogg");
    }

    break;

  case MusicEnding:
    ok = music.openFromFile("media/sound/ending_Music_Is_His_Only_Friend.ogg");
    music.setVolume(parameters.musicVolume * 50 / 100);
    break;

  case MusicBoss:
    if (rand() % 2 == 0)
    {
      ok = music.openFromFile("media/sound/boss_The_Spider_Machine.ogg");
    }
    else
    {
      ok = music.openFromFile("media/sound/boss_Pub_Stomp_Deluxe.ogg");
    }

    break;

  case MusicChallenge:
    ok = music.openFromFile("media/sound/challenge_Under_Siege.ogg");
    break;

  case MusicIntro:
    ok = music.openFromFile("media/sound/menu_Our_Ship_To_Candletown.ogg");
    music.setVolume(parameters.musicVolume * 60 / 100);
    break;
  }

  if (ok)
    music.play();
}

void WitchBlastGame::updateMusicVolume()
{
  if (music.getStatus() == sf::Music::Playing)
  {
    if (parameters.musicVolume == 0)
      music.stop();
    else
      music.setVolume(parameters.musicVolume * 60 / 100);
  }
  else
  {
    if (parameters.musicVolume > 0)
    {
      bool ok = music.openFromFile("media/sound/menu_Our_Ship_To_Candletown.ogg");
      music.setVolume(parameters.musicVolume * 60 / 100);
      if (ok) music.play();
    }
  }
}

void WitchBlastGame::makeShake(float duration)
{
  xGame[xGameTypeShake].active = true;
  xGame[xGameTypeShake].timer = duration;
}

void WitchBlastGame::makeColorEffect(int color, float duration)
{
  xGame[xGameTypeFadeColor].active = true;
  xGame[xGameTypeFadeColor].param = color;
  xGame[xGameTypeFadeColor].timer = duration;
  xGame[xGameTypeFadeColor].duration = duration;
}

void WitchBlastGame::saveGame()
{
  if (player->getPlayerStatus() == PlayerEntity::playerStatusAcquire)
    player->acquireItemAfterStance();
  std::ofstream file(SAVE_FILE.c_str(), std::ios::out | std::ios::trunc);

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
         << std::endl;

    if (now->tm_hour <= 9) file << "0";

    file << (now->tm_hour) << ':';

    if (now->tm_min <= 9) file << "0";
    file << (now->tm_min) << std::endl;

    // floor
    file << level << " " << challengeLevel << std::endl;

    // game age
    file << (int)gameTime << std::endl;

    // player equip
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++) file << player->isEquiped(i) << " ";
    file << std::endl;
    file << player->getShotType() << std::endl;

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
    if (currentMap->isCleared())
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
                int tile = currentFloor->getMap(i, j)->getTile(k, l);
                if (tile == MAP_DOOR && !(k > 0 && k < MAP_WIDTH - 1 && l > 0 && l < MAP_HEIGHT - 1)) tile = 0;
                file << tile << " ";
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

    // fight ?
    if (currentMap->isCleared())
    {
      file << false << std::endl;
    }
    else
    {
      file << true << std::endl;
      file << saveInFight.x << " " << saveInFight.y << " " << saveInFight.direction << std::endl;

      file << saveInFight.monsters.size();
      for (auto monster : saveInFight.monsters)
        file << " " << monster.id << " " << monster.x  << " " << monster.y;
      file << std::endl;
    }

    // player
    file << player->getHp() << " " << player->getHpMax() << " " << player->getGold() << std::endl;
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++) file << player->isEquiped(i) << " ";
    file << std::endl;
    file << player->getX() << " " << player->getY() << std::endl;
    file << player->getShotIndex();
    for (i = 0; i < SPECIAL_SHOT_SLOTS; i++) file << " " << player->getShotType(i) << std::endl;
    file << player->getActiveSpell().spell << std::endl;
    // divinity
    file << player->getDivinity().divinity << " " << player->getDivinity().piety << " "
      << player->getDivinity().level << " " << player->getDivinity().interventions << std::endl;
    // events
    for (i = 0; i < NB_EVENTS; i++) file << worldEvent[i] << " ";
    file.close();
  }
  else
  {
    std::cerr << "[ERROR] Saving the game..." << std::endl;
  }
}

bool WitchBlastGame::loadGame()
{
  std::ifstream file(SAVE_FILE.c_str(), std::ios::in);

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
    file >> challengeLevel;
    file >> gameTime;

    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    {
      bool eq;
      file >> eq;
    }
    file >> n;

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

    // fight ?
    file >> saveInFight.isFight;
    if (saveInFight.isFight)
    {
      currentMap->setCleared(false);
      file >> saveInFight.x;
      file >> saveInFight.y;
      file >> saveInFight.direction;

      file >> n;
      for (i = 0; i < n; i++)
      {
        StructMonster monster;
        file >> j;
        monster.id = (enemyTypeEnum)j;
        file >> monster.x;
        file >> monster.y;
        saveInFight.monsters.push_back(monster);
      }
    }

    // player
    int hp, hpMax, gold;
    file >> hp >> hpMax >> gold;
    player = new PlayerEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                              (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
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

    if (saveInFight.isFight)
    {
      x = saveInFight.x;
      y = saveInFight.y;
      player->move(saveInFight.direction);
      player->setEntering();
    }
    player->moveTo(x, y);

    file >> n;
    player->setShotIndex(n);

    for (i = 0; i < SPECIAL_SHOT_SLOTS; i++)
    {
      file >> n;
      player->setShotType(i, (enumShotType)n);
    }

    file >> n;
    player->setActiveSpell((enumCastSpell)n, saveInFight.isFight);

    // divinity
    {
      int divinityId, piety, divLevel, interventions;
      file >> divinityId;
      file >> piety;
      file >> divLevel;
      file >> interventions;
      player->loadDivinity(divinityId, piety, divLevel, interventions);
    }

    // events
    for (i = 0; i < NB_EVENTS; i++)
    {
      bool event;
      file >> event;
      worldEvent[i] = event;
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

  std::ifstream file(SAVE_FILE.c_str(), std::ios::in);

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

      int challengeLevel;
      file >> challengeLevel;
      file >> saveHeader.gameTime;

      for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
        file >> equipToDisplay[i];
      file >> saveHeader.shotType;
    }
  }
  else
  {
    saveHeader.ok = false;
  }
  return saveHeader;
}

void WitchBlastGame::saveGameData()
{
  // TODO
  std::ofstream file(SAVE_DATA_FILE.c_str(), std::ios::out | std::ios::trunc);

  if (file)
  {
    // version (for compatibility check)
    file << SAVE_VERSION << std::endl;

    // tuto
    for (int i = 0; i < NB_MESSAGES; i++)
    {
      messageStruct msg = getMessage((EnumMessages)i);
      if (msg.messageType == MessageTypeTutorial)
      {
        file << gameMessagesToSkip[i] << " ";
      }
      else file << "0 ";
    }
    file << std::endl;

    file.close();
  }
  else
  {
    std::cout << "[ERROR] Impossible to open save data file.\n";
  }
}

void WitchBlastGame::loadGameData()
{
  std::ifstream file(SAVE_DATA_FILE.c_str(), std::ios::in);

  if (file)
  {
    // version
    std::string v;
    file >> v;

    if (v != SAVE_VERSION)
    {
      file.close();
      remove(SAVE_DATA_FILE.c_str());
      return;
    }

    // tuto
    for (int i = 0; i < NB_MESSAGES; i++)
    {
      file >> gameMessagesToSkip[i];
    }
  }
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

  // parameters
  newMap["language"] = intToString(parameters.language);
  newMap["player_name"] = parameters.playerName;

  // audio volume
  newMap["volume_sound"] = intToString(parameters.soundVolume);
  newMap["volume_music"] = intToString(parameters.musicVolume);

  newMap["zoom_enabled"] = parameters.zoom ? "1" : "0";
  newMap["vsync_enabled"] = parameters.vsync ? "1" : "0";
  newMap["blood_spreading"] = parameters.bloodSpread ? "1" : "0";

  // Keys
  newMap["keyboard_move_up"] = intToString(input[KeyUp]);
  newMap["keyboard_move_down"] = intToString(input[KeyDown]);
  newMap["keyboard_move_left"] = intToString(input[KeyLeft]);
  newMap["keyboard_move_right"] = intToString(input[KeyRight]);
  newMap["keyboard_fire_up"] = intToString(input[KeyFireUp]);
  newMap["keyboard_fire_down"] = intToString(input[KeyFireDown]);
  newMap["keyboard_fire_left"] = intToString(input[KeyFireLeft]);
  newMap["keyboard_fire_right"] = intToString(input[KeyFireRight]);
  newMap["keyboard_spell"] = intToString(input[KeySpell]);
  newMap["keyboard_interact"] = intToString(input[KeyInteract]);
  newMap["keyboard_fire"] = intToString(input[KeyFire]);
  newMap["keyboard_time_control"] = intToString(input[KeyTimeControl]);
  newMap["keyboard_fire_select"] = intToString(input[KeyFireSelect]);

  config.saveToFile(CONFIG_FILE, newMap);
}

void WitchBlastGame::configureFromFile()
{
  // default
  parameters.language = 0;  // english
  parameters.zoom = true;
  parameters.vsync = true;
  parameters.bloodSpread = true;
  parameters.musicVolume = 100;
  parameters.soundVolume = 80;
  parameters.playerName = "";

  input[KeyUp]    = sf::Keyboard::W;
  input[KeyDown]  = sf::Keyboard::S;
  input[KeyLeft]  = sf::Keyboard::A;
  input[KeyRight] = sf::Keyboard::D;
  input[KeyFireUp]    = sf::Keyboard::Up;
  input[KeyFireDown]  = sf::Keyboard::Down;
  input[KeyFireLeft]  = sf::Keyboard::Left;
  input[KeyFireRight] = sf::Keyboard::Right;
  input[KeyFire] = sf::Keyboard::RControl;
  input[KeySpell] = sf::Keyboard::Space;
  input[KeyInteract] = sf::Keyboard::E;
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
  addKey(KeySpell, "keyboard_spell");
  addKey(KeyTimeControl, "keyboard_time_control");
  addKey(KeyFireSelect, "keyboard_fire_select");

  int i = config.findInt("language");
  if (i >= 0) parameters.language = i;

  i = config.findInt("volume_sound");
  if (i >= 0) parameters.soundVolume = i;
  i = config.findInt("volume_music");
  if (i >= 0) parameters.musicVolume = i;

  i = config.findInt("vsync_enabled");
  if (i >= 0) parameters.vsync = i;
  i = config.findInt("zoom_enabled");
  if (i >= 0) parameters.zoom = i;
  i = config.findInt("blood_spreading");
  if (i >= 0) parameters.bloodSpread = i;

  std::string playerName = config.findString("player_name");
  if (playerName.size() > 0) parameters.playerName = playerName;

  tools::setLanguage(languageString[parameters.language]);
}

parameterStruct WitchBlastGame::getParameters()
{
  return parameters;
}

void WitchBlastGame::buildMenu(bool rebuild)
{
  menuMain.items.clear();
  menuConfig.items.clear();
  menuFirst.items.clear();

  if (!rebuild)
  {
    menuState = MenuStateMain;
    menuMain.age = 0.0f;
    menuConfig.age = 0.0f;
    menuConfig.index = 0;
  }

  saveHeader = loadGameHeader();

  menuItemStuct itemName;
  itemName.label = tools::getLabel("player_name");
  itemName.description = tools::getLabel("player_name_desc");
  itemName.id = MenuPlayerName;
  menuMain.items.push_back(itemName);

  if (saveHeader.ok)
  {
    menuItemStuct itemStart;
    itemStart.label = tools::getLabel("start_new_game");
    itemStart.description = tools::getLabel("start_desc");

    itemStart.id = MenuStartNew;
    menuMain.items.push_back(itemStart);

    menuItemStuct itemLoad;
    itemStart.label = tools::getLabel("restore");

    std::ostringstream oss;
    oss << saveHeader.date << " " << tools::getLabel("at") << " " << saveHeader.time << " - " << tools::getLabel("level") << " " << saveHeader.level;

    itemStart.description = oss.str();
    itemStart.id = MenuStartOld;
    menuMain.items.push_back(itemStart);

    if (!rebuild) menuMain.index = 2;
  }
  else
  {
    menuItemStuct itemStart;
    itemStart.label = tools::getLabel("start_new_game");
    itemStart.description = tools::getLabel("begin_journey");

    itemStart.id = MenuStartNew;
    menuMain.items.push_back(itemStart);

    if (!rebuild) menuMain.index = 1;
  }

  if (parameters.playerName.compare("") == 0 && menuState == MenuStateMain)
  {
    menuMain.index = 0;
    menuState = MenuStateChangeName;
  }

  menuItemStuct itemConfig;
  itemConfig.label = tools::getLabel("configure_game");
  itemConfig.description = tools::getLabel("configure_game_desc");
  itemConfig.id = MenuConfig;
  menuMain.items.push_back(itemConfig);

  if (scores.size() > 0)
  {
    menuItemStuct itemHiScores;
    itemHiScores.label = tools::getLabel("hi_scores");
    itemHiScores.description = tools::getLabel("hi_scores_desc");
    itemHiScores.id = MenuHiScores;
    menuMain.items.push_back(itemHiScores);
  }


  menuItemStuct itemCredits;
  itemCredits.label = tools::getLabel("credits");
  itemCredits.description = tools::getLabel("credits_desc");
  itemCredits.id = MenuCredits;
  menuMain.items.push_back(itemCredits);

  menuItemStuct itemExit;
  itemExit.label = tools::getLabel("exit_game");
  itemExit.description = tools::getLabel("return_to_desktop");
  itemExit.id = MenuExit;
  menuMain.items.push_back(itemExit);

  // configuration
  menuItemStuct itemKeys;
  itemKeys.label = tools::getLabel("config_keys");
  itemKeys.description = tools::getLabel("redef_input");
  itemKeys.id = MenuKeys;
  menuConfig.items.push_back(itemKeys);

  menuItemStuct itemLanguage;
  itemLanguage.label = tools::getLabel("config_lang");
  itemLanguage.description = tools::getLabel("config_lang_desc");
  itemLanguage.id = MenuLanguage;
  menuConfig.items.push_back(itemLanguage);

  menuItemStuct itemVolumeSound;
  itemVolumeSound.label = tools::getLabel("volume_sound");
  itemVolumeSound.description = tools::getLabel("volume_sound_desc");
  itemVolumeSound.id = MenuVolumeSound;
  menuConfig.items.push_back(itemVolumeSound);

  menuItemStuct itemVolumeMusic;
  itemVolumeMusic.label = tools::getLabel("volume_music");
  itemVolumeMusic.description = tools::getLabel("volume_sound_desc");
  itemVolumeMusic.id = MenuVolumeMusic;
  menuConfig.items.push_back(itemVolumeMusic);

  menuItemStuct itemTutoReset;
  itemTutoReset.label = tools::getLabel("tuto_reset");
  itemTutoReset.description = tools::getLabel("tuto_reset_desc");
  itemTutoReset.id = MenuTutoReset;
  menuConfig.items.push_back(itemTutoReset);

  menuItemStuct itemConfigBack;
  itemConfigBack.label = tools::getLabel("config_back");
  itemConfigBack.description = tools::getLabel("config_back_desc");
  itemConfigBack.id = MenuConfigBack;
  menuConfig.items.push_back(itemConfigBack);

  //first time screen
  menuFirst.items.push_back(itemLanguage);
}

void WitchBlastGame::buildInGameMenu()
{
  menuInGame.items.clear();

  menuItemStuct itemContinue;
  itemContinue.label = tools::getLabel("menu_continue");
  itemContinue.description = tools::getLabel("menu_continue_desc");
  itemContinue.id = MenuContinue;
  menuInGame.items.push_back(itemContinue);

  menuItemStuct itemSaveQuit;
  itemSaveQuit.label = tools::getLabel("menu_save_quit");
  itemSaveQuit.description = tools::getLabel("menu_save_quit_desc");
  itemSaveQuit.id = MenuSaveAndQuit;
  menuInGame.items.push_back(itemSaveQuit);

  menuItemStuct itemQuit;
  itemQuit.label = tools::getLabel("menu_quit");
  itemQuit.description = tools::getLabel("menu_quit_desc");
  itemQuit.id = MenuExit;
  menuInGame.items.push_back(itemQuit);

  menuInGame.index = 0;
}

void WitchBlastGame::resetKilledEnemies()
{
  for (int i = 0; i < NB_ENEMY; i++) killedEnemies[i] = 0;
}

void WitchBlastGame::addKilledEnemy(enemyTypeEnum enemyType)
{
  if(!player->isDead())
  {
    if (enemyType == NB_ENEMY)
      std::cout << "[ERROR] No enemy type";
    else
    {
      killedEnemies[enemyType]++;
      player->sacrifice(enemyType);
    }
  }
}

void WitchBlastGame::registerLanguage()
{
  // default keys
  if (parameters.language == 1)
  {
    // french keyboard
    input[KeyUp]    = sf::Keyboard::Z;
    input[KeyLeft]  = sf::Keyboard::Q;
  }
  else
  {
    // QWERT / QWERTZ keyboard
    input[KeyUp]    = sf::Keyboard::W;
    input[KeyLeft]  = sf::Keyboard::A;
  }

  input[KeyDown]  = sf::Keyboard::S;
  input[KeyRight] = sf::Keyboard::D;
  input[KeyFireUp]    = sf::Keyboard::Up;
  input[KeyFireDown]  = sf::Keyboard::Down;
  input[KeyFireLeft]  = sf::Keyboard::Left;
  input[KeyFireRight] = sf::Keyboard::Right;
  input[KeyFire] = sf::Keyboard::RControl;
  input[KeyFireSelect] = sf::Keyboard::Tab;
  input[KeyTimeControl] = sf::Keyboard::RShift;
  input[KeySpell]  = sf::Keyboard::Space;

  saveConfigurationToFile();
}

void WitchBlastGame::testAndAddMessageToQueue(EnumMessages type)
{
  if (gameMessagesToSkip[type] == false)
  {
    if (messagesQueue.empty()) SoundManager::getInstance().playSound(SOUND_MESSAGE);

    messageStruct msg = getMessage(type);
    messagesQueue.push(msg);
    if (msg.messageType == MessageTypeTutorial)
    {
      gameMessagesToSkip[type] = true;
      saveGameData();
    }
  }
}

void WitchBlastGame::initEvents()
{
  for (int i = 0; i < NB_EVENTS; i++)
    worldEvent[i] = false;
}

void WitchBlastGame::proceedEvent(EnumWorldEvents event)
{
  if (worldEvent[event]) return;

  worldEvent[event] = true;

  testAndAddMessageToQueue(eventToMessage[event]);
}

void WitchBlastGame::renderPlayer(float x, float y,
                                  bool equip[NUMBER_EQUIP_ITEMS], int shotType,
                                  int frame, int spriteDy)
{
  bool isMirroring = false;
  sf::Sprite sprite;

  if (equip[EQUIP_FAIRY_FIRE])
  {
    sprite.setPosition(x - 35, y -25);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 144, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_TARGET])
  {
    sprite.setPosition(x - 15, y -25);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 216, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY])
  {
    sprite.setPosition(x - 40, y -10);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 0, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_ICE])
  {
    sprite.setPosition(x - 10, y -10);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 72, 48, 60));
    app->draw(sprite);
  }


  sprite.setPosition(x, y);
  sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_BASE));

  int width = 42;
  int height = 80;

  // body
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( frame * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( frame * width, spriteDy * height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_MAGICIAN_ROBE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_BROOCH_FINESSE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_RAGE_AMULET])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_LEATHER_BELT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_DISPLACEMENT_GLOVES])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // hat
  if (equip[EQUIP_MAGICIAN_HAT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // boots
  if (equip[EQUIP_LEATHER_BOOTS])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // staff
  int frameDx = equip[EQUIP_MAHOGANY_STAFF] ? 27 : 3;
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width, spriteDy * height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (30 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (30 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_REAR_SHOT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (33 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (33 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

// shot type
  if (shotType != ShotTypeStandard)
  {
    switch (shotType)
    {
    case ShotTypeIce:
      sprite.setColor(sf::Color(100, 220, 255, 255));
      break;

    case ShotTypeStone:
      sprite.setColor(sf::Color(120, 120, 150, 255));
      break;

    case ShotTypeLightning:
      sprite.setColor(sf::Color(255, 255, 0, 255));
      break;

    case ShotTypeIllusion:
      sprite.setColor(sf::Color(240, 180, 250, 255));
      break;

    case ShotTypeStandard:
      sprite.setColor(sf::Color(255, 255, 255, 0));
      break;

    case ShotTypeFire:
      sprite.setColor(sf::Color(255, 180, 0, 255));
      break;

    default:
      break;
    }

    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (36 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (36 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
}

std::string WitchBlastGame::enemyToString(enemyTypeEnum enemyType)
{
  std::string value = "unknown";

  switch (enemyType)
  {
  case EnemyTypeBat_invocated:
  case EnemyTypeBat:
    value = "enemy_type_bat";
    break;
  case EnemyTypeRat_invocated:
  case EnemyTypeRat:
    value = "enemy_type_rat";
    break;
  case EnemyTypeRatBlack:
    value = "enemy_type_rat_black";
    break;
  case EnemyTypeRatHelmet_invocated:
  case EnemyTypeRatHelmet:
    value = "enemy_type_rat_helmet";
    break;
  case EnemyTypeRatBlackHelmet:
    value = "enemy_type_rat_black_helmet";
    break;
  case EnemyTypeEvilFlower:
    value = "enemy_type_evil_flower";
    break;
  case EnemyTypeEvilFlowerIce:
    value = "enemy_type_evil_flower_ice";
    break;
  case EnemyTypeSnake_invocated:
  case EnemyTypeSnake:
    value = "enemy_type_snake";
    break;
  case EnemyTypeSnakeBlood_invocated:
  case EnemyTypeSnakeBlood:
    value = "enemy_type_snake_blood";
    break;
  case EnemyTypeSlime_invocated:
  case EnemyTypeSlime:
    value = "enemy_type_slime";
    break;
  case EnemyTypeSlimeRed_invocated:
  case EnemyTypeSlimeRed:
    value = "enemy_type_slime_red";
    break;
  case EnemyTypeSlimeBlue_invocated:
  case EnemyTypeSlimeBlue:
    value = "enemy_type_slime_blue";
    break;
  case EnemyTypeSlimeViolet_invocated:
  case EnemyTypeSlimeViolet:
    value = "enemy_type_slime_violet";
    break;
  case EnemyTypeImpBlue:
    value = "enemy_type_imp_blue";
    break;
  case EnemyTypeImpRed:
    value = "enemy_type_imp_red";
    break;
  case EnemyTypePumpkin_invocated:
  case EnemyTypePumpkin:
    value = "enemy_type_pumpkin";
    break;
  case EnemyTypeWitch:
    value = "enemy_type_witch";
    break;
  case EnemyTypeWitchRed:
    value = "enemy_type_witch_red";
    break;
  case EnemyTypeCauldron:
    value = "enemy_type_cauldron";
    break;
  case EnemyTypeGhost:
    value = "enemy_type_ghost";
    break;
  case EnemyTypeZombie_invocated:
  case EnemyTypeZombie:
    value = "enemy_type_zombie";
    break;
  case EnemyTypeZombieDark:
    value = "enemy_type_zombie_dark";
    break;

  // mini boss
  case EnemyTypeBubble:
    value = "enemy_type_bubble";
    break;

  // boss
  case EnemyTypeButcher:
    value = "enemy_type_boss_butcher";
    break;
  case EnemyTypeSlimeBoss:
    value = "enemy_type_boss_slime_giant";
    break;
  case EnemyTypeCyclops:
    value = "enemy_type_boss_cyclops";
    break;
  case EnemyTypeRatKing:
    value = "enemy_type_boss_rat_king";
    break;
  case EnemyTypeSpiderGiant:
    value = "enemy_type_boss_spider_giant";
    break;
  case EnemyTypeFranckyHand:
  case EnemyTypeFranckyHead:
  case EnemyTypeFranckyFoot:
  case EnemyTypeFrancky:
    value = "enemy_type_francky";
    break;

  // invocated
  case EnemyTypeRatGreen:
    value = "enemy_type_green_rat";
    break;
  case EnemyTypeRockFalling:
    value = "enemy_type_rock_falling";
    break;
  case EnemyTypeRockMissile:
    value = "enemy_type_rock_missile";
    break;
  case EnemyTypeSpiderEgg:
  case EnemyTypeSpiderEgg_invocated:
    value = "enemy_type_spider_egg";
    break;
  case EnemyTypeSpiderLittle:
  case EnemyTypeSpiderLittle_invocated:
    value = "enemy_type_spider_little";
    break;
  case EnemyTypeSpiderWeb:
    value = "enemy_type_spider_web";
    break;

  case EnemyTypeNone:
    value = "enemy_type_himself";
    break;

  case NB_ENEMY:
    break;
  }
  return value;
}

std::string WitchBlastGame::sourceToString(sourceTypeEnum sourceType, enemyTypeEnum enemyType)
{
  std::string value = "unknown";

  switch (sourceType)
  {
  case SourceTypeBolt:
  case SourceTypeMelee:
    value = tools::getLabel(enemyToString(enemyType));
    break;
  case SourceTypeExplosion:
    value = tools::getLabel("source_explosion");
    break;
  case SourceTypePoison:
    value = tools::getLabel("source_poison");
  }

  return value;
}

void WitchBlastGame::saveHiScores()
{
  std::ofstream file(HISCORES_FILE.c_str(), std::ios::out | std::ios::trunc);

  if (file)
  {
    // version (for compatibility check)
    file << SAVE_VERSION << std::endl;

    file << scores.size() << std::endl;

    // tuto
    for (unsigned int i = 0; i < scores.size() && i < SCORES_MAX; i++)
    {
      file << scores[i].name << " ";
      file << scores[i].level << " ";
      file << scores[i].score << " ";
      file << scores[i].shotType << " ";
      // player equip
      for (int j = 0; j < NUMBER_EQUIP_ITEMS; j++)
        file << scores[i].equip[j] << " ";
      file << std::endl;
    }
    file << std::endl;

    file.close();
  }
  else
  {
    std::cout << "[ERROR] Impossible to open hi-scores file.\n";
  }
}

void WitchBlastGame::loadHiScores()
{
  scores.clear();

  std::ifstream file(HISCORES_FILE.c_str(), std::ios::in);

  if (file)
  {
    // version
    std::string v;
    file >> v;

    if (v != SAVE_VERSION)
    {
      file.close();
      remove(SAVE_DATA_FILE.c_str());
      return;
    }

    int nbScores;
    file >> nbScores;

    for (int i = 0; i < nbScores; i++)
    {
      StructScore score;
      file >> score.name;
      file >> score.level;
      file >> score.score;
      file >> score.shotType;
      for (int j = 0; j < NUMBER_EQUIP_ITEMS; j++)
        file >> score.equip[j];

      scores.push_back(score);
    }
  }
}

WitchBlastGame &game()
{
  return *gameptr;
}
