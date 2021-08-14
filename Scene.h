#pragma once

class Scene : public sf::Drawable, public IGameEventListener
{
private:

	Menu* menu_;
	Map* map_;

public:

	Scene(sf::RenderWindow* window) :
		menu_(new Menu("data/images/menu.png", "data/images/info.png", window)),
		map_(new Map(sf::Vector2f{ 26, 15 }, "data/images/textures.png", "data/maps/firstMap.txt"))
	{
		menu_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		switch (GAME_STATE)
		{
		case GameEventState::inMenu:
			target.draw(*menu_);
			break;
		case GameEventState::inGame:
			target.draw(*map_);
			break;
		case GameEventState::GameOver:
			break;
		default:
			break;
		}
	}

	void updateByGameEvent(GameEventState event) override
	{
		
	}
};

