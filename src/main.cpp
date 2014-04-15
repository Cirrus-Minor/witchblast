#include <SFML/Graphics.hpp>

#include "WitchBlastGame.h"


int main()
{
    WitchBlastGame game; //1200, 800);

    game.startGame();

    return 0;
}

/*
#include <SFML/Graphics.hpp>

class ClassA
{
public:
  ClassA(sf::Font* font)
  {
    text_A.setFont(*font);
    text_A.setString("Hello world");
    text_A.setCharacterSize(24);
  }

  void display(sf::RenderWindow &window)
  {
     window.draw(text_A);
  }

private:
  sf::Font* font_A;
  sf::Text text_A;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);


    sf::Font* font = new sf::Font();
    if (!font->loadFromFile("media/DejaVuSans-Bold.ttf")) {}
    sf::Text text;
    text.setFont(*font); // font est un sf::Font
    text.setString("Hello world");
    text.setCharacterSize(24); // exprimée en pixels, pas en points !
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    ClassA* classA = new ClassA(font);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        classA->display(window);
        window.display();
    }

    return 0;
}
*/
