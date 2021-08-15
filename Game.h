#pragma once

class Game;

class GameLoader
{
private:

	GameLoader()
	{

	}

public:

	static void loadGame(Game* game, sf::View* view);

};

class GameUpdater
{
private:

	GameUpdater()
	{

	}

public:

	static void gameUpdate(const Game* game, float time);

};

class Game : public sf::Drawable, public IGameEventListener, public IGameEventMaker
{

	friend class GameLoader;
	friend class GameUpdater;

private:

	sf::Clock game_clock_;
	sf::Clock* clock_;
	mutable float game_time_;

	sf::View* view_;
	sf::RenderWindow* window_;
	MovingPlatform* platform_;

public:

	Game(sf::RenderWindow* window, sf::View* view) :
		view_(view),
		window_(window),
		clock_(new sf::Clock()),
		platform_(new MovingPlatform(sf::Vector2f(0, 100), sf::Vector2f(150, 50), sf::Vector2f(0, 500), 3.0f, "data/images/platform.png"))
	{
		
	}

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override
	{
		notifyListeners(GameEventState::inGame);

		game_time_ = game_clock_.getElapsedTime().asSeconds();
		float time = clock_->getElapsedTime().asMicroseconds() / 800;
		clock_->restart();

		GameUpdater::gameUpdate(this, time);

		//target.draw(*LevelManager::level->map);
		target.draw(*platform_);

	}

	void updateByGameEvent(GameEventState event) override
	{
		switch (event)
		{
		case GameEventState::StartGame:
			GameLoader::loadGame(this, view_);
			notifyListeners(GameEventState::inGame);
			break;
		case GameEventState::PlayerDied:
			break;
		case GameEventState::SetNewLevel:
			break;
		}
	}

};

inline void GameUpdater::gameUpdate(const Game* game, float time)
{
	game->platform_->update();
}

inline void GameLoader::loadGame(Game* game, sf::View* view)
{
	game->game_clock_.restart();

	LevelManager::addLevel(
		new Level
		(
			new Map(sf::Vector2f{ 26, 15 }, "data/images/textures.png", "data/maps/firstMap.txt"),
			sf::Vector2f{ 100, 100 }
		)
	);

	LevelManager::addLevel(
		new Level
		(
			new Map(sf::Vector2f{ 15, 10 }, "data/images/textures.png", "data/maps/secondMap.txt"),
			sf::Vector2f{ 100, 100 }
		)
	);

	LevelManager::setNewLevel();
}