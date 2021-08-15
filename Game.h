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

	sf::Clock* game_clock_;
	sf::Clock* clock_;
	mutable float game_time_;

	sf::View* view_;
	sf::RenderWindow* window_;

	Player* player_;

public:

	Game(sf::RenderWindow* window, sf::View* view) :
		view_(view),
		window_(window),
		clock_(new sf::Clock()),
		game_clock_(new sf::Clock())
	{
		
	}

	void draw(sf::RenderTarget& target, sf::RenderStates state) const override
	{
		notifyListeners(GameEventState::inGame);

		// Работа со временем.

		game_time_ = game_clock_->getElapsedTime().asSeconds();
		float time = clock_->getElapsedTime().asMicroseconds() / 800;
		clock_->restart();

		// Обновление игры.

		GameUpdater::gameUpdate(this, time);

		// Отрисовка.

		target.draw(*LevelManager::level->map);

		for (auto enemy : LevelManager::level->enemys)
		{
			target.draw(*enemy);
		}

		for (auto moving_platform : LevelManager::level->moving_platforms)
		{
			target.draw(*moving_platform);
		}

		target.draw(*player_);
		
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
			player_ = new Player(LevelManager::level->start_position, { 49, 112 }, view_, "data/images/player.png");
			break;
		}
	}

};

inline void GameUpdater::gameUpdate(const Game* game, float time)
{
	// Обновляем и проверяем на столкновение.

	for (auto block : LevelManager::level->map->blocks)
	{
		if (game->player_->getRect2f() & block->getRect2f())
		{
			game->player_->handlingCollision(*block);
		}
	}

	for (auto item : LevelManager::level->map->items)
	{
		if (game->player_->getRect2f() & item->getRect2f())
		{
			game->player_->handlingCollision(*item);
		}
	}

	for (auto enemy : LevelManager::level->enemys)
	{
		enemy->update(time);

		if (game->player_->getRect2f() & enemy->getRect2f())
		{
			game->player_->handlingCollision(*enemy);
		}
	}

	for (auto moving_platform : LevelManager::level->moving_platforms)
	{
		moving_platform->update();

		if (game->player_->getRect2f() & moving_platform->getRect2f())
		{
			game->player_->handlingCollision(*moving_platform);
		}
	}

	game->player_->update(time);
}

inline void GameLoader::loadGame(Game* game, sf::View* view)
{
	game->game_clock_->restart();
	 
	// Загрузка уровней.

	Level* level1(new Level(new Map(sf::Vector2f{ 26, 15 }, "data/images/textures.png", "data/maps/firstMap.txt"), sf::Vector2f{ 100, 100 }));
	level1->addMovingPlatform(new MovingPlatform(200, { 0, 500 }, { 150, 50 }, "data/images/platform.png"));

	LevelManager::addLevel(level1);

	Level* level2(new Level(new Map(sf::Vector2f{ 15, 10 }, "data/images/textures.png", "data/maps/secondMap.txt"), sf::Vector2f{ 100, 100 }));
	LevelManager::addLevel(level2);

	LevelManager::setNewLevel();

	// Загружаем персонажа.

	game->player_ = new Player(LevelManager::level->start_position, { 49, 112 }, view, "data/images/player.png");
}