/*
 * Brick.cpp
 *
 *  Created on: 12 mag 2019
 *      Author: Berna
 */

#include "Brick.h"
#include "game.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
using namespace sf;

Brick::Brick(int num, int layer) : row(layer), index(num), life(1), reward((5 - layer) * 100){
	rect = new RectangleShape(Vector2f(GameUtils::RESOLUTION.x/10, 50));
	rect->setPosition(Vector2f(GameUtils::RESOLUTION.x/10 * num , 50 * layer + 75));

	mask = new mask::RectCollisionMask(rect);
	tag = GameObject::BRICK;
}

void Brick::setProperties(sf::Color colorBrick, int thickness,
		sf::Color borderColor){
		rect->setFillColor(colorBrick);
}

void Brick::onCollisionEnter(const GameObject*){
	life--;
	if (life <= 0){
		GameUtils::GAMEPOINTS += reward;
		std::string tmp = "BRICK";
		GameUtils::BRICK = tmp.append(std::to_string(row * 10 + index));
		//delete this;
	}
}

void Brick::draw(RenderWindow& window){
	window.draw(*rect);
}

Brick::~Brick(){
	if (rect!=nullptr)	delete rect;
}

