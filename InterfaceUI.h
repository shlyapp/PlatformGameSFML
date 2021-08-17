#pragma once

//////////////////////////////////////////////////////////////
/// ����� Menu ���������� ������������ �������� ���� � ������.
//////////////////////////////////////////////////////////////
class Menu : 
	public sf::Drawable, 
	public gui::IEventListener, 
	public IGameEventMaker
{
private:

	// ��� ����.
	enum TypeMenu
	{
		Start,
		Info
	};

	mutable TypeMenu type;

	// �������� � ������� ��� �����������.
	sf::Texture menu_texture_;
	sf::Sprite menu_sprite_;

	sf::Texture info_texture_;
	sf::Sprite info_sprite_;

	// �������� ����� ��� ���������.
	gui::TextBlock* start_game_;
	gui::TextBlock* info_;

	// ��������� ��������.
	void loadFiles(const std::string path_menu, const std::string path_info)
	{
		menu_texture_.loadFromFile(path_menu);
		menu_sprite_.setTexture(menu_texture_);

		info_texture_.loadFromFile(path_info);
		info_sprite_.setTexture(info_texture_);
	}

public:

	// ����������� �������� ����.
	Menu(std::string path_menu, std::string path_info, sf::RenderWindow* window) :
		start_game_(new gui::TextBlock(sf::Vector2f(520, 300), "����� ����", window)),
		info_(new gui::TextBlock(sf::Vector2f(520, 360), "� ���������", window)),
		type(TypeMenu::Start)
	{
		loadFiles(path_menu, path_info);

		info_->addListener(this);
		start_game_->addListener(this);

		start_game_->setCharacterSize(50);
		info_->setCharacterSize(50);
	}

	// ��������� ����.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		// ��������� ���������� � ���, ��� ������ ��������� "� ����".
		notifyListeners(GameEventState::inMenu);

		// � ����������� �� ����, ������� ������.
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

	// ������������ ������� ������� ������.
	void updateByGUIEvent(gui::EventType event, const gui::Element* element) override
	{
		// ���� ������ info � ������� �������, �� ������ ��� ���� �� ����� ���������� � ����.
		if (element == info_ && event == gui::EventType::Click)
		{
			type = TypeMenu::Info;
		}
		// ���� ������ ������ ���� � ������� �������.
		if (element == start_game_ && event == gui::EventType::Click)
		{
			// ���������� ���������� � ����� ������� ���� �� �����.
			notifyListeners(GameEventState::StartGame);
		}
	}
};

//////////////////////////////////////////////////////////////
/// ���� ����� �������� �� ����� ���������� � ���������.
//////////////////////////////////////////////////////////////
class PlayerInfoBar : 
	public sf::Drawable,
	public gui::IEventListener,
	public IGameEventMaker
{
private:

	// ������� ����� � �����������.
	gui::TextBlock* health_block_;
	gui::TextBlock* time_block_;
	gui::TextBlock* gears_block_;
	gui::TextBlock* lives_;
	gui::TextBlock* change_player_;

public:

	PlayerInfoBar(sf::RenderWindow* window) :
		health_block_(new gui::TextBlock(sf::Vector2f(0.0f, 50.0f), "��������: ", window)),
		time_block_(new gui::TextBlock(sf::Vector2f(0.0f, 100.0f), "�����: ", window)),
		gears_block_(new gui::TextBlock(sf::Vector2f(0.0f, 150.0f), "��������: ", window)),
		lives_(new gui::TextBlock(sf::Vector2f(0.0f, 200), "�����: ", window)),
		change_player_(new gui::TextBlock(sf::Vector2f(0.0f, 250.0f), "������� ���������", window))
	{
		// ��������� ��������������� � ����, ����� "����� ���������".
		health_block_->disableInteractivity();
		time_block_->disableInteractivity();
		gears_block_->disableInteractivity();
		lives_->disableInteractivity();

		// ��������� ��������� � "������� ���������", ����� �����, ����� ��� �������� � ���������� �� ���� ����.
		change_player_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(*health_block_);
		target.draw(*time_block_);
		target.draw(*gears_block_);
		target.draw(*lives_);
		target.draw(*change_player_);
	}

	void updateByGUIEvent(gui::EventType event, const gui::Element* element) override
	{
		if (event == gui::EventType::Click)
		{
			// ��������� ����, ����� ��� ������� ���������.
			notifyListeners(GameEventState::PlayerChanged);
		}	
	}

	void update(int health, int lives, int time, int num_gears, sf::View& view)
	{

		health_block_->setText("��������: " + std::to_string(health));
		health_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 200.0f });

		time_block_->setText("�����: " + std::to_string(time));
		time_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 170.0f });

		gears_block_->setText("��������: " + std::to_string(num_gears));
		gears_block_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 140.0f });

		lives_->setText("�����:" + std::to_string(lives));
		lives_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 110.0f });

		change_player_->setPosition(view.getCenter() - sf::Vector2f{ 270.0f, 80.0f });
	}

};

//////////////////////////////////////////////////////////////
/// ���� ����� �������� �� ����� �������� � ������� �����������
/// ������ ���� ������.
//////////////////////////////////////////////////////////////
class GameOverMenu :
	public sf::Drawable,
	public gui::IEventListener,
	public IGameEventMaker
{
private:

	// ������ ������ ����.
	sf::Texture texture_;
	sf::Sprite sprite_;

	// ������ ������ ���� �����.
	gui::TextBlock* new_game_;

	// �������� ������.
	void loadFiles()
	{
		texture_.loadFromFile("data/images/menu.png");
		sprite_.setTexture(texture_);
	}

public:

	GameOverMenu(sf::RenderWindow* window) :
		new_game_(new gui::TextBlock(sf::Vector2f(500, 300), "������ ������.", window))
	{
		loadFiles();
		// ��������� ��������� � ������, ����� �����, ���� ��� ������.
		new_game_->addListener(this);
		new_game_->setCharacterSize(50);
	}

	// ������ ������ � ������.
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite_);
		target.draw(*new_game_);
	}

	// �������� ���������� ��������� ������.
	void updateByGUIEvent(gui::EventType event, const gui::Element* element) override
	{
		// ���� ��������.
		if (event == gui::EventType::Click)
		{
			// ������������� ����.
			notifyListeners(GameEventState::RestartGame);
		}
	}

};

//////////////////////////////////////////////////////////////
/// ���� ����� �������� �� �������� ����.
//////////////////////////////////////////////////////////////
class WinMenu : public sf::Drawable
{
private:

	// ���������� ����������� ������, ������ ��� �������� ������.

	sf::Texture texture_;
	sf::Sprite sprite_;

	gui::TextBlock* text_;

	void loadFiles()
	{
		texture_.loadFromFile("data/images/menu.png");
		sprite_.setTexture(texture_);
	}

public:

	WinMenu(sf::RenderWindow* window) :
		text_(new gui::TextBlock(sf::Vector2f(500, 320), "���� ��������!", window))
	{
		loadFiles();
		text_->disableInteractivity();
		text_->setCharacterSize(50.0f);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(sprite_);
		target.draw(*text_);
	}

};

//////////////////////////////////////////////////////////////
/// ���� ����� �������� �� ����� ���������� � �����.
//////////////////////////////////////////////////////////////
class BossInfoBar : public sf::Drawable
{
private:
	
	// �������� ������ � ������� ���������� � ���������.

	gui::TextBlock* health_;

public:

	BossInfoBar(sf::RenderWindow* window) : health_(new gui::TextBlock(sf::Vector2f{0.0f, 0.0f}, "�������� �����: ", window))
	{
		health_->disableInteractivity();
		health_->setColor(sf::Color::Red);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(*health_);
	}

	void update(const int health, sf::View& view)
	{
		health_->setText("�������� �����: " + std::to_string(health));
		health_->setPosition(view.getCenter() + sf::Vector2f(20, -200));
	}

};
