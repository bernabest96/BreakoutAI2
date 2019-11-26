//============================================================================
// Name        : IAProject.cpp
// Author      : ilBerna
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "game.h"
#include "mask/CollisionChecker.h"
#include "Brick.h"
#include "Board.h"
#include "Ball.h"
#include "menu/Menu.h"
#include "GameOver.h"
#include "Winning.h"
#include "ai/SARSA.h"
#include <cmath>
#include <map>
#include <list>
#include <string>
using namespace sf;
using namespace std;
using namespace ai;

/*get undefine reference to error*/
sf::Vector2u GameUtils::RESOLUTION = sf::Vector2u(1080, 680);
unsigned int GameUtils::GAMEPOINTS = 0;
double GameUtils::REWARD = 0;
bool GameUtils::RESET_LIFE = false;
bool GameUtils::INIT_SARSA = true;
int GameUtils::CURRENT_STATE = gamestates::MENU;
double GameUtils::deltaFrame = 1.0/60.0;
string GameUtils::BRICK = "NONE";
unsigned int GameUtils::count_brick = 0;

std::ostream& operator<<(std::ostream& out, const SARSA::State value){
    static std::map<SARSA::State, std::string> strings;
    if (strings.size() == 0){
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(ai::SARSA::CENTER);
        INSERT_ELEMENT(ai::SARSA::DOWN_LEFT);
        INSERT_ELEMENT(ai::SARSA::DOWN_RIGHT);
        INSERT_ELEMENT(ai::SARSA::UP_LEFT);
        INSERT_ELEMENT(ai::SARSA::UP_RIGHT);
		INSERT_ELEMENT(ai::SARSA::GAME_OVER);

#undef INSERT_ELEMENT
    }

    return out << strings[value];
}

std::ostream& operator<<(std::ostream& out, const SARSA::Actions value){
    static std::map<SARSA::Actions, std::string> strings;
    if (strings.size() == 0){
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(SARSA::Actions::LEFT);
        INSERT_ELEMENT(SARSA::Actions::STOP);
        INSERT_ELEMENT(SARSA::Actions::RIGHT);
#undef INSERT_ELEMENT
    }

    return out << strings[value];
}

ai::SARSA::State calculateNextState(const Board* board, const Ball* ball){
	int eps = 8;
	float size_2 = 0.5 * (GameUtils::RESOLUTION.x/10) - eps;
	ai::SARSA::State currentState;
	if (((board->rectCenter.x - size_2) <= ball->circleCenter.x) && (ball->circleCenter.x <= (board->rectCenter.x + size_2))){
		currentState = ai::SARSA::CENTER;
	}else if ((board->rectCenter.x - size_2) > ball->circleCenter.x){
		//LEFT
		if (ball->speed.y < 0){
			currentState = ai::SARSA::UP_LEFT;
		}else{
			currentState = ai::SARSA::DOWN_LEFT;
		}
	}else{
		//RIGHT
		if (ball->speed.y < 0){
			currentState = ai::SARSA::UP_RIGHT;
		}else{
			currentState = ai::SARSA::DOWN_RIGHT;
		}
	}
	if (GameUtils::RESET_LIFE){
		currentState = ai::SARSA::GAME_OVER;
	}
	return currentState;
}

float observeReward(ai::SARSA::State s, ai::SARSA::State s_next, ai::SARSA::Actions a,
		const Board* board, const Ball* ball){
	float reward = GameUtils::REWARD;
	if (s == ai::SARSA::GAME_OVER){
		reward += -100;
		reward += - 100 * abs(ball->circleCenter.x - board->rectCenter.x) *
			(1 / (double)GameUtils::RESOLUTION.x);

	}else{
		reward += -5 * abs(ball->circleCenter.x - board->rectCenter.x) *
				(1 / (double)GameUtils::RESOLUTION.x);

	}
	return reward;
}

int main()
{

	//Initialization
	sf::RenderWindow window(VideoMode(GameUtils::RESOLUTION.x, GameUtils::RESOLUTION.y), "Atari Breakout");
    window.setPosition(Vector2i(10, 0)); 	//distanza 10, 5 rispetto al desktop
    window.setVerticalSyncEnabled(true);	//attivo il vsync

    //Time Counting
    Clock clock;

    //Declaration
    menu::Menu* menu = nullptr;
    Board* board = nullptr;
    Ball* ball = nullptr;
    //GameOver* gameOver = nullptr;
    Winning* winning = nullptr;
    Keyboard::Key key = Keyboard::Key::Unknown;
    bool erase_list = true;
    bool ai = false;

    map<string, GameObject*> objects;
    sf::Color brickColors[5] = {sf::Color::Red,
    				sf::Color::Magenta, sf::Color::Yellow,
    				sf::Color::Green, sf::Color::Blue};

    while (window.isOpen())
    {
        Event event;

        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed){
				key = event.key.code;
			}else if (event.type == Event::KeyReleased){
				key = Keyboard::Key::Unknown;
			}

        }
        //Clear
        //Automa a stati : MENU -> AI o PLAYER
        window.clear(sf::Color::Black);

        /*MENU*/
        if (GameUtils::CURRENT_STATE== GameUtils::gamestates::MENU){
        	//Menu game logic
        	if (menu == nullptr){
        		menu = new menu::Menu();	//dovrebbe essere singleton
        		objects.insert(pair<string, GameObject*>("MENU", menu));
        	}

        }else if (GameUtils::CURRENT_STATE == GameUtils::GAME_INIT_CPU ||
        		GameUtils::CURRENT_STATE == GameUtils::GAME_INIT_PLAYER){
        	//delete menu
        	objects.erase("MENU");
        	delete menu;
        	menu = nullptr;

			board = new Board();
	        objects.insert(pair<string, GameObject*>("BOARD", board));

	        for (int i=0; i< 20; i++){
				int x = i % 10;
				int y = int(i/10);
				Brick* brick = new Brick(x, y);
				brick->setProperties(brickColors[4 - y], 5);
				string brick_str = "BRICK";
				objects.insert(pair<string, GameObject*>(brick_str.append(std::to_string(i)), brick));
			}
	        if (GameUtils::CURRENT_STATE == GameUtils::GAME_INIT_CPU)	GameUtils::CURRENT_STATE = GameUtils::GAME_CPU;
	        if (GameUtils::CURRENT_STATE == GameUtils::GAME_INIT_PLAYER)	GameUtils::CURRENT_STATE = GameUtils::GAME_PLAYER;
        }/*else if (GameUtils::CURRENT_STATE == GameUtils::gamestates::GAME_PLAYER){
        	//Player logic
        	if (ball==nullptr){
        		ball = new Ball();
        		objects.insert(pair<string, GameObject*>("BALL", ball));
        	}
        	if (GameUtils::BRICK != "NONE"){
        		objects.erase(GameUtils::BRICK);
        		GameUtils::count_brick++;
        		GameUtils::BRICK = "NONE";
        	}

        	//RESET
			if (GameUtils::RESET_LIFE){
				GameUtils::RESET_LIFE = false;
				for (int i=0; i< 50; i++){
					string brick_str = "BRICK";
					string brick_str1 = brick_str.append(std::to_string(i));
					//ERASE
					objects.erase(brick_str1);
					//INSERT
					int x = i % 10;
					int y = int(i/10);
					Brick* brick = new Brick(x, y);
					brick->setProperties(brickColors[4 - y], 5);
					objects.insert(pair<string, GameObject*>(brick_str.append(std::to_string(i)), brick));

				}
				board->rect->setPosition(Vector2f(0.5 * (GameUtils::RESOLUTION.x - GameUtils::RESOLUTION.x/10), GameUtils::RESOLUTION.y-10));	//posizione del centro
				board->acc = 0;
				board->speedx = 0;
				ball->ball->setPosition(540 - 10, 680-20-10 -1);	//680-10-10
				std::srand(time(0));
				int sign = rand() % 2;
				if (sign == 0) sign = -1;
				ball->speed = Vector2f(sign * 70, -100);

			}

        	if (GameUtils::count_brick == 50){
				GameUtils::CURRENT_STATE = GameUtils::gamestates::WINNING;
			}

        }*/
        else if (GameUtils::CURRENT_STATE == GameUtils::gamestates::GAME_CPU){
        	//AI logic
        	if (ball==nullptr){
				ball = new Ball();
				objects.insert(pair<string, GameObject*>("BALL", ball));
			}
        	board->setAI();

        	//SARSA ALGORITHM
        	if (GameUtils::INIT_SARSA){
				board->sarsa.s = calculateNextState(board, ball);
				board->sarsa.a = board->sarsa.getActionFromEpsilonGreedy();
				GameUtils::INIT_SARSA = false;
			}else{
				board->sarsa.s_next = calculateNextState(board, ball);
				board->sarsa.r = observeReward(board->sarsa.s, board->sarsa.s_next, board->sarsa.a,
									board, ball);
				cout << "Action: " << (ai::SARSA::Actions)board->sarsa.a << endl;
				cout << "reward: " << board->sarsa.r  << endl;
				cout << "current_state: " << board->sarsa.s_next << endl;
				board->sarsa.a_next = board->sarsa.getActionFromEpsilonGreedy();
				board->sarsa.updateQ();
				board->sarsa.s = board->sarsa.s_next;
				board->sarsa.a = board->sarsa.a_next;
				GameUtils::REWARD = 0;
			}

        	//RESET
			if (GameUtils::RESET_LIFE){

				//ERASE
				for (int i=0; i< 20; i++){
					string brick_str = "BRICK";
					string brick_str1 = brick_str.append(std::to_string(i));
					objects.erase(brick_str1);

				}
				//INSERT
				for (int i=0; i< 20; i++){
					string brick_str = "BRICK";
					int x = i % 10;
					int y = int(i/10);
					Brick* brick = new Brick(x, y);
					brick->setProperties(brickColors[4 - y], 5);
					objects.insert(pair<string, GameObject*>(brick_str.append(std::to_string(i)), brick));
				}
				board->rect->setPosition(Vector2f(0.5 * (GameUtils::RESOLUTION.x - GameUtils::RESOLUTION.x/10), GameUtils::RESOLUTION.y-10));	//posizione del centro
				board->acc = 0;
				board->speedx = 0;
				ball->ball->setPosition(540 - 10, 680-20-10 -1);	//680-10-10
				std::srand(time(0));
				int sign = rand() % 2;
				if (sign == 0) sign = -1;
				ball->speed = Vector2f(sign * 70, -100);
				GameUtils::BRICK = "";
				GameUtils::RESET_LIFE = false;
				GameUtils::INIT_SARSA = true;
				GameUtils::GAMEPOINTS = 0;
				GameUtils::count_brick = 0;

			}
			cout << "GameUtils::BRICK : " << GameUtils::BRICK << endl;
        	//ERASE BRICK
        	if (GameUtils::BRICK != "NONE"){
        		objects.erase(GameUtils::BRICK);
				GameUtils::count_brick++;
				GameUtils::BRICK = "NONE";
			}

        	//WINNING
			if (GameUtils::count_brick == 20){
				GameUtils::CURRENT_STATE = GameUtils::gamestates::WINNING;
			}
        }else if (GameUtils::CURRENT_STATE == GameUtils::gamestates::WINNING){
        	if (ball != nullptr){
				objects.erase("BALL");
				delete ball;
				ball = nullptr;
			}
			if (board != nullptr){
				objects.erase("BOARD");
				ai = board->getAI();
				delete board;
				board = nullptr;
			}
			if (winning == nullptr){
				winning = new Winning(ai);
				objects.insert(pair<string, GameObject*>("WINNING", winning));
			}
			if (erase_list){
				erase_list = false;
				for (int i=0; i< 20; i++){
					string brick_str = "BRICK";
					objects.erase(brick_str.append(std::to_string(i)));
				}
			}


        }/*else if (GameUtils::CURRENT_STATE == GameUtils::gamestates::GAME_OVER){
        	if (ball != nullptr){
				objects.erase("BALL");
				delete ball;
				ball = nullptr;
			}
        	if (board != nullptr){
				objects.erase("BOARD");
				ai = board->getAI();
				delete board;
				board = nullptr;
			}

        	if (ball != nullptr){
				objects.erase("MENU");
				delete menu;
				menu = nullptr;
			}
        	if (!list_brick){
				list_brick = true;
				for (int i=0; i< 50; i++){
					string brick_str = "BRICK";
					objects.erase(brick_str.append(std::to_string(i)));
				}
			}
        	if (gameOver == nullptr){
        		gameOver = new GameOver(ai);
        		objects.insert(pair<string, GameObject*>("GAME_OVER", gameOver));
			}
        }*/

        //Game Logic
        map<string, GameObject*>::iterator i, j, l;

        for (i = objects.begin(); i!=objects.end(); i++){
        	l = i;
        	for (j = ++l; j!=objects.end(); j++){
        		if (mask::CollisionChecker::checkCollision(i->second, j->second)){
        			i->second->onCollisionEnter(j->second);
        			j->second->onCollisionEnter(i->second);
        		}
        	}
        	i->second->update(key);
        	i->second->draw(window);
        }

        //fps count
		double currentTime = clock.restart().asMilliseconds();
		GameUtils::deltaFrame = currentTime / 1000;
		window.display();

    }

    return 0;
}
