#pragma once

class Enemy;
class MovingPlatform;

// Храним типы предметов в игре.
enum class ItemType
{
	Gear,
	Star,
	Chip
};

// Храним типы блоков в игре.
enum class BlockType
{
	Ground,
	Spikes,
	Border,
	Simple
};

//////////////////////////////////////////////////////////////
/// Класс Item наследует от Entity для того, чтобы хранить
/// размер и позицию. Так же этот класс хранит тип предмета.
//////////////////////////////////////////////////////////////
class Item : public Entity
{
private:

	ItemType type;

public:

	Item(sf::Vector2f position, sf::Vector2f size, ItemType type) : Entity(position, size),
		type(type)
	{

	}

	ItemType getType() const
	{
		return type;
	}

};

//////////////////////////////////////////////////////////////
/// Аналогично классу Item.
//////////////////////////////////////////////////////////////
class Block : public Entity
{
private:

	BlockType type;

public:

	Block(sf::Vector2f position, sf::Vector2f size, BlockType type) : Entity(position, size),
		type(type)
	{

	}

	BlockType getType() const
	{
		return type;
	}

};

//////////////////////////////////////////////////////////////
/// Класс Map хранит в себе все, что связанно с картой: размеры,
/// спрайт, блоки, предметы, саму карту и занимается отрисовкой.
//////////////////////////////////////////////////////////////
class Map : public sf::Drawable
{
private:

	sf::Texture texture_;
	sf::Sprite* sprite_;

	// Карта.
	std::vector<std::string> tileMap;

	// Загружаем и устанавливаем текстуру.
	void loadTexture(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_->setTexture(texture_);
	}

	// Читаем из файла карту.
	void loadMap(const std::string path_map)
	{
		// Создаем обьект файл.
		std::fstream file(path_map);

		// Если файл открыт.
		if (file.is_open())
		{
			// Читаем строки, как указано в размере.
			for (int i = 0; i < size.y; i++)
			{
				std::string line;
				getline(file, line);
				// Добавляем строку к остальным.
				tileMap.push_back(line);
			}
		}

		// Закрываем файл.
		file.close();
	}

	// Метод создания обьектов: блоков и предметов.
	void createObjects()
	{
		// Два цикла перебирают всю карту.
		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				// Задаем текущую позицию и размерп блока.
				sf::Vector2f position = { x * 50.0f, y * 50.0f };
				sf::Vector2f size = { 50.0f, 50.0f };

				// В зависимости от элемента создаем нужный обьект.
				switch (tileMap[y][x])
				{
				case 't':
					blocks.push_back(new Block(position, size, BlockType::Ground));
					blocks.push_back(new Block(sf::Vector2f{ x * 50.0f, y * 50.0f + 1 }, sf::Vector2f{ 50.0f, 48.0f }, BlockType::Border));
					blocks.push_back(new Block(sf::Vector2f{ x * 50.0f, y * 50.0f + 49 }, sf::Vector2f{ 50.0f, 1.0f }, BlockType::Ground));
					break;
				case 'p':
					blocks.emplace_back(new Block(position, size, BlockType::Spikes));
					break;
				case 'q':
					blocks.emplace_back(new Block(position, size, BlockType::Border));
					break;
				case '1':
					items.emplace_back(new Item(position, size, ItemType::Star));
					break;
				case 'l':
					items.emplace_back(new Item(position, size, ItemType::Chip));
					break;
				case 'a':
					items.emplace_back(new Item(position, size, ItemType::Gear));
					break;
				}
			}
		}
	}

public:

	// Листы предметов и блоков.
	std::list<Item*> items;
	std::list<Block*> blocks;

	// Размер карты.
	const sf::Vector2f size;

	// Конструткор для создания карты.
	Map(sf::Vector2f size_map, std::string path_textures, std::string path_map) :
		sprite_(new sf::Sprite()),
		size(size_map)
	{
		loadTexture(path_textures);
		loadMap(path_map);
		createObjects();
	}

	// Метод отрисовки карты.
	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		// Проходим по каждому элементу.
		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				// В зависимости от элемента обрезаем текстуру.
				switch (tileMap[y][x])
				{
				case ' ':
					sprite_->setTextureRect(sf::IntRect(0, 0, 50, 50));
					break;
				case 't':
					sprite_->setTextureRect(sf::IntRect(50, 0, 50, 50));
					break;
				case 'o':
					sprite_->setTextureRect(sf::IntRect(200, 0, 50, 50));
					break;
				case 's':
					sprite_->setTextureRect(sf::IntRect(150, 0, 50, 50));
					break;
				case 'a':
					sprite_->setTextureRect(sf::IntRect(300, 0, 50, 50));
					break;
				case 'd':
					sprite_->setTextureRect(sf::IntRect(350, 0, 50, 50));
					break;
				case 'l':
					sprite_->setTextureRect(sf::IntRect(400, 0, 50, 50));
					break;
				case 'q':
					sprite_->setTextureRect(sf::IntRect(250, 0, 50, 50));
					break;
				case 'y':
					sprite_->setTextureRect(sf::IntRect(450, 0, 50, 50));
					break;
				case 'p':
					sprite_->setTextureRect(sf::IntRect(500, 0, 50, 50));
					break;
				case '0':
					sprite_->setTextureRect(sf::IntRect(550, 0, 50, 50));
					break;
				case '1':
					sprite_->setTextureRect(sf::IntRect(600, 0, 50, 50));
					break;
				case '2':
					sprite_->setTextureRect(sf::IntRect(100, 0, 50, 50));
					break;
				}

				// Устанавливаем текстуру в нужную позицию.
				sprite_->setPosition(x * 50, y * 50);
				// Выводим на экран.
				target.draw(*sprite_);

			}
		}
	}

};

//////////////////////////////////////////////////////////////
/// Level описывает уровень в игре: он хранит в себе саму карту,
/// стартовую позицию игрока, врагов и движущие платформы.
//////////////////////////////////////////////////////////////
class Level
{
public:

	Map* map;

	// Стартовая позиция.
	sf::Vector2f start_position;

	// Враги и платформы на карте.
	std::list<Enemy*> enemys;
	std::list<MovingPlatform*> moving_platforms;

	// Конструктор для создания.
	Level(Map* map_level, sf::Vector2f start_position_player) :
		map(map_level),
		start_position(start_position_player)
	{

	}

	// Враги и платформы могут быть, а могут и не быть, поэтиому добавляем сеттер для них.
	void setEnemys(std::list<Enemy*> enemys)
	{
		this->enemys = enemys;
	}

	void setMovingPlatform(std::list<MovingPlatform*> moving_platforms)
	{
		this->moving_platforms = moving_platforms;
	}
};

//////////////////////////////////////////////////////////////
/// LevelManager отвечает за текуший уровень, список всех
/// уровней.
//////////////////////////////////////////////////////////////
class LevelManager
{
private:

	// Внутренний класс, который будет оповещать игру о событиях с уровнями.
	static inline class Notify : IGameEventMaker
	{
	public:

		void levelsOver()
		{
			// Если закончились уровни, то оповещаем игру о завершении.
			notifyListeners(GameEventState::GameOver);
		}

		void loadNewLevel()
		{
			// Если поставили новый уровень, то оповещаем игру о смене уровня (она уже сама решает, что делать с этой информацией).
			notifyListeners(GameEventState::SetNewLevel);
		}

	};

	// Текущий номер уровня.
	inline static int num_level_ = -1;

	// Вектор, который хранит все уровни.
	inline static std::vector<Level*> levels_;

	// Обьект класса оповещения.
	inline static Notify nt;

	LevelManager()
	{

	}

public:

	// Текущий уровень игры.
	static inline Level* level;

	// Метод добавления нового уровня.
	static void addLevel(Level* level)
	{
		levels_.push_back(level);
	}

	// Устанавливаем уровень.
	static void setNewLevel()
	{
		num_level_++;

		if (num_level_ <= levels_.size() - 1)
		{
			level = levels_[num_level_];
			// Оповещаем о новом уровне.
			nt.loadNewLevel();
			return;
		}
		
		// Оповещаем о том, что уровни закончились.
		nt.levelsOver();
	}

};