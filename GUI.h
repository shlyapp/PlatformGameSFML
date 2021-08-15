#pragma once

namespace gui
{
	// Обьявляем, чтобы использовать в IEventListener
	class Element;

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
	/// (реализовать интерфейс) на нужный элемент - паттерн "Наблюдатель".
	//////////////////////////////////////////////////////////////
	class IEventListener
	{
	public:
		
		// Наблюдатель обязан реализовать этот метод.
		virtual void updateByGUIEvent(EventType type, const Element* element) = 0;

	};

	//////////////////////////////////////////////////////////////
	/// Класс Element является родительским для всех классов,
	/// связанных с графическим интерфейсом игры.
	/// Наследуем от sf::Drawable c целью получить метод draw()
	/// для своих целей.
	//////////////////////////////////////////////////////////////
	class Element : public Entity, public sf::Drawable
	{	
	protected:

		// Задержка для нажатия.
		inline static sf::Clock clock_;

		// Время нажатия.
		mutable unsigned long long int time_;

		// Для хранения слушателей событий.
		std::list<IEventListener*> listeners_;

		// Для нахождения координаты мыши.
		sf::RenderWindow* window_;

		// Хранит событие элемента.
		mutable EventType event_;

		// Конструктор для инициализации.
		Element(sf::Vector2f position, sf::Vector2f size, sf::RenderWindow* window) : Entity(position, size),
			window_(window),
			event_(EventType::MouseLeave),
			time_(0)
		{

		}

		// Метод оповещения слушателей.
		void notifyListeners() const
		{
			// Перебираем всех слушателей.
			for (auto listener : listeners_)
			{
				// Вызываем у слушателей метод обновления и передаем событие, которое произошло.
				listener->updateByGUIEvent(event_, this);
			}
		}

		// Метод для изменения состояния при клике на элемент.
		virtual void click() const
		{
			// Если еще не нажата.
			if (event_ != EventType::Click && clock_.getElapsedTime().asMilliseconds() > time_ + 300)
			{
				// Нажимаем.
				event_ = EventType::Click;
				// Оповещаем о изменениях.
				notifyListeners();

				// Обновляем время нажатия.
				time_ = clock_.getElapsedTime().asMilliseconds();
			}
		}

		// Аналогично.
		virtual void enter() const
		{
			if (event_!= EventType::MouseEnter)
			{
				event_ = EventType::MouseEnter;
				notifyListeners();
			}
		}

		// Аналогично.
		virtual void leave() const
		{
			if (event_!= EventType::MouseLeave)
			{
				event_ = EventType::MouseLeave;
				notifyListeners();
			}
		}

	public:
		
		// Добавляем слушателя.
		void addListener(IEventListener* listener)
		{
			listeners_.push_back(listener);
		}

		// Удаляем слушателя.
		void remoteListener(IEventListener* listener)
		{
			listeners_.remove(listener);
		}

		// Метод отрисовки, в котором происходит так же обновлением состояния.
		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{
			// Получаем корректное положением курсора.
			sf::Vector2f mousePositon = window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

			// Проверяем на вхождение курсора в область элемента.
			if (sf::IntRect(position_.x, position_.y, size_.x, size_.y).contains(mousePositon.x, mousePositon.y))
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					// Если вошел и была нажата левая кнопка мыши, то вызываем соответствующий метод, который меняет состояния и оповещает об этом.
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
	/// TextBlock отображает текст и обрабатывает состояние.
	//////////////////////////////////////////////////////////////
	class TextBlock : public Element
	{
	private:
		
		// Хранит в себе два цвета: .x - цвет при наведении, .y - обычный цвет.
		sf::Vector2<sf::Color> colors_;
		// Шрифт текста.
		sf::Font font_;
		// Сам текст, mutable с целью изменять цвет в const методе.
		mutable sf::Text text_;

		// Строка для текста.
		std::string str_;
		
		// Отвечает за интерактивность (будет ли смена цвета и оповещение при событиях).
		bool is_interactive_;

		// Загружаем шрифт и уставливем текст.
		void loadText()
		{
			font_.loadFromFile("data/CyrilicOld.TTF");

			text_.setPosition(position_);
			text_.setFont(font_);
			text_.setFillColor(colors_.y);
			text_.setString(str_);
			text_.setCharacterSize(30);
		}

		// Переопределяем родительский метод для того, чтобы изменять цвет текста.
		void enter() const override
		{
			// Вызываем родительский метод с целью оповещения слушателей.
			Element::enter();

			text_.setFillColor(colors_.x);
		}

		// Аналогично.
		void leave() const override
		{
			Element::leave();

			text_.setFillColor(colors_.y);
		}

	public:

		// Конструктор для инициализации.
		TextBlock(sf::Vector2f position, std::string text, sf::RenderWindow* window) : Element(position, {0, 0}, window),
			str_(text),
			is_interactive_(true),
			colors_(sf::Vector2<sf::Color>(sf::Color::Red, sf::Color::Green))
		{
			loadText();
			// Устанавливаем корректный размер в зависимости от размера шрифта и количества символов.
			setSize(sf::Vector2f{ text_.getGlobalBounds().width, text_.getGlobalBounds().height});
		}

		// Сеттер для изменения размера шрифта.
		void setCharacterSize(const float size)
		{
			text_.setCharacterSize(size);
			setSize(sf::Vector2f{ text_.getGlobalBounds().width, text_.getGlobalBounds().height });
		}

		// Сеттер для изменения цветов событий, если стандартные не устривают, можно поставить любые.
		void setColors(sf::Color color_in_enter, sf::Color color_in_leave)
		{
			colors_ = { color_in_enter, color_in_leave };
		}

		// Для выключения смены цветов и оповещения, если TextBlock должен быть просто статичным.
		void disableInteractivity()
		{
			is_interactive_ = false;
		}

		// Для изменения начального текста.
		void setText(const std::string str)
		{
			text_.setString(str);
			setSize(sf::Vector2f{ float(str_.size() * text_.getCharacterSize()), float(text_.getCharacterSize()) });
		}

		// Переопределяем для корректной установки позиции.
		void setPosition(const sf::Vector2f position) override
		{
			position_ = position;
			text_.setPosition(position);
		}

		// Отрисовка текста.
		void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
		{

			// Если текст интерактивный, то обновляем его родительским draw().
			if (is_interactive_)
			{
				Element::draw(target, animation_state);
			}
			
			// В любом случае выводим на экран сам текст.
			target.draw(text_);
		}
	};
}