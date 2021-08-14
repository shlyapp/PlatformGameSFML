#pragma once

enum class GameEventState
{
	None,
	StartGame,
	inGame,
	PlayerDied,
	SetNewLevel,
	GameOver
};

GameEventState GAME_STATE;

class IGameEventListener
{
public:

	virtual void updateByGameEvent(GameEventState event) = 0;

};

class IGameEventMaker
{
private:

	inline static std::list<IGameEventListener*> listeners_;

protected:

	void notifyListeners(GameEventState event)
	{
		if (event != GAME_STATE)
		{
			GAME_STATE = event;

			switch (GAME_STATE)
			{
			case GameEventState::StartGame:
				std::cout << "start game!\n";
				break;
			case GameEventState::inGame:
				std::cout << "in game!\n";
				break;
			case GameEventState::PlayerDied:
				std::cout << "player died!\n";
				break;
			case GameEventState::SetNewLevel:
				std::cout << "set new level!\n";
				break;
			case GameEventState::GameOver:
				std::cout << "game over!\n";
				break;
			default:
				break;
			}


			for (auto listener : listeners_)
			{
				listener->updateByGameEvent(event);
			}
		}	
	}

public:

	void addListener(IGameEventListener* listener)
	{
		listeners_.push_back(listener);
	}

	void removeListener(IGameEventListener* listener)
	{
		listeners_.remove(listener);
	}

};
