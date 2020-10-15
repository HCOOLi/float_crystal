
#pragma once         // not used
#ifndef CRYSTAL_LOG_H
#define CRYSTAL_LOG_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class logger {
    std::ofstream file;

    logger(string filename): file(filename,ios::out|ios::trunc) {
    }
    void log(){
        time_t now = time(0);
        char* dt = ctime(&now);
        file << dt << endl;
    }

    template<typename T>
    logger& operator<<(T data){
        time_t now = time(0);
        char* dt = ctime(&now);
        file << dt << ": "<<data<<endl;
        return (*this)
    }

    ~logger() {
        file.close();
    }

};


#endif //CRYSTAL_LOG_H
