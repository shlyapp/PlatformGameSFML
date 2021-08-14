#pragma once

namespace gui
{

	//////////////////////////////////////////////////////////////
	/// EventType ������� � ���� ���� �������.
	//////////////////////////////////////////////////////////////

	enum class EventType
	{
		Click,
		MouseEnter,
		MouseLeave
	};


	//////////////////////////////////////////////////////////////
	/// IGUIListener - ��������� ��������� �������.
	/// ���� �����-�� ������ �� ��� ������ ����� � ���������
	/// ������� ������������ ��������, �� �� ������ �����������
	/// (����������� ���������) �� ������ ������� - ������� "�����������".
	//////////////////////////////////////////////////////////////

	class IEventListener
	{
	public:
		
		// ����������� ������ ����������� ���� �����.
		virtual void updateByGUIEvent(EventType type) = 0;

	};

	//////////////////////////////////////////////////////////////
	/// ����� Element �������� ������������ ��� ���� �������,
	/// ��������� � ����������� ����������� ����.
	/// ��������� �� sf::Drawable c ����� �������� ����� draw()
	/// ��� ����� �����.
	//////////////////////////////////////////////////////////////

	class Element : public Entity, public sf::Drawable
	{	
	protected:

		// ��� �������� ���������� �������.
		std::list<IEventListener*> listeners_;

		// ��� ���������� ���������� ����.
		sf::RenderWindow* window_;

		// ������ ������� ��������.
		mutable EventType event_;

		// ����������� ��� �������������.
		Element(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) : Entity(position, size),
			window_(window),
			event_(EventType::MouseLeave)
		{

		}

		// ����� ���������� ����������.
		void notifyListeners() const
		{
			// ���������� ���� ����������.
			for (auto listener : listeners_)
			{
				// �������� � ���������� ����� ���������� � �������� �������, ������� ���������.
				listener->updateByGUIEvent(event_);
			}
		}

		// ����� ��� ��������� ��������� ��� ����� �� �������.
		virtual void click() const
		{
			// ���� ��� �� ������.
			if (event_ != EventType::Click)
			{
				// ��������.
				event_ = EventType::Click;
				// ��������� � ����������.
				notifyListeners();
			}
		}

		// ����������.
		virtual void enter() const
		{
			if (event_!= EventType::MouseEnter)
			{
				event_ = EventType::MouseEnter;
				notifyListeners();
			}
		}

		// ����������.
		virtual void leave() const
		{
			if (event_!= EventType::MouseLeave)
			{
				event_ = EventType::MouseLeave;
				notifyListeners();
			}
		}

	public:
		
		// ��������� ���������.
		void addListener(IEventListener* listener)
		{
			listeners_.push_back(listener);
		}

		// ������� ���������.
		void remoteListener(IEventListener* listener)
		{
			listeners_.remove(listener);
		}

		// ����� ���������, � ������� ���������� ��� �� ����������� ���������.
		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			// �������� ���������� ���������� �������.
			sf::Vector2f mousePositon = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

			// ��������� �� ��������� ������� � ������� ��������.
			if (sf::IntRect(position_.x, position_.y, size_.x, size_.y).contains(mousePositon.x, mousePositon.y))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// ���� ����� � ���� ������ ����� ������ ����, �� �������� ��������������� �����, ������� ������ ��������� � ��������� �� ����.
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

	//////////////////////////////////////////////////////////////
	/// TextBlock ���������� ����� � ������������ ���������.
	//////////////////////////////////////////////////////////////

	class TextBlock : public Element
	{
	private:
		
		// ������ � ���� ��� �����: .x - ���� ��� ���������, .y - ������� ����.
		sf::Vector2<sf::Color> colors_;
		// ����� ������.
		sf::Font font_;
		// ��� �����, mutable � ����� �������� ���� � const ������.
		mutable sf::Text text_;

		// ������ ��� ������.
		std::string str_;
		
		// �������� �� ��������������� (����� �� ����� ����� � ���������� ��� ��������).
		bool is_interactive_;

		// ��������� ����� � ���������� �����.
		void loadText()
		{
			font_.loadFromFile("data/CyrilicOld.TTF");

			text_.setPosition(position_);
			text_.setFont(font_);
			text_.setFillColor(colors_.y);
			text_.setString(str_);
			text_.setCharacterSize(30);
		}

		// �������������� ������������ ����� ��� ����, ����� �������� ���� ������.
		void enter() const override
		{
			// �������� ������������ ����� � ����� ���������� ����������.
			Element::enter();

			text_.setFillColor(colors_.x);
		}

		// ����������.
		void leave() const override
		{
			Element::leave();

			text_.setFillColor(colors_.y);
		}

	public:

		// ����������� ��� �������������.
		TextBlock(sf::Vector2f position, std::string text, sf::RenderWindow* window) : Element(position, {0, 0}, window),
			str_(text),
			is_interactive_(true),
			colors_(sf::Vector2<sf::Color>(sf::Color::Red, sf::Color::Green))
		{
			loadText();
			// ������������� ���������� ������ � ����������� �� ������� ������ � ���������� ��������.
			setSize(sf::Vector2f{ float (str_.size() * text_.getCharacterSize()), float (text_.getCharacterSize()) });
		}

		// ������ ��� ��������� ������� ������.
		void setCharacterSize(const float size)
		{
			text_.setCharacterSize(size);
			setSize(sf::Vector2f{ float(str_.size() * text_.getCharacterSize()), float(text_.getCharacterSize()) });
		}

		// ������ ��� ��������� ������ �������, ���� ����������� �� ���������, ����� ��������� �����.
		void setColors(sf::Color color_in_enter, sf::Color color_in_leave)
		{
			colors_ = { color_in_enter, color_in_leave };
		}

		// ��� ���������� ����� ������ � ����������, ���� TextBlock ������ ���� ������ ���������.
		void disableInteractivity()
		{
			is_interactive_ = false;
		}

		// ��� ��������� ���������� ������.
		void setText(const std::string str)
		{
			text_.setString(str);
			setSize(sf::Vector2f{ float(str_.size() * text_.getCharacterSize()), float(text_.getCharacterSize()) });
		}

		// ��������� ������.
		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			// ���� ����� �������������, �� ��������� ��� ������������ draw().
			if (is_interactive_)
			{
				Element::draw(target, animation_state);
			}
			
			// � ����� ������ ������� �� ����� ��� �����.
			target.draw(text_);
		}
	};
}