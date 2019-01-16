//connect4.cpp
//the functions that deal with the actual representation of the board go here

#include "connect4.h"

unsigned long long initstate(int player) {
    unsigned long long init = 2;
    unsigned long long state = 0;
    for (int i = 0; i < 7; i++) {
	state += init;
	init = init << 8;
    }

    if(player > 0)
	state += 0xff00000000000000;

    return state;
}

bool myturn(unsigned long long state) {
    if (state < 0x100000000000000) return true;
    return false;
}

unsigned long long move(unsigned long long state, int column) {

    short x = (state >> (48-column*8)) % 256;

    if (x > 128) {
	return state;
    }

    x = x << 1;
    if (state > 0x100000000000000) x++;

    unsigned long long mask1 = ~((unsigned long long)0xff << (48-column*8));
    unsigned long long mask2 = ((unsigned long long)x << (48-column*8));
    unsigned long long mask3 = (unsigned long long)0xff << 56;

    //clear column being changed
    state = state & mask1;

    //insert new column
    state = state | mask2;

    //changed player value
    state = state ^ mask3; 

    return state;
}

int gameover(unsigned long long state) {

    int p1, p2;

    inarow(state, 4, p1, p2);

    bool win1, win2;
    if (p1 > 0) win1 = true;
    if (p2 > 0) win2 = true;

    //check if board is full
    bool full = true;
    for (int i = 0; i < 7; i++) {
	short x = (state >> (48-i*8)) % 256;
	if (log2(x) < 7) full = false;
    }

    if (!win1 && !win2 && !full) return 0;
    if (win1 && !win2) return 1;
    if (win2 && !win1) return 2;
    return 3;
}

void inarow(unsigned long long state, int num, int &p1, int &p2) {
    p1 = 0;
    p2 = 0;

    //check for horizontal wins

    short rows1[6] = {0, 0, 0, 0, 0, 0};
    short rows2[6] = {0, 0, 0, 0, 0, 0};

    for (int i = 0; i < 7; i++) {
	short x = (state >> (48-i*8)) % 256;
	int height = log2(x)-1;
	for (int j = 6-height; j < 6; j++) {
	    if (x%2 == 0) rows1[j] += (1 << i);
	    else rows2[j] += (1 << i);
	    x = x >> 1;
	}
    }

    for (int i = 0; i < 6; i++) {
	if (winner(rows1[i], num)) p1++;
	if (winner(rows2[i], num)) p2++;
    }

    //check for vertical wins
    for (int i = 0; i < 7; i++) {
	short column = (state >> (48-i*8)) % (1 << height(state, i));
	if (winner(column, num)) p2++;
	short mask = (1 << height(state, i)) - 1;
	column = column^mask;
	if (winner(column, num)) p1++;
    }

    //check for diagonal wins
    short series1 = 0, series2;
    for (int i = -8; i < 8; i++) {
	series1 = series2 = 0;
	for (int j = 0; j < 6; j++) {
	    if (i+j >= 0 && i+j < 7) {
		series1 += (rows1[j])&(1 << (j+i));
		series2 += (rows2[j])&(1 << (j+i));
	    }
	}
	if (winner(series1, num)) p1++;
	if (winner(series2, num)) p2++;
	
    }
    for (int i = -8; i < 8; i++) {
	series1 = series2 = 0;
	for (int j = 0; j < 6; j++) {
	    if (i+j >= 0 && i+j < 7) {
		series1 += (rows1[5-j])&(1 << (j+i));
		series2 += (rows2[5-j])&(1 << (j+i));
	    }
	}
	if (winner(series1, num)) p1++;
	if (winner(series2, num)) p2++;
	
    }
}

void print(unsigned long long state) {
    char arr[6][7];
    for (int i = 0; i < 7; i++) {
	short x = (state >> (48-i*8)) % 256;
	int height = log2(x)-1;
	for (int j = 6-height; j < 6; j++) {
	    if (x%2 == 0) arr[j][i] = player1piece;
	    else arr[j][i] = player2piece;
	    x = x >> 1;
	}
	for (int j = 0; j < 6-height; j++) {
	    arr[j][i] = 32;
	}
    }

    for (int i = 0; i < 6; i++) {
	for (int j = 0; j < 7; j++) {
	    cout << "[" << arr[i][j] << "]";
	}
	cout << endl;
    }

    cout << " 0  1  2  3  4  5  6\n";
}

int bitval(unsigned long long state, int x, int y) {
    return (state >> x*8+y) % 2;
}

int height(unsigned long long state, int column) {
    short x = (state >> (48-column*8)) % 256;
    return log2(x)-1;
}

bool winner(short row, int num) {
    short mask = (1 << num) - 1;
    for (int i = 0; i < 9-num; i++) {
	short x = ((mask << i)^row)&(mask << i);

	if (x == 0) return true;
    }
    return false;
}
