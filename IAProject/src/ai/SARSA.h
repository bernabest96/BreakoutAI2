/*
 * SARSA.h
 *
 *  Created on: 19 nov 2019
 *      Author: Berna
 */

#ifndef AI_SARSA_H_
#define AI_SARSA_H_

#include <map>
#include <list>
using namespace std;

namespace ai {

class SARSA {
public:
	enum State {CENTER, UP_LEFT, DOWN_LEFT,
				UP_RIGHT, DOWN_RIGHT, GAME_OVER, LAST};
	enum Actions {LEFT, STOP, RIGHT, ALAST};
	//Variables
	State s, s_next;
	Actions a, a_next;
	double r;
	float alpha, gamma;
	double epsilon;
	map<pair<State, Actions>, double> Q;	//(s,a) --> Q(s, a)
	SARSA();
	Actions getActionFromEpsilonGreedy();
	void updateQ();
	virtual ~SARSA();
private:
	Actions argmax();
	Actions chooseOtherActions();
};

} /* namespace ai */

#endif /* AI_SARSA_H_ */
