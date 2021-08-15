#pragma once
template<typename _Value_type_0, typename _Value_type_1>
inline bool collision_square_position(sf::Vector2<sf::Vector2<_Value_type_0> > vsq0, sf::Vector2<sf::Vector2<_Value_type_1> > vsq1)
{

	//�������� ������� ����
	if (vsq1.x.x <= vsq0.y.x)
	{
		if (vsq1.x.x >= vsq0.x.x)
		{
			if (vsq1.x.y <= vsq0.y.y)
			{
				if (vsq1.x.y >= vsq0.x.y)
				{
					return true;
				}
			}
		}
	}

	//�������� ������� ����
	if (vsq1.y.x <= vsq0.y.x)
	{
		if (vsq1.y.x >= vsq0.x.x)
		{
			if (vsq1.y.y <= vsq0.y.y)
			{
				if (vsq1.y.y >= vsq0.x.y)
				{
					return true;
				}
			}
		}
	}
	//�������� �������� ����
	if (vsq1.x.x + (vsq1.y.x - vsq1.x.x) <= vsq0.y.x)
	{
		if (vsq1.x.x + (vsq1.y.x - vsq1.x.x) >= vsq0.x.x)
		{
			if (vsq1.x.y <= vsq0.y.y)
			{
				if (vsq1.x.y >= vsq0.x.y)
				{
					return true;
				}
			}
		}
	}
	//�������� ��������� ����
	if (vsq1.x.x <= vsq0.y.x)
	{
		if (vsq1.x.x >= vsq0.x.x)
		{
			if (vsq1.x.y + (vsq1.y.y - vsq1.x.y) <= vsq0.y.y)
			{
				if (vsq1.x.y + (vsq1.y.y - vsq1.x.y) >= vsq0.x.y)
				{
					return true;
				}
			}
		}
	}

	//�������� ������� ����
	if (vsq0.x.x <= vsq1.y.x)
	{
		if (vsq0.x.x >= vsq1.x.x)
		{
			if (vsq0.x.y <= vsq1.y.y)
			{
				if (vsq0.x.y >= vsq1.x.y)
				{
					return true;
				}
			}
		}
	}

	//�������� ������� ����
	if (vsq0.y.x <= vsq1.y.x)
	{
		if (vsq0.y.x >= vsq1.x.x)
		{
			if (vsq0.y.y <= vsq1.y.y)
			{
				if (vsq0.y.y >= vsq1.x.y)
				{
					return true;
				}
			}
		}
	}
	//�������� �������� ����
	if (vsq0.x.x + (vsq0.y.x - vsq0.x.x) <= vsq1.y.x)
	{
		if (vsq0.x.x + (vsq0.y.x - vsq0.x.x) >= vsq1.x.x)
		{
			if (vsq0.x.y <= vsq1.y.y)
			{
				if (vsq0.x.y >= vsq1.x.y)
				{
					return true;
				}
			}
		}
	}
	//�������� ��������� ����
	if (vsq0.x.x <= vsq1.y.x)
	{
		if (vsq0.x.x >= vsq1.x.x)
		{
			if (vsq0.x.y + (vsq0.y.y - vsq0.x.y) <= vsq1.y.y)
			{
				if (vsq0.x.y + (vsq0.y.y - vsq0.x.y) >= vsq1.x.y)
				{
					return true;
				}
			}
		}
	}

	//�� ������� - ���������� false
	return false;
}

template<typename _Value_type_0, typename _Value_type_1>
inline bool operator & (sf::Vector2<sf::Vector2<_Value_type_0> > vsq0, sf::Vector2<sf::Vector2<_Value_type_1> > vsq1)
{
	return collision_square_position(vsq0, vsq1);
}