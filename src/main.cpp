#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <SFML/Graphics.hpp>
#include <time.h>
#include <fstream>
#include <cmath>
#include "Blocks.h"
#include "LevelLoader.h"

using namespace sf;

bool GamePlay()
{
	//DEFAULTS
	#pragma region defaults

		//screen
		int sc_width = 640, 
			sc_height = 480;

		//framerate
		int framelimit = 60;

		//level
		std::ifstream fin("settings.txt");
		std::string level;
		getline(fin, level);

		//game name
		std::string name = "BreakOut";

		//ball
		int x = 320, y = 440;
		float dx = 6, dy = 6;

		//gameplay
		int lives = 5;
		bool game_active = false;
		bool flag = false;
		bool imm = false;

	#pragma endregion defaults JustDefaultValues

	//Window
	RenderWindow rw_app(VideoMode(sc_width, sc_height), "BreakOut");
	rw_app.setFramerateLimit(framelimit);

	//Load level
	LevelLoader* current_level = new LevelLoader;
	current_level->Load_level(level);
	rw_app.setTitle(name + ": " + current_level->getName());

	Texture t_bg;       //background image
	Texture t_ball;
	Texture t_paddle;
	Texture t_heart;
	Texture t_pause;
	Texture t_gg;
	Texture t_block[5];

	t_bg.loadFromFile("images/background.jpg");
	t_ball.loadFromFile("images/ball.png");
	t_paddle.loadFromFile("images/paddle.png");
	t_heart.loadFromFile("images/heart.png");
	t_pause.loadFromFile("images/pause.png");
	t_gg.loadFromFile("images/gg.png");

	for (auto i = 0; i < 5; i++)
		t_block[i].loadFromFile("images/block0" + std::to_string(i + 1) + ".png");

	//Set sprites
	Sprite sBackground(t_bg), sBall(t_ball), sPaddle(t_paddle), sPause(t_pause), sGG(t_gg);

	sPaddle.setOrigin(45, 0);
	sPaddle.setPosition(320, 460);

	sBall.setOrigin(6, 6);

	sPause.setOrigin(146, 64);
	sPause.setPosition(sc_width/2, sc_height/2);

	sGG.setOrigin(146, 64);
	sGG.setPosition(sc_width / 2, sc_height / 2);

	Sprite sLives[5];
	for (auto i = 0; i < 5; i++)
	{
		sLives[i].setTexture(t_heart);
		sLives[i].setPosition(10 + i * 30, 10);
	}

	//Set sprite for blocks + load object types
	block_pattern* state[130];
	auto n = 0;
	auto d = 0;

	for (auto j = 1; j <= current_level->getWidth(); j++)
	for (auto i = 1; i <= current_level->getHeight(); i++)
	{
		auto tmp = current_level->getType(n);
		switch (tmp)
		{
		case 0:
			state[n] = new block_IDB(t_block[0]);
			break;
		default:
			state[n] = new block_DB(t_block[tmp], tmp);
			d++;
			break;
		}
		state[n]->getSprite().setPosition(i * 43, 20 + j * 20);
		n++;
	}

	//Live
	while (rw_app.isOpen())
	{
		Event e;

		//Process recieved events
		while (rw_app.pollEvent(e))
		{
			switch (e.type)
			{
			case Event::Closed:
					rw_app.close();
				break;
			default:
				/*doNothing*/
				break;
			}
		}

		//move
		if (game_active)
		{
			float px = x;
			float py = y;

			if (Keyboard::isKeyPressed(Keyboard::Right) && sPaddle.getGlobalBounds().left + sPaddle.getGlobalBounds().width < sc_width) sPaddle.move(9, 0);
			if (Keyboard::isKeyPressed(Keyboard::Left) && sPaddle.getGlobalBounds().left > 0) sPaddle.move(-9, 0);
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				dy = -(rand() % 3 + 4);
				dx = -(rand() % 3 + 4);
			}

			x += dx;
			y += dy;

			if (!imm)
			{
				for (auto i = 0; i < n; i++)
				{
					FloatRect temp = state[i]->getSprite().getGlobalBounds();

					if (FloatRect(x - 3, y - 3, 6, 6).intersects(temp))
					{
						int cdx = temp.left + temp.width / 2 - px;
						int cdy = temp.top + temp.height / 2 - py;

						if (abs(cdy)*(temp.width / temp.height) < abs(cdx))
						{
							dx = -dx;
						}
						else
						{
							dy = -dy;
						}

						if (state[i]->Collision())
						{
							if (state[i]->health() > 0)
							{
								state[i]->getSprite().setTexture(t_block[state[i]->health()]);
							}
							else
							{
								d--;
								if (d == 0)
								{
									game_active = false;
								}
							}
						}
						imm = true;
					}
				}
			}
			else imm = false;

			FloatRect tmp_ball = sBall.getGlobalBounds();

			if (x < tmp_ball.width || x > sc_width - tmp_ball.width)  dx = -dx;
			if (y < tmp_ball.height)  dy = -dy;
			if (y > sc_height - tmp_ball.height / 2)
			{
				dy = -dy;
				lives--;

				if (lives == 0)
				{
					return false;
				}

				sPaddle.setPosition(320, 460);
				x = 320, y = 440;

				game_active = false;
			}
		}

		//Game Control
		//Pause
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			flag = true;
		}
		else if (flag == true)
		{
			game_active = !game_active;
			flag = false;
		}
		//Restart
		if (Keyboard::isKeyPressed(Keyboard::R))
		{
			return false;
		}
		//Quit
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			break;
		}

		if (FloatRect(x - 6, y - 6, 12, 12).intersects(sPaddle.getGlobalBounds())) dy = -(rand() % 5 + 2);

		sBall.setPosition(x, y);

		rw_app.clear();
		rw_app.draw(sBackground);
		rw_app.draw(sBall);
		rw_app.draw(sPaddle);

		for (auto i = 0; i < lives; i++)
			rw_app.draw(sLives[i]);

		for (auto i = 0; i<n; i++)
			rw_app.draw(state[i]->getSprite());

		if (!game_active)
		{
			if (d == 0)
				rw_app.draw(sGG);
			else
				rw_app.draw(sPause);
		}

		rw_app.display();
	}

	return true;
}

int main()
{
	srand(time(nullptr));
	while (GamePlay() == false) {};
	return 0;
}