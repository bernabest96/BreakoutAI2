/*
 * GameOver.cpp
 *
 *  Created on: 19 ott 2019
 *      Author: Berna
 */

#include "GameOver.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "game.h"
using namespace sf;
using namespace std;

GameOver::GameOver(bool ai) : ai(ai){
	if (ai){
		str_text = "\nAI lose!!!!!";
	}else{
		str_text = "\nYou lose!!!!!";
	}
	str_text.append("\nPoints : ");
	str_text.append(std::to_string(GameUtils::GAMEPOINTS));
	if (!font.loadFromFile("ARIAL.TTF"))
	{
		throw new std::exception();
	}
	text.setFont(font); // font is a sf::Font
	// set the string to display
	text.setString(str_text);
	// set the character size
	text.setCharacterSize(36); // in pixels, not points!
	// set the color
	text.setFillColor(sf::Color::White);
}

void GameOver::update(Keyboard::Key& key){}

void GameOver::draw(sf::RenderWindow& window){
	window.draw(text);
}

GameOver::~GameOver() {}

