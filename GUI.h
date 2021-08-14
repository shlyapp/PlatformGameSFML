#pragma once

namespace gui
{

	//////////////////////////////////////////////////////////////
	/// EventType хранить в себе типы событий.
	//////////////////////////////////////////////////////////////

	enum class EventType
	{
		Click,
		MouseEnter,
		MouseLeave
	};


	//////////////////////////////////////////////////////////////
	/// IGUIListener - интерфейс слушателя события.
	/// Если какой-то обьект из вне желает знать о изменении
	/// события графического элемента, то он должен подписаться
	/// (реализовать интерфейс) на нужный элемент.
	//////////////////////////////////////////////////////////////

	class IEventListener
	{
	public:

		virtual void updateByGUIEvent(EventType type) = 0;

	};

	//////////////////////////////////////////////////////////////
	/// Класс Element является родительским для всех классов,
	/// связанных с графическим интерфейсом игры.
	//////////////////////////////////////////////////////////////

	class Element : public Entity, public sf::Drawable
	{	
	protected:

		std::list<IEventListener*> listeners_;

		sf::RenderWindow* window_;

		mutable EventType event_;

		Element(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) : Entity(position, size),
			window_(window),
			event_(EventType::MouseLeave)
		{

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
			if (event_ != EventType::Click)
			{
				event_ = EventType::Click;
				notifyListeners();
			}
		}

		virtual void enter() const
		{
			if (event_!= EventType::MouseEnter)
			{
				event_ = EventType::MouseEnter;
				notifyListeners();
			}
		}

		virtual void leave() const
		{
			if (event_!= EventType::MouseLeave)
			{
				event_ = EventType::MouseLeave;
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

	class TextBlock : public Element
	{
	private:

		sf::Vector2<sf::Color> colors_;
		sf::Font font_;
		mutable sf::Text text_;

		std::string str_;
		
		bool enabled_;

		void loadText()
		{
			font_.loadFromFile("data/CyrilicOld.TTF");

			text_.setPosition(position_);
			text_.setFont(font_);
			text_.setFillColor(colors_.y);
			text_.setString(str_);
			text_.setCharacterSize(30);
		}

		void enter() const override
		{
			Element::enter();

			text_.setFillColor(colors_.x);
		}

		void leave() const override
		{
			Element::leave();

			text_.setFillColor(colors_.y);
		}

	public:

		TextBlock(sf::Vector2f position, std::string text, sf::RenderWindow* window) : Element(position, {0, 0}, window),
			str_(text),
			enabled_(true),
			colors_(sf::Vector2<sf::Color>(sf::Color::Red, sf::Color::Green))
		{
			loadText();
			setSize(sf::Vector2f{ float (str_.size() * text_.getCharacterSize()), float (text_.getCharacterSize()) });
		}

		void setCharacterSize(const float size)
		{
			text_.setCharacterSize(size);
			setSize(sf::Vector2f{ float(str_.size() * text_.getCharacterSize()), float(text_.getCharacterSize()) });
		}

		void setColors(sf::Color color_in_enter, sf::Color color_in_leave)
		{
			colors_ = { color_in_enter, color_in_leave };
		}

		void disableText()
		{
			enabled_ = false;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			if (enabled_)
			{
				Element::draw(target, animation_state);
			}
			
			target.draw(text_);
		}
	};
}