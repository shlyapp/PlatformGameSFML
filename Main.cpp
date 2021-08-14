#include <iostream>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////
/// Подключение заголовочных файлов с кодом.
//////////////////////////////////////////////////////////////

#include "Entity.h"
#include "GUI.h"
#include "InterfaceUI.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "YouGame");
    window.setFramerateLimit(60);

    Menu menu("data/images/menu.png", "data/images/info.png", &window);

    while (window.isOpen())
    {

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.clear();
        window.draw(menu);
        window.display();
    }

    return 0;
}
