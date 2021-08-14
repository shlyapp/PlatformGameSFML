#pragma once

enum class GameEventType
{
	inMenu,
	inGame,
	PlayerDied,
	SetNewLevel,
	GameOver
};

class IGameEventListener
{
public:

	virtual void updateByGameEvent(GameEventType event) = 0;

};

class IGameEventMaker
{
private:

	static std::list<IGameEventListener*> listeners_;

protected:

	void notifyListeners(GameEventType event)
	{
		for (auto listener : listeners_)
		{
			listener->updateByGameEvent(event);
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

std::list<IGameEventListener*> IGameEventMaker::listeners_;
