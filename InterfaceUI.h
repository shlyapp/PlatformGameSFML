#pragma once

//////////////////////////////////////////////////////////////
/// Класс Menu занимается отображением главного меню и кнопок.
//////////////////////////////////////////////////////////////
class Menu : public sf::Drawable, public gui::IEventListener, public IGameEventMaker
{
private:

	// Тип меню.
	enum TypeMenu
	{
		Start,
		Info
	};

	mutable TypeMenu type;

	// Текстуры и спрайты для изображений.
	sf::Texture menu_texture_;
	sf::Sprite menu_sprite_;

	sf::Texture info_texture_;
	sf::Sprite info_sprite_;

	// Тестовые блоки для переходов.
	gui::TextBlock* start_game_;
	gui::TextBlock* info_;

	// Загружаем текстуры.
	void loadFiles(const std::string path_menu, const std::string path_info)
	{
		menu_texture_.loadFromFile(path_menu);
		menu_sprite_.setTexture(menu_texture_);

		info_texture_.loadFromFile(path_info);
		info_sprite_.setTexture(info_texture_);
	}

public:

	// Конструктор создания меню.
	Menu(std::string path_menu, std::string path_info, sf::RenderWindow* window) :
		start_game_(new gui::TextBlock(sf::Vector2f(520, 300), "Новая игра", window)),
		info_(new gui::TextBlock(sf::Vector2f(520, 360), "О программе", window)),
		type(TypeMenu::Start)
	{
		loadFiles(path_menu, path_info);

		info_->addListener(this);
		start_game_->addListener(this);

		start_game_->setCharacterSize(50);
		info_->setCharacterSize(50);
	}

	// Отрисовка меню.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		// Оповещаем слушателей о том, что сейчас состояния "В меню".
		notifyListeners(GameEventState::inMenu);

		// В зависимости от типа, выводим нужное.
		if (type == TypeMenu::Start)
		{
			target.draw(menu_sprite_);
			target.draw(*start_game_);
			target.draw(*info_);
		}
		else
		{
			target.draw(info_sprite_);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				type = TypeMenu::Start;
			}
		}
	}

	// Обрабатываем события нажатия кнопок.
	void updateByGUIEvent(gui::EventType event, const gui::Element* element) override
	{
		// Если кнопка info и событие нажатие, то меняем тип меню на показ информации о игре.
		if (element == info_ && event == gui::EventType::Click)
		{
			type = TypeMenu::Info;
		}
		// Если кнопка старта игры и событие нажатия.
		if (element == start_game_ && event == gui::EventType::Click)
		{
			// Уведомляем слушателей о смене события игры на старт.
			notifyListeners(GameEventState::StartGame);
		}
	}
};

class PlayerInfoBar : 
	public sf::Drawable,
	public gui::IEventListener,
	public IGameEventMaker
{
private:

	gui::TextBlock* health_block_;
	gui::TextBlock* time_block_;
	gui::TextBlock* gears_block_;
	gui::TextBlock* change_player_;

public:

	PlayerInfoBar(sf::RenderWindow* window) :
		health_block_(new gui::TextBlock(sf::Vector2f(0.0f, 50.0f), "Здоровье: ", window)),
		time_block_(new gui::TextBlock(sf::Vector2f(0.0f, 100.0f), "Время: ", window)),
		gears_block_(new gui::TextBlock(sf::Vector2f(0.0f, 150.0f), "Шестерни: ", window)),
		change_player_(new gui::TextBlock(sf::Vector2f(0.0f, 200.0f), "Сменить персонажа", window))
	{
		health_block_->disableInteractivity();
		time_block_->disableInteractivity();
		gears_block_->disableInteractivity();

		change_player_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(*health_block_);
		target.draw(*time_block_);
		target.draw(*gears_block_);
		target.draw(*change_player_);
	}

	void updateByGUIEvent(gui::EventType event, const gui::Element* element) override
	{

		switch (event)
		{
		case gui::EventType::Click:
			std::cout << "click!\n";
			break;
		case gui::EventType::MouseEnter:
			std::cout << "enter!\n";
			break;
		case gui::EventType::MouseLeave:
			std::cout << "leave!\n";
			break;
		default:
			break;
		}

		if (event == gui::EventType::Click)
		{
			notifyListeners(GameEventState::PlayerChanged);
		}	
	}

	void update(int health, int time, int num_gears, sf::View& view)
	{

		health_block_->setText("Здоровье: " + std::to_string(health));
		health_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 200.0f });

		time_block_->setText("Время: " + std::to_string(time));
		time_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 170.0f });

		gears_block_->setText("Шестерни: " + std::to_string(num_gears));
		gears_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 140.0f });

		change_player_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 110.0f });
	}

};