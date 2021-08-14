
#include <iostream>
#include <list>

#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////
/// Подключение заголовочных файлов с кодом.
//////////////////////////////////////////////////////////////

#include "Entity.h"
#include "GUI.h"

class TestEventsWork : public gui::IEventListener
{
public:

    void updateByGUIEvent(gui::Event event) override
    {
        switch (event.type)
        {
        case gui::Event::Click:
            std::cout << "click\n";
            break;
        case gui::Event::MouseEnter:
            std::cout << "enter!\n";
            break;
        case gui::Event::MouseLeave:
            std::cout << "leave!\n";
        }
    }

};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "YouGame");
    window.setFramerateLimit(60);

    TestEventsWork test;

    gui::Button btn(sf::Vector2f(0, 0), sf::Vector2f(100, 100), &window);
    btn.addListener(&test);

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
        window.draw(btn);
        window.display();
    }

    return 0;
}


