/*
 * SARSA.cpp
 *
 *  Created on: 19 nov 2019
 *      Author: Berna
 */

#include "SARSA.h"
#include <random>
#include <iostream>
#include <map>
using namespace std;

namespace ai {
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

unsigned int SARSA::MAX_ATTEMPTS = 50;

SARSA::SARSA() : s((State) 0), s_next((State) 0), a(STOP), a_next(STOP), r(0.0),
		alpha(0.8), gamma(1), epsilon(.1), attempts(1){
	for (int i = 0; i!= State::LAST; i++){
		for (int j = Actions::LEFT; j!= Actions::ALAST; j++){
			State s = static_cast<State>(i);
			Actions a = static_cast<Actions>(j);
			Q[pair<State, Actions>(s, a)] = 0;
		}
	}
	statistics.open("statistics.txt", std::ios_base::app);
}

SARSA::Actions SARSA::getActionFromEpsilonGreedy(){
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	double number = distribution(generator);
	Actions a_policy;
	if (number < epsilon){
		a_policy = chooseOtherActions();
	}else{
		a_policy = argmax();
	}
	return a_policy;
}

SARSA::Actions SARSA::argmax(){
	Actions aMax;
	if (Q[pair<State, Actions>(s_next, LEFT)] > Q[pair<State, Actions>(s_next, STOP)] &&
			Q[pair<State, Actions>(s_next, LEFT)] > Q[pair<State, Actions>(s_next, RIGHT)]){
		aMax = LEFT;
	}else{
		if (Q[pair<State, Actions>(s_next, RIGHT)] > Q[pair<State, Actions>(s_next, STOP)]){
			aMax = RIGHT;
		}else{
			aMax = STOP;
		}
	}
	return aMax;
}

SARSA::Actions SARSA::chooseOtherActions(){
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	double number = distribution(generator);
	Actions remaining_actions[2];
	Actions a_max = argmax();
	for (int i = Actions::LEFT; i!= Actions::ALAST; i++){
		Actions a = static_cast<Actions>(i);
		if ( a != a_max){
			remaining_actions[i%2] = a;
		}
	}
	if (number < 0.5){
		return remaining_actions[0];
	}else{
		return remaining_actions[1];
	}
}

void SARSA::updateQ(){
	Q[pair<State, Actions>(s, a)] += alpha * (r + gamma * Q[pair<State, Actions>(s_next, a_next)] - Q[pair<State, Actions>(s, a)]);
	cout << "Q(" << s <<", " << a << "): " << Q[pair<State, Actions>(s, a)] << endl;
}

void SARSA::saveAndCloseFile(){
	if (attempts >= MAX_ATTEMPTS){
		statistics << "attempts:INFINITY" << endl;

	}else{
		statistics << "attempts:" << attempts << endl;
	}
	statistics.close();
}

void SARSA::recordBlocks(unsigned int num_blocks){
	statistics << "number of blocks:" << num_blocks << endl;
}

void SARSA::saveEpsilon(double eps){
	this->epsilon = eps;
	statistics << "epsilon:" << epsilon << endl;
}

SARSA::~SARSA() {}

} /* namespace ai */
