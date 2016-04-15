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

#include "WitchBlastMenu.h"

#include "WitchBlastGame.h"

#include "sfml_game/EntityManager.h"
#include "sfml_game/SoundManager.h"
#include "sfml_game/ImageManager.h"

#include "sfml_game/SpriteEntity.h"

#include "Constants.h"

WitchBlastMenu::WitchBlastMenu(WitchBlastGame *game)
  : game(game)
{

}

WitchBlastMenu::~WitchBlastMenu()
{

}

void WitchBlastMenu::updateMenu()
{
    /*SoundManager::getInstance().playSound(SOUND_NIGHT, false);
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
                startNewGame(false, 1);
                remove(SAVE_FILE.c_str());
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
    }*/
}

void WitchBlastMenu::renderMenu()
{
/*
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
  write(oss.str(), 17, 5, 680, ALIGN_LEFT, sf::Color(255, 255, 255, 255), app, 1, 1, 0);*/
}

void WitchBlastMenu::prepareIntro()
{
    EntityManager::getInstance().clean();

    introSprites[0] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
    introSprites[0]->setX(SCREEN_WIDTH / 2);
    introSprites[0]->setY(SCREEN_HEIGHT - 202);

    introSprites[1] = new SpriteEntity(ImageManager::getInstance().getImage(IMAGE_FOG));
    introSprites[1]->setX(SCREEN_WIDTH / 2 + 970);
    introSprites[1]->setY(SCREEN_HEIGHT - 202);

    // FIXME: Re-enable
    /*gameState = gameStateIntro;*/
    introState = 0;
    introSoundState = 0;

    SoundManager::getInstance().playSound(SOUND_NIGHT);
}

void WitchBlastMenu::updateIntro()
{/*
  float deltaTime = game->getDeltaTime();
  float gameTime = game->getGameTime();
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
    }*/
}
