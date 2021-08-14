#pragma once

//////////////////////////////////////////////////////////////
/// ����� Position ������ ������� � ��������� �� ��������.
//////////////////////////////////////////////////////////////

class Position
{
protected:

	sf::Vector2f position_;

public:

	virtual void setPosition(sf::Vector2f const position)
	{
		position_ = position;
	}

	const sf::Vector2f& getPosition() const
	{
		return position_;
	}
};

//////////////////////////////////////////////////////////////
/// ����� Entity �������� ������������ ��� ���������� ������
/// �������, �� ������ �������� �������������� ��������.
//////////////////////////////////////////////////////////////

class Entity : virtual public Position
{
protected:

	sf::Vector2f size_;

	Entity()
	{

	}

	Entity(sf::Vector2f position)
	{
		position_ = position;
	}

	Entity(sf::Vector2f position, sf::Vector2f size)
	{
		position_ = position;
		size_ = size;
	}

public:

	void setSize(const sf::Vector2f size)
	{
		size_ = size;
	}

	sf::Vector2f getSize() const
	{
		return size_;
	}

	sf::Vector2<sf::Vector2<float>> getRect2f() const
	{
		return { position_, position_ + size_ };
	}

};
