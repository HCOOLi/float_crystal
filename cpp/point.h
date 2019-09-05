#pragma once
#include<iostream>
#include"vec.h"

class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
    int movable;//0canmove,1can't move
	int type;
	vec location;
	int true_position;
	

public:
    Point() = default;

    Point(vec loc, int c_num, int p_i_c, int ty = 1, int movable = 0, int true_p = 0) :
		location(loc), chain_num(c_num),type(ty), pos_in_chain(p_i_c), movable(movable) ,true_position(true_p){}

};
ostream & operator<<(ostream & o, Point & p);