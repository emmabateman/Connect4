//main.cpp

#include <iostream>
#include <climits>
#include <ctime>
#include "connect4.h"
using namespace std;

const int DEPTH = 7;

int utility(unsigned long long);

int min(unsigned long long, int, int);

int max(unsigned long long, int, int);

int main() {

    cout << "Let's play Connect 4. I'm player 1 and you're player 2. Who goes first? (enter '1' or '2')" << endl;

    int first;
    cin >> first;

    unsigned long long state;

    if (first == 1) {
	cout << "I'll go first." << endl;
	state = initstate(0);
	state = move(state, 3);
    }
    else {
	cout << "You go first." << endl;
	state = initstate(1);
    }

    int gamestate = 0;
    int userselection = 0;

    //for testing
    //int turns = 0;
    //double totalseconds = 0;

    while (gamestate == 0) {
	cout << endl;
	print(state);

	if (myturn(state)) {
	    cout << "it's my turn." << endl;

	    //for testing
	    //clock_t start = clock();

	    int bestmove = 3;
	    int bestvalue = INT_MIN;
	    for (int i = 0; i < 7; i++) {
		unsigned long long newstate = move(state, i);
		int newvalue = min(newstate, 1, INT_MIN);
		if (newstate != state && newvalue > bestvalue) {
		    bestvalue = newvalue;
		    bestmove = i;
		}
	    }
	    state = move(state, bestmove);

	}
	else {
	    cout << "it's your turn. enter the name of the column you want to play in." << endl;
	    cin >> userselection;
	    if (userselection > 6 || userselection < 0) cout << "that is not a valid answer. try again." << endl;
	    else if (move(state, userselection) == state) cout << "that column is full! try again.\n";
	    else state = move(state, userselection);
	}

	gamestate = gameover(state);
    }

    print(state);

    switch(gamestate) {
	case 1:
		cout << "player 1 wins!!" << endl;
		break;
	case 2:
		cout << "player 2 wins!!" << endl;
		break;
	case 3:
		cout << "it's a tie!!" << endl;
		break;
	default:
		cout << "game over error " << gamestate << endl;
		break;
    }
}

int min(unsigned long long state, int d, int alpha) {
    //check if game ends at this state
    int winner = gameover(state);
    if (winner == 1) return INT_MAX - d; //favor shortest path to win
    if (winner == 2) return INT_MIN + d; //favor longest path to loss
    if (winner == 3) return 0;

    if (d >= DEPTH) return utility(state);

    unsigned long long newstate;
    int newvalue;
    int value = INT_MAX;
    for (int i = 0; i < 7; i++) {
	newstate = move(state, i);
	if (newstate != state) {
	    newvalue = max(newstate, d+1, value);
	    if (newvalue < alpha) return newvalue; // alpha-beta pruning
	    if (newvalue < value) value = newvalue;
	}
    }
    return value;
}

int max(unsigned long long state, int d, int beta) {
    //check if game ends at this state
    int winner = gameover(state);
    if (winner == 1) return INT_MAX - d; //favor shortest path to win
    if (winner == 2) return INT_MIN + d; //favor longest path to loss
    if (winner == 3) return 0;

    if (d >= DEPTH) return utility(state);
    else {
	unsigned long long newstate;
	int newvalue;
	int value = INT_MIN;
	for (int i = 0; i < 7; i++) {
	    newstate = move(state, i);
	    if (newstate != state) {
		newvalue = min(newstate, d+1, value);
		if (newvalue > beta) return newvalue; // alpha-beta pruning
		if (newvalue > value) value = newvalue;
	    }
	}
	return value;
    }
}


int utility(unsigned long long state) {
    int my2s, your2s, my3s, your3s, my4s, your4s;

    inarow(state, 2, my2s, your2s);
    inarow(state, 3, my3s, your3s);
    inarow(state, 4, my4s, your4s);

    if (my4s > 0 && your4s == 0) return INT_MAX;
    if (your4s < 0 && my4s == 0) return INT_MIN;

    int utility = my2s + (my3s*10) + (my4s*100) - your2s - (your3s*10) - (your4s*100);

    return utility;
}

