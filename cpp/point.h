#pragma once
#include <iostream>
#include "vec.h"
#define TYPE

//#define TRUE_POSITION
//#pragma message("using TRUE_POSITION")
class Point //A point class
{

public:
	int chain_num;
	int pos_in_chain;
	bool movable; 
	vec location;
#ifdef TYPE
	int type;
#endif

#ifdef TRUE_POSITION
	int true_position;
#endif

public:
	Point() = default;

	Point(vec loc, int c_num, int p_i_c
#ifdef TYPE
		  ,
		  int ty = 1
#endif
		  ,
		  int mable = 0
#ifdef TRUE_POSITION
		  ,
		  int true_p = 0
#endif
	)
	{
		location = loc;
		chain_num = c_num;
#ifdef TYPE
		type = ty;
#endif
		pos_in_chain = p_i_c;
		movable = mable;
#ifdef TRUE_POSITION
		true_position = true_p;
#endif
	}

friend ostream &operator<<(ostream &o, Point &p);
	
};
ostream &operator<<(ostream &o, Point &p);