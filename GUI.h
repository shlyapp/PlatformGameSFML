#pragma once

namespace gui
{

	//////////////////////////////////////////////////////////////
	/// ����� GUIEvent �������� �� ������� � GUI.
	//////////////////////////////////////////////////////////////

	class Event
	{
	public:

		enum EventType
		{
			Click,
			MouseEnter,
			MouseLeave
		};

		mutable EventType type;

	};

	//////////////////////////////////////////////////////////////
	/// IGUIListener - ��������� ��������� �������.
	/// ���� �����-�� ������ �� ��� ������ ����� � ���������
	/// ������� ������������ ��������, �� �� ������ �����������
	/// (����������� ���������) �� ������ �������.
	//////////////////////////////////////////////////////////////

	class IEventListener
	{
	public:

		virtual void updateByGUIEvent(Event type) = 0;

	};

	//////////////////////////////////////////////////////////////
	/// ����� Element �������� ������������ ��� ���� �������,
	/// ��������� � ����������� ����������� ����.
	//////////////////////////////////////////////////////////////

	class Element : public Entity, public sf::Drawable
	{	
	protected:

		std::list<IEventListener*> listeners_;

		sf::RenderWindow* window_;

		Event event_;

		Element(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) : Entity(position, size),
			window_(window)
		{
			event_.type = Event::MouseLeave;
		}

		void notifyListeners() const
		{
			for (auto listener : listeners_)
			{
				listener->updateByGUIEvent(event_);
			}
		}

		virtual void click() const
		{
			if (event_.type != Event::Click)
			{
				event_.type = Event::Click;
				notifyListeners();
			}
		}

		virtual void enter() const
		{
			if (event_.type != Event::MouseEnter)
			{
				event_.type = Event::MouseEnter;
				notifyListeners();
			}
		}

		virtual void leave() const
		{
			if (event_.type != Event::MouseLeave)
			{
				event_.type = Event::MouseLeave;
				notifyListeners();
			}
		}

	public:

		void addListener(IEventListener* listener)
		{
			listeners_.push_back(listener);
		}

		void remoteListener(IEventListener* listener)
		{
			listeners_.remove(listener);
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			sf::Vector2f mousePositon = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

			if (sf::IntRect(position_.x, position_.y, size_.x, size_.y).contains(mousePositon.x, mousePositon.y))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					click();
				}
				else
				{
					enter();
				}
			}
			else
			{
				leave();
			}
		}
	};

	class Button : public Element
	{
	public:

		Button(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) : Element(position, size, window)
		{

		}

	};
}