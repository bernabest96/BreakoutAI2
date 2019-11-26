/*
 * Winning.h
 *
 *  Created on: 18 nov 2019
 *      Author: Berna
 */

#ifndef WINNING_H_
#define WINNING_H_

#include "GameObject.h"
#include <string.h>

class Winning : public GameObject{
private:
	std::string str_text;
	sf::Text text;
	sf::Font font;
	bool ai;
public:
	virtual void update(sf::Keyboard::Key&){};
	virtual void onCollisionEnter(const GameObject*){};
	//virtual bool collideWith(const GameObject*){return false;};
	virtual void draw(sf::RenderWindow&);
	Winning(bool ai);
	virtual ~Winning();
};
#endif /* WINNING_H_ */
