#pragma once

//////////////////////////////////////////////////////////////
/// Класс Scene отвечает за отрисовку игру и меню, конца игры.
//////////////////////////////////////////////////////////////
class Scene : 
	public sf::Drawable, 
	public IGameEventListener,
	public IGameEventMaker
{
private:

	// Храним состояния сцены.
	enum SceneState
	{
		InMenu,
		InGame,
		GameOver,
		WinGame
	};

	// Состояние текущее.
	SceneState state_;

	// Обьекты сцены.
	Menu* menu_;
	Game* game_;
	GameOverMenu* game_over_;
	WinMenu* win_menu_;

	sf::View view_;
	sf::RenderWindow* window_;

	sf::View view_menu_;

public:

	Scene(sf::RenderWindow* window) :
		menu_(new Menu("data/images/menu.png", "data/images/info.png", window)),
		game_(new Game(window, &view_)),
		game_over_(new GameOverMenu(window)),
		win_menu_(new WinMenu(window)),
		window_(window),
		state_(SceneState::InMenu)
	{
		//////////////////////////////////////////////////////////////
		/// Добавление слушателей к меню. В меню есть кнопка старта,
		/// и при нажатии на нее сцена и игра получат оповещение. Сцена
		/// сменит режим на "В игре", а игра загрузит уровни и все необходимое.
		//////////////////////////////////////////////////////////////
		menu_->addListener(game_);
		menu_->addListener(this);

		view_menu_.reset(sf::FloatRect(0, 0, 1280, 720));
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		// В зависимости от состояния отрисовываем либо меню, либо игру, либо экран завершения.
		switch (state_)
		{
		case SceneState::InMenu:
			target.draw(*menu_);
			target.setView(view_menu_);
			break;
		case SceneState::InGame:
			target.draw(*game_);
			target.setView(view_);
			break;
		case SceneState::GameOver:
			target.draw(*game_over_);
			target.setView(view_menu_);
			break;
		case SceneState::WinGame:
			target.draw(*win_menu_);
			target.setView(view_menu_);
			break;
		}
	}

	// Обновяляем состояния сцены в зависимости от состояния игры.
	void updateByGameEvent(GameEventState event) override
	{
		// Если игра началась, то изменяем состояния на "В игре"
		if (event == GameEventState::inGame)
		{
			view_.reset(sf::FloatRect(0, 0, 550, 400));
			state_ = SceneState::InGame;
		}
		// Если игра закончилась, то изменяем на "Игра окончена."
		if (event == GameEventState::GameOver)
		{
			//window_->setView(view);
			state_ = SceneState::GameOver;
		}
		
		if (event == GameEventState::RestartGame)
		{
			state_ = SceneState::InGame;
		}

		if (event == GameEventState::WinGame)
		{
			state_ = SceneState::WinGame;
		}
	}

};

