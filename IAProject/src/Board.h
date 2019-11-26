/*
 * Board.h
 *
 *  Created on: 15 mag 2019
 *      Author: Berna
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "game.h"
#include "ai/SARSA.h"
#include "GameObject.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

class Board : public GameObject{
public:
	sf::RectangleShape* rect;
	sf::Vector2f rectCenter;
	float acc;
	float speedx;
	ai::SARSA sarsa;
	Board();
	virtual void setPlayer();
	virtual void setAI();
	virtual bool getAI();
	virtual void update(sf::Keyboard::Key&);
	void onCollisionEnter(const GameObject*);
	void draw(sf::RenderWindow&);
	virtual ~Board();
private:
	bool ai, first;
	float eps;
	float speedMax;
	ai::SARSA::Actions command;
	int currentState;
	float f;
};

#endif /* BOARD_H_ */
