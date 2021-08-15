#pragma once

//////////////////////////////////////////////////////////////
/// ����� Menu ���������� ������������ �������� ���� � ������.
//////////////////////////////////////////////////////////////

class Menu : public sf::Drawable, public gui::IEventListener, public IGameEventMaker
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