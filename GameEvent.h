#pragma once

//////////////////////////////////////////////////////////////
/// GameEventState ��������� �������, ������� ����� �����������
/// � ���� � ��� �������� �������� ��������� �������.
//////////////////////////////////////////////////////////////
enum class GameEventState
{
	None,
	inMenu,
	RestartGame,
	WinGame,
	StartGame,
	inGame,
	PlayerDied,
	PlayerChanged,
	SetNewLevel,
	GameOver
};

// ��������� ������ ��������� ����.
GameEventState GAME_STATE;

//////////////////////////////////////////////////////////////
/// IGameEventListener - ��������� ��������� �������,
/// ������������ � ����. ���� ������-�� ������ ���������� �����
/// � ��������� ����, �� �� ��������� ������ ���������.
//////////////////////////////////////////////////////////////
class IGameEventListener
{
public:

	virtual void updateByGameEvent(GameEventState event) = 0;

};

//////////////////////////////////////////////////////////////
/// IGameEventMaker - ���� �����-�� ����� ����� ������ ��������� ����
/// �� �� ��������� ������ �����. ��������: ����� Player ������ �����������
/// ���� �����, ����� ��� ����� ������ �� ��������� ������, � ���� � ���� �������
/// �������� �� ��� ���������� � ������� �� ������ ������ ��� ��������� ����.
//////////////////////////////////////////////////////////////
class IGameEventMaker
{
private:

	inline static std::list<IGameEventListener*> listeners_;

protected:

	void notifyListeners(GameEventState event) const
	{
		if (event != GAME_STATE)
		{
			GAME_STATE = event;
			
			// ��� �������� ������, ������� ��������� � �������.
			switch (GAME_STATE)
			{
			case GameEventState::StartGame:
				std::cout << "start game!\n";
				break;
			case GameEventState::WinGame:
				std::cout << "win game!\n";
				break;
			case GameEventState::RestartGame:
				std::cout << "restart game!\n";
				break;
			case GameEventState::PlayerChanged:
				std::cout << "player changed!\n";
				break;
			case GameEventState::inMenu:
				std::cout << "in menu!\n";
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
