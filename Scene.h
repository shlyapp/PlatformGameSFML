#pragma once

//////////////////////////////////////////////////////////////
/// ����� Scene �������� �� ��������� ���� � ����, ����� ����.
//////////////////////////////////////////////////////////////
class Scene : public sf::Drawable, public IGameEventListener
{
private:

	// ������ ��������� �����.
	enum SceneState
	{
		InMenu,
		InGame,
		GameOver
	};

	// ��������� �������.
	SceneState state_;

	// ������� �����.
	Menu* menu_;
	Game* game_;

	sf::View view_;
	
public:

	Scene(sf::RenderWindow* window) :
		menu_(new Menu("data/images/menu.png", "data/images/info.png", window)),
		game_(new Game(window, &view_)),
		state_(SceneState::InMenu)
	{
		//////////////////////////////////////////////////////////////
		/// ���������� ���������� � ����. � ���� ���� ������ ������,
		/// � ��� ������� �� ��� ����� � ���� ������� ����������. �����
		/// ������ ����� �� "� ����", � ���� �������� ������ � ��� �����������.
		//////////////////////////////////////////////////////////////
		menu_->addListener(game_);
		menu_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		// � ����������� �� ��������� ������������ ���� ����, ���� ����, ���� ����� ����������.
		switch (state_)
		{
		case SceneState::InMenu:
			target.draw(*menu_);
			break;
		case SceneState::InGame:
			target.draw(*game_);
			target.setView(view_);
			break;
		case SceneState::GameOver:
			break;
		default:
			break;
		}
	}

	// ���������� ��������� ����� � ����������� �� ��������� ����.
	void updateByGameEvent(GameEventState event) override
	{
		// ���� ���� ��������, �� �������� ��������� �� "� ����"
		if (event == GameEventState::inGame)
		{
			view_.reset(sf::FloatRect(0, 0, 550, 400));
			state_ = SceneState::InGame;
		}
		// ���� ���� �����������, �� �������� �� "���� ��������."
		if (event == GameEventState::GameOver)
		{
			state_ = SceneState::GameOver;
		}
	}

};

