//
// Created by 1007 on 2019/9/12.
//
#pragma once
#ifndef CRYSTAL_LOG_H
#define CRYSTAL_LOG_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class logger {
    std::ofstream file;

    logger(string filename) {
        file = fopen(filename, "w+");
    }

    template<typenameT>
    logger& operator<<(T data){
        file<<data;
        return (*this)

    }

    ~logger() {
        file.close();
    }

};


#endif //CRYSTAL_LOG_H
