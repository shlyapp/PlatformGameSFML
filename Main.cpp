#include <iostream>
#include <list>
#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

using Rect2f = sf::Vector2<sf::Vector2f>;

//////////////////////////////////////////////////////////////
/// Подключение заголовочных файлов с кодом.
//////////////////////////////////////////////////////////////

#include "Entity.h"
#include "GUI.h"
#include "GameEvent.h"
#include "Level.h"
#include "Collision.h"
#include "Abilities.h"

#include "InterfaceUI.h"

#include "Game.h"
#include "Scene.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "YouGame");
    window.setFramerateLimit(60);

    Scene scene(&window);

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
        window.draw(scene);
        window.display();
    }

    return 0;
}
