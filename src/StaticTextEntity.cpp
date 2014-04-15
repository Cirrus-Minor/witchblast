#include "StaticTextEntity.h"
#include <iostream>

StaticTextEntity::StaticTextEntity()
{
  //ctor
}

/*void StaticTextEntity::Write()
{
  static sf::font font;
}*/

void StaticTextEntity::Write(sf::RenderWindow* app, std::string str, int size, float x, float y, int align)
{
  static bool fontDefined = false;
  static sf::Font statFont;
  static sf::Text statText;

  if (fontDefined == false)
  {
    fontDefined = true;

    if (!statFont.loadFromFile("media/DejaVuSans-Bold.ttf"))
    {
        // erreur...
    }
    statText.setFont(statFont);
  }

  statText.setString(str);
  statText.setCharacterSize(size);
  if (align == ALIGN_CENTER)
    statText.setPosition(x - statText.getLocalBounds().width / 2, y);
  else
    statText.setPosition(x, y);
  app->draw(statText);
}
