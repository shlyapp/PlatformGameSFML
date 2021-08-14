#pragma once

class Scene : public sf::Drawable, public IGameEventListener
{
private:

	Menu* menu_;
	Map* map_;
	GameEventType type_;

public:

	Scene(sf::RenderWindow* window) :
		menu_(new Menu("data/images/menu.png", "data/images/info.png", window)),
		map_(new Map(sf::Vector2f{ 26, 15 }, "data/images/textures.png", "data/maps/firstMap.txt")),
		type_(GameEventType::inMenu)
	{
		menu_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		switch (type_)
		{
		case GameEventType::inMenu:
			target.draw(*menu_);
			break;
		case GameEventType::inGame:
			target.draw(*map_);
			break;
		case GameEventType::GameOver:
			break;
		default:
			break;
		}
	}

	void updateByGameEvent(GameEventType event) override
	{
		type_ = event;
	}

};
