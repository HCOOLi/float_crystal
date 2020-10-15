#pragma once
#include <iostream>
#include "vec.h"
#define TYPE
#define TRUE_POSITION
class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
	int movable; //0canmove,1can't move
	vec location;
#ifdef TYPE
	int type;
#endif

#ifdef TRUE_POSITION
	int true_position;
#endif

public:
	Point() = default;

	Point(vec loc, int c_num, int p_i_c,
		  int ty = 1, int movable = 0, int true_p = 0)
		: location(loc), chain_num(c_num), type(ty),
		  pos_in_chain(p_i_c), movable(movable),
		  true_position(true_p)
	{
	}
};
ostream &operator<<(ostream &o, Point &p);