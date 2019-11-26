/*
 * Winning.cpp
 *
 *  Created on: 18 nov 2019
 *      Author: Berna
 */

#include "Winning.h"
#include "GameOver.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "game.h"
using namespace sf;
using namespace std;

Winning::Winning(bool ai) : ai(ai){
	if (ai){
		str_text = "\nAI WIN!!!!!";
	}else{
		str_text = "\nYou WIN!!!!!";
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

void Winning::draw(sf::RenderWindow& window){
	window.draw(text);
}

Winning::~Winning() {}
