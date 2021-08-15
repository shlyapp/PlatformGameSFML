#pragma once

//////////////////////////////////////////////////////////////
/// Класс Camera следит за игроком, можно задать скорость
/// и тем самым можно сделать плавную камеру.
//////////////////////////////////////////////////////////////
class Camera
{
private:

	// Позиция, скорость, вид.
	sf::Vector2f position_;
	sf::Vector2f speed_;
	sf::View* view_;

	// Границы для камер.
	sf::Vector2f borders_x_;
	sf::Vector2f borders_y_;

public:

	// Конструктор для инициализации.
	Camera(sf::View* view, sf::Vector2f speed) :
		view_(view),
		speed_(speed),
		position_(view->getSize().x / 2, view->getSize().y / 2),
		borders_x_(sf::Vector2f{ view_->getSize().x / 2, 50 * LevelManager::level->map->size.x - view_->getSize().x / 2 }),
		borders_y_(sf::Vector2f{ view_->getSize().y / 2, 50 * LevelManager::level->map->size.y - view_->getSize().y / 2 })
	{

	}

	// Обновление камеры исходя из позиции персонажа.
	void updateByPosition(sf::Vector2f position)
	{
		// Расчитваем границы.
		borders_x_ = { view_->getSize().x / 2, 50 * LevelManager::level->map->size.x - view_->getSize().x / 2 };
		borders_y_ = { view_->getSize().y / 2, 50 * LevelManager::level->map->size.y - view_->getSize().y / 2 };

		// Проверяем на пересечение границ.
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

		// Движение камеры.
		position_.x -= (position_.x - position.x) / speed_.x;
		position_.y -= (position_.y - position.y) / speed_.y;

		// Устанавливаем камеру.
		view_->setCenter(position_);
	}
};

//////////////////////////////////////////////////////////////
/// Класс GameEntity описывает игровую сущность, ее здоровье,
/// урон.
//////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////
/// MovingPlatform является движущейся платформой, которая
/// перемещается по двум контрольным точкам.
//////////////////////////////////////////////////////////////
class MovingPlatform :
	public sf::Drawable,
	public IMoveAble,
	public Entity
{
private:

	// Направление движения.
	enum DirectionType
	{
		Left,
		Right
	};

	DirectionType dir_;

	// Контрольные точки.
	sf::Vector2f control_points_;

	// Текстуры, спрайт.
	sf::Texture texture_;
	sf::Sprite sprite_;

	// Загрузка текстур.
	void loadFiles(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_.setTexture(texture_);
	}

public:

	// Конструктор для инициализации.
	MovingPlatform(float y, sf::Vector2f control_points, sf::Vector2f size, float speed, std::string path_texture) :
		IMoveAble(speed),
		Entity(sf::Vector2f{ control_points.x, y }, size),

		control_points_(control_points),
		dir_(DirectionType::Right)
	{
		loadFiles(path_texture);
	}

	// Метод обновления, в котором осущетвляется двидение платформы.
	void update()
	{
		// Проверяем направление.
		if (dir_ == DirectionType::Right)
		{
			// Если персекло контрольную точку.
			if (position_.x > control_points_.y)
			{
				// Меняем направление.
				dir_ = DirectionType::Left;
			}
			else
			{
				// Иначи продолжаем движение.
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

		// Обновляем позицию.
		IMoveAble::updateByMove();
		sprite_.setPosition(position_);
	}

	// Отрисовка спрайта.
	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		target.draw(sprite_);
	}

};