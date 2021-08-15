#pragma once

class Camera
{
private:

	sf::Vector2f position_;
	sf::Vector2f speed_;
	sf::View* view_;

	sf::Vector2f borders_x_;
	sf::Vector2f borders_y_;

public:

	Camera(sf::View* view, sf::Vector2f speed) :
		view_(view),
		speed_(speed),
		position_(view->getSize().x / 2, view->getSize().y / 2),
		borders_x_({ view_->getSize().x / 2, 50 * LevelManager::level->map->size.x - view_->getSize().x / 2 }),
		borders_y_({ view_->getSize().y / 2, 50 * LevelManager::level->map->size.y - view_->getSize().y / 2 })
	{

	}

	void updateByPosition(sf::Vector2f position)
	{
		borders_x_ = { view_->getSize().x / 2, 50 * LevelManager::level->map->size.x - view_->getSize().x / 2 };
		borders_y_ = { view_->getSize().y / 2, 50 * LevelManager::level->map->size.y - view_->getSize().y / 2 };

		if (position.x < borders_x_.x)
		{
			position.x = borders_x_.x;
		}

		if (position.x > borders_x_.y)
		{
			position.x = borders_x_.y;
		}

		if (position.y < borders_y_.x)
		{
			position.y = borders_y_.x;
		}

		if (position.y > borders_y_.y)
		{
			position.y = borders_y_.y;
		}

		position_.x -= (position_.x - position.x) / speed_.x;
		position_.y -= (position_.y - position.y) / speed_.y;

		view_->setCenter(position_);
	}
};

class GameEntity
{
protected:

	float health_;
	float damage_;

	bool is_live_;

	GameEntity(float health, float damage) :
		health_(health),
		damage_(damage),
		is_live_(true)
	{

	}

public:

	virtual void update(const float time) = 0;

	void dealDamage(const float value)
	{
		health_ -= value;

		if (health_ <= 0)
		{
			is_live_ = false;
		}
	}

	float getDamageValue() const
	{
		return damage_;
	}

	bool isLive() const
	{
		return is_live_;
	}

};

class MovingPlatform :
	public sf::Drawable,
	public IMoveAble,
	public Entity
{
private:

	enum DirectionType
	{
		Left,
		Right
	};

	DirectionType dir_;

	sf::Vector2f control_points_;

	sf::Texture texture_;
	sf::Sprite sprite_;

	void loadFiles(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_.setTexture(texture_);
	}

public:

	MovingPlatform(sf::Vector2f position, sf::Vector2f size, sf::Vector2f control_points, float speed, std::string path_texture) :
		IMoveAble(speed),
		Entity(position, size),

		control_points_(control_points),
		dir_(DirectionType::Right)
	{
		loadFiles(path_texture);
	}

	void update()
	{
		if (dir_ == DirectionType::Right)
		{
			if (position_.x > control_points_.y)
			{
				dir_ = DirectionType::Left;
			}
			else
			{
				speed_.x = speed_value_;
			}
		}
		else
		{
			if (position_.x < control_points_.x)
			{
				dir_ = DirectionType::Right;
			}
			else
			{
				speed_.x = -speed_value_;
			}
		}

		IMoveAble::updateByMove();
		sprite_.setPosition(position_);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		target.draw(sprite_);
	}

};