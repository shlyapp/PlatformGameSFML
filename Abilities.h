#pragma once

//////////////////////////////////////////////////////////////
/// Следующие 5 интерфейсов отвечают за столкновения с каким-либо
/// обьектом. Если класс подразумевается под собой столкновение,
/// например, с блоком, он должен унаследовать ICollisionWithBlockAble
/// и реализовать метод handlingCollision(Block& block).
//////////////////////////////////////////////////////////////
class ICollisionWithBlockAble
{
protected:

	ICollisionWithBlockAble()
	{

	}

	virtual void handlingCollision(Block& block) = 0;

};

// Аналогично
class ICollisionWithItemAble
{
protected:

	ICollisionWithItemAble()
	{

	}

	virtual void handlingCollision(Item& item) = 0;

};

// Аналогично.
class ICollisionWithEnemyAble
{
protected:

	ICollisionWithEnemyAble()
	{

	}

	virtual void handlingCollision(Enemy& enemy) = 0;

}; 

// Аналогично.
class ICollisionWithPlatformAble
{
protected:

	ICollisionWithPlatformAble()
	{

	}

	virtual void handlingCollision(MovingPlatform& platform) = 0;

};

//////////////////////////////////////////////////////////////
/// Интерфейс IMoveAble отвечает за передвижение. Если что-то
/// должно двигаться в игре, то оно реализует данный интерфейс.
//////////////////////////////////////////////////////////////
class IMoveAble : virtual public Position
{
protected:

	// Скорость и ускорение по осям.
	sf::Vector2f speed_;
	sf::Vector2f acceleration_;

	float speed_value_;

	// Конструктор с инициализацией по умолчанию.
	IMoveAble(float speed) :
		speed_({0.0f, 0.0f}),
		acceleration_({0.0f, 0.0f}),
		speed_value_(speed)
	{

	}

	// Метод для обновления позиции.
	void updateByMove()
	{
		speed_ += acceleration_;
		position_ += speed_;
	}

	// Сеттер для скорости.
	void setSpeed(const float speed)
	{
		speed_value_ = speed;
	}
};

//////////////////////////////////////////////////////////////
/// Интерфейс IAnimationAble отвечает за анимацию и предоставляет
/// готовый метод для анимации. Если что-то должно анимироваться, то
/// следует просто унаследовать IAnimationAble.
//////////////////////////////////////////////////////////////
class IAnimationAble : public sf::Drawable
{
private:

	// Текуший кадр.
	float current_frame_;
	// Скорость анимации.
	float speed_anim_;
	// Количество кадров.
	int frames_;

	// Текстура, спрайт, размер.
	sf::Texture texture_;
	sf::Vector2f size_sprite_;

	// Направление взгляда.
	enum ViewDirectionState
	{
		Left,
		Right
	};

	ViewDirectionState view_state_;

	// Загрузка текстур.
	void loadFiles(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_.setTexture(texture_);
	}

protected:
	
	// Состояние анимации.
	enum AnimationState
	{
		MoveLeft,
		MoveRight,
		Stay,
		Jump
	};

	AnimationState anim_state_;
	sf::Sprite sprite_;

	// Конструктор для инициализации.
	IAnimationAble(std::string path_texture, int frames, float speed_anim, sf::Vector2f size) : 
		speed_anim_(speed_anim),
		frames_(frames),
		size_sprite_(size),
		current_frame_(0),
		anim_state_(AnimationState::Stay),
		view_state_(ViewDirectionState::Right)
	{
		loadFiles(path_texture);
	}

	// Метод смены кадров анимации.
	virtual void changeFrameAnimation(float time)
	{
		// Проверяем на предел.
		if (current_frame_ > frames_ - 1)
		{
			current_frame_ -= frames_ - 1;
		}

		// В зависимости от состояния, выводим нужную анимацию.
		switch (anim_state_)
		{
		case AnimationState::MoveRight:
			sprite_.setTextureRect(sf::IntRect(size_sprite_.x * int(current_frame_), 0, size_sprite_.x, size_sprite_.y));
			view_state_ = ViewDirectionState::Left;
			break;
		case AnimationState::MoveLeft:
			sprite_.setTextureRect(sf::IntRect(size_sprite_.x * int(current_frame_), size_sprite_.y + 1, size_sprite_.x, size_sprite_.y));
			view_state_ = ViewDirectionState::Right;
			break;
		default:
			if (view_state_ == ViewDirectionState::Right)
			{
				sprite_.setTextureRect(sf::IntRect(0, 0, size_sprite_.x, size_sprite_.y));
			}
			else
			{
				sprite_.setTextureRect(sf::IntRect(size_sprite_.x, size_sprite_.y + 1, size_sprite_.x, size_sprite_.y));
			}
			break;
		}

		// Увеличиваем кадр.
		current_frame_ += time * speed_anim_;

	}

	// Отрисовываем спрайт.
	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		target.draw(sprite_);
	}

	// Сеттер количества кадров.
	void setFrames(const int frames)
	{
		frames_ = frames;
	}

	// Сеттер скорости анимации.
	void setSpeedAnimation(const float speed)
	{
		speed_anim_ = speed;
	}
};
