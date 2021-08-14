#pragma once

enum class GameEventState
{
	inMenu,
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
