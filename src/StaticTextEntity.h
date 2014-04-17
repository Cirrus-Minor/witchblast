#ifndef STATICTEXTENTITY_H
#define STATICTEXTENTITY_H

#include <SFML/Graphics.hpp>

const int ALIGN_LEFT    = 0;
const int ALIGN_RIGHT   = 1;
const int ALIGN_CENTER  = 2;

class StaticTextEntity
{
  public:

    static void Write(sf::RenderWindow* app, std::string str, int size, float x, float y, int align, sf::Color color);

  protected:
  private:
    StaticTextEntity();
};

#endif // STATICTEXTENTITY_H
