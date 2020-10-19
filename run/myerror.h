#pragma once
#include<iostream>
#include"vec.h"
#include<string>

#define DEBUG
// error types //To be done
class WrongPoint:public exception{
public:
    WrongPoint(string &a, vec b) {
		std::cerr << this->what();
		std::cerr << a;
		std::cerr << b;
	}

    explicit WrongPoint(vec b) {
		std::cerr << this->what();
		std::cerr << b;
	}
};

