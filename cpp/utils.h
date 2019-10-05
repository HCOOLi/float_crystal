//
// Created by 1007 on 2019/9/25.
//
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<sstream>

using namespace std;

inline double randfloat() {//generate a random float
    return rand() / (double)(RAND_MAX);
}
template<typename T>
inline bool find_in_que(const deque<T> &que, T p) {
    if (que.empty()) return false;
    auto iter = std::find(que.begin(), que.end(), p);
    return iter != que.end();
}
inline std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
   std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
      tokens.push_back(token);
       }
  return tokens;
}

inline vector<double> range(double from, double to, double step) {//generate a random float
    vector<double> temp;
    for (double i = from; i < to; i += step) {
        temp.emplace_back(i);
    }
    return temp;
}
