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

	// Значением смещения (чтобы ноги персонажа были на земле).
	const sf::Vector2f SPRITE_POSITION_INACCUARACY = { 0.0f, 20.0f };

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

	void setDamage(const float damage)
	{
		damage_ = damage;
	}

	void setHealth(const float health)
	{
		health_ = health;
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
	MovingPlatform(float y, sf::Vector2f control_points, sf::Vector2f size, std::string path_texture) :
		IMoveAble(3.0f),
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

//////////////////////////////////////////////////////////////
/// Enemy описывает противника, который двигается по двум
/// контрольным точкам и наносит урон противнику при столкновении.
//////////////////////////////////////////////////////////////
class Enemy :
	public IMoveAble,
	public IAnimationAble,
	public Entity,
	public GameEntity
{
private:

	// Контрольные точки.
	sf::Vector2f control_points_;

public:

	// Конструктор, которые выставляет стандартные параметры, если есть необходимость что-то изменить, 
	// то можно воспользоваться сеттером. Например, не устраивает скорость движения 2.0f и есть враг,
	// у которого скорость должна быть больше, то при помощи setSpeed() уставливаем другую скорость.
	Enemy(float y, sf::Vector2f control_points, sf::Vector2f size, std::string path_texture) :
		IMoveAble(2.0f),
		IAnimationAble(path_texture, 4, 0.01f, size),
		Entity(sf::Vector2f(control_points.x, y), size),
		GameEntity(100, 10),

		control_points_(control_points)
	{
		anim_state_ = AnimationState::MoveRight;
		sprite_.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	}

	// Метод обновлени, похож на обновление у MovingPlatform, но еще добавляется анимация.
	void update(float time) override
	{
		if (anim_state_ == AnimationState::MoveRight)
		{
			if (position_.x > control_points_.y)
			{
				anim_state_ = AnimationState::MoveLeft;
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
				anim_state_ = AnimationState::MoveRight;
			}
			else
			{
				speed_.x = -speed_value_;
			}
		}

		IMoveAble::updateByMove();
		sprite_.setPosition(position_ + SPRITE_POSITION_INACCUARACY);
		changeFrameAnimation(time);
	}
};

//////////////////////////////////////////////////////////////
/// Класс Player является основным, он описывает поведение игрока
/// его передвижение и обрабатывает столковения со всемия обьектами.
//////////////////////////////////////////////////////////////
class Player:
	public ICollisionWithBlockAble, public ICollisionWithEnemyAble, public ICollisionWithItemAble, public ICollisionWithPlatformAble,
	public IMoveAble,
	public IAnimationAble,
	public Entity,
	public GameEntity
{
private:

	// Логический флаг нахождения персонажа на земле.
	bool on_ground_;
	// Количество собраных шестерней.
	int gears_;

	// Камера, которая следит за персонажем.
	Camera camera_;

	// Проверка на столкновение с краями карты.
	void handlingCollisionWithBorder()
	{
		if (LevelManager::level->map->size.x * 50 < this->position_.x)
		{
			setPosition(sf::Vector2f(LevelManager::level->map->size.x * 50, getPosition().y));
		}

		if (this->position_.x < 0)
		{
			setPosition(sf::Vector2f(0, getPosition().y));
		}

		if (position_.y > LevelManager::level->map->size.y * 50)
		{
			this->dealDamage(health_);
		}
	}

public:

	// Конструктор для инициализации по умолчанию.
	Player(sf::Vector2f position, sf::Vector2f size, sf::View* view, std::string path_texture) :
		IMoveAble(5.0f),
		IAnimationAble(path_texture, 9, 0.01f, size),
		Entity(position, size),
		GameEntity(100.0f, 50.0f),

		camera_(view, {10, 20}),
		gears_(0)
	{

	}

	// Обвление персонажа, тут происходит логика движения.
	void update(const float time) override
	{
		acceleration_.y += 0.02f;
		speed_.x = 0;

		if (on_ground_ && anim_state_ == AnimationState::Stay)
		{
			acceleration_ = { 0, 0 };
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && on_ground_)
		{
			anim_state_ = AnimationState::Jump;
			speed_.y = -28 / (4 / 2) / 2 + 2 / 4;
		}
		else
		{
			if (on_ground_) anim_state_ = AnimationState::Jump;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			anim_state_ = AnimationState::MoveRight;
			speed_.x = speed_value_;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			anim_state_ = AnimationState::MoveLeft;
			speed_.x = -speed_value_;
		}

		handlingCollisionWithBorder();

		IMoveAble::updateByMove();

		sprite_.setPosition(position_ + SPRITE_POSITION_INACCUARACY);

		changeFrameAnimation(time);

		camera_.updateByPosition(position_);
	}

	// Метод для обработки стокновений.
	void handlingCollision(Block& block) override
	{
		switch (block.getType())
		{
		case BlockType::Ground:
			if (getRect2f().y.y < block.getRect2f().y.y)
			{
				position_.y -= speed_.y;
				on_ground_ = true;
				speed_.y = 0;
				anim_state_ = AnimationState::Stay;
			}
			break;
		case BlockType::Border:
			if (speed_.x > 0)
			{
				position_.x = block.getRect2f().x.x - size_.x + 10;
			}
			else if (speed_.x < 0)
			{
				position_.x = block.getRect2f().y.x - 10;
			}
			break;
		case BlockType::Spikes:
			dealDamage(5);
			break;
		default:
			on_ground_ = false;
			break;
		}
	}

	void handlingCollision(Item& item) override
	{
		if (item.getType() == ItemType::Star)
		{
			LevelManager::setNewLevel();
		}
	}

	void handlingCollision(Enemy& enemy) override
	{
		if (enemy.getRect2f().y.y > this->getRect2f().y.y)
		{
			enemy.dealDamage(damage_);
		}
		else
		{
			this->dealDamage(enemy.getDamageValue());
		}
	}

	void handlingCollision(MovingPlatform& platform) override
	{
		if (getRect2f().y.y <= platform.getRect2f().y.y)
		{
			position_.y -= speed_.y;
			on_ground_ = true;
			speed_.y = 0;
			anim_state_ = AnimationState::Stay;
		}
	}
};