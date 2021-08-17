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

class GameUpdater : public IGameEventMaker
{
private:

	GameUpdater()
	{

	}

public:

	static void gameUpdate(const Game* game, float time);

};

class Game : 
	public sf::Drawable, 
	public IGameEventListener, 
	public IGameEventMaker
{

	friend class GameLoader;
	friend class GameUpdater;

private:

	int lives_;

	sf::Clock* game_clock_;
	sf::Clock* clock_;
	mutable float game_time_;

	sf::View* view_;
	sf::RenderWindow* window_;

	Player* main_player_;
	Player* player1_;
	Player* player2_;

	PlayerInfoBar* info_;
	BossInfoBar* boss_info_;

public:

	Game(sf::RenderWindow* window, sf::View* view) :
		view_(view),
		window_(window),
		clock_(new sf::Clock()),
		game_clock_(new sf::Clock()), 

		lives_(3)
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

		target.draw(*info_);

		if (LevelManager::level->boss != nullptr)
		{
			if (LevelManager::level->boss->isLive())
			{
				target.draw(*LevelManager::level->boss);
				target.draw(*boss_info_);
			}
		}

		target.draw(*main_player_);
		
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
			lives_--;
			restartPlayer();
			break;

		case GameEventState::PlayerChanged:
			changePlayer();
			break;

		case GameEventState::SetNewLevel:
			restartPlayer();
			break;

		case GameEventState::RestartGame:
			LevelManager::deleteLevels();
			GameLoader::loadGame(this, view_);
			break;
		}

	}

	void changePlayer()
	{
		if (main_player_ == player1_)
		{
			main_player_ = player2_;
		}
		else if (main_player_ == player2_)
		{
			main_player_ = player1_;
		}
	}

	void restartPlayer()
	{
		if (lives_ == 0)
		{
			notifyListeners(GameEventState::GameOver);
		}

		player1_ = new Player(LevelManager::level->start_position, { 49, 112 }, view_, "data/images/player.png");
		player2_ = new Player(LevelManager::level->start_position, { 49, 112 }, view_, "data/images/player.png");
		main_player_ = player1_;
	}

	void restartGame()
	{
		game_clock_->restart();

		lives_ = 3;
		LevelManager::setLevel(0);
	}

};

inline void GameUpdater::gameUpdate(const Game* game, float time)
{
	// Обновляем и проверяем на столкновение.

	game->main_player_->update(time);

	for (auto block : LevelManager::level->map->blocks)
	{
		if (game->main_player_->getRect2f() & block->getRect2f())
		{
			game->main_player_->handlingCollision(*block);
		}
	}

	for (auto it = LevelManager::level->map->items.begin(); it != LevelManager::level->map->items.end(); ++it)
	{
		Item* item = *it;

		if (LevelManager::level->map->items.size() == 0)
		{
			break;
		}

		if (game->main_player_->getRect2f() & item->getRect2f())
		{
			game->main_player_->handlingCollision(*item);
		}

		// Если в процессе сменился уровень, то выходим из цикла.
		if (GAME_STATE == GameEventState::SetNewLevel)
		{
			break;
		}

		if (!item->isActive())
		{
			it = LevelManager::level->map->items.erase(it);
			delete item;
		}
	}

	for (auto it = LevelManager::level->enemys.begin(); it != LevelManager::level->enemys.end(); ++it)
	{
		Enemy* enemy = *it;

		enemy->update(time);

		if (game->main_player_->getRect2f() & enemy->getRect2f())
		{
			game->main_player_->handlingCollision(*enemy);
		}

		if (!enemy->isLive())
		{
			it = LevelManager::level->enemys.erase(it);
			delete enemy;
			break;
		}
	}

	for (auto moving_platform : LevelManager::level->moving_platforms)
	{
		moving_platform->update();

		if (game->main_player_->getRect2f() & moving_platform->getRect2f())
		{
			game->main_player_->handlingCollision(*moving_platform);
		}
	}
	
	if (LevelManager::level->boss != nullptr)
	{
		if (LevelManager::level->boss->isLive())
		{
			LevelManager::level->boss->update(time);
			LevelManager::level->boss->updateBullets(*game->main_player_);

			game->boss_info_->update(LevelManager::level->boss->getHealth(), *game->view_);

			if (game->main_player_->getRect2f() & LevelManager::level->boss->getRect2f())
			{
				game->main_player_->handlingCollision(*LevelManager::level->boss);
			}
		}
	}

	game->info_->update(game->main_player_->getHealth(), game->lives_, game->game_time_, game->main_player_->getGearsNum(), *game->view_);

	if (!LevelManager::level->boss->isLive())
	{
		notifyListeners(GameEventState::WinGame);
	}
}

inline void GameLoader::loadGame(Game* game, sf::View* view)
{
	game->game_clock_->restart();

	game->lives_ = 3;
	 
	// Загрузка уровней.

	Level* level1(new Level(new Map(sf::Vector2f{ 26, 15 }, "data/images/textures.png", "data/maps/firstMap.txt"), sf::Vector2f{ 100, 100 }));
	level1->addMovingPlatform(new MovingPlatform(200, sf::Vector2f{ 0, 500 }, sf::Vector2f{ 150, 50 }, "data/images/platform.png"));
	level1->addEnemy(new Enemy(340, sf::Vector2f{ 200, 500 }, sf::Vector2f{ 35, 48 }, "data/images/chip.png"));

	LevelManager::addLevel(level1);
	
	Level* level2(new Level(new Map(sf::Vector2f{ 15, 10 }, "data/images/textures.png", "data/maps/secondMap.txt"), sf::Vector2f{ 50, 100 }));
	level2->addMovingPlatform(new MovingPlatform(300, sf::Vector2f(100, 300), sf::Vector2f(150, 50), "data/images/platform.png"));
	LevelManager::addLevel(level2);

	Level* level3(new Level(new Map(sf::Vector2f{ 15, 10 }, "data/images/textures.png", "data/maps/thirdMap.txt"), sf::Vector2f{ 100, 100 }));
	level3->addBoss(new Boss(300, sf::Vector2f{ 100, 600 }, sf::Vector2f{ 123, 144 }, "data/images/boss.png"));
	LevelManager::addLevel(level3);

	LevelManager::setNewLevel();

	// Загружаем персонажа.

	game->player1_ = new Player(LevelManager::level->start_position, sf::Vector2f{ 49, 112 }, view, "data/images/player.png");
	game->player2_ = new Player(LevelManager::level->start_position, sf::Vector2f{ 49, 112 }, view, "data/images/player.png");

	game->main_player_ = game->player1_;

	game->info_ = new PlayerInfoBar(game->window_);
	game->boss_info_ = new BossInfoBar(game->window_);
}