#pragma once

class Scene : public sf::Drawable, public IGameEventListener
{
private:

	Menu* menu_;
	GameEventType type_;

public:

	Scene(sf::RenderWindow* window) :
		menu_(new Menu("data/images/menu.png", "data/images/info.png", window)),
		type_(GameEventType::inMenu)
	{
		menu_->addListener(this);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		switch (type_)
		{
		case inMenu:
			target.draw(*menu_);
			break;
		case inGame:
			break;
		case PlayerDied:
			break;
		case SetNewLevel:
			break;
		case GameOver:
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
