//////////////////////////////////////////////////////////////
/// Необходимые библиотеки.
//////////////////////////////////////////////////////////////

#include <iostream>
#include <list>
#include <string>
#include <fstream>

#include <SFML/Graphics.hpp>

// Определяем для удобного взаимодействия, типо "свой тип данных"
using Rect2f = sf::Vector2<sf::Vector2f>;

//////////////////////////////////////////////////////////////
/// Подключение заголовочных файлов с кодом.
//////////////////////////////////////////////////////////////

#include "Entity.h" // <-- Базовый класс для почти всего.
#include "GUI.h" // <-- Графический интерфейс, создание кнопок.
#include "GameEvent.h" // <-- События игры.
#include "Level.h" // <-- Уровни, карта.
#include "Collision.h" // <-- Столкновения.
#include "Abilities.h" // <-- Интерфейсы - возможности.
#include "GameObject.h" // <-- Все игровые обькты.
#include "InterfaceUI.h" // <-- Меню, блоки информации.
#include "Game.h" // <-- Сама игра, обновления.
#include "Scene.h" // <-- Сцена.

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "YouGame");
    window.setFramerateLimit(60);

    // Создаем сцену.
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
        // Выводим сцену, а в ней уже выводится все остальное в зависимости от состояния игры.
        window.draw(scene);
        window.display();
    }

    return 0;
}
