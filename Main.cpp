#include <iostream>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>

//////////////////////////////////////////////////////////////
/// Подключение заголовочных файлов с кодом.
//////////////////////////////////////////////////////////////

#include "Entity.h"
#include "GUI.h"

class TestEventsWork : public gui::IEventListener
{
public:

    void updateByGUIEvent(gui::EventType event) override
    {
        switch (event)
        {
        case gui::EventType::Click:
            std::cout << "click\n";
            break;
        case gui::EventType::MouseEnter:
            std::cout << "enter!\n";
            break;
        case gui::EventType::MouseLeave:
            std::cout << "leave!\n";
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "YouGame");
    window.setFramerateLimit(60);

    TestEventsWork test;

    gui::TextBlock btn(sf::Vector2f(0, 0), "hello", &window);
    btn.addListener(&test);

    btn.setColors(sf::Color::Green, sf::Color::Red);
    btn.setCharacterSize(100);

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
