//connect4.h

#include <iostream>
#include <math.h>
using namespace std;

const char player1piece = 49;
const char player2piece = 50;

unsigned long long initstate(int);

bool myturn(unsigned long long);

unsigned long long move(unsigned long long, int);

void print(unsigned long long);

int gameover(unsigned long long); //0 = game still going; 1 = player 1 wins; 2 = player 2 wins; 3 = tie

void inarow(unsigned long long, int, int&, int&);

bool winner(short, int);

int bitval(unsigned long long, int, int);

int height(unsigned long long, int);