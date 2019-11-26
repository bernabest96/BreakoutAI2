/*
 * Board.cpp
 *
 *  Created on: 15 mag 2019
 *      Author: Berna
 */
#include "Board.h"
#include "mask/RectCollisionMask.h"
#include "game.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
using namespace sf;

Board::Board() : rect(nullptr), ai(false), first(true), eps(20), acc(0),
		speedx(0), speedMax(200), command(ai::SARSA::STOP), currentState(0),
		f(1.1)
{
	rect = new RectangleShape(Vector2f(GameUtils::RESOLUTION.x/10, 20));
	rectCenter = Vector2f(0.5 * (GameUtils::RESOLUTION.x - GameUtils::RESOLUTION.x/10), GameUtils::RESOLUTION.y-10);
	rect->setPosition(Vector2f(0.5 * (GameUtils::RESOLUTION.x - GameUtils::RESOLUTION.x/10), GameUtils::RESOLUTION.y-10));	//posizione del centro
	rect->setFillColor(Color::White);
	mask = new mask::RectCollisionMask(rect);
	tag = GameObject::BOARD;
}

void Board::setAI(){
	ai = true;
}

bool Board::getAI(){
	return ai;
}

void Board::setPlayer(){
	ai = false;
}

void Board::update(sf::Keyboard::Key& key){
	double ds, dv;
	sf::Vector2f pos;

	if (!ai){
		if (key == Keyboard::Key::D){
			command = ai::SARSA::RIGHT;
		}else if (key == Keyboard::Key::A){
			command = ai::SARSA::LEFT;
		}else if (key == Keyboard::Key::Unknown){
			command = ai::SARSA::STOP;
		}
	}else{
		command = sarsa.a;
	}

	if ( command == ai::SARSA::RIGHT){
		acc = f * 1000;
		if (!first)	first = true;
	}else if (command == ai::SARSA::LEFT){
		acc = -f * 1000;
		if (!first)	first = true;
	}else if (command == ai::SARSA::STOP){
		if (first){
			acc = - acc;
			first = false;
		}
		if (-eps < speedx && speedx < eps){
			speedx = 0;
			acc = 0;
		}

	}
	//std::cout << acc << std::endl;
	//std::cout << speedx << std::endl;

	dv = acc * GameUtils::deltaFrame;
	speedx += dv;
	if (speedx > speedMax){
		speedx = speedMax;
	}
	if (speedx < -speedMax){
		speedx = -speedMax;
	}
	ds = speedx * GameUtils::deltaFrame;
	pos = rect->getPosition() + Vector2f(ds, 0);
	if ((pos.x + 108 - 6) > GameUtils::RESOLUTION.x || (pos.x + 6) < 0){
		pos = rect->getPosition();
		speedx = 0;
		acc = 0;
	}
	rect->setPosition(pos);
	rectCenter = rect->getPosition() +
				Vector2f(rect->getSize().x/2, rect->getSize().y/2);
}

void Board::draw(RenderWindow& window){
	window.draw(*rect);
}

void Board::onCollisionEnter(const GameObject* object){
	GameUtils::REWARD += 100;
}

Board::~Board() {
	if (rect != nullptr)	delete rect;
}
