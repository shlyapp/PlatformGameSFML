#pragma once

class ICollisionWithBlockAble
{
protected:

	ICollisionWithBlockAble()
	{

	}

	virtual void handlingCollision(Block& block) = 0;

};

class ICollisionWithItemAble
{
protected:

	ICollisionWithItemAble()
	{

	}

	virtual void handlingCollision(Item& item) = 0;

};

class ICollisionWithEnemyAble
{
protected:

	ICollisionWithEnemyAble()
	{

	}

	virtual void handlingCollision(Enemy& enemy) = 0;

};

class ICollisionWithPlatformAble
{
protected:

	ICollisionWithPlatformAble()
	{

	}

	virtual void handlingCollision(MovingPlatform& platform) = 0;

};

class IMoveAble : virtual public Position
{
protected:

	sf::Vector2f speed_;
	sf::Vector2f acceleration_;

	float speed_value_;

	IMoveAble(float speed) :
		speed_({0.0f, 0.0f}),
		acceleration_({0.0f, 0.0f}),
		speed_value_(speed)
	{

	}

	void updateByMove()
	{
		speed_ += acceleration_;
		position_ += speed_;
	}
};

class IAnimationAble : public sf::Drawable
{
private:

	float current_frame_;
	float speed_anim_;
	int frames_;

	sf::Texture texture_;
	sf::Sprite sprite_;
	sf::Vector2f size_;

	enum ViewDirectionState
	{
		Left,
		Right
	};

	enum AnimationState
	{
		MoveLeft,
		MoveRight,
		Stay,
		Jump
	};

	ViewDirectionState view_state_;
	AnimationState anim_state_;

	void loadFiles(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_.setTexture(texture_);
	}

protected:
	
	IAnimationAble(std::string path_texture, int frames, float speed_anim, sf::Vector2f size) : 
		speed_anim_(speed_anim),
		frames_(frames),
		size_(size),
		current_frame_(0),
		anim_state_(AnimationState::Stay),
		view_state_(ViewDirectionState::Right)
	{
		loadFiles(path_texture);
	}

	virtual void changeFrameAnimation(float time)
	{
		if (current_frame_ > frames_ - 1)
		{
			current_frame_ -= frames_ - 1;
		}

		switch (anim_state_)
		{
		case AnimationState::MoveLeft:
			sprite_.setTextureRect(sf::IntRect(size_.x * int(current_frame_), 0, size_.x, size_.y));
			view_state_ = ViewDirectionState::Left;
			break;
		case AnimationState::MoveRight:
			sprite_.setTextureRect(sf::IntRect(size_.x * int(current_frame_), size_.y + 1, size_.x, size_.y));
			view_state_ = ViewDirectionState::Right;
			break;
		default:
			if (view_state_ == ViewDirectionState::Right)
			{
				sprite_.setTextureRect(sf::IntRect(0, 0, size_.x, size_.y));
			}
			else
			{
				sprite_.setTextureRect(sf::IntRect(size_.x, size_.y + 1, size_.x, size_.y));
			}
			break;
		}

		current_frame_ += time * speed_anim_;

	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		target.draw(sprite_);
	}
};