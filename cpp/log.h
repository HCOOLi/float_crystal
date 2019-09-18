//
// Created by 1007 on 2019/9/12.
//

#ifndef CRYSTAL_LOG_H
#define CRYSTAL_LOG_H

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class logger {
    std::ofstream file;

    logger(string filename) {
        file = fopen(filename, "w+");
    }

    void log(string data) {

        file << data << endl;
    }

    ~logger() {
        file.close();
    }

};


#endif //CRYSTAL_LOG_H
