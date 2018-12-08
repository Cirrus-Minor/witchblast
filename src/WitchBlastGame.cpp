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
#include "LargeSlimeEntity.h"
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
#include "BogeymanEntity.h"
#include "LittleSpiderEntity.h"
#include "SpiderEggEntity.h"
#include "FranckyEntity.h"
#include "VampireEntity.h"
#include "ObstacleEntity.h"
#include "ArtefactDescriptionEntity.h"
#include "PnjEntity.h"
#include "TextEntity.h"
#include "StandardRoomGenerator.h"
#include "Scoring.h"
#include "MessageGenerator.h"
#include "TextMapper.h"
#include "SlimePetEntity.h"
#include "SausageEntity.h"
#include "FairyEntity.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

//#define LEVEL_TEST_MODE

#ifdef ONLINE_MODE
#include "OnlineScoring.h"
#endif // ONLINE_MODE

const float PORTRAIT_DIAPLAY_TIME = 5.0f;
const float ENDING_TIMER = 8.0f;
const float BONUS_TIMER = 4.0f;
const unsigned int ACHIEV_LINES = 3;

const int VolumeModifier = 55;

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
  else if (key >= sf::Keyboard::F1 && key <= sf::Keyboard::F15)
  {
    std::ostringstream oss;
    oss << "F" << (int)key - (int)sf::Keyboard::F1;
    s = oss.str();
  }
  else if (key == sf::Keyboard::Escape) s = "Escape";
  else if (key == sf::Keyboard::LControl) s = "Left Control";
  else if (key == sf::Keyboard::LShift) s = "Left Shift";
  else if (key == sf::Keyboard::LAlt) s = "Left Alt";
  else if (key == sf::Keyboard::LSystem) s = "Left System";
  else if (key == sf::Keyboard::RControl) s = "Right Control";
  else if (key == sf::Keyboard::RShift) s = "Right Shift";
  else if (key == sf::Keyboard::RAlt) s = "Right Alt";
  else if (key == sf::Keyboard::RSystem) s = "Right System";
  else if (key == sf::Keyboard::Space) s = "Space";
  else if (key == sf::Keyboard::Tab) s = "Tab";
  else if (key == sf::Keyboard::Left) s = "Left arrow";
  else if (key == sf::Keyboard::Right) s = "Right arrow";
  else if (key == sf::Keyboard::Up) s = "Up arrow";
  else if (key == sf::Keyboard::Down) s = "Down arrow";
  else if (key == sf::Keyboard::LBracket) s = "[";
  else if (key == sf::Keyboard::RBracket) s = "]";
  else if (key == sf::Keyboard::SemiColon) s = ";";
  else if (key == sf::Keyboard::Comma) s = ",";
  else if (key == sf::Keyboard::Period) s = ".";
  else if (key == sf::Keyboard::Quote) s = "Quote";
  else if (key == sf::Keyboard::Slash) s = "/";
  else if (key == sf::Keyboard::BackSlash) s = "\\";
  else if (key == sf::Keyboard::Tilde) s = "~";
  else if (key == sf::Keyboard::Equal) s = "=";
  else if (key == sf::Keyboard::Dash) s = "_";
  else if (key == sf::Keyboard::Return) s = "Return";
  else if (key == sf::Keyboard::BackSpace) s = "BackSpace";
  else if (key == sf::Keyboard::PageUp) s = "Page Up";
  else if (key == sf::Keyboard::PageDown) s = "Page Down";
  else if (key == sf::Keyboard::End) s = "End";
  else if (key == sf::Keyboard::Home) s = "Home";
  else if (key == sf::Keyboard::Insert) s = "Insert";
  else if (key == sf::Keyboard::Delete) s = "Delete";
  else if (key == sf::Keyboard::Add) s = "+";
  else if (key == sf::Keyboard::Subtract) s = "-";
  else if (key == sf::Keyboard::Multiply) s = "*";
  else if (key == sf::Keyboard::Divide) s = "/";
  else if (key == sf::Keyboard::Pause) s = "Pause";
  else if (key == sf::Keyboard::KeyCount) s = "Key Count";

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
  { EventFindTemple,          MsgTutoTemple },
  { EventGetItem,             MsgTutoItems },
  { EventGetSpecialShot,      MsgTutoShots },
  { EventGetSpell,            MsgTutoSpell },
  { EventAchievement,         MsgTutoAchievements },
  { EventConsumable,          MsgTutoConsumables },
  { EventPotion,              MsgTutoPotions },
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

static sf::View getFullScreenLetterboxView(sf::View view, int clientWidth, int clientHeight)
{
  sf::View returnView(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

  float clientRatio = clientWidth / (float) clientHeight;
  float viewRatio = view.getSize().x / (float) view.getSize().y;
  float sizeX = 1;
  float sizeY = 1;
  float posX = 0;
  float posY = 0;

  bool horizontalSpacing = true;
  if (clientRatio > viewRatio)
    horizontalSpacing = false;

  // If horizontalSpacing is true, the black bars will appear on the left and right side.
  // Otherwise, the black bars will appear on the top and bottom.

  if (horizontalSpacing)
  {
    sizeX = clientRatio / viewRatio;
    posX = (1 - sizeX) / 2.0;
  }

  else
  {
    sizeY = viewRatio / clientRatio;
    posY = (1 - sizeY) / 2.0;
  }

  returnView.setViewport( sf::FloatRect(posX, posY, sizeX, sizeY) );

  return returnView;
}

static enemyTypeEnum enemyStringToEnum(std::string enemy)
{
  enemyTypeEnum result = NB_ENEMY;
  for (int i = 0; i < NB_ENEMY; i++)
  {
    if (enemyString[i].compare(enemy) == 0)
      result = (enemyTypeEnum)i;
  }
  return result;
}

static enumAchievementType achievementStringToEnum(std::string achievString)
{
  enumAchievementType result = NB_ACHIEVEMENTS;
  for (int i = 0; i < NB_ACHIEVEMENTS; i++)
  {
    if (achievements[i].label.compare(achievString) == 0)
      result = achievements[i].id;
  }
  return result;
}

static std::string achievementEnumToString(enumAchievementType achiev)
{
  std::string result = "";
  for (int i = 0; i < NB_ACHIEVEMENTS; i++)
  {
    if (achievements[i].id == achiev)
      result = achievements[i].label;
  }
  return result;
}

static EnumMessages messageStringToEnum(std::string msgString)
{
  EnumMessages result = NB_MESSAGES;
  for (int i = 0; i < NB_MESSAGES; i++)
  {
    if (msgMap[(EnumMessages)i].key.compare(msgString) == 0)
      result = (EnumMessages)i;
  }
  return result;
}

static std::string messageEnumToString(EnumMessages msg)
{
  if (msg < NB_MESSAGES) return msgMap[msg].key;
  else return "";
}

namespace
{
WitchBlastGame* gameptr;
}

WitchBlastGame::WitchBlastGame()
{
  gameptr = this;

  gameFromSaveFile = false;
  configureFromFile();

  if (parameters.vsync == false)
  {
    //app->setVerticalSyncEnabled(false);
    app->setFramerateLimit(60);
  }

  // Fullscreen ?
  if (parameters.fullscreen)
  {
    create(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME + " V" + APP_VERSION, true, parameters.vsync);
    sf::View view = app->getDefaultView();
    view = getFullScreenLetterboxView( view, SCREEN_WIDTH, SCREEN_HEIGHT );
    app->setView(view);
  }
  else
  {
    create(SCREEN_WIDTH, SCREEN_HEIGHT, APP_NAME + " V" + APP_VERSION, false, parameters.vsync);
  }

  // init current music
  currentStandardMusic = 0;

  // loading resources
  const char *const images[] =
  {
    "media/player_0.png",      "media/player_1.png",
    "media/player_2.png",
    "media/bolt.png",          "media/tiles.png",
    "media/rat.png",           "media/minimap.png",
    "media/map_background.png",
    "media/items.png",         "media/items_equip.png",
    "media/items.png",         "media/items_equip.png",
    "media/chest.png",         "media/bat.png",
    "media/evil_flower.png",   "media/slime.png",
    "media/imp.png",           "media/spider_egg.png",
    "media/spider_web.png",    "media/little_spider.png",
    "media/bubble.png",        "media/eyeball.png",
    "media/witch.png",
    "media/cauldron.png",      "media/snake.png",
    "media/pumpkin.png",       "media/ghost.png",
    "media/zombie.png",        "media/bogeyman.png",
    "media/sausage.png",
    "media/butcher.png",       "media/giant_slime.png",
    "media/king_rat.png",      "media/cyclops.png",
    "media/giant_spider.png",  "media/francky.png",
    "media/vampire.png",       "media/vampire_bat.png",
    "media/vampire_part.png",
    "media/blood.png",
    "media/corpses.png",       "media/corpses_big.png",
    "media/star.png",          "media/star2.png",
    "media/hurt_impact.png",
    "media/interface.png",     "media/hud_shots.png",
    "media/explosion.png",     "media/keys_qwer.png",
    "media/keys_azer.png",
    "media/night.png",         "media/title.png",
    "media/overlay_00.png",    "media/light_cone.png",
    "media/divinity.png",
    "media/pnj.png",           "media/fairy.png",
    "media/key_area.png",
    "media/ui_life.png",          "media/ui_mana.png",
    "media/ui_spells.png",        "media/ui_message.png",
    "media/ui_top_layer.png",     "media/ui_achiev.png",
    "media/fog.png",              "media/title_animation.png",
    "media/splatter.png",         "media/witch_intro.png",
    "media/item_description.png", "media/death_certificate.png",
    "media/achievements.png",     "media/boss_pictures.png",
    "media/portrait_part.png",    "media/dungeon_random.png",
    "media/dungeon_objects.png",  "media/shadows_standard.png",
    "media/shadows_corners.png",  "media/shadows_medium.png",
    "media/shadows_small.png",    "media/doors.png",
    "media/destroyable_objects.png",  "media/hall_of_fame.png",
    "media/lightning.png",
    "media/win_seal.png",         "media/hof_win_seal.png",
    "media/bag.png",              "media/ui_pause.png",
    "media/score_font.png",       "media/effect_zone.png",
  };

  for (const char *const filename : images)
    ImageManager::getInstance().addImage(filename);

  const char *const sounds[] =
  {
    "media/sound/blast00.ogg",        "media/sound/blast01.ogg",
    "media/sound/blast_elec.ogg",     "media/sound/blast_fire.ogg",
    "media/sound/blast_ice.ogg",      "media/sound/blast_illusion.ogg",
    "media/sound/blast_poison.ogg",   "media/sound/blast_stone.ogg",
    "media/sound/door_closing.ogg",   "media/sound/door_opening.ogg",
    "media/sound/chest_opening.ogg",  "media/sound/impact.ogg",
    "media/sound/bonus.ogg",          "media/sound/drink.ogg",
    "media/sound/eat.ogg",            "media/sound/player_hit.ogg",
    "media/sound/player_die.ogg",     "media/sound/ennemy_dying.ogg",
    "media/sound/coin.ogg",           "media/sound/pay.ogg",
    "media/sound/wall_impact.ogg",    "media/sound/big_wall_impact.ogg",
    "media/sound/king_rat_cry_1.ogg", "media/sound/king_rat_cry_2.ogg",
    "media/sound/king_rat_die.ogg",   "media/sound/slime_jump.ogg",
    "media/sound/slime_impact.ogg",   "media/sound/slime_impact_weak.ogg",
    "media/sound/slime_impact_boss.ogg",
    "media/sound/slime_smash.ogg",    "media/sound/ice_charge.ogg",
    "media/sound/electric.ogg",       "media/sound/select.ogg",
    "media/sound/heart.ogg",          "media/sound/rat_die.ogg",
    "media/sound/bat_die.ogg",        "media/sound/imp_hurt.ogg",
    "media/sound/imp_die.ogg",        "media/sound/rock_impact.ogg",
    "media/sound/rock_impact_medium.ogg", "media/sound/rock_impact_heavy.ogg",
    "media/sound/throw.ogg",          "media/sound/cyclop00.ogg",
    "media/sound/cyclop_die.ogg",     "media/sound/cyclops_impact.ogg",
    "media/sound/butcher_00.ogg",     "media/sound/butcher_01.ogg",
    "media/sound/butcher_hurt.ogg",   "media/sound/butcher_die.ogg",
    "media/sound/vib.ogg",
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
    "media/sound/electricity.ogg",
    "media/sound/electric_blast.ogg", "media/sound/francky_00.ogg",
    "media/sound/francky_01.ogg",     "media/sound/francky_02.ogg",
    "media/sound/francky_die.ogg",    "media/sound/om.ogg",
    "media/sound/glass.ogg",          "media/sound/hiccup.ogg",
    "media/sound/splatch.ogg",        "media/sound/intro_witch.ogg",
    "media/sound/force_field.ogg",    "media/sound/door_opening_boss.ogg",
    "media/sound/achievement.ogg",
    "media/sound/vampire_flying.ogg", "media/sound/vampire_flap.ogg",
    "media/sound/vampire_sonic.ogg",  "media/sound/vampire_laughing.ogg",
    "media/sound/vampire_transform_bolt.ogg", "media/sound/vampire_transform_bat.ogg",
    "media/sound/vampire_hypnosis.ogg", "media/sound/vampire_cry.ogg",
    "media/sound/vampire_dying.ogg",  "media/sound/ice_block.ogg",
    "media/sound/bogeyman_die.ogg",   "media/sound/bogeyman_attack.ogg",
    "media/sound/bogeyman_vortex_00.ogg", "media/sound/bogeyman_vortex_01.ogg",
    "media/sound/barrel_hit.ogg",     "media/sound/barrel_smash.ogg",
    "media/sound/secret.ogg",         "media/sound/scroll.ogg",
    "media/sound/tic_tac.ogg",        "media/sound/bottle.ogg",
    "media/sound/thunder.ogg",        "media/sound/bonus_score.ogg",
    "media/sound/stone_hit.ogg",
  };

  // AA in fullscreen
  if (parameters.fullscreen) enableAA(true);

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
  graphicsFont.loadFromFile("media/Caudex-Bold.ttf");

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
  for (i = 0; i < NB_ACHIEVEMENTS; i++) achievementState[i] = AchievementUndone;
  for (i = 0; i < NB_ENEMY; i++) globalData.killedMonster[i] = 0;

  saveInFight.monsters.clear();

  showLogical = false;
  showGameTime = false;

  uiSprites.shotsSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_HUD_SHOTS));
  uiSprites.topLayer.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_TOP_LAYER));
  uiSprites.topLayer.setPosition(0, SCREEN_HEIGHT - 124);
  uiSprites.msgBoxSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_MESSAGE));
  uiSprites.mapBgSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_MAP_BACKGROUND));
  uiSprites.mapBgSprite.setPosition(342, 23);
  uiSprites.pauseSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_PAUSE));
  uiSprites.pauseSprite.setPosition(SCREEN_WIDTH - 453, 0);
  uiSprites.bagSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_BAG));
  uiSprites.bagSprite.setPosition(116, 606);
  uiSprites.numberSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_SCORE_FONT));

  introScreenSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_INTRO));
  titleSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_TITLE));
  titleSprite.setOrigin(titleSprite.getTextureRect().width / 2, titleSprite.getTextureRect().height / 2);

  scoreState = ScoreOK;
  loopCounter = 0;

  loadGameData();
  loadHiScores();
  receiveScoreFromServer();

  srand(time(NULL));

  fairySpriteOffsetY = 0;
  if (isAdvanced()) fairySpriteOffsetY = 72 + 72 * (rand() % 4);

  // init keys
  for (int p = 0; p < NB_PLAYERS_MAX; p++)
  {
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      actionKey[p][i].isPressed = false;
      actionKey[p][i].isTriggered = false;
    }
  }
  nbPlayers = 1;

  // click zones
  for (int i = 0; i < 4; i++)
    buttons.push_back(ButtonStruct { sf::IntRect(161 + 36 * i, 615, 32, 32), ButtonConsumable, i });

  for (int i = 0; i < 5; i++)
    buttons.push_back(ButtonStruct { sf::IntRect(124 + 55 * i, 655, 48, 48), ButtonShotType, i });
}

void WitchBlastGame::enableAA(bool enable)
{
  for (int i = 0; i < NB_IMAGES; i++)
  {
    if (i != IMAGE_TILES && i != IMAGE_DUNGEON_OBJECTS
        && i != IMAGE_TILES_SHADOW && i != IMAGE_TILES_SHADOW_CORNER  && i != IMAGE_TILES_SHADOW_MEDIUM
        && i != IMAGE_MINIMAP
        && i != IMAGE_DOORS
        && i != IMAGE_FOG
        && i != IMAGE_ITEMS_PRES && i != IMAGE_ITEMS_EQUIP_PRES && i != IMAGE_CORPSES
        && i != IMAGE_CORPSES_BIG)
      ImageManager::getInstance().getImage(i)->setSmooth(enable);
  }
}

WitchBlastGame::~WitchBlastGame()
{
  // cleaning all entities
  EntityManager::getInstance().clean();

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (currentFloor != NULL) delete (currentFloor);

  if (sendScoreThread.joinable()) sendScoreThread.join();
  if (receiveScoreThread.joinable()) receiveScoreThread.join();
}

DungeonMap* WitchBlastGame::getCurrentMap()
{
  return currentMap;
}

GameFloor* WitchBlastGame::getCurrentFloor()
{
  return currentFloor;
}

int WitchBlastGame::getFloorX()
{
  return floorX;
}

int WitchBlastGame::getFloorY()
{
  return floorY;
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

int WitchBlastGame::getSecretsFound()
{
  return secretsFound;
}

bool WitchBlastGame::getShowLogical()
{
  return showLogical;
}

float WitchBlastGame::getDeltaTime()
{
  return deltaTime;
}

void WitchBlastGame::addHurtingStat(int hpLost)
{
  statsData.hurtCounter ++;
  statsData.hpLost += hpLost;
}

void WitchBlastGame::addHealingStat(int hpHeal)
{
  statsData.healCounter ++;
  statsData.hpHeal += hpHeal;
}

void WitchBlastGame::saveStats()
{
  statsData.hpEnd = player->getHp();
  statsData.hpMaxEnd = player->getHpMax();
  statsData.dam = player->getDamage();
  std::ostringstream oss;
  oss << "{";
  oss << int(gameTime);
  oss << "(" << statsData.hpIni << "/" << statsData.hpMax << "->(" << statsData.hpEnd << "/" << statsData.hpMaxEnd << ")[" << statsData.dam << "]";
  oss << "(" << statsData.hurtCounter << "x[" << statsData.hpLost << "])";
  oss << "(" << statsData.healCounter << "x[" << statsData.hpHeal << "])";
  oss << "$" << player->getGold();
  oss << "}";
  statsStr += oss.str();
}

float WitchBlastGame::getGameTime()
{
  return gameTime;
}

void WitchBlastGame::onUpdate()
{
  if (gameState == gameStatePlaying)
  {
    if (isPlayerAlive) player->setItemToBuy(NULL);
    loopCounter++;
    if (loopCounter > 3) loopCounter = 0;

    // time stop
    if (player->isSpecialStateActive(SpecialStateTime))
    {
      if (loopCounter == 0)
        EntityManager::getInstance().animate(deltaTime);
      else
        player->animate(deltaTime);

      SoundManager::getInstance().playSound(SOUND_CLOCK, false);
    }
    else
    {
      EntityManager::getInstance().animate(deltaTime);
      if (isPressing(0, KeyTimeControl, false))
      {
        EntityManager::getInstance().animate(deltaTime);
        SoundManager::getInstance().playSound(SOUND_VIB, false);
      }
    }

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
              saveStats();
              startNewLevel();
            }
            else
              startNewGame(false, 1);
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
  else if (gameState == gameStatePlayingDisplayBoss)
  {
    bossDisplayTimer += deltaTime;
    if (bossDisplayTimer >= PORTRAIT_DIAPLAY_TIME) gameState = gameStatePlaying;
  }
}

void WitchBlastGame::startNewGame(bool fromSaveFile, int startingLevel)
{
  gameState = gameStateInit;
  level = 1;
  score = 0;
  bodyCount = 0;
  scoreDisplayed = 0;
  scoreBonus = "";
  scoreBonusTimer = -1.0f;
  challengeLevel = 1;
  secretsFound = 0;
  gameTime = 0.0f;
  initEvents();
  scoreSaveFile = "";
  interaction.active = false;
  endingTimer = -1.0f;
  statsStr = "";

  // cleaning all entities
  EntityManager::getInstance().clean();
  SoundManager::getInstance().stopSound(SOUND_NIGHT);

  // cleaning the message queue
  std::queue<messageStruct> empty;
  std::swap( messagesQueue, empty );

  // cleaning the achievements queue
  std::queue<achievementStruct> empty2;
  std::swap( achievementsQueue, empty2 );

  // cleaning data
  if (miniMap != NULL) delete (miniMap);
  if (currentFloor != NULL) delete (currentFloor);
  miniMap = NULL;
  currentFloor = NULL;

  // init in game menu
  buildInGameMenu();
  dungeonEntity = new DungeonMapEntity();

  // the interface
  uiSprites.gui.setTexture(*ImageManager::getInstance().getImage(IMAGE_INTERFACE));

  // key symbol on the interface
  uiSprites.keySprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
  uiSprites.keySprite.setTextureRect(sf::IntRect(ITEM_WIDTH * EQUIP_BOSS_KEY, 0,  ITEM_WIDTH, ITEM_HEIGHT));
  uiSprites.keySprite.setPosition(582, 612);

  miniMap = new GameMap(FLOOR_WIDTH, FLOOR_HEIGHT);
  // minimap on the interface
  miniMapEntity = new MiniMapEntity(ImageManager::getInstance().getImage(IMAGE_MINIMAP), miniMap, 19, 15, 10);
  miniMapEntity->setTileBox(20, 16);
  miniMapEntity->setX(705);
  miniMapEntity->setY(610);
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
    {
      fromSaveFile = false;
    }
    else
    {
      gameFromSaveFile = true;
      playLevel(saveInFight.isFight);
    }
  }
  if (!fromSaveFile)
  {
    gameFromSaveFile = false;
    // the player
    player = new PlayerEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                              (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
    // Add a fairy - co op players
    if (nbPlayers == 2)
    {
      int r = rand() % 10;
      int hp = 12;
      int fairy = ItemFairy - FirstEquipItem;
      switch (r)
      {
        case 0: fairy = ItemFairyIce - FirstEquipItem; break;
        case 1: fairy = ItemFairyStone - FirstEquipItem; break;
        case 2: fairy = ItemFairyPoison - FirstEquipItem; break;
        case 3: fairy = ItemFairyFire - FirstEquipItem; break;
      }
      player->setEquipped(fairy, true, true);
      player->setHpMax(hp);
      player->setHp(hp);
    }
    resetKilledEnemies();
    randomizePotionMap();

    if (startingLevel > 1)
    {
      player->acquireItem(ItemScrollRevelation);
      player->acquireItem(enumItemType(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED));

      for (int i = 1; i < startingLevel; i++)
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

      if (startingLevel > 2)
      {
        player->loadDivinity(rand() % NB_DIVINITY, (startingLevel - 2) * 200, 1, 0);
      }
    }
    startNewLevel();
  }
}

void WitchBlastGame::startNewLevel()
{
  // reset floor items
  player->resetFloorItem();

  // reset present items
  resetPresentItems();

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

  if (level <= 7) testAndAddMessageToQueue((EnumMessages)(MsgInfoLevel1 + level - 1));
  if (level == 1)
  {
    testAndAddMessageToQueue(MsgTutoBasics);
    testAndAddMessageToQueue(MsgTutoTips);
  }

  playLevel(false);
}

void WitchBlastGame::playLevel(bool isFight)
{
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

  statsData.goldIni = player->getGold();
  statsData.hpIni = player->getHp();
  statsData.hpMax = player->getHpMax();
  statsData.hpLost = 0;
  statsData.hpHeal = 0;
  statsData.hurtCounter = 0;
  statsData.healCounter = 0;

  if (autosave) saveGame(true);
}

void WitchBlastGame::prepareIntro()
{
  EntityManager::getInstance().clean();

  introSprites[0] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
  introSprites[0]->setX(SCREEN_WIDTH / 2);
  introSprites[0]->setY(SCREEN_HEIGHT - 202);

  introSprites[1] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
  introSprites[1]->setX(SCREEN_WIDTH / 2 + 970);
  introSprites[1]->setY(SCREEN_HEIGHT - 202);

  gameState = gameStateIntro;
  gameTime = 0.0f;
  introState = 0;
  introSoundState = 0;

  SoundManager::getInstance().playSound(SOUND_NIGHT);
}

void WitchBlastGame::updateIntro()
{
  gameTime += deltaTime;
  bool toMenu = false;

  for (int i = 0; i < 2; i++)
  {
    introSprites[i]->setX(introSprites[i]->getX() - deltaTime * 35);
    if (introSprites[i]->getX() < - SCREEN_WIDTH / 2) introSprites[i]->setX(introSprites[i]->getX() + 2 * SCREEN_WIDTH);
  }

  if (gameTime > 2.0f && introSoundState == 0)
  {
    SoundManager::getInstance().playSound(SOUND_INTRO_WITCH);
    introSoundState++;
  }
  else if (gameTime > 7.0f && introSoundState == 1)
  {
    playMusic(MusicIntro);
    introSoundState++;
  }

  if (introState == 0 && gameTime > 2.5f)
  {
    introState = 1;
    introSprites[2] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_TITLE_ANIM),
                                       SCREEN_WIDTH / 2,
                                       SCREEN_HEIGHT / 2,
                                       310, 278, 3);
    introSprites[2]->setZ(50);
  }
  else if (introState == 1)
  {
    int frame = (gameTime - 2.5f) * 8.0f;
    if (frame > 16)
    {
      frame = 16;
      introState = 2;
      introSprites[3] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_WITCH_INTRO));
      introSprites[3]->setX(368 + 1000);
      introSprites[3]->setY(424 - 480);
      introSprites[3]->setZ(40);
      EntityManager::getInstance().sortByZ();
    }
    introSprites[2]->setFrame(frame);
  }
  else if (introState == 2)
  {
    float xSprite = introSprites[3]->getX() - deltaTime * 800;
    float ySprite = introSprites[3]->getY() + deltaTime * 400;
    if (xSprite < 368)
    {
      introState = 3;
      introSprites[3]->setDying(true);
      EntityManager::getInstance().animate(deltaTime);
      xSprite = 368;
      ySprite = 424;
      introSprites[4] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_SPLATTER));
      introSprites[4]->setX(368);
      introSprites[4]->setY(424);
      introSprites[4]->setZ(45);
      introSprites[4]->setScale(0.5f, 0.5f);
      EntityManager::getInstance().sortByZ();
      SoundManager::getInstance().stopSound(SOUND_INTRO_WITCH);
      SoundManager::getInstance().playSound(SOUND_SPLATCH);
    }
    else
    {
      introSprites[3]->setX(xSprite);
      introSprites[3]->setY(ySprite);
      introSprites[3]->setAngle(gameTime * 500);
    }
  }
  else if (introState == 3)
  {
    float scale = introSprites[4]->getScaleX();
    scale += deltaTime * 5.0f;
    if (scale > 1.0f) scale = 1.0f;
    introSprites[4]->setScale(scale, scale);
    if (gameTime > 8.0f)
    {
      introState = 4;
      introSprites[2]->setDying(true);
      introSprites[4]->setDying(true);
      EntityManager::getInstance().animate(deltaTime);
      toMenu = true;
    }
  }

  sf::Event event;
  bool stopDemo = false;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
      //saveGameData();
      app->close();
    }

    if (event.type == sf::Event::Resized)
    {
      enableAA(true);
      sf::View view = app->getDefaultView();
      view = getLetterboxView( view, event.size.width, event.size.height );
      app->setView(view);
    }

    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape) stopDemo = true;
    }
  }
  if (isPressing(0, KeyFireDown, true)) stopDemo = true;

  if (stopDemo)
  {
    if (introState < 4)
    {
      if (introState > 0) introSprites[2]->setDying(true);
      if (introState == 2) introSprites[3]->setDying(true);
      if (introState > 2) introSprites[4]->setDying(true);
      EntityManager::getInstance().animate(deltaTime);

      if (introState < 3)
      {
        SoundManager::getInstance().stopSound(SOUND_INTRO_WITCH);
        SoundManager::getInstance().playSound(SOUND_SPLATCH);
      }
    }
    toMenu = true;
  }

  if (toMenu)
  {
    if (introSoundState <= 1) playMusic(MusicIntro);
    switchToMenu();
  }
}

void WitchBlastGame::renderIntro()
{
  app->draw(introScreenSprite);
  titleSprite.setPosition(SCREEN_WIDTH / 2 - 15, 371);
  if (introState == 4) app->draw(titleSprite);
  EntityManager::getInstance().render(app);
}

void WitchBlastGame::updateRunningGame()
{
  bool backToMenu = false;
  bool escape = false;

  // Process events
  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
      if ((gameState == gameStatePlaying && !player->isDead()) || gameState == gameStatePlayingPause) saveGame(false);
      saveGameData();
      app->close();
    }

    if (event.type == sf::Event::Resized)
    {
      enableAA(true);
      sf::View view = app->getDefaultView();
      view = getLetterboxView( view, event.size.width, event.size.height );
      app->setView(view);
    }

    if (event.type == sf::Event::MouseWheelMoved)
    {
      if (gameState == gameStatePlaying) player->selectNextShotType();
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (gameState == gameStatePlaying)
      {
        sf::Vector2i mousePositionInWindow = sf::Mouse::getPosition(*app);
        sf::Vector2f mousePosition = app->mapPixelToCoords(mousePositionInWindow);

        int mouseButton = 1;
        if (event.mouseButton.button == sf::Mouse::Left) mouseButton = 0;

        tryToClick(mousePosition.x, mousePosition.y, mouseButton);
      }
    }

    if (event.type == sf::Event::KeyPressed)
    {
      if (event.key.code == sf::Keyboard::Escape)
      {
        escape = true;
      }

      if (event.key.code == sf::Keyboard::Return && gameState == gameStatePlaying)
      {
        if (!achievementsQueue.empty())
        {
          if (achievementsQueue.front().timer > 1.0f)
            achievementsQueue.front().timer = 1.0f;
        }
        else if (!messagesQueue.empty())
        {
          if (messagesQueue.front().timer > 0.5f)
            messagesQueue.front().timer = 0.5f;
        }
      }

      if (event.key.code == sf::Keyboard::X)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) startNewGame(false, 1);
      }

      if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num8)
      {
#ifdef LEVEL_TEST_MODE
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
          startNewGame(false, event.key.code - sf::Keyboard::Num1 + 1);
        }
        else
#endif
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        {
          player->dropConsumables(event.key.code - sf::Keyboard::Num1);
        }
        else
        {
          player->tryToConsume(event.key.code - sf::Keyboard::Num1);
        }
      }

      if (event.key.code == sf::Keyboard::F1)
      {
        if (!isPlayerAlive && player->getEndAge() > 3.5f)
        {
          if (scoreSaveFile.compare("") == 0) saveDeathScreen();
        }
        else
        {
          saveScreen();
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
      /*if (event.key.code == sf::Keyboard::Delete)
      {
        StructHurt hurt;
        hurt.critical = false;
        hurt.damage = 1;
        hurt.enemyType = EnemyTypeGhost; //EnemyTypeNone;
        hurt.goThrough = false;
        hurt.hurtingType = ShotTypeStandard;
        hurt.level = 0;
        hurt.sourceType = SourceTypeMelee;
        player->hurt(hurt);
      }*/
      if (event.key.code == sf::Keyboard::F5)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 70,
                           OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, BubbleTriple, 0);
          new BubbleEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 70,
                           OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2, BubbleIce, 0);
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
          findPlaceMonsters(EnemyTypeBat, 1);
          findPlaceMonsters(EnemyTypeBatSkeleton, 2);
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
          findPlaceMonsters(EnemyTypeEvilFlowerFire, 1);
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
          findPlaceMonsters(EnemyTypeCauldronElemental, 1);
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
          findPlaceMonsters(EnemyTypeSpiderLittle, 2);
          findPlaceMonsters(EnemyTypeSpiderTarantula, 2);
        }
      }
      if (event.key.code == sf::Keyboard::F12)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
          new VampireEntity(OFFSET_X + (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                            OFFSET_Y + (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
        }
        else
        {
          initMonsterArray();
          findPlaceMonsters(EnemyTypeZombie, 2);
          findPlaceMonsters(EnemyTypeZombieDark, 2);
          findPlaceMonsters(EnemyTypeGhost, 2);

          findPlaceMonsters(EnemyTypeBogeyman, 2);
        }
      }
      if (event.key.code == sf::Keyboard::F4)
      {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
          for (int i = 0; i < NUMBER_ITEMS - NUMBER_EQUIP_ITEMS; i++)
            new ItemEntity((enumItemType)i, 100 + (i % 14) * 58, 100 + (i / 14) * 60);
        else
          for (int i = NUMBER_ITEMS - NUMBER_EQUIP_ITEMS; i < NUMBER_ITEMS; i++)
            new ItemEntity((enumItemType)i, 100 + ( (i - NUMBER_ITEMS + NUMBER_EQUIP_ITEMS) % 14) * 58, 100 + ((i - NUMBER_ITEMS + NUMBER_EQUIP_ITEMS) / 14) * 60);
      }
#endif // TEST_MODE
    }
  }

  // POST EVENT
  if (escape)
  {
    if (player->isDead() || player->getPlayerStatus() == PlayerEntity::playerStatusVictorious) backToMenu = true;
    else if (gameState == gameStatePlaying) gameState = gameStatePlayingPause;
    else if (gameState == gameStatePlayingPause) gameState = gameStatePlaying;
    else if (gameState == gameStatePlayingDisplayBoss) gameState = gameStatePlaying;
  }

  if ( (player->isDead() || player->getPlayerStatus() == PlayerEntity::playerStatusVictorious) && !xGame[xGameTypeFade].active && isPressing(0, KeyFireDown, true))
  {
    if (player->getEndAge() < DEATH_CERTIFICATE_DELAY) player->setEndAge(DEATH_CERTIFICATE_DELAY);
    else backToMenu = true;
  }

  else if (gameState == gameStatePlayingPause)
  {
    if (isPressing(0, KeyDown, true))
    {
      menuInGame.index++;
      if (menuInGame.index == menuInGame.items.size()) menuInGame.index = 0;
      SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
    }
    else if (isPressing(0, KeyUp, true))
    {
      if (menuInGame.index == 0) menuInGame.index = menuInGame.items.size() - 1;
      else menuInGame.index--;
      SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
    }
    else if (isPressing(0, KeyFireDown, true))
    {
      switch (menuInGame.items[menuInGame.index].id)
      {
      case MenuStartNew:
      case MenuStartOld:
      case MenuKeys:
      case MenuJoystick:
      case MenuConfig:
      case MenuTutoReset:
      case MenuConfigBack:
      case MenuLanguage:
      case MenuCredits:
      case MenuHiScores:
      case MenuPlayerName:
      case MenuVolumeMusic:
      case MenuVolumeSound:
      case MenuAchievements:
        std::cout << "[ERROR] Bad Menu ID\n";
        break;

      case MenuExit:
        backToMenu = true;
        remove(SAVE_FILE.c_str());
        break;

      case MenuContinue:
        gameState = gameStatePlaying;
        break;

      case MenuSaveAndQuit:
        if (currentMap->isCleared()) saveGame(false);
        backToMenu = true;
        break;
      }
    }
  }

  else if (gameState == gameStatePlaying)
  {
    if (player->canMove()) player->setVelocity(Vector2D(0.0f, 0.0f));

    if (isPressing(0, KeyLeft, false))
    {
      if (isPressing(0, KeyUp, false))
        player->move(7);
      else if (isPressing(0, KeyDown, false))
        player->move(1);
      else
        player->move(4);
    }
    else if (isPressing(0, KeyRight, false))
    {
      if (isPressing(0, KeyUp, false))
        player->move(9);
      else if (isPressing(0, KeyDown, false))
        player->move(3);
      else
        player->move(6);
    }
    else if (isPressing(0, KeyUp, false))
    {
      player->move(8);
    }
    else if (isPressing(0, KeyDown, false))
    {
      player->move(2);
    }

    if (isPressing(0, KeyInteract, true))
    {
      if (!player->isDead() && interaction.active) player->interact(interaction.type, interaction.id);
    }

    if (isPressing(0, KeyFireSelect, true))
    {
      if (gameState == gameStatePlaying) player->selectNextShotType();
    }

    if (isPressing(0, KeySpell, true))
    {
      if (gameState == gameStatePlaying) player->castSpell();
    }

    player->resetFireDirection();

    if (isPressing(0, KeyFireLeft, false))
      player->fire(4);
    else if (isPressing(0, KeyFireRight, false))
      player->fire(6);
    else if (isPressing(0, KeyFireUp, false))
      player->fire(8);
    else if (isPressing(0, KeyFireDown, false))
      player->fire(2);
    // alternative "one button" gameplay
    else if (isPressing(0, KeyFire, false))
    {
      if (gameState == gameStatePlaying && isPressing(0, KeyFire, true))
        firingDirection = player->getFacingDirection();
      player->fire(firingDirection);
    }
    // alternative "firing with the mouse" gameplay
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      sf::Vector2i mousePositionInWindow = sf::Mouse::getPosition(*app);
      sf::Vector2f mousePosition = app->mapPixelToCoords(mousePositionInWindow);

      int xm = mousePosition.x - player->getX();
      int ym = mousePosition.y - player->getY();

      if (mousePosition.x >= xOffset && mousePosition.x <= xOffset + GAME_WIDTH
          && mousePosition.y >= yOffset && mousePosition.y <= yOffset + GAME_HEIGHT)
      {
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
    }

    // spell (right click)
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (gameState == gameStatePlaying))
    {
      sf::Vector2i mousePositionInWindow = sf::Mouse::getPosition(*app);
      sf::Vector2f mousePosition = app->mapPixelToCoords(mousePositionInWindow);

      if (mousePosition.x >= xOffset && mousePosition.x <= xOffset + GAME_WIDTH
          && mousePosition.y >= yOffset && mousePosition.y <= yOffset + GAME_HEIGHT)
      {
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
    }

    // Joystick control for fairy
    if (nbPlayers == 2)
    {
      auto fairy = player->getFairy(0);
      if (fairy)
      {
        fairy->setVelocity(Vector2D{0.0f, 0.0f});

        if (isPressing(1, KeyLeft, false))
        {
          if (isPressing(1, KeyUp, false))
            fairy->move(7);
          else if (isPressing(1, KeyDown, false))
            fairy->move(1);
          else
            fairy->move(4);
        }
        else if (isPressing(1, KeyRight, false))
        {
          if (isPressing(1, KeyUp, false))
            fairy->move(9);
          else if (isPressing(1, KeyDown, false))
            fairy->move(3);
          else
            fairy->move(6);
        }
        else if (isPressing(1, KeyUp, false))
        {
          fairy->move(8);
        }
        else if (isPressing(1, KeyDown, false))
        {
          fairy->move(2);
        }

        if (isPressing(1, KeyFireLeft, false))
          fairy->fire(4, true);
        else if (isPressing(1, KeyFireRight, false))
          fairy->fire(6, true);
        else if (isPressing(1, KeyFireUp, false))
          fairy->fire(8, true);
        else if (isPressing(1, KeyFireDown, false))
          fairy->fire(2, true);
      }
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

    // achievement queue
    if (!achievementsQueue.empty() && achievementsQueue.front().hasStarted)
    {
      achievementsQueue.front().timer -= deltaTime;
      if (achievementsQueue.front().timer < 0.0f)
      {
        achievementsQueue.pop();
      }
    }

    // victorious end of game ?
    if (endingTimer > 0.0f)
    {
      endingTimer -= deltaTime;
      if (endingTimer <= 0.0f)
      {
        calculateScore();

        player->setPlayerStatus(PlayerEntity::playerStatusVictorious);
        player->setVelocity(Vector2D(0, 0));
      }
      else if (endingTimer <= 5.5f && !isBonusTimeAdded)
      {
        isBonusTimeAdded = true;
        int timeScore = getTimeScore((int)(gameTime / 60.0f));
        addBonusScore(BonusTime, timeScore);
      }
    }
  }
  else if (gameState == gameStatePlayingDisplayBoss)
  {
    if (isPressing(0, KeyFireDown, true)) gameState = gameStatePlaying;
  }

  onUpdate();

  verifyDoorUnlocking();
  checkInteraction();

  if (roomClosed)
  {
    if (getEnemyCount() == 0)
    {
      currentMap->setCleared(true);
      if (currentMap->getRoomType() == roomTypeKey)
        new ItemEntity( (enumItemType)(ItemBossKey),
                        MAP_WIDTH / 2 * TILE_WIDTH + TILE_WIDTH / 2,
                        MAP_HEIGHT / 2 * TILE_HEIGHT + TILE_HEIGHT / 2);
      player->onClearRoom();
      openDoors();
      if (!autosave) remove(SAVE_FILE.c_str());
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

        addBonusScore(BonusChallenge, getChallengeScore(challengeLevel));

        challengeLevel++;
        if (challengeLevel == 5) registerAchievement(AchievementChallenges);
        player->offerChallenge();
      }
    }
  }

  if (backToMenu)
  {
    if (nbPlayers == 1 && (player->isDead() || player->getPlayerStatus() == PlayerEntity::playerStatusVictorious))
    {
      EntityManager::getInstance().clean();

      introSprites[0] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
      introSprites[0]->setX(SCREEN_WIDTH / 2);
      introSprites[0]->setY(SCREEN_HEIGHT - 202);

      introSprites[1] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
      introSprites[1]->setX(SCREEN_WIDTH / 2 + 970);
      introSprites[1]->setY(SCREEN_HEIGHT - 202);

      switchToMenu();
      updateActionKeys();

      menuState = MenuStateHiScores;
#ifdef ONLINE_MODE
      menuScoreIndex = 0;
#else
      menuScoreIndex = 2;
#endif
    }
    else
    {
      prepareIntro();
      switchToMenu();
      menuState = MenuStateMain;
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

void WitchBlastGame::renderBossPortrait()
{
  if (gameState == gameStatePlayingDisplayBoss)
  {
    float transitionTime = 2.5f;
    float endTime = PORTRAIT_DIAPLAY_TIME;

    int xBoss = GAME_WIDTH / 2 - 267;
    int fade = 255;
    if (bossDisplayTimer < 0.25f)
    {
      xBoss += (0.25f - bossDisplayTimer) * 3000;
      //fade = 255 - (0.25f - bossDisplayTimer) * 1000;
    }
    else if (bossDisplayTimer > endTime - 0.75f)
    {
      fade = (endTime - bossDisplayTimer) * 333;
      if (fade < 0) fade = 0;
    }

    // background
    sf::RectangleShape rectangle;
    rectangle.setFillColor(sf::Color(0, 0, 0, fade));
    rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
    rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH, MAP_HEIGHT * TILE_HEIGHT));
    app->draw(rectangle);

    // boss
    if (bossDisplayTimer > transitionTime) // + 1.0f)
    {
      EnemyEntity* boss = getBoss();
      if (boss)
      {
        sf::View view = app->getView();
        view.move(-5, -5);
        app->setView(view);
        boss->render(app);
        view.move(5, 5);
        app->setView(view);
      }

      /*if (bossDisplayTimer < transitionTime + 1.5f)
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, (1.5 - (bossDisplayTimer - transitionTime)) * 510));
        app->draw(rectangle);
        std::cout << 1.5 - (bossDisplayTimer - transitionTime) << " ";
      }*/
      //if (bossDisplayTimer < transitionTime + 2.0f)
      {
        //rectangle.setFillColor(sf::Color(0, 0, 0, (2.0 - (bossDisplayTimer - transitionTime)) * 255));
        //app->draw(rectangle);
        //std::cout << 1.5 - (bossDisplayTimer - transitionTime) << " ";
      }
    }

    // boss portrait
    if (bossDisplayTimer < transitionTime)
    {
      sf::Sprite bossSprite;
      bossSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_BOSS_PICTURES));
      bossSprite.setPosition(xBoss, 0);

      app->draw(bossSprite);
    }
    else
    {
      if (bossDisplayState == 0)
      {
        SoundManager::getInstance().playSound(SOUND_BOOM_00);

        for (int i = 0; i < 250; i++)
        {
          SpriteEntity* spriteStar = new SpriteEntity(
            ImageManager::getInstance().getImage(IMAGE_PORTRAIT_PART),
            GAME_WIDTH / 2 - 150 + rand() % 300, GAME_HEIGHT / 2 - 150 + rand() % 300);
          spriteStar->setScale(5.0f, 5.0f);
          spriteStar->setZ(7000.0f);
          spriteStar->setSpin(-100 + rand()%200);
          spriteStar->setVelocity(Vector2D(400 + rand()%800));
          spriteStar->setFading(true);
          spriteStar->setLifetime(4.1f + (rand() % 100) * 0.003f );
          spriteStar->setColor(sf::Color(rand() % 255, rand() % 255, 255, 128));
          spriteStar->setColor(sf::Color(40, 4, 40, 128));
          spriteStar->setType(ENTITY_EFFECT);
          if (rand()% 2 == 0) spriteStar->setRenderAdd();
        }

        bossDisplayState = 1;
      }
      else if (bossDisplayState == 1 && bossDisplayTimer > transitionTime + 0.2f)
      {
        bossDisplayState = 2;
        for (int i = 0; i < 50; i++)
        {
          SpriteEntity* spriteStar = new SpriteEntity(
            ImageManager::getInstance().getImage(IMAGE_PORTRAIT_PART),
            GAME_WIDTH / 2 - 150 + rand() % 300, GAME_HEIGHT / 2 - 150 + rand() % 300);
          spriteStar->setScale(4.0f, 4.0f);
          spriteStar->setZ(7000.0f);
          spriteStar->setSpin(-100 + rand()%200);
          spriteStar->setVelocity(Vector2D(400 + rand()%800));
          spriteStar->setFading(true);
          spriteStar->setLifetime(4.1f + (rand() % 100) * 0.003f );
          spriteStar->setColor(sf::Color(rand() % 255, rand() % 255, 255, 128));
          spriteStar->setColor(sf::Color(20, 2, 20, 128));
          spriteStar->setType(ENTITY_EFFECT);
          spriteStar->setRenderAdd();
        }
      }
      else if (bossDisplayState == 2 && bossDisplayTimer > transitionTime + 0.4f)
      {
        bossDisplayState = 3;
        for (int i = 0; i < 50; i++)
        {
          SpriteEntity* spriteStar = new SpriteEntity(
            ImageManager::getInstance().getImage(IMAGE_PORTRAIT_PART),
            GAME_WIDTH / 2 - 150 + rand() % 300, GAME_HEIGHT / 2 - 150 + rand() % 300);
          spriteStar->setScale(3.0f, 3.0f);
          spriteStar->setZ(7000.0f);
          spriteStar->setSpin(-100 + rand()%200);
          spriteStar->setVelocity(Vector2D(400 + rand()%800));
          spriteStar->setFading(true);
          spriteStar->setLifetime(4.1f + (rand() % 100) * 0.003f );
          spriteStar->setColor(sf::Color(rand() % 255, rand() % 255, 255, 128));
          spriteStar->setColor(sf::Color(20, 2, 20, 128));
          spriteStar->setType(ENTITY_EFFECT);
          spriteStar->setRenderAdd();
        }
      }
      animateEffects();
    }
  }
}

void WitchBlastGame::renderGame()
{
  lifeBar.toDisplay = false;
  EntityManager::getInstance().renderUnder(app, 5000);
  EntityManager::getInstance().renderUnder(app, 5000);
}

void WitchBlastGame::generateUiParticle(float x, float y)
{
  SpriteEntity* particle = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_BOLT),
      x, y, BOLT_WIDTH, BOLT_HEIGHT);
  particle->setFading(true);
  particle->setImagesProLine(BOLT_PRO_LINE);
  particle->setZ(10000);
  particle->setLifetime(0.5f);
  particle->setVelocity(Vector2D(30 + rand() % 50));
  particle->setWeight(200);
  particle->setType(ENTITY_EFFECT);
  //particle->setFrame(BOLT_PRO_LINE + 2);
  particle->setFrame(BOLT_PRO_LINE * 2 + 6);
  particle->setRenderAdd();
  particle->setScale(0.35f, 0.35f);
  particle->setFading(true);
}

void WitchBlastGame::renderHud()
{
  // boss life bar ?
  if (lifeBar.toDisplay && gameState != gameStatePlayingDisplayBoss) renderLifeBar();

  //renderBossPortrait();

  // interaction text ?
  if (interaction.active)
  {
    if (interaction.type == InteractionTypeTemple)
      write(interaction.label, 20, GAME_WIDTH / 2, 480, ALIGN_CENTER,sf::Color::White, app, 2, 2, 0);
    else if (interaction.type == InteractionTypeMerchandise)
      write(interaction.label, 20, GAME_WIDTH / 2, 480, ALIGN_CENTER,sf::Color::White, app, 2, 2, 0);
  }

  // light cone ?
  float fade = player->getFadingDivinity(true);
  if (fade > 0.0f)
  {
    sf::Sprite cone;
    cone.setTexture(*ImageManager::getInstance().getImage(IMAGE_LIGHT_CONE));
    cone.setPosition(player->getX() - 64, player->getY() - 580);
    cone.setColor(sf::Color(255, 255, 255, 255 * fade));
    app->draw(cone, sf::BlendAdd);
  }

  app->draw(uiSprites.gui);
  EntityManager::getInstance().renderAfter(app, 5000);

  if (gameState == gameStatePlayingDisplayBoss)
  {
    // boss name
    int fade = 255;
    if (bossDisplayTimer < 0.25f)
    {
      fade = 255 - (0.25f - bossDisplayTimer) * 1000;
    }
    else if (bossDisplayTimer > PORTRAIT_DIAPLAY_TIME - 0.25f)
    {
      fade = (PORTRAIT_DIAPLAY_TIME - bossDisplayTimer) * 1000;
      if (fade < 0) fade = 0;
    }

    write("THE COOK", 27, GAME_WIDTH / 2, 540, ALIGN_CENTER, sf::Color(255, 255, 255, fade), app, 0, 0, 0);
  }
}

void WitchBlastGame::renderScore()
{
  if (scoreDisplayed < score - 300) scoreDisplayed += 100;
  else if (scoreDisplayed < score) scoreDisplayed++;

  float x = 24, y = 19;
  int scoretemp = scoreDisplayed;
  int n = scoretemp / 10000;
  uiSprites.numberSprite.setTextureRect(sf::IntRect(n * 20, 0, 20, 28));
  uiSprites.numberSprite.setPosition(x , y);
  app->draw(uiSprites.numberSprite);

  scoretemp %= 10000;
  n = scoretemp / 1000;
  uiSprites.numberSprite.setTextureRect(sf::IntRect(n * 20, 0, 20, 28));
  uiSprites.numberSprite.setPosition(x + 17, y);
  app->draw(uiSprites.numberSprite);

  scoretemp %= 1000;
  n = scoretemp / 100;
  uiSprites.numberSprite.setTextureRect(sf::IntRect(n * 20, 0, 20, 28));
  uiSprites.numberSprite.setPosition(x + 2 * 17, y);
  app->draw(uiSprites.numberSprite);

  scoretemp %= 100;
  n = scoretemp / 10;
  uiSprites.numberSprite.setTextureRect(sf::IntRect(n * 20, 0, 20, 28));
  uiSprites.numberSprite.setPosition(x + 3 * 17, y);
  app->draw(uiSprites.numberSprite);

  scoretemp %= 10;
  n = scoretemp;
  uiSprites.numberSprite.setTextureRect(sf::IntRect(n * 20, 0, 20, 28));
  uiSprites.numberSprite.setPosition(x + 4 * 17, y);
  app->draw(uiSprites.numberSprite);

  if (scoreBonusTimer > 0.0f)
  {
    sf::Color color;
    if (scoreBonusTimer < 1.0f) color = sf::Color(255, 255, 255, 255 * scoreBonusTimer);
    else color = sf::Color::White;
    write(scoreBonus, 13, 116, 51, ALIGN_RIGHT,color, app, 0, 0, 116);
    scoreBonusTimer -= deltaTime;
  }
}

void WitchBlastGame::renderLifeBar()
{
  if (lifeBar.toDisplay)
  {
    int label_dy = 0;
    int xBarOffset = 100;
    float l = lifeBar.hp * ((MAP_WIDTH - 1) * TILE_WIDTH - xBarOffset) / lifeBar.hpMax;

    sf::RectangleShape rectangle(sf::Vector2f((MAP_WIDTH - 1) * TILE_WIDTH - xBarOffset, 24));
    rectangle.setFillColor(sf::Color(0, 0, 0,128));
    rectangle.setPosition(sf::Vector2f(TILE_WIDTH / 2 + xBarOffset, label_dy + 22));
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color(200, 200, 200, 200));
    app->draw(rectangle);

    rectangle.setSize(sf::Vector2f(l, 24));
    rectangle.setFillColor(sf::Color(190, 20, 20));
    rectangle.setOutlineThickness(0);
    //rectangle.setPosition(sf::Vector2f(TILE_WIDTH / 2, label_dy + 22));
    app->draw(rectangle);

    game().write(           lifeBar.label,
                            18,
                            TILE_WIDTH / 2 + 10.0f + xBarOffset,
                            label_dy + 22,
                            ALIGN_LEFT,
                            sf::Color(255, 255, 255),
                            app, 0 , 0, 0);
  }
}

void WitchBlastGame::renderRunningGame()
{
  EntityManager::getInstance().sortByZ();
  getCurrentMapEntity()->computeBoltParticulesVertices();

  if (!isPlayerAlive)
  {
    sf::View view = app->getView();
    sf::View viewSave = app->getView();

    float endAge = player->getEndAge();

    if (!parameters.zoom || endAge > 4.0f)
    {
      // do nothing
    }
    else if (endAge > 3.0f)
    {
      view.zoom(1.0f - 0.75f * (4.0f - endAge));
      float xDiff = view.getCenter().x - player->getX();
      float yDiff = view.getCenter().y - player->getY();
      view.setCenter(view.getCenter().x - xDiff * (4.0f - endAge),
                     view.getCenter().y - yDiff * (4.0f - endAge));
    }
    else if (endAge > 1.0f)
    {
      view.zoom(0.25f);
      view.setCenter(player->getX(), player->getY());
    }
    else
    {
      view.zoom(1.0f - 0.75f * endAge);
      float xDiff = view.getCenter().x - player->getX();
      float yDiff = view.getCenter().y - player->getY();
      view.setCenter(view.getCenter().x - xDiff * endAge,
                     view.getCenter().y - yDiff * endAge);
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

  sf::RectangleShape rectangle(sf::Vector2f(200, 25));

  // effects
  if (player->isSpecialStateActive(SpecialStateTime))
  {
    sf::RectangleShape whiteLine = sf::RectangleShape(sf::Vector2f(GAME_WIDTH, 2));
    whiteLine.setFillColor(sf::Color(255, 255, 255, 32));
    for (int i = 0; i < 8; i++)
    {
      whiteLine.setPosition(xOffset, yOffset + rand() % GAME_HEIGHT);
      app->draw(whiteLine);
    }
  }
  if (isPressing(0, KeyTimeControl, false) && gameState == gameStatePlaying)
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
  if (player->isSpecialStateActive(SpecialStateConfused))
  {
    specialStateStuct specialState = player->getSpecialState(SpecialStateConfused);

    // effect
    int effectFade = 150 + cos(3.0f * getAbsolutTime()) * 100;

    // fade
    int fade = 55;
    if (specialState.timer < 0.4f)
      fade = 55 - (0.4f - specialState.timer) / 0.4f * 55;
    else if (specialState.timer > specialState.param1 - 0.4f)
      fade = (specialState.param1 - specialState.timer) / 0.4f * 55;
    if (fade < 0) fade = 0;
    else if (fade > 55) fade = 55;

    fade *= 1.5f;

    rectangle.setFillColor(sf::Color(255 - effectFade, 0, effectFade, fade));
    rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
    rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
    sf::RenderStates r;
    r.blendMode = sf::BlendAlpha ;
    app->draw(rectangle, r);
  }
  else if (xGame[xGameTypeFadeColor].active)
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

  if (gameState != gameStatePlayingDisplayBoss) renderMessages();
  app->draw(uiSprites.topLayer);
  miniMapEntity->display(app);

  std::ostringstream oss;
  oss << player->getGold();
  write(oss.str(), 18, 367, 619, ALIGN_CENTER, sf::Color::White, app, 0, 0, 0);

  myText.setColor(sf::Color(0, 0, 0, 255));
  myText.setCharacterSize(16);

  oss.str("");
  oss << tools::getLabel("level") << " " << level;

  writeGraphic(oss.str(), 16, levelStrPosition.x, levelStrPosition.y, ALIGN_CENTER, sf::Color::Black, app, 0, 0, 0);

  //if (gameState == gameStatePlaying)
  {
    sf::Sprite hpSprite;
    hpSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_LIFE));
    int hpFade = 88.0f * (float)player->getHpDisplay() / (float)player->getHpMax();
    if (hpFade < 0) hpFade = 0;
    else if (hpFade > 88) hpFade = 88;
    hpSprite.setPosition(477, 619 + 88 - hpFade);
    hpSprite.setTextureRect(sf::IntRect(0, 88 - hpFade, 88, hpFade));
    app->draw(hpSprite);

    oss.str("");
    oss << player->getHp() << "/" << player->getHpMax();
    write(oss.str(), 16, 521, 654, ALIGN_CENTER, sf::Color::White, app, 0, 0, 0);

    // mana
    sf::Sprite manaSprite;
    manaSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_MANA));
    int manaFade = player->getPercentFireDelay() * 98;
    manaSprite.setPosition(10, 614 + 98 - manaFade);
    manaSprite.setTextureRect(sf::IntRect(0, 98 - manaFade, 98, manaFade));
    app->draw(manaSprite);


    if (player->getActiveSpell().spell != SpellNone)
    {
      sf::Sprite spellSprite;
      spellSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_SPELLS));
      spellSprite.setPosition(20, 624);
      int frame = player->getActiveSpell().spell;
      spellSprite.setTextureRect(sf::IntRect(frame * 78, 78, 78, 78));
      app->draw(spellSprite);

      int spellFade = player->getPercentSpellDelay() * 78;
      spellSprite.setPosition(20, 624 + 78 - spellFade);
      spellSprite.setTextureRect(sf::IntRect(frame * 78, 78 - spellFade, 78, spellFade));
      app->draw(spellSprite);

      if (player->canCastSpell())
      {
        float fade = (cos(8.0f * getAbsolutTime()) + 1.0f) * 0.5f;
        spellSprite .setColor(sf::Color(255, 255, 255, 255 * fade));
        app->draw(spellSprite, sf::BlendAdd);
      }
    }

    // drawing the key on the interface
    if (player->isEquiped(EQUIP_BOSS_KEY)) app->draw(uiSprites.keySprite);
    // drawing the level items
    if (player->isEquiped(EQUIP_FLOOR_MAP))
    {
      sf::Sprite mapSprite;
      mapSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      mapSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * 3, ITEM_HEIGHT * 4,  ITEM_WIDTH, ITEM_HEIGHT));
      mapSprite.setPosition(582, 647);
      app->draw(mapSprite);
    }
    if (player->isEquiped(EQUIP_ALCOHOL))
    {
      sf::Sprite alcSprite;
      alcSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      alcSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * 4, ITEM_HEIGHT * 4,  ITEM_WIDTH, ITEM_HEIGHT));
      alcSprite.setPosition(582, 680);
      app->draw(alcSprite);
    }
    if (player->isEquiped(EQUIP_LUCK))
    {
      sf::Sprite alcSprite;
      alcSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      alcSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * 5, ITEM_HEIGHT * 4,  ITEM_WIDTH, ITEM_HEIGHT));
      alcSprite.setPosition(615, 680);
      app->draw(alcSprite);
    }
    if (player->isEquiped(EQUIP_FAIRY_POWDER))
    {
      sf::Sprite alcSprite;
      alcSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
      alcSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * 6, ITEM_HEIGHT * 4,  ITEM_WIDTH, ITEM_HEIGHT));
      alcSprite.setPosition(648, 680);
      app->draw(alcSprite);
    }
    // drawing the consumable
    if (!player->isEquiped(EQUIP_BAG))
    {
      sf::RectangleShape shadow(sf::Vector2f(70, 33));
      shadow.setFillColor(sf::Color(0, 0, 0, 128));
      shadow.setPosition(sf::Vector2f(231, 614));
      app->draw(shadow);
    }
    else
    {
      app->draw(uiSprites.bagSprite);
    }
    for (int i = 0; i < MAX_SLOT_CONSUMABLES; i++)
    {
      int item = player->getConsumable(i);
      if (item > -1)
      {
        sf::Sprite consSprite;
        consSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS));
        consSprite.setPosition(161 + 36 * i, 615);

        if (item < FirstEquipItem && item >= ItemPotion01 + NUMBER_UNIDENTIFIED)
        {
          int bottle = (int)(getPotion((enumItemType)item));
          consSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * (bottle % 10), ITEM_HEIGHT * (bottle / 10),  ITEM_WIDTH, ITEM_HEIGHT));
          app->draw(consSprite);
        }

        consSprite.setTextureRect(sf::IntRect(ITEM_WIDTH * (item % 10), ITEM_HEIGHT * (item / 10),  ITEM_WIDTH, ITEM_HEIGHT));
        app->draw(consSprite);
      }
    }

    // drawing the divinity
    if (player->getDivinity().divinity >= 0)
    {
      sf::Sprite divSprite;
      divSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_DIVINITY));
      divSprite.setPosition(405, 616);
      divSprite.setTextureRect(sf::IntRect(player->getDivinity().divinity * 48, 0, 48, 85));
      app->draw(divSprite);

      float fade = player->getFadingDivinity(false);
      if (fade > 0.0f && player->getPlayerStatus() != PlayerEntity::playerStatusPraying)
      {
        divSprite.setTextureRect(sf::IntRect(player->getDivinity().divinity * 48, 85, 48, 85));
        divSprite.setColor(sf::Color(255, 255, 255, 255 * fade));
        app->draw(divSprite);
      }

      rectangle.setOutlineThickness(0);
      if (player->getDivinity().interventions + 1 < player->getDivinity().level)
      {
        int fade = 50 + 50 * cosf(game().getAbsolutTime() * 8);
        rectangle.setFillColor(sf::Color(100 + fade, 100 + fade, 200 + fade / 2, 255));
      }

      else
        rectangle.setFillColor(sf::Color(100, 100, 200, 255));

      rectangle.setPosition(sf::Vector2f(407, 692));
      rectangle.setSize(sf::Vector2f(45 * player->getDivinity().percentsToNextLevels, 8));
      app->draw(rectangle);

      std::ostringstream oss;
      if (player->getDivinity().level == MAX_DIVINITY_LEVEL + 1) oss << "MAX";
      else oss << "lvl " << player->getDivinity().level;
      write(oss.str(), 11, 429, 702, ALIGN_CENTER, sf::Color::White, app, 0, 0, 0);
    }

    // render the shots
    renderHudShots(app);

    // render PAUSE screen
    if (gameState == gameStatePlayingPause)
    {
      // background
      rectangle.setFillColor(sf::Color(0, 0, 0, 200));
      rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
      rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH, MAP_HEIGHT * TILE_HEIGHT));
      app->draw(rectangle);

      app->draw(uiSprites.pauseSprite);

      float x = 588;
      float y = 388;
      // items
      //write(tools::getLabel("inventory"), 16, x, y, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
      int n = 0;

      for (auto i: sortedEquipement)
      {
        if (i != EQUIP_BOSS_KEY && i != EQUIP_FLOOR_MAP &&
            i != EQUIP_LUCK && i != EQUIP_ALCOHOL &&
            i != EQUIP_FAIRY_POWDER && player->isEquiped(i))
        {
          sf::Sprite itemSprite;
          itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
          itemSprite.setPosition(x + (n % 8) * 40, y + (n / 8) * 40);
          itemSprite.setTextureRect(sf::IntRect((i % 10) * 32, (i / 10) * 32, 32, 32));
          app->draw(itemSprite);
          n++;
        }
      }
      renderInGameMenu();

      // texts in the right UI part

      // player name
      writeGraphic(parameters.playerName, 32, 734, 52, ALIGN_CENTER, sf::Color::Black, app);

      // game time
      std::ostringstream oss;
      oss << "Game time: ";
      int time = int(gameTime);
      if (time > 3600)
      {
        int hours = time / 3600;
        oss << hours << ":";
        time -= 3600 * hours;
      }
      int minutes = time / 60;
      if (minutes < 10) oss << "0";
      oss << minutes;
      time -= 60 * minutes;
      oss << ":";
      if (time < 10) oss << "0";
      oss << time;
      writeGraphic(oss.str(), 18, 734, 143, ALIGN_CENTER, sf::Color::Black, app);

      oss.str("");
      oss << tools::getLabel("ui_base_damage") << ": " << player->getDamage();
      writeGraphic(oss.str(), 17, 588, 190, ALIGN_LEFT, sf::Color::Black, app);

      oss.str("");
      oss << tools::getLabel("ui_fire_rate") << ": " << std::fixed << std::setprecision(1) << player->getFireRate()
        << " / " << tools::getLabel("ui_second");
      writeGraphic(oss.str(), 17, 588, 222, ALIGN_LEFT, sf::Color::Black, app);

      oss.str("");
      oss << tools::getLabel("dc_killed_monsters") << ": " << bodyCount;
      writeGraphic(oss.str(), 17, 588, 254, ALIGN_LEFT, sf::Color::Black, app);

      oss.str("");
      oss << tools::getLabel("dc_challenges") << ": " << challengeLevel - 1;
      writeGraphic(oss.str(), 17, 588, 286, ALIGN_LEFT, sf::Color::Black, app);

      oss.str("");
      oss << tools::getLabel("ui_temple_donation") << ": " << player->getDonation();
      writeGraphic(oss.str(), 17, 588, 318, ALIGN_LEFT, sf::Color::Black, app);

      // potions
      int iPotion = 0;
      for (int i = 0; i < 4; i++)
      {
        int currentPotion = player->getConsumable(i);
        if (currentPotion > -1)
        {
          bool ok = true;
          for (int j = 0; j < i; j++) if (currentPotion == player->getConsumable(j)) ok = false;
          if (ok)
          {
            int xPotion = 40;
            int yPotion = 440 + 30 * iPotion;
            sf::Sprite itemSprite;
            itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS));
            itemSprite.setPosition(xPotion, yPotion);

            if (currentPotion >= ItemPotion01 + NUMBER_UNIDENTIFIED )
            {
              int basisPotion = getPotion(enumItemType(currentPotion));
              itemSprite.setTextureRect(sf::IntRect((basisPotion % 10) * 32, (basisPotion / 10) * 32, 32, 32));
              app->draw(itemSprite);
            }

            itemSprite.setTextureRect(sf::IntRect((currentPotion % 10) * 32, (currentPotion / 10) * 32, 32, 32));
            app->draw(itemSprite);

            // write(tools::getLabel(items[currentPotion].name), 13, xPotion + 37, yPotion + 11, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
            // write(tools::getLabel(items[currentPotion].description), 12, xPotion + 3, yPotion + 36, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
            write(tools::getLabel(items[currentPotion].description), 12, xPotion + 37, yPotion + 11, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);

            iPotion++;
          }
        }
      }
    }
    // score TODO
    renderScore();

    if (player->isDead())
    {
      float deathAge = player->getEndAge();

      if (deathAge > DEATH_CERTIFICATE_DELAY)
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, 180));
        rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
        rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
        app->draw(rectangle);

        renderDeathScreen(80, 110);

        if (scoreSaveFile.compare("") == 0)
        {
          write(tools::getLabel("certificate_capture"), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
        }
        else
        {
          std::stringstream ss;
          ss << tools::getLabel("certificate_saved") << " " << scoreSaveFile;
          write(ss.str(), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
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
    else if (player->getPlayerStatus() == PlayerEntity::playerStatusVictorious)
    {
      if (player->getEndAge() > 2.0f)
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, 180));
        rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
        rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
        app->draw(rectangle);

        renderDeathScreen(80, 110);

        if (scoreSaveFile.compare("") == 0)
        {
          write(tools::getLabel("certificate_capture"), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
        }
        else
        {
          std::stringstream ss;
          ss << tools::getLabel("certificate_saved") << " " << scoreSaveFile;
          write(ss.str(), 16, 80, 430, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
        }
      }
      else
      {
        rectangle.setFillColor(sf::Color(0, 0, 0, 90 * (player->getEndAge())));
        rectangle.setPosition(sf::Vector2f(xOffset, yOffset));
        rectangle.setSize(sf::Vector2f(MAP_WIDTH * TILE_WIDTH , MAP_HEIGHT * TILE_HEIGHT));
        app->draw(rectangle);

        renderDeathScreen(80 + (2.0f - player->getEndAge() ) * 1000, 110);
      }
    }
    else if (player->getPlayerStatus() != PlayerEntity::playerStatusVictorious && currentMap->getRoomType() == roomTypeExit && level >= LAST_LEVEL)
    {
      float x0 = (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2;

      write(tools::getLabel("congratulations_1"), 25, x0, 220, ALIGN_CENTER, sf::Color::White, app, 2, 2, 0);
      write(tools::getLabel("congratulations_2"), 23, x0, 250, ALIGN_CENTER, sf::Color::White, app, 2, 2, 0);
      write(tools::getLabel("congratulations_3"), 23, x0, 280, ALIGN_CENTER, sf::Color::White, app, 2, 2, 0);

      registerAchievement(AchievementWin);
      if (endingTimer < 0.0f)
      {
        endingTimer = ENDING_TIMER;
        isBonusTimeAdded = false;
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
    write(ss.str(), 14, 4, 4, ALIGN_LEFT, sf::Color::Green, app, 0, 0, 0);
  }

// achievements ?
  if (!achievementsQueue.empty()) // && (currentMap->isCleared() || achievementsQueue.front().hasStarted) )
  {
    if (!achievementsQueue.front().hasStarted)
    {
      SoundManager::getInstance().playSound(SOUND_ACHIEVEMENT);
      achievementsQueue.front().hasStarted = true;
      achievementState[achievementsQueue.front().type] = AchievementDone;
      saveGameData();
      proceedEvent(EventAchievement);

      // text
      float x0 = MAP_WIDTH * 0.5f * TILE_WIDTH;
      float y0 = MAP_HEIGHT * 0.5f * TILE_HEIGHT + 40.0f;
      TextEntity* text = new TextEntity(tools::getLabel("achievement_complete"), 30, x0, y0);
      text->setAlignment(ALIGN_CENTER);
      text->setLifetime(2.5f);
      text->setWeight(-36.0f);
      text->setZ(1200);
      text->setColor(TextEntity::COLOR_FADING_WHITE);
    }

    int xPos = 560;
    int yPos = 4;
    int opening = 384;
    float achievAge = ACHIEVEMENT_DELAY_MAX - achievementsQueue.front().timer;
    sf::Sprite spriteScroll;
    if (achievAge < 1.0f)
      opening = 32;
    else if (achievAge < 2.0f)
      opening = 32 + (achievAge - 1.0f) * (384 - 32);
    else if (achievAge > ACHIEVEMENT_DELAY_MAX - 1.0f)
      opening = 32 + (ACHIEVEMENT_DELAY_MAX - achievAge) * (384 - 32);
    if (achievAge < 0.5f)
      spriteScroll.setColor(sf::Color(255, 255, 255, 500 * achievAge));
    else if (achievAge > ACHIEVEMENT_DELAY_MAX - 0.5f)
      spriteScroll.setColor(sf::Color(255, 255, 255, 500 * (ACHIEVEMENT_DELAY_MAX - achievAge)));

    spriteScroll.setTexture(*ImageManager::getInstance().getImage(IMAGE_UI_ACHIEV));
    spriteScroll.setTextureRect(sf::IntRect(128 + 384 - opening, 0, opening, 64));
    spriteScroll.setPosition(xPos + 384 - opening, yPos);
    app->draw(spriteScroll);

    if ((achievAge > 1.0f && achievAge < 2.0f) || achievAge > ACHIEVEMENT_DELAY_MAX - 1.0f)
      spriteScroll.setTextureRect(sf::IntRect(32 * ((int)(achievAge * 8) % 4), 0, 32, 64));
    else
      spriteScroll.setTextureRect(sf::IntRect(0, 0, 32, 64));
    spriteScroll.setPosition(xPos + 394 - opening - 16, yPos);
    app->draw(spriteScroll);

    if (achievAge > 2.0f && achievAge < ACHIEVEMENT_DELAY_MAX - 1.0f)
    {
      sf::Sprite icon;
      icon.setTexture(*ImageManager::getInstance().getImage(IMAGE_ACHIEVEMENTS));
      icon.setTextureRect(sf::IntRect( ((achievementsQueue.front().type + 1) % 10) * 64,
                                       ((achievementsQueue.front().type + 1) / 10) * 64, 64, 64));

      icon.setPosition(xPos + 308, yPos + 9);
      icon.setScale(0.7f, 0.7f);
      app->draw(icon);
      icon.setColor(sf::Color(255, 255, 255, 50 + 50 * cosf(getAbsolutTime() * 4)));
      app->draw(icon, sf::BlendAdd);

      game().write(achievementsQueue.front().message, 13, xPos + 34, yPos + 10, ALIGN_LEFT, sf::Color::Black, app, 0, 0, 0);

      if (achievements[achievementsQueue.front().type].unlockType == UnlockItem)
      {
        game().write(tools::getLabel(items[achievements[achievementsQueue.front().type].unlock].name),
                     13, xPos + 70, yPos + 34, ALIGN_LEFT, sf::Color::Black, app, 0, 0, 0);
      }
      else if (achievements[achievementsQueue.front().type].unlockType == UnlockFunctionality)
      {
        game().write(tools::getLabel(functionalityLabel[achievements[achievementsQueue.front().type].unlock]),
                     13, xPos + 70, yPos + 34, ALIGN_LEFT, sf::Color::Black, app, 0, 0, 0);
      }
    }

    //if (achievAge < 1.0f)
    {
      int i = achievementsQueue.front().counter;

      if (i < 64)
      {
        for (int j = 0; j < 2; j++)
        {
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40, yPos - i + 64);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40, yPos - i + 64 + 1);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40, yPos - i + 64 + 2);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40, yPos - i + 64 + 3);
        }
      }
      else if (i < 40 + 64)
      {
        for (int j = 0; j < 2; j++)
        {
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40 + i - 64, yPos);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40 + i - 63, yPos);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40 + i - 62, yPos);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - 40 + i - 61, yPos);
        }
      }
      else if (i < 40 + 64 + 64)
      {
        for (int j = 0; j < 2; j++)
        {
          generateUiParticle(xPos + 394 + 16 + 4 - opening, yPos + i - 40 - 64);
          generateUiParticle(xPos + 394 + 16 + 4 - opening, yPos + i - 40 - 64 + 1);
          generateUiParticle(xPos + 394 + 16 + 4 - opening, yPos + i - 40 - 64 + 2);
          generateUiParticle(xPos + 394 + 16 + 4 - opening, yPos + i - 40 - 64 + 3);
        }
      }
      else if (i < 40 + 64 + 40 + 64)
      {
        for (int j = 0; j < 2; j++)
        {
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - i + 144, yPos + 64);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - i + 144 + 1, yPos + 64);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - i + 144 + 2, yPos + 64);
          generateUiParticle(xPos + 394 + 16 + 4 - 32 - i + 144 + 3, yPos + 64);
        }
      }

      achievementsQueue.front().counter += 4;
    }
  }
}

void WitchBlastGame::renderMessages()
{
  int dy = 0;
  // message queue
  if (!messagesQueue.empty())
  {
    dy = 40;

    if (messagesQueue.front().timer < 0.5f)
    {
      dy *= (2 * messagesQueue.front().timer);
    }
    else if (messagesQueue.front().timerMax - messagesQueue.front().timer < 0.5f)
    {
      dy *= (2 * (messagesQueue.front().timerMax - messagesQueue.front().timer));
    }

    uiSprites.msgBoxSprite.setTextureRect(sf::IntRect(0, 0, 970, dy + 2));
    uiSprites.msgBoxSprite.setPosition(0, 600 - dy);
    app->draw(uiSprites.msgBoxSprite);

    std::stringstream ss;
    ss << messagesQueue.front().message[0];
    ss << ": ";
    ss << messagesQueue.front().message[1];
    ss << std::endl;
    ss << messagesQueue.front().message[2];


    write(ss.str(), 16, 10, 602 - dy, ALIGN_LEFT, sf::Color::White, app, 0, 0, 0);
  }

  // show player effects
  if (isPlayerAlive)
  {
    float x0 = 18, y0 = 585, xStep = 160;

    for (int i = 0; i < NB_SPECIAL_STATES; i++)
    {
      if ( (player->isSpecialStateActive((enumSpecialState)i) && player->getSpecialState((enumSpecialState)i).timer > 0.35f)
          || player->getSpecialState((enumSpecialState)i).waitUnclear)
      {
        std::stringstream oss;
        oss << tools::getLabel(specialStateToLabel[i]) << " : ";
        if (player->getSpecialState((enumSpecialState)i).waitUnclear)
        {
          oss << "waiting";
          int n = (int)(getAbsolutTime() * 3) % 3;
          if (n == 0) oss << ".";
          else if (n == 1) oss << "..";
          else oss << "...";
        }
        else
          oss << (int)(player->getSpecialState((enumSpecialState)i).timer);

        write(oss.str(), 12, x0, y0 - dy, ALIGN_LEFT, specialStateToColor[i], app, 0, 0, 0);
        x0 += xStep;
      }
    }
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

  int width = 810, height = 300, border = 4;
  int x = 80, y = 110;
  sf::Image screenShot(app->capture());
  sf::Image savedImage;
  savedImage.create(width + border * 2, height + border * 2);
  savedImage.copy(screenShot,0 , 0, sf::IntRect( x - border, y - border, width + border * 2, height + border * 2));
  savedImage.saveToFile(ss.str());
}

void WitchBlastGame::saveScreen()
{
  std::stringstream ss;
  ss << "screenshot_";
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

  sf::Image screenShot(app->capture());
  screenShot.saveToFile(ss.str());
}

void WitchBlastGame::renderDeathScreen(float x, float y)
{
  int xRect = 810;

  sf::Sprite rectangle;
  rectangle.setTexture(*ImageManager::getInstance().getImage(IMAGE_DEATH_CERTIFICATE));
  rectangle.setPosition(x - 4, y - 4);
  app->draw(rectangle);

  std::stringstream ss;
  if (player->isDead())
    ss << parameters.playerName << " - " << tools::getLabel("dc_certificate");
  else
  {
    ss << parameters.playerName << " - " << tools::getLabel("dc_victory");

    sf::Sprite seal;
    seal.setTexture(*ImageManager::getInstance().getImage(IMAGE_WIN_SEAL));
    seal.setPosition(x + 650, y + 50);
    app->draw(seal);
  }

  write(ss.str(), 18, x + xRect / 2, y + 5, ALIGN_CENTER, sf::Color::Black, app, 0, 0, 0);

  ss.str(std::string());
  ss.clear();

  int minutes = (int)gameTime / 60;
  if (minutes < 1) minutes = 1;

  if (player->isDead())
  {
    ss << tools::getLabel("dc_killed_by") << " " << sourceToString(player->getLastHurtingSource(), player->getLastHurtingEnemy()) << "." << std::endl;
    ss << tools::getLabel("dc_died_level") << " " << level << " " << tools::getLabel("dc_after") << " " << minutes << " " << tools::getLabel("dc_minutes") << "." << std::endl;
  }
  else
  {
    ss << tools::getLabel("dc_after_victory") << " " << minutes << " " << tools::getLabel("dc_minutes") << "." << std::endl;
  }

  ss << tools::getLabel("dc_killed_monsters") << ": " << bodyCount << std::endl;
  ss << tools::getLabel("dc_gold") << ": " << player->getGold() << std::endl;
  ss << tools::getLabel("dc_challenges") << ": " << challengeLevel - 1 << std::endl;

  write(ss.str(), 16, x + 112, y + 50, ALIGN_LEFT, sf::Color::Black, app, 0, 0, 0);

  // player
  renderPlayer(x + 40, y + 48, player->getEquipment(), player->getShotType(), 1, 0);

  // items
  write(tools::getLabel("inventory"), 16, x + 14, y + 165, ALIGN_LEFT, sf::Color::Black, app, 0, 0, 0);
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
  write(ss.str(), 24, x + xRect / 2, y + 240, ALIGN_CENTER, sf::Color::Black, app, 0, 0, 0);

}

bool compareScores(WitchBlastGame::StructScore s1, WitchBlastGame::StructScore s2)
{
  return s1.score > s2.score;
}

void WitchBlastGame::calculateScore()
{
  saveStats();

  std::ostringstream oss;

  int goldScore = getGoldScore(player->getGold());

  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
  {
    if (player->isEquiped(i)) goldScore += getItemScore((item_equip_enum)i);
    lastScore.equip[i] = player->isEquiped(i);
  }

  addBonusScore(BonusPossession, goldScore);

  // time
  if (!player->isDead())
  {
    lastScore.level = -1;
  }
  else
    lastScore.level = level;

  // to save
  lastScore.name = parameters.playerName;
  lastScore.score = score;

  lastScore.shotType = player->getShotType();

  lastScore.divinity = player->getDivinity().divinity;

  lastScore.killedBy = player->getLastHurtingEnemy();

  lastScore.time = (int)gameTime;

  if (nbPlayers > 1) return;

  scores.push_back(lastScore);

  std::sort (scores.begin(), scores.end(), compareScores);

  saveHiScores();

  // Online
#ifdef ONLINE_MODE
  if (autosave || !gameFromSaveFile)
  {
    sendScoreToServer();
  }
  else
  {
    receiveScoreFromServer();
  }

#endif // ONLINE_MODE
}

void WitchBlastGame::switchToMenu()
{
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
  SoundManager::getInstance().playSound(SOUND_NIGHT, false);
  menuStuct* menu = NULL;

  bool noMenu = false;

  if (menuState == MenuStateMain)
    menu = &menuMain;
  else if (menuState == MenuStateConfig)
    menu = &menuConfig;
  else if (menuState == MenuStateFirst)
    menu = &menuFirst;
  else
    noMenu = true;


  bool escape = false;

  EntityManager::getInstance().animate(deltaTime);
  for (int i = 0; i < 2; i++)
  {
    introSprites[i]->setX(introSprites[i]->getX() - deltaTime * 35);
    if (introSprites[i]->getX() < - SCREEN_WIDTH / 2) introSprites[i]->setX(introSprites[i]->getX() + 2 * SCREEN_WIDTH);
  }

  // Process events
  sf::Event event;
  while (app->pollEvent(event))
  {
    // Close window : exit
    if (event.type == sf::Event::Closed)
    {
      saveGameData();
      app->close();
    }

    if (event.type == sf::Event::Resized)
    {
      enableAA(true);
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
    else if (menuState == MenuStateJoystick)
    {
      bool alreadyUsed = false;
      int nbButtons = sf::Joystick::getButtonCount(0);

      bool found = false;
      JoystickInputStruct jInput;

      // Escape = out
      if (event.key.code == sf::Keyboard::Escape)
      {
        menuState = MenuStateConfig;
        saveConfigurationToFile();
        return;
      }

      // button pressed ?
      for (int i = 0; !found && i < nbButtons; i++)
      {
        if (sf::Joystick::isButtonPressed(0, i))
        {
          jInput.isButton = true;
          jInput.value = i;
          jInput.axis = sf::Joystick::X;
          found = true;
        }
      }

      if (!found)
      {
        // axis ?
        for (int i = sf::Joystick::X; i <= sf::Joystick::PovY; i++)
        {
          if (sf::Joystick::hasAxis(0, (sf::Joystick::Axis)i))
          {
            if (sf::Joystick::getAxisPosition(0, (sf::Joystick::Axis)i) < -50)
            {
              jInput.isButton = false;
              jInput.value = -1;
              jInput.axis = (sf::Joystick::Axis)i;
              found = true;
            }
            else if (sf::Joystick::getAxisPosition(0, (sf::Joystick::Axis)i) > 50)
            {
              jInput.isButton = false;
              jInput.value = 1;
              jInput.axis = (sf::Joystick::Axis)i;
              found = true;
            }
          }
        }
      }

      if (found)
      {
        // already exist ?
        for (unsigned int i = 0; i < menuKeyIndex; i++)
        {
          if (jInput.isButton && joystickInput[i].isButton
              && joystickInput[i].value == jInput.value)
            alreadyUsed = true;

          if (!jInput.isButton && !joystickInput[i].isButton
              && joystickInput[i].axis == jInput.axis
              && joystickInput[i].value == jInput.value)
            alreadyUsed = true;
        }

        if (!alreadyUsed)
        {
          joystickInput[menuKeyIndex] = jInput;
          menuKeyIndex++;
          if (menuKeyIndex == NumberKeys)
          {
            menuState = MenuStateConfig;
            saveConfigurationToFile();
          }
        }
      }
    }

    else
    {
      if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::F1)
        {
            saveScreen();
        }
        if (menuState == MenuStateChangeName)
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
          escape = true;
        }
      }
    }
  }
  // END EVENT PROCESSING

  if (menuState == MenuStateAchievements)
  {
    if (isPressing(0, KeyRight, true))
    {
      if (menuAchIndex % 8 < 7) menuAchIndex++;
    }
    else if (isPressing(0, KeyLeft, true))
    {
      if (menuAchIndex % 8 > 0) menuAchIndex--;
    }
    else if (isPressing(0, KeyDown, true))
    {
      if (menuAchIndex / 8 < ACHIEV_LINES) menuAchIndex += 8;
    }
    else if (isPressing(0, KeyUp, true))
    {
      if (menuAchIndex / 8 > 0) menuAchIndex -= 8;
    }
    else if (isPressing(0, KeyFireDown, true))
    {
      if (menuAchIndex / 8 >= ACHIEV_LINES) menuState = MenuStateMain;
    }
    if (escape) menuState = MenuStateMain;
  }

  else if (menuState == MenuStateCredits)
  {
    if (escape || isPressing(0, KeyFireDown, true)) menuState = MenuStateMain;
  }

  else if (menuState == MenuStateHiScores)
  {
    if (escape || isPressing(0, KeyFireDown, true))
    {
      menuScoreIndex++;
      if (menuScoreIndex > 2)
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
  }

  if (!noMenu)
  {
    if (escape)
    {
      saveGameData();
      app->close();
    }

    else if (isPressing(0, KeyDown, true))
    {
      menu->index++;
      if (menu->index == menu->items.size()) menu->index = 0;
      SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
    }

    else if (isPressing(0, KeyUp, true))
    {
      if (menu->index == 0) menu->index = menu->items.size() - 1;
      else menu->index--;

      SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
    }
    else if (isPressing(0, KeyRight, true))
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
      else if (menu->items[menu->index].id == MenuStartNew)
      {
        nbPlayers++;
        if (nbPlayers > NB_PLAYERS_MAX) nbPlayers = 1;
        if (nbPlayers > 1 && !sf::Joystick::isConnected(0)) nbPlayers = 1;
        SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        buildMenu(true);
      }
    }
    else if (isPressing(0, KeyLeft, true))
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
      else if (menu->items[menu->index].id == MenuStartNew)
      {
        nbPlayers--;
        if (nbPlayers <= 0) nbPlayers = NB_PLAYERS_MAX;
        if (nbPlayers > 1 && !sf::Joystick::isConnected(0)) nbPlayers = 1;
        SoundManager::getInstance().playSound(SOUND_SHOT_SELECT);
        buildMenu(true);
      }
    }
    else if (isPressing(0, KeyFireDown, true))
    {
      switch (menu->items[menu->index].id)
      {
      case MenuStartNew:
        remove(SAVE_FILE.c_str());
        startNewGame(false, 1);
        break;
      case MenuStartOld:
        startNewGame(true, 1);
        break;
      case MenuKeys:
        menuState = MenuStateKeys;
        menuKeyIndex = 0;
        break;
      case MenuJoystick:
        if (sf::Joystick::isConnected(0))
        {
          buildMenu(true);
          menuState = MenuStateJoystick;
          menuKeyIndex = 0;
        }
        else
          buildMenu(true);
        break;
      case MenuCredits:
        menuState = MenuStateCredits;
        break;
      case MenuHiScores:
        menuState = MenuStateHiScores;
#ifdef ONLINE_MODE
        menuScoreIndex = 0;
#else
        menuScoreIndex = 2;
#endif
        receiveScoreFromServer();
        break;
      case MenuAchievements:
        menuState = MenuStateAchievements;
        menuAchIndex = 0;
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
        saveGameData();
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
        saveGameData();
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

void WitchBlastGame::renderMenu()
{
  if (menuState == MenuStateCredits)
  {
    renderCredits();
    return;
  }
  else if (menuState == MenuStateHiScores)
  {
    if (menuScoreIndex == 0)
      renderScores(scoresOnline, "Best Players (ON-LINE)", true);
    else if (menuScoreIndex == 1)
      renderScores(scoresOnlineDay, "Best TODAY Scores (ON-LINE)", true);
    else
      renderScores(scores, "Best Scores (local)", false);
    return;
  }
  else if (menuState == MenuStateAchievements)
  {
    renderAchievements();
    return;
  }

  app->draw(introScreenSprite);
  if (titleSprite.getPosition().y > 160) titleSprite.move(0, -8);
  else if (titleSprite.getPosition().y < 160) titleSprite.setPosition(SCREEN_WIDTH / 2 - 15, 160);
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
  int yTop = 320;
  int yStep = 40;

  if (menuState == MenuStateKeys)
  {
    // menu keys
    if (config.configFileExists())
      write(tools::getLabel("key_configuration"), 18, xAlign, 295, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
    else
      write(tools::getLabel("key_configuration_desc"), 18, xAlign, 295, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      sf::Color itemColor;
      if (menuKeyIndex == i) itemColor = sf::Color(255, 255, 255, 255);
      else itemColor = sf::Color(180, 180, 180, 255);
      std::ostringstream oss;
      oss << tools::getLabel(inputKeyString[i]) << ": ";
      if (menuKeyIndex == i) oss << tools::getLabel("key_configuration_insert");
      else if (menuKeyIndex > i) oss << keyToString(input[i]);
      write(oss.str(), 16, xAlign, 330 + i * 25, ALIGN_LEFT, itemColor, app, 1, 1, 0);
    }
  }

  else if (menuState == MenuStateJoystick)
  {
    // menu keys
    if (config.configFileExists())
      write(tools::getLabel("joystick_configuration"), 18, xAlign, 295, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
    else
      write(tools::getLabel("joystick_configuration_desc"), 18, xAlign, 295, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      sf::Color itemColor;
      if (menuKeyIndex == i) itemColor = sf::Color(255, 255, 255, 255);
      else itemColor = sf::Color(180, 180, 180, 255);
      std::ostringstream oss;
      oss << tools::getLabel(inputKeyString[i]) << ": ";
      if (menuKeyIndex == i) oss << tools::getLabel("joystick_configuration_insert");
      else if (menuKeyIndex > i) oss << "OK";
      write(oss.str(), 16, xAlign, 330 + i * 25, ALIGN_LEFT, itemColor, app, 1, 1, 0);
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
        fairySprite.setTextureRect(sf::IntRect( 48 * ((int)(20 *getAbsolutTime()) % 2), fairySpriteOffsetY, 48, 48));
        fairySprite.setPosition(xAlign - 60, yTop - 10 + i * yStep + 5 * cos( 6 * getAbsolutTime()));
        app->draw(fairySprite);
      }
      else itemColor = sf::Color(120, 120, 120, 255);

      std::string label = menu->items[i].label;
      if (menu->items[i].id == MenuLanguage)
      {
        std::ostringstream oss;
        oss << label << " : " << tools::getLabel(languageString[parameters.language]);
        oss << languageState[parameters.language];
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

      write(label, 21, xAlign, yTop + i * yStep, ALIGN_LEFT, itemColor, app, 1, 1, 0);
    }
    write(menu->items[menu->index].description, 18, xAlign,
          yTop + menu->items.size() * yStep + 8, ALIGN_LEFT, sf::Color(60, 80, 220), app, 0, 0, 0);

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
      write(tools::getLabel("keys_move"), 16, xKeys + 190, yKeys + 10, ALIGN_LEFT, sf::Color::White, app, 1, 1, 0);
      write(tools::getLabel("keys_time"), 16, xKeys + 295, yKeys + 14, ALIGN_LEFT, sf::Color::White, app, 1, 1, 0);
      write(tools::getLabel("keys_fire"), 16, xKeys + 360, yKeys + 54, ALIGN_LEFT, sf::Color::White, app, 1, 1, 0);
      write(tools::getLabel("key_spell"), 16, xKeys + 148, yKeys + 184, ALIGN_CENTER, sf::Color::White, app, 1, 1, 0);
      // TODO key interact
      std::ostringstream oss;
      oss << tools::getLabel("keys_select_1") << std::endl << tools::getLabel("keys_select_2");
      write(oss.str(), 16, xKeys + 4, yKeys + 100, ALIGN_LEFT, sf::Color::White, app, 1, 1, 0);
    }
  }

  std::ostringstream oss;
  oss << APP_NAME << " v" << APP_VERSION << "  - 2014-2015 - " << " Seby (code), Pierre \"dejam0rt\" Baron (2D art)";
  write(oss.str(), 17, 5, 680, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
}

void WitchBlastGame::renderAchievements()
{
  app->draw(introScreenSprite);
  if (titleSprite.getPosition().y > 160) titleSprite.move(0, -8);
  else if (titleSprite.getPosition().y < 160) titleSprite.setPosition(SCREEN_WIDTH / 2 - 15, 180);
  app->draw(titleSprite);

  // achievements
  write(tools::getLabel("menu_achievements"), 30, 485, 280, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 1, 1, 0);

  int achWidth = 64, achHeight = 64, x0 = 180, y0 = 380, xStep = 16, yStep = 16, nbProLine = 8;

  if (menuAchIndex / 8 < ACHIEV_LINES)
  {
    sf::RectangleShape rectangle(sf::Vector2f(achWidth, achHeight));
    rectangle.setPosition(x0 + (menuAchIndex % nbProLine) * (achWidth + xStep), y0 + (menuAchIndex / nbProLine) * (achHeight + yStep));
    rectangle.setOutlineThickness(3);
    rectangle.setOutlineColor(sf::Color(50, 255, 50));
    app->draw(rectangle);
  }

  sf::Sprite sprite;
  sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ACHIEVEMENTS));

  for (int k : sortedAchievements)
  {
    int i = sortedAchievements[k];
    sprite.setPosition(x0 + (k % nbProLine) * (achWidth + xStep), y0 + (k / nbProLine) * (achHeight + yStep));
    if (achievementState[i] == AchievementDone)
    {
      sprite.setTextureRect(sf::IntRect( ((i + 1) % 10) * achWidth, ((i + 1) / 10) * achHeight, achWidth, achHeight));
    }
    else
    {
      sprite.setTextureRect(sf::IntRect(0, 0, achWidth, achHeight));
    }
    app->draw(sprite);
  }

  if (menuAchIndex / 8 >= ACHIEV_LINES)
  {
    write(tools::getLabel("config_back"), 17, 485, 620, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
  }
  else
  {
    write(tools::getLabel("config_back"), 17, 485, 620, ALIGN_CENTER, sf::Color(180, 180, 180, 255), app, 1, 1, 0);

    sf::Color fontColor = sf::Color::White;
    std::stringstream oss;
    int achIndex = sortedAchievements[menuAchIndex];

    if (!achievementState[achIndex] == AchievementDone && (
          achIndex == AchievementGiantSlime
          || achIndex == AchievementCyclops
          || achIndex == AchievementRatKing
          || achIndex == AchievementGiantSpider
          || achIndex == AchievementFrancky
          || achIndex == AchievementVampire)
       )
      oss << "???";
    else
      oss << tools::getLabel(achievements[achIndex].label);
    oss << ": ";
    if (achievementState[achIndex] == AchievementDone)
    {
      oss << tools::getLabel(achievements[achIndex].label + "_desc");
      if (achievements[achIndex].unlockType == UnlockItem && achievements[achIndex].unlock > -1)
        oss << "\nUNLOCK: " << tools::getLabel(items[achievements[achIndex].unlock].name);
      else if (achievements[achIndex].unlockType == UnlockFunctionality && achievements[achIndex].unlock > -1)
        oss << "\nUNLOCK: " << tools::getLabel(functionalityLabel[achievements[achIndex].unlock]);
    }
    else
    {
      if (isFunctionalityLocked(FunctionalityAllAchievements))
        oss << "???";
      else
        oss << tools::getLabel(achievements[achIndex].label + "_desc");
      fontColor = sf::Color(150, 150, 150);
    }

    write(oss.str(), 19, 100, 650, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
  }
}

void WitchBlastGame::renderCredits()
{
  app->draw(introScreenSprite);
  if (titleSprite.getPosition().y > 160) titleSprite.move(0, -8);
  else if (titleSprite.getPosition().y < 160) titleSprite.setPosition(SCREEN_WIDTH / 2 - 15, 180);
  app->draw(titleSprite);

  // credits
  write(tools::getLabel("credits"), 30, 485, 275, ALIGN_CENTER, sf::Color(255, 255, 255, 255), app, 1, 1, 0);

  int yCursorInit = 365;
  int yStep = 30;
  int xLeft = 30;
  int xRight = 470;
  int xMarging = 20;

  int yCursor = yCursorInit;
  write("Code", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0, 0, 0);
  yCursor += yStep;
  int i = 0;
  while (creditsCode[i] != "END")
  {
    write(creditsCode[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("2D Art", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0, 0, 0);
  yCursor += yStep;
  i = 0;
  while (credits2D[i] != "END")
  {
    write(credits2D[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("Sound", 22, xLeft, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0, 0, 0);
  yCursor += yStep;
  i = 0;
  while (creditsSound[i] != "END")
  {
    write(creditsSound[i], 19, xLeft + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;


  ////// RIGHT

  yCursor = yCursorInit + yStep;
  write("Music", 22, xRight, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0, 0, 0);
  yCursor += yStep;
  i = 0;
  while (creditsMusic[i] != "END")
  {
    write(creditsMusic[i], 19, xRight + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;

  write("Translation", 22, xRight, yCursor, ALIGN_LEFT, sf::Color(210, 210, 255, 255), app, 0, 0, 0);
  yCursor += yStep;
  i = 0;
  while (creditsTranslate[i] != "END")
  {
    write(creditsTranslate[i], 19, xRight + xMarging, yCursor, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
    yCursor += yStep;
    i++;
  }
  yCursor += yStep;
}

void WitchBlastGame::renderScores(std::vector <StructScore> scoresToRender, std::string title, bool blinkingName)
{
  sf::Sprite bgSprite;
  bgSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_HALL_OF_FAME));
  app->draw(bgSprite);

  // hi-scores-title
  writeGraphic(title, 30, 485, 60, ALIGN_CENTER, sf::Color(0, 0, 0, 255), app, 0, 0, 0);

  int xEquip = 540;
  //int yEquip = 360;

  int xPlayerLeft = 60;
  int xPlayerRight = 850;

  int xName = 260;
  int xTime = 420;
  int xLevel = 480;
  int xScore = 180;

  int y0 = 130;
  int yStep = 50;

  sf::Sprite itemSprite;

  for (unsigned int i = 0; i < scoresToRender.size() && i < SCORES_MAX; i++)
  {
    sf::Color color = sf::Color( 15, 15, 15);
    if (scoresToRender[i].score == lastScore.score
        && scoresToRender[i].level == lastScore.level
        && scoresToRender[i].name == lastScore.name
        && scoresToRender[i].time == lastScore.time)
    {
      int fade = 1 + cosf(getAbsolutTime() * 8) * 63;
      color = sf::Color(255 - fade, 128, 255 - fade);
    }
    else if (blinkingName && scoresToRender[i].name == parameters.playerName)
    {
      int fade = 70 + sinf(getAbsolutTime() * 4) * 70;
      color = sf::Color(fade * 1.5, fade, 0);
    }

    // equipment
    int n = 0;
    for (auto ii: scoreEquipement)
      if (ii != EQUIP_BOSS_KEY && scoresToRender[i].equip[ii]) n++;

    int dx = 30;
    if (n > 14) dx = 16;
    else if (n > 8) dx = 20;
    n = 0;

    itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_ITEMS_EQUIP));
    for (auto ii: scoreEquipement)
    {
      if (ii != EQUIP_BOSS_KEY && scoresToRender[i].equip[ii])
      {
        itemSprite.setPosition(xEquip + n * dx, y0 + 22 + yStep * i);
        itemSprite.setTextureRect(sf::IntRect((ii % 10) * 32, (ii / 10) * 32, 32, 32));
        app->draw(itemSprite);
        n++;
      }
    }

    if (i < 2)
      renderPlayer((i % 2 == 0) ? xPlayerLeft : xPlayerRight, y0 - 14 + yStep * i, scoresToRender[i].equip, scoresToRender[i].shotType, 2, 7);
    else if (i >= 8)
      renderPlayer((i % 2 == 0) ? xPlayerLeft : xPlayerRight, y0 - 14 + yStep * i, scoresToRender[i].equip, scoresToRender[i].shotType, 0, 8);
    else
      renderPlayer((i % 2 == 0) ? xPlayerLeft : xPlayerRight, y0 - 14 + yStep * i, scoresToRender[i].equip, scoresToRender[i].shotType, 1, 0);

    write(scoresToRender[i].name, 16, xName, y0 + 28 + yStep * i, ALIGN_LEFT, color, app, 0, 0, 0);

    if (scoresToRender[i].level == -1)
    {
      itemSprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_WIN_SEAL_HOF));
      itemSprite.setTextureRect(sf::IntRect(0, 0, 48, 48));
      itemSprite.setPosition(475, y0 + 20 + yStep * i);
      app->draw(itemSprite);
    }
    else
    {
      std::stringstream levelSS;
      levelSS << "lvl " << scoresToRender[i].level;
      write(levelSS.str(), 16, xLevel, y0 + 28 + yStep * i, ALIGN_LEFT, color, app, 0, 0, 0);
      write(intToString(scoresToRender[i].score), 17, xScore, y0 + 28 + yStep * i, ALIGN_LEFT, color, app, 0, 0, 0);
    }

    std::stringstream timeSS;
    if (scoresToRender[i].time < 100)
    {
      timeSS << scoresToRender[i].time << " s";
    }
    else
    {
      int minutes = scoresToRender[i].time / 60;
      if (minutes < 1) minutes = 1;
      timeSS << minutes << " m";
    }

    write(timeSS.str(), 16, xTime, y0 + 28 + yStep * i, ALIGN_LEFT, color, app, 0, 0, 0);
    write(intToString(scoresToRender[i].score), 17, xScore, y0 + 28 + yStep * i, ALIGN_LEFT, color, app, 0, 0, 0);
  }

  // retrieving from DB ?
  if ( (menuScoreIndex == 0 && scoreState == ScoreLoading)
       || (menuScoreIndex == 1 && ((scoreState == ScoreLoading) || (scoreState == ScoreLoadingDay))))
  {
    std::stringstream oss;
    oss << "Loading";
    int n = (int)(getAbsolutTime() * 3) % 3;
    if (n == 0) oss << ".";
    else if (n == 1) oss << "..";
    else oss << "...";

    write(oss.str(), 15, 20, 20, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);
  }
}

void WitchBlastGame::renderInGameMenu()
{
  menuStuct* menu = &menuInGame;

  int xAlign = 90;
  int yAlign = 100;

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
        fairySprite.setTextureRect(sf::IntRect( 48 * ((int)(8 *getAbsolutTime()) % 2), fairySpriteOffsetY, 48, 48));
        fairySprite.setPosition(xAlign - 60, yAlign + i * 90 + 5 * cos( 6 * getAbsolutTime()));
        app->draw(fairySprite);
      }
      else itemColor = sf::Color(120, 120, 120, 255);

      std::string label = menu->items[i].label;
      write(label, 23, xAlign, yAlign + 10 + i * 90, ALIGN_LEFT, itemColor, app, 1, 1, 0);
      write(menu->items[i].description, 15, xAlign, yAlign + i * 90 + 50, ALIGN_LEFT, itemColor, app, 0, 0, 0);
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
    if (deltaTime < 0.008f)
    {
      float sleepTime = 1.0f / 60.0f - deltaTime;
      sf::sleep(sf::seconds(sleepTime));
      deltaTime = getAbsolutTime() - lastTime;
    }

    lastTime = getAbsolutTime();
    if (deltaTime > 0.05f) deltaTime = 0.05f;

    if (app->hasFocus())
    {
      updateActionKeys();

      switch (gameState)
      {
      case gameStateInit:
      case gameStateKeyConfig:
      case gameStateJoystickConfig:
      case gameStateMenu:
        updateMenu();
        break;
      case gameStateIntro:
        updateIntro();
        break;
      case gameStatePlaying:
      case gameStatePlayingPause:
      case gameStatePlayingDisplayBoss:
        updateRunningGame();
        break;
      }
    }
    else
    {
      // Process events
      sf::Event event;
      while (app->pollEvent(event))
      {
        // Close window : exit
        if (event.type == sf::Event::Closed)
        {
          saveGameData();
          app->close();
        }

        if (event.type == sf::Event::LostFocus)
        {
          if (parameters.pauseOnFocusLost && gameState == gameStatePlaying && !player->isDead())
            gameState = gameStatePlayingPause;
        }
      }
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
      if (currentMap->isDoor(i, 0)) currentMap->closeDoor(i, 0);
      if (currentMap->isDoor(i, MAP_HEIGHT - 1)) currentMap->closeDoor(i, MAP_HEIGHT - 1);
    }
    for(i = 0; i < MAP_HEIGHT; i++)
    {
      if (currentMap->isDoor(0, i)) currentMap->closeDoor(0, i);
      if (currentMap->isDoor(MAP_WIDTH - 1, i)) currentMap->closeDoor(MAP_WIDTH - 1, i);
    }
    roomClosed = true;
  }
}

void WitchBlastGame::openDoors()
{
  int i;

  for(i = 0; i < MAP_WIDTH; i++)
  {
    if (currentMap->isDoor(i, 0)) currentMap->openDoor(i, 0);
    if (currentMap->isDoor(i, MAP_HEIGHT - 1)) currentMap->openDoor(i, MAP_HEIGHT - 1);
  }
  for(i = 0; i < MAP_HEIGHT; i++)
  {
    if (currentMap->isDoor(0, i)) currentMap->openDoor(0, i);
    if (currentMap->isDoor(MAP_WIDTH - 1, i)) currentMap->openDoor(MAP_WIDTH - 1, i);
  }

  roomClosed = false;
  SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);

  if (currentMap->hasNeighbourUp() == 2 && !bossRoomOpened)
    currentMap->closeDoor(MAP_WIDTH/2, 0);
  else if (currentMap->hasNeighbourUp() == 1)
    doorEntity[0]->openDoor();

  if (currentMap->hasNeighbourLeft() == 2 && !bossRoomOpened)
    currentMap->closeDoor(0, MAP_HEIGHT / 2);
  else if (currentMap->hasNeighbourLeft() == 1)
    doorEntity[1]->openDoor();

  if (currentMap->hasNeighbourDown() == 2 && !bossRoomOpened)
    currentMap->closeDoor(MAP_WIDTH / 2, MAP_HEIGHT - 1);
  else if (currentMap->hasNeighbourDown() == 1)
    doorEntity[2]->openDoor();

  if (currentMap->hasNeighbourRight() == 2 && !bossRoomOpened)
    currentMap->closeDoor(MAP_WIDTH - 1, MAP_HEIGHT / 2);
  else if (currentMap->hasNeighbourRight() == 1)
    doorEntity[3]->openDoor();
}

EnemyEntity* WitchBlastGame::getBoss()
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() == ENTITY_ENEMY_BOSS) return static_cast<EnemyEntity*> (e);
  }

  return NULL;
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

int WitchBlastGame::getUndeadCount()
{
  int n=0;

  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX_COUNT)
    {
      EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(e);
      if (enemy->canCollide() && (enemy->getEnemyType() == EnemyTypeZombie || enemy->getEnemyType() == EnemyTypeZombieDark
                                  || enemy->getEnemyType() == EnemyTypeGhost || enemy->getEnemyType() == EnemyTypeVampire || enemy->getEnemyType() == EnemyTypeBat_invocated) )
        n++;
    }
  }

  return n;
}

bool WitchBlastGame::existsEffectZone()
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() == ENTITY_EFFECT_ZONE) return true;
  }

  return false;
}

void WitchBlastGame::killArtefactDescription()
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() == ENTITY_ARTIFACT_DESCRIPTION)
    {
      e->setDying(true);
    }
  }
}

void WitchBlastGame::animateEffects()
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() == ENTITY_EFFECT)
      e->animate(deltaTime);
  }
}

void WitchBlastGame::destroyUndead(int damage)
{
  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() >= ENTITY_ENEMY && e->getType() <= ENTITY_ENEMY_MAX_COUNT)
    {
      EnemyEntity* enemy = dynamic_cast<EnemyEntity*>(e);
      if (enemy->canCollide() && (enemy->getEnemyType() == EnemyTypeZombie || enemy->getEnemyType() == EnemyTypeZombieDark
                                  || enemy->getEnemyType() == EnemyTypeGhost || enemy->getEnemyType() == EnemyTypeVampire || enemy->getEnemyType() == EnemyTypeBat_invocated) )
      {
        enemy->hurt(BaseCreatureEntity::getHurtParams(damage, ShotTypeStandard, 0, false, SourceTypeMelee, EnemyTypeNone, false));

        SpriteEntity* spriteCone = new SpriteEntity(
          ImageManager::getInstance().getImage(IMAGE_LIGHT_CONE),
          enemy->getX(), enemy->getZ() - 290);
        spriteCone->setZ(1000.0f);
        spriteCone->setFading(true);
        spriteCone->setAge(-1.2f);
        spriteCone->setLifetime(2.4f);
        spriteCone->setRenderAdd();

      }

    }
  }
}

int WitchBlastGame::getItemsCount()
{
  int n=0;

  EntityManager::EntityList* entityList =EntityManager::getInstance().getList();
  EntityManager::EntityList::iterator it;

  for (it = entityList->begin (); it != entityList->end ();)
  {
    GameEntity *e = *it;
    it++;

    if (e->getType() == ENTITY_ITEM) n++;
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

void WitchBlastGame::setDoorVisible(int n)
{
  if (n >= 0 && n < 4)
    doorEntity[n]->setVisible(true);
}

void WitchBlastGame::checkDoor(int doorId, roomTypeEnum roomCurrent, roomTypeEnum roomNeighbour, bool isNeighbourKnown)
{
  if (roomNeighbour == roomTypeNULL)
  {
    doorEntity[doorId]->setVisible(false);
    return;
  }
  doorEntity[doorId]->setVisible(isNeighbourKnown);

  doorEntity[doorId]->setDoorType(currentMap->getDoorType(doorId));

  if (roomNeighbour == roomTypeBoss && !bossRoomOpened)
  {
    doorEntity[doorId]->setOpen(false);
    switch (doorId)
    {
    case 0:
      currentMap->closeDoor(MAP_WIDTH/2, 0);
      break;
    case 1:
      currentMap->closeDoor(0, MAP_HEIGHT / 2);
      break;
    case 2:
      currentMap->closeDoor(MAP_WIDTH/2, MAP_HEIGHT - 1);
      break;
    case 3:
      currentMap->closeDoor(MAP_WIDTH - 1, MAP_HEIGHT / 2);
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
  bool generateMap =  !(currentFloor->getMap(floorX, floorY)->isVisited());
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
                    (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT,
                    0);
    }
  }

  // check doors
  checkDoor(0, currentMap->getRoomType(), currentMap->getNeighbourUp(), currentMap->hasKnownNeighbour(North, true));
  checkDoor(1, currentMap->getRoomType(), currentMap->getNeighbourLeft(), currentMap->hasKnownNeighbour(West, true));
  checkDoor(2, currentMap->getRoomType(), currentMap->getNeighbourDown(), currentMap->hasKnownNeighbour(South, true));
  checkDoor(3, currentMap->getRoomType(), currentMap->getNeighbourRight(), currentMap->hasKnownNeighbour(East, true));

  // pet slime
  if (player->isEquiped(EQUIP_PET_SLIME) && currentMap->getRoomType() != roomTypeTemple) new SlimePetEntity();

  // barrels
  checkDestroyableObjects();
}

void WitchBlastGame::refreshMinimap()
{
  for (int j=0; j < FLOOR_HEIGHT; j++)
    for (int i=0; i < FLOOR_WIDTH; i++)
    {
      int roomType = currentFloor->getRoom(i, j);

      if (roomType > roomTypeNULL && currentFloor->getMap(i, j)->isVisited()&& currentFloor->getMap(i, j)->isKnown())
      {
        if (roomType == roomTypeStarting
            || roomType == roomTypeChallenge
            || roomType == roomTypeBonus
            || roomType == roomTypeKey
            || roomType == roomTypeBoss
            || roomType == roomTypeStandard
            || roomType == roomTypeSecret)
        {
          if ( currentFloor->getMap(i, j)->containsHealth())
            miniMap->setTile(i, j, 5);
          else
            miniMap->setTile(i, j, roomTypeStandard);
        }

        else
        {
          if (roomType == roomTypeMerchant)
            miniMap->setTile(i, j, 3);
          else if (roomType == roomTypeTemple)
            miniMap->setTile(i, j, 7);
          else if (roomType == roomTypeSecret)
            miniMap->setTile(i, j, 8);
          else
            miniMap->setTile(i, j, currentFloor->getRoom(i, j));
        }
      }
      else if (roomType > roomTypeNULL && currentFloor->getMap(i, j)->isKnown())
      {
        switch (roomType)
        {
        case roomTypeBoss:
          miniMap->setTile(i, j, 12);
          proceedEvent(EventFindBossDoor);
          break;
        case roomTypeChallenge:
          miniMap->setTile(i, j, 15);
          proceedEvent(EventFindBossDoor);
          break;
        case roomTypeMerchant:
          miniMap->setTile(i, j,
                           game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP) ? 13 : 11 );
          break;
        case roomTypeKey:
          miniMap->setTile(i, j,
                           game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP) ? 14 : 11 );
          break;
        case roomTypeExit:
          miniMap->setTile(i, j,
                           game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP) ? 16 : 11 );
          break;
        case roomTypeTemple:
          miniMap->setTile(i, j,
                           game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP) ? 17 : 11 );
          break;
        case roomTypeBonus:
          miniMap->setTile(i, j,
                           game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP) ? 2 : 11 );
          break;
        case roomTypeSecret:
          miniMap->setTile(i, j, 18 );
          break;
        case roomTypeStandard:
        case roomTypeStarting:
          miniMap->setTile(i, j, 11);
          break;
        case roomTypeNULL:
          miniMap->setTile(i, j, 0);
          break;
        }
      }
      else
        miniMap->setTile(i, j, 0);
    }
  miniMap->setTile(floorX, floorY, 10);

  levelStrPosition.x = 824;
  levelStrPosition.y = 602;

  if (miniMap->getTile(FLOOR_WIDTH / 2 - 1, 0) > 0
      || miniMap->getTile(FLOOR_WIDTH / 2, 0) > 0
      || miniMap->getTile(FLOOR_WIDTH / 2 + 1, 0) > 0 )
  {
    if (miniMap->getTile(0, 0) <= 0
      || miniMap->getTile(1, 0) <= 0
      || miniMap->getTile(2, 0) <= 0 )
      // no place top-middle
      levelStrPosition.x = 729;
  else if (miniMap->getTile(FLOOR_WIDTH - 1, 0) <= 0
      || miniMap->getTile(FLOOR_WIDTH - 2, 0) <= 0
      || miniMap->getTile(FLOOR_WIDTH - 3, 0) <= 0 )
      // no place top-left
      levelStrPosition.x = 921;
  else
    // no place top
    levelStrPosition.y = 594;
  }
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

      // SCORE perfect
      if (player->getLostHp(level) == 0) addBonusScore(BonusPerfect, getPerfectScore(level));
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

    if (autosave || !currentMap->isCleared())
    {
      saveGame(true);
    }
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
  case gameStateJoystickConfig:
  case gameStateMenu:
    renderMenu();
    break;
  case gameStatePlaying:
  case gameStatePlayingPause:
  case gameStatePlayingDisplayBoss:
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
  int xHud = 124;
  int yHud = 655;
  int index = 0;

  for (int i = 0; i < SPECIAL_SHOT_SLOTS; i++)
  {
    if (i == 0 || player->getShotType(i) != ShotTypeStandard)
    {
      int type_shot = player->getShotType(i);
      uiSprites.shotsSprite.setPosition(xHud + 55 * index, yHud);
      if (index == player->getShotIndex())
      {
        uiSprites.shotsSprite.setTextureRect(sf::IntRect(0, 0,  48, 48));
        app->draw(uiSprites.shotsSprite);
      }
      uiSprites.shotsSprite.setTextureRect(sf::IntRect(48 * ( 1 + type_shot), 0,  48, 48));
      app->draw(uiSprites.shotsSprite);

      // level
      if (i > 0)
      {
        std::ostringstream oss;
        oss << "lvl " << player->getShotLevel(i) + 1;
        write(oss.str(), 10, xHud + 55 * index + 10, yHud + 48, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 0, 0, 0);
      }

      index++;
    }
  }
}

void WitchBlastGame::generateBlood(float x, float y, BaseCreatureEntity::enumBloodColor bloodColor)
{
  dungeonEntity->generateBlood(x, y, bloodColor);
  // double blood if the "Blood Snake" object is equipped
  if (player->isEquiped(EQUIP_BLOOD_SNAKE)) dungeonEntity->generateBlood(x, y, bloodColor);
}

void WitchBlastGame::addCorpse(float x, float y, int frame)
{
  dungeonEntity->addCorpse(x, y, frame);
}

void WitchBlastGame::showArtefactDescription(enumItemType itemType)
{
  killArtefactDescription();
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
    currentMap->generateKeyRoom();

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
      (MAP_WIDTH / 2 - 1) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
    item1->setMerchandise(true);

    ItemEntity* item3 = new ItemEntity(
      ItemHealthSmall,
      (MAP_WIDTH / 2 + 1) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
    item3->setMerchandise(true);

    int bonusType = getRandomEquipItem(true, true);
    ItemEntity* item2 = new ItemEntity(
      (enumItemType)(FirstEquipItem + bonusType),
      (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
      (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
    item2->setMerchandise(true);

    // level item ?
    if (rand() % 3 == 0)
    {
      int r = rand() % 4;
      int item4Type = -1;
      switch (r)
      {
      case 0:
        if (!game().getPlayer()->isEquiped(EQUIP_FLOOR_MAP)) item4Type = ItemFloorMap;
        break;
      case 1:
        if (!game().getPlayer()->isEquiped(EQUIP_ALCOHOL)) item4Type = ItemAlcohol;
        break;
      case 2:
        if (!game().getPlayer()->isEquiped(EQUIP_LUCK)) item4Type = ItemLuck;
        break;
      default:
        if (!game().getPlayer()->isEquiped(EQUIP_FAIRY_POWDER) && player->getFairieNumber() > 0) item4Type = ItemFairyPowder;
        break;
      }
      if (item4Type > -1)
      {
        ItemEntity* item4 = new ItemEntity(
          (enumItemType)(item4Type),
          (MAP_WIDTH / 2 + 2) * TILE_WIDTH + TILE_WIDTH / 2,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
        item4->setMerchandise(true);
      }
    }

    // consumable item
    if (rand() % 3 == 0)  // potion
    {
      ItemEntity* item5 = new ItemEntity(
          (enumItemType)(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED),
          (MAP_WIDTH / 2 - 2) * TILE_WIDTH + TILE_WIDTH / 2,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
      item5->setMerchandise(true);
    }
    else  // scroll
    {
      ItemEntity* item5 = new ItemEntity(
          (enumItemType)ItemScrollRevelation,
          (MAP_WIDTH / 2 - 2) * TILE_WIDTH + TILE_WIDTH / 2,
          (MAP_HEIGHT / 2) * TILE_HEIGHT + 5);
      item5->setMerchandise(true);
    }

    new PnjEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                  (MAP_HEIGHT / 2 - 1) * TILE_HEIGHT,
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
      addMonster(challengeLevel >= 3 ? EnemyTypeBubbleIce : EnemyTypeBubble,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 - 80,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      addMonster(challengeLevel >= 4 ? EnemyTypeBubbleGreater : EnemyTypeBubble,
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
      addMonster(EnemyTypeVampire,
                 (MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                 (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      testAndAddMessageToQueue(MsgInfoVampire);
    }
    else if (level == 8)
    {
      // TODO
      FranckyEntity* b2 = new FranckyEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b2->setLabelDy(-530);

      KingRatEntity* b3 = new KingRatEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 100,
                                            (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
      b3->setLabelDy(-20);

      new CyclopsEntity((MAP_WIDTH / 2) * TILE_WIDTH + TILE_WIDTH / 2 + 120,
                        (MAP_HEIGHT / 2) * TILE_HEIGHT + TILE_HEIGHT / 2);
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

    // boss screen
    if (parameters.displayBossPortrait)
    {
      gameState = gameStatePlayingDisplayBoss;
      bossDisplayTimer = 0.0f;
      bossDisplayState = 0;
      dungeonEntity->animate(0.0f);
    }
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
    proceedEvent(EventFindTemple);
  }
  else if (currentMap->getRoomType() == roomTypeSecret)
  {
    currentMap->generateSecretRoom();

    if (rand() % 7 > 0)
    {
      new ChestEntity((TILE_WIDTH * MAP_WIDTH * 0.5f + TILE_WIDTH),
                    (TILE_HEIGHT * MAP_HEIGHT * 0.5f),
                    ChestBasic, false);

      new ChestEntity((TILE_WIDTH * MAP_WIDTH * 0.5f - TILE_WIDTH),
                    (TILE_HEIGHT * MAP_HEIGHT * 0.5f),
                    ChestBasic, false);
    }
    else
    {
      int bonusType = getRandomEquipItem(false, false);

      if (items[FirstEquipItem + bonusType].familiar > FamiliarNone)
      {
        new ChestEntity(TILE_WIDTH * MAP_WIDTH * 0.5f,
                        TILE_HEIGHT * MAP_HEIGHT * 0.5f,
                        ChestFairy + items[FirstEquipItem + bonusType].familiar, false);
      }
      else
      {
        new ItemEntity( (enumItemType)(FirstEquipItem + bonusType),
                       TILE_WIDTH * MAP_WIDTH * 0.5f,
                       TILE_HEIGHT * MAP_HEIGHT * 0.5f);
      }
    }

    currentMap->setCleared(true);

    // secret found text
    TextEntity* text = new TextEntity(tools::getLabel("secret_found"),
                                      25,
                                      MAP_WIDTH * 0.5f * TILE_WIDTH,
                                      MAP_HEIGHT * 0.5f * TILE_HEIGHT + 10.0f);
    text->setAlignment(ALIGN_CENTER);
    text->setLifetime(2.5f);
    text->setWeight(-36.0f);
    text->setZ(1200);
    text->setColor(TextEntity::COLOR_FADING_WHITE);

    SoundManager::getInstance().playSound(SOUND_SECRET);
    secretsFound++;

    addBonusScore(BonusSecret, getSecretScore());
  }
  else  // "normal" room
    currentMap->randomize(currentMap->getRoomType());
}

void WitchBlastGame::write(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow, int yShadow, int maxWidth)
{
  myText.setString(str);
  myText.setString(sf::String::fromUtf8(str.begin(), str.end()));
  myText.setCharacterSize(size);

  if (maxWidth > 0)
  {
    while (myText.getLocalBounds().width > maxWidth && size > 5)
    {
      size--;
      myText.setCharacterSize(size);
    }
  }
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

void WitchBlastGame::writeGraphic(std::string str, int size, float x, float y, int align, sf::Color color, sf::RenderTarget* app, int xShadow, int yShadow, int maxWidth)
{
  if (parameters.language == 4) // russian
  {
    write(str, size, x, y, align, color, app, xShadow, yShadow, maxWidth);
  }
  else
  {
    myText.setFont(graphicsFont);
    write(str, size, x, y, align, color, app, xShadow, yShadow, maxWidth);
    myText.setFont(font);
  }
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
    new BatEntity(xm, ym, BatStandard, false);
    proceedEvent(EventMeetRatsOrBats);
    break;
  case EnemyTypeBatSkeleton:
    new BatEntity(xm, ym, BatSkeleton, false);
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
  case EnemyTypeEvilFlowerFire:
    new EvilFlowerEntity(xm, ym - 2, FlowerTypeFire);
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
    new CauldronEntity(xm, ym, CauldronTypeStandard);
    break;
  case EnemyTypeCauldronElemental:
    new CauldronEntity(xm, ym, CauldronTypeElemental);
    break;
  case EnemyTypeSpiderEgg:
    new SpiderEggEntity(xm, ym, false);
    break;
  case EnemyTypeSpiderEgg_invocated:
    new SpiderEggEntity(xm, ym, true);
    break;
  case EnemyTypeSpiderLittle:
    new LittleSpiderEntity(xm, ym, SpiderTypeStandard, false);
    break;
  case EnemyTypeSpiderTarantula:
    new LittleSpiderEntity(xm, ym, SpiderTypeTarantula, false);
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
  case EnemyTypeBogeyman:
    new BogeymanEntity(xm, ym);
    break;
  case EnemyTypeSausage:
    new SausageEntity(xm, ym, false);
    break;

  case EnemyTypeSpiderLittle_invocated:
    new LittleSpiderEntity(xm, ym, SpiderTypeStandard, true);
    break;
  case EnemyTypeSpiderTarantula_invocated:
    new LittleSpiderEntity(xm, ym, SpiderTypeTarantula, true);
    break;

  case EnemyTypeBubble:
    new BubbleEntity(xm, ym, BubbleStandard, 0);
    break;
  case EnemyTypeBubbleIce:
    new BubbleEntity(xm, ym, BubbleIce, 0);
    break;
  case EnemyTypeBubbleGreater:
    new BubbleEntity(xm, ym, BubbleTriple, 0);
    break;
  case EnemyTypeSlimeLarge:
    new LargeSlimeEntity(xm, ym, SlimeTypeStandard);
    break;
  case EnemyTypeSlimeBlueLarge:
    new LargeSlimeEntity(xm, ym, SlimeTypeBlue);
    break;
  case EnemyTypeSlimeRedLarge:
    new LargeSlimeEntity(xm, ym, SlimeTypeRed);
    break;
  case EnemyTypeSlimeVioletLarge:
    new LargeSlimeEntity(xm, ym, SlimeTypeViolet);
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
  case EnemyTypeVampire:
    new VampireEntity(xm, ym);
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

  bool isMonsterLarge = monsterType == EnemyTypeSlimeLarge
                        || monsterType == EnemyTypeSlimeBlueLarge
                        || monsterType == EnemyTypeSlimeRedLarge
                        || monsterType == EnemyTypeSlimeVioletLarge;

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

      if (isMonsterLarge)
      {
        xm = 2 +rand() % (MAP_WIDTH - 5);
        ym = 2 +rand() % (MAP_HEIGHT - 5);
      }
      else
      {
        xm = 1 +rand() % (MAP_WIDTH - 3);
        ym = 1 +rand() % (MAP_HEIGHT - 3);
      }

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

bool WitchBlastGame::isAdvanced()
{
  return (achievementState[AchievementVampire] == AchievementDone);
}

bool WitchBlastGame::isAdvancedLevel()
{
  if (isAdvanced())
  {
    return (rand() % 150 < getAchievementsPercents());
  }
  else
    return false;
}

void WitchBlastGame::generateStandardMap()
{
  initMonsterArray();
  saveInFight.monsters.clear();
  generateStandardRoom(level, isAdvancedLevel());
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
    if (itemOk && presentItems[i]) itemOk = false;
    // TODO item already in floor
    if (itemOk && toSale && !items[eq].canBeSold) itemOk = false;
    if (itemOk && !toSale && !items[eq].canBeFound) itemOk = false;
    if (itemOk && items[eq].level > level) itemOk = false;
    if (itemOk && isItemLocked((enumItemType)eq)) itemOk = false;
    if (itemOk && items[eq].requirement >= FirstEquipItem
        && !player->isEquiped(items[eq].requirement - FirstEquipItem)) itemOk = false;

    if (itemOk && toSale && level == 1 && items[eq].price > 25) itemOk = false;
    if (itemOk && toSale && level == 2 && items[eq].price > 35) itemOk = false;

    if (itemOk && (items[eq].specialShot != ShotTypeStandard && items[eq].level < 3) && player->canGetNewShot(false))
      itemOk = false;

    if (itemOk && (items[eq].specialShot != ShotTypeStandard && items[eq].level >= 3) && player->canGetNewShot(true))
      itemOk = false;

    if (itemOk && i == EQUIP_BOOK_DUAL && (player->isEquiped(EQUIP_BOOK_DUAL_QUICK) || player->isEquiped(EQUIP_RAPID_SHOT)))
      itemOk = false;

    if (itemOk && i == EQUIP_BOOK_DUAL_QUICK && (player->isEquiped(EQUIP_BOOK_DUAL) || player->isEquiped(EQUIP_RAPID_SHOT)))
      itemOk = false;

    if (itemOk && i == EQUIP_RAPID_SHOT && (player->isEquiped(EQUIP_BOOK_DUAL_QUICK) || player->isEquiped(EQUIP_BOOK_DUAL)))
      itemOk = false;

    if (itemOk && noFairy && items[eq].familiar != FamiliarNone) itemOk = false;

    if (itemOk)
    {
      int n = 0;
      switch (items[eq].rarity)
      {
      case RarityCommon:
        n = 8;
        break;
      case RarityUncommon:
        n = 4;
        break;
      case RarityRare:
        n = 2;
        break;
      case RarityVeryRare:
        n = 1;
        break;
      }

      //if (itemOk && eq == ItemPetSlime) n = 15000;

      for (int j = 0; j < n; j++)
      {
        bonusSet.push_back(i);
        setSize++;
      }
    }
  }
  int bonusType = 0;
  if (setSize > 0) bonusType = bonusSet[rand() % setSize];

  //addPresentItem(bonusType);
  return (item_equip_enum) bonusType;
}

enumItemType WitchBlastGame::getItemSpell()
{
  bool ok = false;
  enumItemType item = ItemMagicianHat;

  while (!ok)
  {
    enumCastSpell n = (enumCastSpell)(rand() % SPELL_MAX);

    if (player->getActiveSpell().spell != n) ok = true;

    if (ok)
    {
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
      case SpellFlower:
        item = ItemSpellFlower;
        break;
      case SpellFairy:
        item = ItemSpellFairy;
        break;
      case SpellTime:
        item = ItemSpellTime;
        break;
      case SpellLightning:
        item = ItemSpellLightning;
        break;
      }
      ok = !isItemLocked(item);
    }
  }

  return item;
}

void WitchBlastGame::generateChallengeBonus(float x, float y)
{
  // TODO (jumping)
  // loot
  if (player->getActiveSpell().spell == SpellNone || rand() % 2 == 0)
  {
    enumItemType spell1 = getItemSpell();
    ItemEntity* spellItem = new ItemEntity(spell1, x, y);
    spellItem->setVelocity(Vector2D(100.0f + rand()% 250));
    if (spellItem->getVelocity().y < 0.0f) spellItem->setVelocity(Vector2D(spellItem->getVelocity().x, -spellItem->getVelocity().y));
    spellItem->setViscosity(0.96f);
    spellItem->startsJumping();

    if (!isFunctionalityLocked(FunctionalityDoubleSpellDrop))
    {
      enumItemType spell2 = spell1;
      while (spell2 == spell1) spell2 = getItemSpell();
      ItemEntity* spellItem2 = new ItemEntity(spell2, x, y);
      spellItem2->setVelocity(Vector2D(100.0f + rand()% 250));
      if (spellItem2->getVelocity().y < 0.0f) spellItem2->setVelocity(Vector2D(spellItem2->getVelocity().x, -spellItem2->getVelocity().y));
      spellItem2->setViscosity(0.96f);
      spellItem2->startsJumping();
    }

    ItemEntity* healthItem1 = new ItemEntity(ItemHealthVerySmall, x, y);
    healthItem1->setVelocity(Vector2D(100.0f + rand()% 250));
    healthItem1->setViscosity(0.96f);
    healthItem1->startsJumping();

    ItemEntity* healthItem2 = new ItemEntity(ItemHealthVerySmallPoison, x, y);
    healthItem2->setVelocity(Vector2D(100.0f + rand()% 250));
    healthItem2->setViscosity(0.96f);
    healthItem2->startsJumping();
  }
  else
  {
    ItemEntity* newItem;

    if (challengeLevel < 4 || rand() % (player->isEquiped(EQUIP_LUCK) ? 5 : 8) > 0)
      newItem = new ItemEntity(ItemBonusHealth, x, y);
    else
      newItem = new ItemEntity((enumItemType)(FirstEquipItem + getRandomEquipItem()), x, y);

    if (newItem->getVelocity().y < 0.0f) newItem->setVelocity(Vector2D(newItem->getVelocity().x, -newItem->getVelocity().y));
    newItem->setVelocity(Vector2D(100.0f + rand()% 250));
    newItem->setViscosity(0.96f);
    newItem->startsJumping();

    int gold = 2 + rand() % (player->isEquiped(EQUIP_LUCK) ? 9 : 14);
    for (int i = 0; i < gold; i++)
    {
      ItemEntity* newItem = new ItemEntity(ItemCopperCoin, x, y);
      newItem->setVelocity(Vector2D(90.0f + rand()% 150));
      newItem->setViscosity(0.96f);
      newItem->startsJumping();
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
      ss << "[" << keyToString(input[KeyInteract]) << "] - ";
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
        ss << tools::getLabel(divinityLabel[divinity] + "_0");
      }

      interaction.label = ss.str();
    }
  }
  else if (player->getItemToBuy() != NULL)
  {
    interaction.active = true;
    interaction.type = InteractionTypeMerchandise;
    interaction.id = player->getItemToBuy()->getItemType();

    std::stringstream ss;
    ss << tools::getLabel(items[interaction.id].name);
    ss << ": ";
    ss << tools::getLabel(items[interaction.id].description);
    if (player->getItemToBuy()->canBePickedUp())
    {
      ss << std::endl;
      ss << "[" << keyToString(input[KeyInteract]) << "] - ";
      ss << tools::getLabel("interact_shop");
    }
    interaction.label = ss.str();
  }
}

void WitchBlastGame::verifyDoorUnlocking()
{
  int collidingDirection = (player->getCollidingDirection());

  if (collidingDirection != 5 && currentMap->isCleared() && !bossRoomOpened && player->isEquiped(EQUIP_BOSS_KEY))
  {
    int xt = (player->getX()) / TILE_WIDTH;
    int yt = (player->getY()) / TILE_HEIGHT;

    if (yt <= 1 && xt >= MAP_WIDTH / 2 - 1 && xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourUp() == 2)
    {
      doorEntity[0]->openDoor();
      currentMap->openDoor(MAP_WIDTH / 2, 0);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (yt >= MAP_HEIGHT - 2 && xt >= MAP_WIDTH / 2 - 1 &&xt <= MAP_WIDTH / 2 + 1 && currentMap->hasNeighbourDown() == 2)
    {
      doorEntity[2]->openDoor();
      currentMap->openDoor(MAP_WIDTH / 2, MAP_HEIGHT - 1);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt <= 1 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourLeft() == 2)
    {
      doorEntity[1]->openDoor();
      currentMap->openDoor(0, MAP_HEIGHT / 2);
      SoundManager::getInstance().playSound(SOUND_DOOR_OPENING);
      player->useBossKey();
      bossRoomOpened = true;
    }
    if (xt >= MAP_WIDTH - 2 && yt >= MAP_HEIGHT / 2 - 1 && yt <= MAP_HEIGHT / 2 + 1 && currentMap->hasNeighbourRight() == 2)
    {
      doorEntity[3]->openDoor();
      currentMap->openDoor(MAP_WIDTH - 1, MAP_HEIGHT / 2);
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
  music.setVolume(parameters.musicVolume * VolumeModifier / 100);
  bool ok = false;

  switch (musicChoice)
  {
  case MusicDungeon:
    {
      int r = currentStandardMusic;
      while (r == currentStandardMusic) r = rand() % 3;

      switch (r)
      {
        case 0: ok = music.openFromFile("media/sound/WitchBlastTheme.ogg"); break;
        case 1: ok = music.openFromFile("media/sound/SavageLife.ogg"); break;
        case 2: ok = music.openFromFile("media/sound/HauntedLighthouse.ogg"); break;
      }

      currentStandardMusic = r;
    }
    break;

  case MusicEnding:
    ok = music.openFromFile("media/sound/AmbiantMedieval.ogg");
    break;

  case MusicBoss:
    ok = music.openFromFile("media/sound/ShowMeThePower.ogg");
    break;

  case MusicChallenge:
    ok = music.openFromFile("media/sound/HellsFire.ogg");
    break;

  case MusicIntro:
    ok = music.openFromFile("media/sound/WitchBlastTheme.ogg");
    break;
  }

  if (ok)
    music.play();
}

void WitchBlastGame::pauseMusic()
{
  music.pause();
}

void WitchBlastGame::resumeMusic()
{
  music.play();
}

void WitchBlastGame::updateMusicVolume()
{
  if (music.getStatus() == sf::Music::Playing)
  {
    if (parameters.musicVolume == 0)
      music.stop();
    else
      music.setVolume(parameters.musicVolume * VolumeModifier / 100);
  }
  else
  {
    if (parameters.musicVolume > 0)
    {
      bool ok = music.openFromFile("media/sound/wb.ogg");
      music.setVolume(parameters.musicVolume * VolumeModifier / 100);
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

void WitchBlastGame::saveGame(bool autosave)
{
  if (nbPlayers > 1) return;
  if (!currentMap->isCleared() && !autosave) return;

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
    file << level << " " << challengeLevel << " " << secretsFound << std::endl;

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

    // potions
    for(auto it = potionMap.begin(); it != potionMap.end(); ++it)
    {
      file << it->first << " " << it->second.effect << " " << it->second.known << std::endl;
    }

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
               << currentFloor->getMap(i, j)->isRevealed() << " "
               << currentFloor->getMap(i, j)->isCleared() << std::endl;
          if (currentFloor->getMap(i, j)->isVisited())
          {
            for (l = 0; l < MAP_HEIGHT; l++)
            {
              for (k = 0; k < MAP_WIDTH; k++)
              {
                int tile = currentFloor->getMap(i, j)->getTile(k, l);
                file << tile << " ";
                tile = currentFloor->getMap(i, j)->getObjectTile(k, l);
                if (tile == MAPOBJ_DOOR_CLOSED) tile = MAPOBJ_DOOR_OPEN;
                file << tile << " ";
                file << currentFloor->getMap(i, j)->getLogicalTile(k, l) << " ";
              }
              file << std::endl;
            }
            // style
            file << currentFloor->getMap(i, j)->getFloorOffset() << " "
                 << currentFloor->getMap(i, j)->getWallType() << std::endl;
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

            // doors
            for (int k = 0; k < 4; k++)
              file << currentFloor->getMap(i, j)->getDoorType(k) << " ";
            file << std::endl;

            // random sprite
            for (int k = 0; k < NB_RANDOM_TILES_IN_ROOM; k++)
            {
              file << currentFloor->getMap(i, j)->getRandomTileElement(k).type
                   << " " <<currentFloor->getMap(i, j)->getRandomTileElement(k).x
                   << " " << currentFloor->getMap(i, j)->getRandomTileElement(k).y
                   << " " << currentFloor->getMap(i, j)->getRandomTileElement(k).rotation << std::endl;
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
    file << player->getHp() << " " << player->getHpMax() << " " << player->getGold() << " " << player->getDonation() << std::endl;
    // score
    file << score << " " << bodyCount << std::endl;
    // lost hp
    for (i = 1; i <= LAST_LEVEL; i++) file << player->getLostHp(i) << " ";
    file << std::endl;
    // equip
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++) file << player->isEquiped(i) << " ";
    file << std::endl;
    file << player->getX() << " " << player->getY() << std::endl;
    file << player->getShotIndex();
    for (i = 0; i < SPECIAL_SHOT_SLOTS; i++) file << " " << player->getShotType(i) << std::endl;
    file << player->getActiveSpell().spell << std::endl;
    for (i = 0; i < MAX_SLOT_CONSUMABLES; i++)
    {
      file << player->getConsumable(i) << " ";
    }
    file << std::endl;
    // divinity
    file << player->getDivinity().divinity << " " << player->getDivinity().piety << " "
         << player->getDivinity().level << " " << player->getDivinity().interventions << std::endl;
    // events
    for (i = 0; i < NB_EVENTS; i++) file << worldEvent[i] << " ";
    file << std::endl;
    // special states
    for (int i = 0; i < NB_SPECIAL_STATES; i++)
    {
      file << player->getSpecialState((enumSpecialState)i).active << " "
      << player->getSpecialState((enumSpecialState)i).waitUnclear << " "
      << player->getSpecialState((enumSpecialState)i).timer << " "
      << player->getSpecialState((enumSpecialState)i).param1 << " "
      << player->getSpecialState((enumSpecialState)i).param2 << " "
      << player->getSpecialState((enumSpecialState)i).param3 << std::endl;

    }
    file.close();
  }
  else
  {
    std::cerr << "[ERROR] Saving the game..." << std::endl;
  }
}

bool WitchBlastGame::loadGame()
{
  nbPlayers = 1;

  resetPresentItems();
  saveInFight.monsters.clear();
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
    file >> secretsFound;
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

    // potions
    for (i = 0; i < NUMBER_UNIDENTIFIED * 2; i++)
    {
      int source, effect;
      bool known;

      file >> source >> effect >> known;
      addPotionToMap((enumItemType)source, (enumItemType)effect, known);
    }

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
      if (iMap->getRoomType() == roomTypeSecret) iMap->setRevealed(flag);
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
            file >> n;
            iMap->setObjectTile(i, j, n);
            file >> n;
            iMap->setLogicalTile(i, j, (logicalMapStateEnum)n);
          }
        }
        // style
        file >> n;
        iMap->setFloorOffset(n);
        file >> n;
        iMap->setWallType(n);

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

        // doors
        for (int index = 0; index < 4; index++)
        {
          file >> n;
          iMap->setDoorType(index, (doorEnum)n);
        }

        // random sprite
        for (int index = 0; index < NB_RANDOM_TILES_IN_ROOM; index++)
        {
          DungeonMap::RandomTileElement rd;
          file >> rd.type;
          file >> rd.x;
          file >> rd.y;
          file >> rd.rotation;
          iMap->setRandomTileElement(index, rd);
        }
        // random sprite
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
    int hp, hpMax, gold, donation;
    file >> hp >> hpMax >> gold >> donation;
    player = new PlayerEntity((TILE_WIDTH * MAP_WIDTH * 0.5f),
                              (TILE_HEIGHT * MAP_HEIGHT * 0.5f));
    player->setHp(hp);
    player->setHpMax(hpMax);
    player->setGold(gold);
    player->setDonation(donation);
    // score
    file >> score;
    scoreDisplayed = score;
    file >> bodyCount;
    // lost hp
    for (i = 1; i <= LAST_LEVEL; i++)
    {
      file >> n;
      player->setLostHp(i, n);
    }

    // equip
    for (i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    {
      bool eq;
      file >> eq;
      player->setEquipped(i, eq);
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

    for (i = 0; i < MAX_SLOT_CONSUMABLES; i++)
    {
      file >> n;
      player->setConsumable(i, n);
    }

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
    // special states
    for (int i = 0; i < NB_SPECIAL_STATES; i++)
    {
      bool active, waitUnclear;
      float timer, param1, param2, param3;
      file >> active >> waitUnclear >> timer >> param1 >> param2 >> param3;
      player->setSpecialState(enumSpecialState(i), active, timer, param1, param2, waitUnclear);
    }

    player->computePlayer();
    file.close();
    if (!saveInFight.isFight && !autosave) remove(SAVE_FILE.c_str());
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
  std::ofstream file(SAVE_DATA_FILE.c_str(), std::ios::out | std::ios::trunc);

  if (file)
  {
    // version (for compatibility check)
    file << SAVE_VERSION << std::endl;
    int i;

    // tuto
    file << NB_MESSAGES << std::endl;
    for (i = 0; i < NB_MESSAGES; i++)
    {
      messageStruct msg = getMessage((EnumMessages)i);
      file << messageEnumToString((EnumMessages)i) << " ";
      if (msg.messageType == MessageTypeTutorial)
      {
        file << gameMessagesToSkip[i] << " ";
      }
      else file << "0 ";
    }
    file << std::endl;

    // achievements
    file << NB_ACHIEVEMENTS << std::endl;
    for (i = 0; i < NB_ACHIEVEMENTS; i++)
    {
      file << achievementEnumToString((enumAchievementType)i) << " ";
      if (achievementState[i] == AchievementDone) file << "1 ";
      else file << "0 ";
    }
    file << std::endl;

    // monsters
    file << NB_ENEMY << std::endl;
    for (i = 0; i < NB_ENEMY; i++)
    {
      file << enemyString[i] << " " << globalData.killedMonster[i] << " ";
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
  int i;

  if (file)
  {
    // version
    std::string v;
    file >> v;

    if (v == "SAVE_0.7")
    {
      // tuto
      for (i = 0; i < NB_MESSAGES; i++)
      {
        if (i == MsgInfoDivIllusion)
        {
          i += 3;
        }
        file >> gameMessagesToSkip[i];
      }

        // Achievements
      for (i = 0; i < NB_ACHIEVEMENTS; i++)
      {
        int n;
        file >> n;
        if (n == 1) achievementState[i] = AchievementDone;
        else achievementState[i] = AchievementUndone;
      }

      // Monsters
      for (i = 0; i < NB_ENEMY; i++)
        file >> globalData.killedMonster[i];
    }
    else if (v != SAVE_VERSION)
    {
      file.close();
      remove(SAVE_DATA_FILE.c_str());
      return;
    }
    else
    {
      int index, value;
      std::string key;

      // tuto
      file >> index;
      for (i = 0; i < index; i++)
      {
        file >> key;
        file >> value;
        gameMessagesToSkip[messageStringToEnum(key)] = value;
      }

      // Achievements
      file >> index;
      for (i = 0; i < index; i++)
      {
        file >> key;
        file >> value;

        if (value == 1) achievementState[achievementStringToEnum(key)] = AchievementDone;
        else achievementState[achievementStringToEnum(key)] = AchievementUndone;
      }

      // Monsters
      file >> index;
      for (i = 0; i < index; i++)
      {
        file >> key;
        file >> value;
        gameMessagesToSkip[messageStringToEnum(key)] = value;
        globalData.killedMonster[enemyStringToEnum(key)] = value;
      }
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
  if (parameters.playerName.length() == 0)
    parameters.playerName = "Player";
  newMap["language"] = intToString(parameters.language);
  newMap["player_name"] = parameters.playerName;

  // audio volume
  newMap["volume_sound"] = intToString(parameters.soundVolume);
  newMap["volume_music"] = intToString(parameters.musicVolume);

  newMap["zoom_enabled"] = parameters.zoom ? "1" : "0";
  newMap["vsync_enabled"] = parameters.vsync ? "1" : "0";
  newMap["blood_spreading"] = parameters.bloodSpread ? "1" : "0";
  newMap["fullscreen"] = parameters.fullscreen ? "1" : "0";
  newMap["pause_on_focus_lost"] = parameters.pauseOnFocusLost ? "1" : "0";
  newMap["particles_batching"] = parameters.particlesBatching ? "1" : "0";
  newMap["low_particles"] = parameters.lowParticles ? "1" : "0";
  newMap["display_boss_portrait"] = parameters.displayBossPortrait ? "1" : "0";

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

  // Joystick
  for (unsigned int i = 0; i < NumberKeys; i++)
  {
    std::stringstream oss_button;
    oss_button << "joy_" << inputKeyStr[i] << "_button";
    std::stringstream oss_value;
    oss_value << "joy_" << inputKeyStr[i] << "_value";
    std::stringstream oss_axis;
    oss_axis << "joy_" << inputKeyStr[i] << "_axis";

    newMap[oss_button.str()] = joystickInput[i].isButton ? "1" : "0";
    newMap[oss_value.str()] = intToString(joystickInput[i].value);
    newMap[oss_axis.str()] = intToString(joystickInput[i].axis);
  }

  config.saveToFile(CONFIG_FILE, newMap);
}

void WitchBlastGame::configureFromFile()
{
  // default
  parameters.language = 0;  // english
  parameters.zoom = true;
  parameters.vsync = true;
  parameters.bloodSpread = true;
  parameters.fullscreen = false;
  parameters.pauseOnFocusLost = true;
  parameters.particlesBatching = true;
  parameters.lowParticles = false;
  parameters.musicVolume = 80;
  parameters.soundVolume = 80;
  parameters.playerName = "";
  parameters.displayBossPortrait = false;

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

  // Joystick
  joystickInput[KeyUp].isButton = false;
  joystickInput[KeyUp].axis = sf::Joystick::Y;
  joystickInput[KeyUp].value = -1;

  joystickInput[KeyDown].isButton = false;
  joystickInput[KeyDown].axis = sf::Joystick::Y;
  joystickInput[KeyDown].value = 1;

  joystickInput[KeyLeft].isButton = false;
  joystickInput[KeyLeft].axis = sf::Joystick::X;
  joystickInput[KeyLeft].value = -1;

  joystickInput[KeyRight].isButton = false;
  joystickInput[KeyRight].axis = sf::Joystick::X;
  joystickInput[KeyRight].value = 1;

  for (unsigned int i = KeyFireUp; i < NumberKeys; i++)
  {
    joystickInput[i].isButton = true;
    joystickInput[i].axis = sf::Joystick::X;

    if (i > KeyFireRight) joystickInput[i].value = i - 4;
  }
  joystickInput[KeyFireUp].value = 3;
  joystickInput[KeyFireDown].value = 0;
  joystickInput[KeyFireLeft].value = 2;
  joystickInput[KeyFireRight].value = 1;

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
  addKey(KeyInteract, "keyboard_interact");
  addKey(KeyTimeControl, "keyboard_time_control");
  addKey(KeyFireSelect, "keyboard_fire_select");

  // Joystick
  for (unsigned int i = 0; i < NumberKeys; i++)
  {
    std::stringstream oss_button;
    oss_button << "joy_" << inputKeyStr[i] << "_button";
    std::stringstream oss_value;
    oss_value << "joy_" << inputKeyStr[i] << "_value";
    std::stringstream oss_axis;
    oss_axis << "joy_" << inputKeyStr[i] << "_axis";

    int isButton = config.findInt(oss_button.str());
    if (isButton > -1000)
      joystickInput[i].isButton = isButton;
    int n = config.findInt(oss_value.str());
    if (n > -1000)
      joystickInput[i].value = n;
    n = config.findInt(oss_axis.str());
    if (n >= sf::Joystick::Axis::X && n <= sf::Joystick::Axis::PovY)
      joystickInput[i].axis = (sf::Joystick::Axis)n;
  }

  int i = config.findInt("language");
  if (i >= 0 && i < NB_LANGUAGES) parameters.language = i;

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
  i = config.findInt("fullscreen");
  if (i >= 0) parameters.fullscreen = i;
  i = config.findInt("pause_on_focus_lost");
  if (i >= 0) parameters.pauseOnFocusLost = i;
  i = config.findInt("particles_batching");
  if (i >= 0) parameters.particlesBatching = i;
  i = config.findInt("low_particles");
  if (i >= 0) parameters.lowParticles = i;
  i = config.findInt("display_boss_portrait");
  if (i >= 0) parameters.displayBossPortrait = i;

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
    std::ostringstream oss;
    oss << tools::getLabel("start_new_game");
    if (nbPlayers == 1) oss << " (1 " << tools::getLabel("player") << ")";
    else oss << " (" << nbPlayers << " " << tools::getLabel("players") << ")";
    itemStart.label = oss.str();
    itemStart.description = tools::getLabel("start_desc");

    itemStart.id = MenuStartNew;
    menuMain.items.push_back(itemStart);

    menuItemStuct itemLoad;
    itemStart.label = tools::getLabel("restore");

    oss.str("");
    oss << saveHeader.date << " " << tools::getLabel("at") << " " << saveHeader.time << " - " << tools::getLabel("level") << " " << saveHeader.level;

    itemStart.description = oss.str();
    itemStart.id = MenuStartOld;
    menuMain.items.push_back(itemStart);

    if (!rebuild) menuMain.index = 2;
  }
  else
  {
    menuItemStuct itemStart;
    std::ostringstream oss;
    oss << tools::getLabel("start_new_game");
    if (nbPlayers == 1) oss << " (1 " << tools::getLabel("player") << ")";
    else oss << " (" << nbPlayers << " " << tools::getLabel("players") << ")";
    itemStart.label = oss.str();
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

  if (!isFunctionalityLocked(FunctionalityAchievementsScreen))
  {
    menuItemStuct itemAchiev;
    std::ostringstream oss;
    oss << tools::getLabel("menu_achievements");
    oss << " (" << getAchievementsPercents() << "%)";
    itemAchiev.label = oss.str();
    itemAchiev.description = tools::getLabel("menu_achievements_desc");
    itemAchiev.id = MenuAchievements;
    menuMain.items.push_back(itemAchiev);
  }

  //if (scores.size() > 0)
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

  menuItemStuct itemJoystick;
  itemJoystick.label = tools::getLabel("config_joystick");
  if (sf::Joystick::isConnected(0))
    itemJoystick.description = tools::getLabel("redef_joystick");
  else
    itemJoystick.description = tools::getLabel("joystick_not_found");
  itemJoystick.id = MenuJoystick;
  menuConfig.items.push_back(itemJoystick);

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

void WitchBlastGame::addKilledEnemy(enemyTypeEnum enemyType, enumShotType hurtingType)
{
  if(!player->isDead())
  {
    if (enemyType == NB_ENEMY)
      std::cout << "[ERROR] No enemy type";
    else
    {
      killedEnemies[enemyType]++;
      globalData.killedMonster[enemyType]++;
      player->offerMonster(enemyType, hurtingType);
      score += getMonsterScore(enemyType);

      if (enemyType != EnemyTypeDestroyable) bodyCount++;

      // achievements
      if (enemyType == EnemyTypeButcher) registerAchievement(AchievementButcher);
      else if (enemyType == EnemyTypeSlimeBoss) registerAchievement(AchievementGiantSlime);
      else if (enemyType == EnemyTypeCyclops) registerAchievement(AchievementCyclops);
      else if (enemyType == EnemyTypeRatKing) registerAchievement(AchievementRatKing);
      else if (enemyType == EnemyTypeSpiderGiant) registerAchievement(AchievementGiantSpider);
      else if (enemyType == EnemyTypeFranckyHead) registerAchievement(AchievementFrancky);
      else if (enemyType == EnemyTypeVampire) registerAchievement(AchievementVampire);
      else if ((enemyType == EnemyTypeRat || enemyType == EnemyTypeRatHelmet || enemyType == EnemyTypeRat_invocated
                || enemyType == EnemyTypeRatBlack || enemyType == EnemyTypeRatBlackHelmet))
      {
        if (globalData.killedMonster[EnemyTypeRat] + globalData.killedMonster[EnemyTypeRatHelmet] + globalData.killedMonster[EnemyTypeRat_invocated]
            + globalData.killedMonster[EnemyTypeRatBlack] + globalData.killedMonster[EnemyTypeRatBlackHelmet] >= 250)
          registerAchievement(AchievementRats);
      }
      else if (enemyType == EnemyTypeWitch || enemyType == EnemyTypeWitchRed)
      {
        if (globalData.killedMonster[EnemyTypeWitch] + globalData.killedMonster[EnemyTypeWitchRed] >= 50)
          registerAchievement(AchievementWitches);
      }
      else if (enemyType == EnemyTypeGhost)
      {
        if (globalData.killedMonster[EnemyTypeGhost] >= 50)
          registerAchievement(AchievementGhostbuster);
      }
      else if (enemyType == EnemyTypeSpiderEgg || enemyType == EnemyTypeSpiderEgg_invocated)
      {
        if (globalData.killedMonster[EnemyTypeSpiderEgg] + globalData.killedMonster[EnemyTypeSpiderEgg_invocated] >= 100)
          registerAchievement(AchievementEggs);
      }
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
}

int WitchBlastGame::getAchievementsPercents()
{
  int n = 0;
  for (int i = 0; i < NB_ACHIEVEMENTS; i++)
    if (achievementState[i] == AchievementDone) n++;

  return (n * 100) / NB_ACHIEVEMENTS;
}

void WitchBlastGame::testAndAddMessageToQueue(EnumMessages type)
{
  if (gameMessagesToSkip[type] == false)
  {
    if (messagesQueue.empty()) SoundManager::getInstance().playSound(SOUND_MESSAGE);

    messageStruct msg = getMessage(type);

    if (type == MsgInfoDivGift || type == MsgInfoDivIntervention)
    {
      std::stringstream ss;
      ss << tools::getLabel(divinityLabel[player->getDivinity().divinity] + "_0");
      ss << " ";
      if (type == MsgInfoDivGift)
        ss << tools::getLabel("divinity_gift_1");
      else // (type == MsgInfoDivIntervention)
        ss << tools::getLabel("divinity_intervention_1");
      msg.message[1] = ss.str();
    }

    messagesQueue.push(msg);
    if (msg.messageType == MessageTypeTutorial)
    {
      gameMessagesToSkip[type] = true;
      saveGameData();
    }
  }
}

void WitchBlastGame::addDivLevelMessageToQueue(std::string label)
{
  if (label.compare("") == 0) return;

  if (messagesQueue.empty()) SoundManager::getInstance().playSound(SOUND_MESSAGE);

  messageStruct msg = getMessage(MsgInfoDivLevel);

  std::stringstream ss;
  ss << tools::getLabel(divinityLabel[player->getDivinity().divinity] + "_0");
  ss << " ";
  ss << tools::getLabel(label);
  msg.message[1] = ss.str();
  messagesQueue.push(msg);
}

void WitchBlastGame::initEvents()
{
  for (int i = 0; i < NB_EVENTS; i++)
    worldEvent[i] = false;
}

void WitchBlastGame::proceedEvent(EnumWorldEvents event)
{
  // achievements
  if (event == EventGetCoin)
  {
    if (achievementState[Achievement100] == AchievementUndone && player->getGold() > 100)
      registerAchievement(Achievement100);
  }
  else if (event == EventPietyMax)
  {
    if (achievementState[AchievementPietyMax] == AchievementUndone) registerAchievement(AchievementPietyMax);
  }
  else if (event == Event4Hits)
  {
    if (achievementState[Achievement4Hits] == AchievementUndone) registerAchievement(Achievement4Hits);
  }

  // message ?
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
    sprite.setPosition(x - 35, y - 25);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 144, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_ICE])
  {
    sprite.setPosition(x - 5, y - 25);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 216, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY])
  {
    sprite.setPosition(x - 40, y);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 0, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_TARGET])
  {
    sprite.setPosition(x - 10, y);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 72, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_POISON])
  {
    sprite.setPosition(x + 20, y);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 288, 48, 60));
    app->draw(sprite);
  }
  if (equip[EQUIP_FAIRY_STONE])
  {
    sprite.setPosition(x - 5, y + 15);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_FAIRY));
    sprite.setTextureRect(sf::IntRect( 0, 360, 48, 60));
    app->draw(sprite);
  }

  // slime
  if (equip[EQUIP_PET_SLIME])
  {
    sprite.setPosition(x - 20, y + 24);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_SLIME));
    sprite.setTextureRect(sf::IntRect( 64, 256, 64, 64));
    app->draw(sprite);
  }

  sprite.setPosition(x, y);
  sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));

  int width = 64;
  int height = 96;

  // body
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( frame * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( frame * width, spriteDy * height, width, height));
  app->draw(sprite);

  // boots
  if (equip[EQUIP_BOOTS_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_LEATHER_BOOTS])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_ROBE_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_MAGICIAN_ROBE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (12 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_GLOVES_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_DISPLACEMENT_GLOVES])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (21 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_CRITICAL_ADVANCED])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (24 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
  else if (equip[EQUIP_CRITICAL])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }

  if (equip[EQUIP_RAGE_AMULET])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (18 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }
  else if (equip[EQUIP_AMULET_RETALIATION])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_2));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }

  if (equip[EQUIP_BELT_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_LEATHER_BELT])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (15 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  // hat
  if (equip[EQUIP_HAT_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (9 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_MAGICIAN_HAT])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (6 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }

  // staff
  int frameDx = equip[EQUIP_MAHOGANY_STAFF] ? 6 : 3;
  if (isMirroring)
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width + width, spriteDy * height, -width, height));
  else
    sprite.setTextureRect(sf::IntRect( (frameDx + frame) * width, spriteDy * height, width, height));
  app->draw(sprite);

  if (equip[EQUIP_BLOOD_SNAKE])
  {
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (27 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
  }

  if (equip[EQUIP_REAR_SHOT_ADVANCED])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_2));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
  }
  else if (equip[EQUIP_REAR_SHOT])
  {
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
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

    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_1));
    if (isMirroring)
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width + width, spriteDy * height, -width, height));
    else
      sprite.setTextureRect(sf::IntRect( (3 + frame) * width, spriteDy * height, width, height));
    app->draw(sprite);
    sprite.setTexture(*ImageManager::getInstance().getImage(IMAGE_PLAYER_0));
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
  case EnemyTypeBatSkeleton_invocated:
  case EnemyTypeBatSkeleton:
    value = "enemy_type_bat_skeleton";
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
  case EnemyTypeEvilFlowerFire:
    value = "enemy_type_evil_flower_fire";
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
  case EnemyTypeCauldronElemental:
    value = "enemy_type_cauldron_elemental";
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
  case EnemyTypeBogeyman:
    value = "enemy_type_bogeyman";
    break;
  case EnemyTypeSausage_invocated:
  case EnemyTypeSausage:
    value = "enemy_type_sausage";
    break;

  // mini boss
  case EnemyTypeBubble:
    value = "enemy_type_bubble";
    break;

  case EnemyTypeBubbleIce:
    value = "enemy_type_bubble_ice";
    break;

  case EnemyTypeBubbleGreater:
    value = "enemy_type_bubble_greater";
    break;

  case EnemyTypeSlimeLarge:
    value = "enemy_type_slime_large";
    break;

  case EnemyTypeSlimeBlueLarge:
    value = "enemy_type_slime_blue_large";
    break;

  case EnemyTypeSlimeRedLarge:
    value = "enemy_type_slime_red_large";
    break;

  case EnemyTypeSlimeVioletLarge:
    value = "enemy_type_slime_violet_large";
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
  case EnemyTypeVampire:
  case EnemyTypeVampireDead:
    value = "enemy_type_vampire";
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
  case EnemyTypeSpiderTarantula:
  case EnemyTypeSpiderTarantula_invocated:
    value = "enemy_type_spider_tarantula";
    break;
  case EnemyTypeSpiderWeb:
    value = "enemy_type_spider_web";
    break;

  case EnemyTypeNone:
    value = "enemy_type_himself";
    break;

  case EnemyTypeDestroyable:
    value = "";
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

std::string WitchBlastGame::equipToString(bool equip[NUMBER_EQUIP_ITEMS])
{
  std::stringstream oss;
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++)
    oss << (equip[i] ? 1 : 0);
  return oss.str();
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
      file << scores[i].divinity << " ";
      file << scores[i].killedBy << " ";
      file << scores[i].time << " ";
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
      remove(HISCORES_FILE.c_str());
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
      file >> score.divinity;
      file >> score.killedBy;
      file >> score.time;

      for (int j = 0; j < NUMBER_EQUIP_ITEMS; j++)
        file >> score.equip[j];

      scores.push_back(score);
    }
  }
}

void WitchBlastGame::revealFloor()
{
  currentFloor->reveal();
  refreshMinimap();
}

void WitchBlastGame::activateKeyRoomEffect(bool withColorEffect)
{
  dungeonEntity->activateKeyRoomEffect();
  if (withColorEffect) makeColorEffect(X_GAME_COLOR_VIOLET, 0.35f);
  SoundManager::getInstance().playSound(SOUND_FORCE_FIELD);
}

void WitchBlastGame::generateStar(sf::Color starColor, float xStar, float yStar)
{
  SpriteEntity* spriteStar = new SpriteEntity(
    ImageManager::getInstance().getImage(IMAGE_STAR_2),
    xStar, yStar);
  spriteStar->setScale(0.8f, 0.8f);
  spriteStar->setZ(1000.0f);
  spriteStar->setSpin(-100 + rand()%200);
  spriteStar->setVelocity(Vector2D(10 + rand()%40));
  spriteStar->setWeight(-150);
  spriteStar->setFading(true);
  spriteStar->setAge(-0.8f);
  spriteStar->setLifetime(0.1f + (rand() % 100) * 0.003f );
  spriteStar->setColor(starColor);
  spriteStar->setType(ENTITY_EFFECT);
}

void WitchBlastGame::registerAchievement(enumAchievementType achievement)
{
  if (achievementState[achievement] == AchievementUndone)
  {
    achievementState[achievement] = AchievementPending;
    achievementStruct ach;
    ach.type = achievement;
    ach.message = tools::getLabel(achievements[achievement].label);
    ach.timer = ACHIEVEMENT_DELAY_MAX;
    ach.counter = 0;
    ach.hasStarted = false;
    achievementsQueue.push(ach);
  }
}

bool WitchBlastGame::isItemLocked(enumItemType item)
{
  for (int i = 0; i < NB_ACHIEVEMENTS; i++)
  {
    if (achievementState[i] == AchievementUndone)
    {
      if (achievements[i].unlockType == UnlockItem)
        if (achievements[i].unlock == (int)item) return true;
    }
  }
  return false;
}

bool WitchBlastGame::isFunctionalityLocked(enumFunctionalityType func)
{
  for (int i = 0; i < NB_ACHIEVEMENTS; i++)
  {
    if (achievementState[i] == AchievementUndone)
    {
      if (achievements[i].unlockType == UnlockFunctionality)
        if (achievements[i].unlock == (int)func) return true;
    }
  }
  return false;
}

void WitchBlastGame::renderDoors()
{
  for (int i = 0; i < 4; i++) doorEntity[i]->renderDoors(app);
}

void WitchBlastGame::resetPresentItems()
{
  for (int i = 0; i < NUMBER_EQUIP_ITEMS; i++) presentItems[i] = false;
}

bool WitchBlastGame::isPresentItem(int n)
{
  if (n >= 0 && n < NUMBER_EQUIP_ITEMS) return presentItems[n];
  else return false;
}

void WitchBlastGame::addPresentItem(int n)
{
  if (n >= 0 && n < NUMBER_EQUIP_ITEMS) presentItems[n] = true;
}

bool WitchBlastGame::isPressing(int p, inputKeyEnum k, bool oneShot)
{
  if (gameState != gameStatePlaying || nbPlayers == 0)
    return ((actionKey[0][k].isPressed && (!oneShot || actionKey[0][k].isTriggered))
            || (actionKey[1][k].isPressed && (!oneShot || actionKey[1][k].isTriggered)));
  else
    return (actionKey[p][k].isPressed && (!oneShot || actionKey[p][k].isTriggered));
}

bool WitchBlastGame::getPressingState(int p, inputKeyEnum k)
{
  if (p == 0 || gameState != gameStatePlaying)
  {
    // arrows in menu
    if (gameState != gameStatePlaying
        || player->isDead()
        || player->getPlayerStatus() == PlayerEntity::playerStatusVictorious)
    {
      if (k == KeyLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) return true;
      if (k == KeyRight && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) return true;
      if (k == KeyUp && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) return true;
      if (k == KeyDown && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) return true;

      if (k == KeyFireDown && sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) return true;
    }

    // keyboard
    if (sf::Keyboard::isKeyPressed(input[k])) return true;
  }

  if (p == 1 || gameState != gameStatePlaying || nbPlayers == 1)
  {
    if (!sf::Joystick::isConnected(0)) return false;

    // joystick
    if (joystickInput[k].isButton)
    {
      // button
      if (joystickInput[k].value >= 0)
        if (sf::Joystick::isButtonPressed(0, joystickInput[k].value)) return true;
    }
    else
    {
      // axis
      if (joystickInput[k].value < 0 && sf::Joystick::getAxisPosition(0, joystickInput[k].axis) < -40) return true;
      else if (joystickInput[k].value > 0 && sf::Joystick::getAxisPosition(0, joystickInput[k].axis) > 40) return true;
    }
  }

  return false;
}

void WitchBlastGame::updateActionKeys()
{
  for (int p = 0; p < NB_PLAYERS_MAX; p++)
  {
    for (unsigned int i = 0; i < NumberKeys; i++)
    {
      bool oldState = actionKey[p][i].isPressed;
      actionKey[p][i].isPressed = getPressingState(p, (inputKeyEnum)i);
      actionKey[p][i].isTriggered = actionKey[p][i].isPressed && !oldState;
    }
  }
}

void WitchBlastGame::sendScoreToServer()
{
  if (sendScoreThread.joinable()) sendScoreThread.join();
  sendScoreThread = std::thread(&WitchBlastGame::sendScoreToServerThread, this);
  receiveScoreFromServer();
}

void WitchBlastGame::sendScoreToServerThread()
{
#ifdef ONLINE_MODE
  sendScore(lastScore.score,
            lastScore.level,
            lastScore.name,
            equipToString(lastScore.equip),
            lastScore.shotType,
            lastScore.divinity,
            lastScore.killedBy,
            lastScore.time,
            statsStr,
            SCORE_VERSION);
#endif
}

void WitchBlastGame::receiveScoreFromServer()
{
#ifdef ONLINE_MODE
  scoreState = ScoreLoading;

  if (receiveScoreThread.joinable()) receiveScoreThread.join();
  receiveScoreThread = std::thread(&WitchBlastGame::receiveScoreFromServerThread, this);
#endif
}

void WitchBlastGame::receiveScoreFromServerThread()
{
  if (sendScoreThread.joinable()) sendScoreThread.join();
  loadHiScoresOnline(false);
  scoreState = ScoreLoadingDay;
  loadHiScoresOnline(true);
  scoreState = ScoreOK;
}

void WitchBlastGame::loadHiScoresOnline(bool fromDayOnly)
{
#ifdef ONLINE_MODE

  std::vector <StructScore> scoresTemp;

  std::vector<std::string> receivedScores = receiveScores(fromDayOnly);
  int nbParameters = 8;

  if (receivedScores.size() > 0
      && receivedScores.size() % nbParameters == 0
      && receivedScores.size() / nbParameters <= 10)
  {
    int nbScores = receivedScores.size() / nbParameters;
    for (int i = 0; i < nbScores; i++)
    {
      StructScore score;
      std::istringstream scoreStr(receivedScores[i * nbParameters]);
      scoreStr >> score.score;

      score.name = receivedScores[i * nbParameters + 1];

      std::istringstream levelStr(receivedScores[i * nbParameters + 2]);
      levelStr >> score.level;

      for (int j = 0; j < NUMBER_EQUIP_ITEMS; j++)
      {
        score.equip[j] = (receivedScores[i * nbParameters + 3])[j] == '1';
      }

      std::istringstream shotStr(receivedScores[i * nbParameters + 4]);
      shotStr >> score.shotType;

      std::istringstream divStr(receivedScores[i * nbParameters + 5]);
      divStr >> score.divinity;

      std::istringstream killStr(receivedScores[i * nbParameters + 6]);
      killStr >> score.killedBy;

      std::istringstream timeStr(receivedScores[i * nbParameters + 7]);
      timeStr >> score.time;

      scoresTemp.push_back(score);
    }

    if (fromDayOnly)
    {
      scoresOnlineDay.clear();
      scoresOnlineDay = scoresTemp;
    }
    else
    {
      scoresOnline.clear();
      scoresOnline = scoresTemp;
    }
  }
#endif
}

void WitchBlastGame::checkDestroyableObjects()
{
  for (int i = 0; i < MAP_WIDTH; i++)
    for (int j = 0; j < MAP_HEIGHT; j++)
    {
      if (currentMap->getObjectTile(i, j) >= MAPOBJ_BARREL/* && currentMap->getObjectTile(i, j) < MAPOBJ_BARREL_EXPL + 3*/)
      {
        new ObstacleEntity(i * TILE_WIDTH + TILE_WIDTH / 2, j * TILE_HEIGHT + TILE_HEIGHT / 2, currentMap->getObjectTile(i, j));
      }
    }
}

void WitchBlastGame::randomizePotionMap()
{
  potionMap.clear();

  std::vector<int> potionEffect;
  for (int i = 0; i < NUMBER_UNIDENTIFIED; i++) potionEffect.push_back(i);

  for (int i = 0; i < NUMBER_UNIDENTIFIED; i++)
  {
    int r = rand() % potionEffect.size();
    addPotionToMap((enumItemType)(ItemPotion01 + i), (enumItemType)(ItemPotionHealth + potionEffect[r]), false);
    potionEffect.erase(potionEffect.begin() + r);
  }
}

void WitchBlastGame::acquireAlchemyBook()
{
  for (int i = 0; i < NUMBER_UNIDENTIFIED; i++)
  {
    if (rand() % 2 > 0)
    {
      enumItemType potion = (enumItemType)(ItemPotion01 + i);
      if (!potionEffectKnown(potion)) setPotionToKnown(potion);
    }
  }

  // drop potion
  ItemEntity* potionItem = new ItemEntity( (enumItemType)(ItemPotion01 + rand() % NUMBER_UNIDENTIFIED), player->getX(), player->getY());
  potionItem->setVelocity(Vector2D(100.0f + rand()% 250));
  potionItem->setViscosity(0.96f);
  potionItem->startsJumping();
}

void WitchBlastGame::addPotionToMap(enumItemType source, enumItemType effect, bool known)
{
  potionMap[source] = structPotionMap { effect, known};
  potionMap[effect] = structPotionMap { source, known};
}

enumItemType WitchBlastGame::getPotion(enumItemType source)
{
  return potionMap[source].effect;
}

bool WitchBlastGame::potionEffectKnown(enumItemType source)
{
  return potionMap[source].known;
}

void WitchBlastGame::setPotionToKnown(enumItemType source)
{
  potionMap[source].known = true;
  potionMap[potionMap[source].effect].known = true;

  // message
  if (messagesQueue.empty()) SoundManager::getInstance().playSound(SOUND_MESSAGE);

  messageStruct msg = getMessage(MsgInfoPotionId);

  std::stringstream ss;
  ss << msg.message[1];
  ss << " ";
  ss << tools::getLabel(items[getPotion(source)].name);

  msg.message[1] = ss.str();
  msg.message[2] = tools::getLabel(items[getPotion(source)].description);
  messagesQueue.push(msg);
}

void WitchBlastGame::forgetPotions()
{
  for (int i = ItemPotion01; i < FirstEquipItem; i++)
  {
    potionMap[(enumItemType)i].known = false;
    potionMap[potionMap[(enumItemType)i].effect].known = false;
  }
}

void WitchBlastGame::forget()
{
  // forget map
  if (!player->isEquiped(EQUIP_FLOOR_MAP))
  {
    currentFloor->forget(floorX, floorY);
    refreshMinimap();
  }

  // forget potions
  if (!player->isEquiped(EQUIP_BOOK_ALCHEMY))
  {
    forgetPotions();
  }
}

void WitchBlastGame::gainMultiplayerPower()
{
  if (nbPlayers == 2)
  {
    auto fairy = player->getFairy(0);
    if (fairy)
    {
      fairy->gainNewPower();
    }
  }
}

void WitchBlastGame::addBonusScore(EnumScoreBonus bonusType, int points)
{
  if (points <= 0) return;

  score += points;

  std::ostringstream oss;
  std::ostringstream ossStats;
  ossStats << ":";
  switch (bonusType)
  {
    case BonusSecret:  oss << "Secret!"; ossStats << "S"; break;
    case BonusPerfect:  oss << "Perfect!"; ossStats << "P"; break;
    case BonusChallenge:  oss << "Challenge!"; ossStats << "C"; break;
    case BonusPossession:  oss << "Stuff"; ossStats << "E=" << points; break;
    case BonusTime:  oss << "Time bonus"; ossStats << "T=" << points; break;
  }
  statsStr += ossStats.str();
  oss << " +" << points;
  scoreBonus = oss.str();
  scoreBonusTimer = BONUS_TIMER;
  SoundManager::getInstance().playSound(SOUND_SCORE_BONUS);
}

void WitchBlastGame::tryToClick(int xMouse, int yMouse, int mouseButton)
{
  for (ButtonStruct but : buttons)
  {
    if (xMouse >= but.zone.left && xMouse <= but.zone.left + but.zone.width
        && yMouse >= but.zone.top && yMouse <= but.zone.top + but.zone.height)
    {
      if (but.type == ButtonConsumable)
      {
        if (mouseButton == 0) player->tryToConsume(but.index);
        else player->dropConsumables(but.index);
      }
      else if (but.type == ButtonShotType)
      {
        player->selectShotType(but.index);
      }
      return;
    }
  }
}

WitchBlastGame &game()
{
  return *gameptr;
}
