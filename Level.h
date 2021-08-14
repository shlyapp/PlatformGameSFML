#pragma once

enum class ItemType
{
	Gear,
	Star,
	Chip
};

enum class BlockType
{
	Ground,
	Spikes,
	Border,
	Simple
};

class Item : public Entity
{
private:

	ItemType type_;

public:

	Item(sf::Vector2f position, sf::Vector2f size, ItemType type) : Entity(position, size),
		type_(type)
	{

	}

	ItemType getType() const
	{
		return type_;
	}

};

class Block : public Entity
{
private:

	BlockType type_;

public:

	Block(sf::Vector2f position, sf::Vector2f size, BlockType type) : Entity(position, size),
		type_(type)
	{

	}

	BlockType getType() const
	{
		return type_;
	}

};

class Map : public sf::Drawable
{
private:

	sf::Texture texture_;
	sf::Sprite* sprite_;

	void loadTexture(const std::string path_texture)
	{
		texture_.loadFromFile(path_texture);
		sprite_->setTexture(texture_);
	}

	void loadMap(const std::string path_map)
	{
		std::fstream file(path_map);

		if (file.is_open())
		{
			for (int i = 0; i < size.y; i++)
			{
				std::string line;
				getline(file, line);
				tileMap.push_back(line);
			}
		}
	}

	void createObjects()
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
				sf::Vector2f position = { x * 50.0f, y * 50.0f };
				sf::Vector2f size = { 50.0f, 50.0f };

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

	std::vector<std::string> tileMap;
	std::list<Item*> items;
	std::list<Block*> blocks;

	const sf::Vector2f size;

	Map(sf::Vector2f size_map, std::string path_textures, std::string path_map) :
		sprite_(new sf::Sprite()),
		size(size_map)
	{
		loadTexture(path_textures);
		loadMap(path_map);
		createObjects();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates animation_state) const override
	{
		for (int y = 0; y < size.y; y++)
		{
			for (int x = 0; x < size.x; x++)
			{
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

				sprite_->setPosition(x * 50, y * 50);
				target.draw(*sprite_);

			}
		}
	}

};
