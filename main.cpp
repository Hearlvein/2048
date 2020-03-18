#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

int grid[8][8];
bool playing = false;
RenderWindow window(VideoMode(800, 800), "2048");
bool gridModified = true;
const float tileSize = 100.0f;
Texture texture;
map<int, Sprite> tiles;
bool gameover = false;

void printGrid()
{
	for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
            cout << grid[x][y] << " ";
        cout << "\n";
    }
    cout << "---------------------------------" << endl;
}

void swipeUp()
{
	for (auto& col : grid)
        for (int i = 1; i < 8; ++i)
        {
        	int j = i;
        	while (j - 1 >= 0 && (col[j - 1] == 0 || col[j - 1] == col[i])) j--;

        	if (j != i)
            {
                col[j] += col[i];
                col[i] = 0;
            }
        }
}

void swipeDown()
{
	for (auto& col : grid)
        for (int i = 6; i >= 0; --i)
        {
        	int j = i;
        	while (j + 1 < 8 && (col[j + 1] == 0 || col[j + 1] == col[i])) j++;

        	if (j != i)
            {
                col[j] += col[i];
                col[i] = 0;
            }
        }
}

void swipeLeft()
{
	for (int rowNumber = 0; rowNumber < 8; ++rowNumber)
    {
        int* row[8];
        for (int k = 0; k < 8; ++k)
            row[k] = &grid[k][rowNumber];

        for (int i = 1; i < 8; ++i)
        {
            int j = i;
            while (j - 1 >= 0 && (*row[j - 1] == 0 || *row[j - 1] == *row[i])) j--;

            if (j != i)
            {
                *row[j] += *row[i];
                *row[i] = 0;
            }
        }
    }
}

void swipeRight()
{
	for (int rowNumber = 0; rowNumber < 8; ++rowNumber)
    {
        int* row[8];
        for (int k = 0; k < 8; ++k)
            row[k] = &grid[k][rowNumber];

        for (int i = 6; i >= 0; --i)
        {
            int j = i;
            while (j + 1 < 8 && (*row[j + 1] == 0 || *row[j + 1] == *row[i])) j++;

            if (j != i)
            {
                *row[j] += *row[i];
                *row[i] = 0;
            }
        }
    }
}

void addRandom()
{
	int nbFreeCells = 0;
	for (int i = 0; i < 8; ++i)
        nbFreeCells += count(&grid[i][0], &grid[i][0] + 8, 0);

    if (nbFreeCells == 0)
    {
    	gameover = true;
    	cout << "game over" << endl;
    	return;
    }

	int randomIndex = rand() % nbFreeCells;
    int counter = 0;
    for (int i = 0; i < 8; ++i)
    {
    	for (int j = 0; j < 8; ++j)
    	{
    		if (grid[i][j] == 0 && counter++ == randomIndex)
    			grid[i][j] = (rand() % 2 == 0) ? 2 : 4;
    	}
    }
}

void init()
{
	srand(time(0));
	texture.loadFromFile("tiles.png");
	tiles[0] = Sprite(texture, IntRect(0, 0, tileSize, tileSize));
	for (int i = 1; i <= 11; ++i)
		tiles[pow(2, i)] = Sprite(texture, IntRect(i * tileSize, 0, tileSize, tileSize));
    window.setFramerateLimit(30);
}

void handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
            window.close();

        else if (event.type == Event::KeyPressed)
        {
        	if (!gameover && playing)
        	{
        		switch (event.key.code)
        		{
        			case Keyboard::Up:
		                swipeUp();
		                break;

		            case Keyboard::Down:
		                swipeDown();
		                break;

		            case Keyboard::Left:
		                swipeLeft();
		                break;

		            case Keyboard::Right:
		                swipeRight();
		                break;

                    default:
                        break;
        		}

        		gridModified = true;
        		playing = false;
        	}

            if (event.key.code == Keyboard::D)
            	printGrid();
        }
    }
}

void update()
{
	if (!playing)
	{
		addRandom();
		playing = true;
		gridModified = true;
	}
}

void render()
{
    if (gridModified)
    {
        printGrid();
        gridModified = false;
    }

    window.clear();

    for (int i = 0; i < 8; ++i)
    {
    	for (int j = 0; j < 8; ++j)
    	{
    		tiles[ grid[i][j] ].setPosition(i * tileSize, j * tileSize);
    		window.draw(tiles[ grid[i][j] ]);
    	}
    }

    window.display();
}

int main()
{
	init();

	while (window.isOpen())
	{
		handleEvents();
		update();
		render();
	}

	return 0;
}
