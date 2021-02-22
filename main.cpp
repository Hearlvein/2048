#include <SFML/Graphics.hpp>
#include <iostream>
#include <cassert>
#include <vector>
#include <ctime>

const int TILE_SIZE = 100;
const int GRID_SIZE = 8;
std::vector<std::vector<int>> grid;	// Stores the power of 2
sf::Sprite cell;
sf::Texture texture;
bool didDown = false;	// for automated play

bool putRandom(const int nbBlocks, const std::vector<int>& powers)
{
	assert(nbBlocks > 0);
	assert(nbBlocks == powers.size());

	int freeLocations = 0;
	for (auto& col : grid)
		freeLocations += std::count(col.begin(), col.end(), 0);
	
	for (int k = 0; k < nbBlocks; ++k)
	{
		if (freeLocations == 0)
			return false;

		int randomCell = rand() % freeLocations;
		int counter = 0;

		for (int i = 0; i < GRID_SIZE; ++i)
		{
			for (int j = 0; j < GRID_SIZE; ++j)
			{
				if (grid[i][j] != 0)
					continue;
				else if (counter++ == randomCell)
					grid[i][j] = powers[k];
			}
		}

		--freeLocations;
	}

	return true;
}

void moveDown()
{
	for (auto& col : grid)
	{
		for (int y = GRID_SIZE - 2; y >= 0; --y)
		{
			for (int z = y; z < GRID_SIZE - 1; ++z)
			{
				int& current = col[z];
				int& under = col[z + 1];

				if (under == 0)
					std::swap(current, under);
				else if (under == current)
				{
					++under;
					current = 0;
				}
			}
		}
	}
}

void moveUp()
{
	for (auto& col : grid)
	{
		for (int y = 1; y < GRID_SIZE; ++y)
		{
			for (int z = y; z > 0; --z)
			{
				int& current = col[z];
				int& above = col[z - 1];

				if (above == 0)
					std::swap(current, above);
				else if (above == current)
				{
					++above;
					current = 0;
				}
			}
		}
	}
}

void moveLeft()
{
	for (int row = 0; row < GRID_SIZE; ++row)
	{
		for (int x = 1; x < GRID_SIZE; ++x)
		{
			for (int z = x; z > 0; --z)
			{
				int& current = grid[z][row];
				int& left = grid[z - 1][row];

				if (left == 0)
					std::swap(current, left);
				else if (left == current)
				{
					++left;
					current = 0;
				}
			}
		}
	}
}

void moveRight()
{
	for (int row = GRID_SIZE - 1; row >= 0; --row)
	{
		for (int x = GRID_SIZE - 2; x >= 0; --x)
		{
			for (int z = x; z < GRID_SIZE - 1; ++z)
			{
				int& current = grid[z][row];
				int& right = grid[z + 1][row];

				if (right == 0)
					std::swap(current, right);
				else if (right == current)
				{
					++right;
					current = 0;
				}
			}
		}
	}
}

void printConsole()
{
	for (int j = 0; j < GRID_SIZE; ++j)
	{
		for (int i = 0; i < GRID_SIZE; ++i)
			std::cout << grid[i][j];
		std::cout << '\n';
	}
	std::cout << "----------------------------------------\n";
}

int main()
{
	using namespace std;
	srand(static_cast<unsigned int>(time(0)));

	// Initialization
	sf::RenderWindow window(sf::VideoMode(TILE_SIZE * GRID_SIZE, TILE_SIZE * GRID_SIZE), "two thousands fourty eight");
	//window.setVerticalSyncEnabled(true);
	texture.loadFromFile("tiles.png");
	cell.setTexture(texture);

	grid.resize(GRID_SIZE);
	for (auto& col : grid)
		col.resize(GRID_SIZE);

	// Fills the grid
	putRandom(2, std::vector<int>(2, 1));

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			else if (event.type == sf::Event::KeyPressed)
			{
				bool played = true;

				if		(event.key.code == sf::Keyboard::Down)	moveDown();
				else if (event.key.code == sf::Keyboard::Up)	moveUp();
				else if (event.key.code == sf::Keyboard::Left)	moveLeft();
				else if (event.key.code == sf::Keyboard::Right)	moveRight();
				else											played = false;

				if (played)
				{
					if (!putRandom(1, { rand() % 2 }))
						std::cout << "The Game is Up!" << std::endl;
				}
			}
				
		}

		if (didDown)
		{
			moveRight();
			putRandom(1, { rand() % 2 });
			didDown = false;
		}
		else
		{
			moveDown();
			putRandom(1, { rand() % 2 });
			didDown = true;
		}

		window.clear();

		// Draw
		for (int i = 0; i < GRID_SIZE; ++i)
		{
			for (int j = 0; j < GRID_SIZE; ++j)
			{
				if (int power = grid[i][j]; power != 0)
				{
					cell.setPosition((float)i * TILE_SIZE, (float)j * TILE_SIZE);
					int xTextureCoords = power * TILE_SIZE;
					cell.setTextureRect(sf::IntRect(xTextureCoords, 0, TILE_SIZE, TILE_SIZE));
					window.draw(cell);
				}
			}
		}

		window.display();
	}

	return 0;
}