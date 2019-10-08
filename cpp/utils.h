//
// Created by 1007 on 2019/9/25.
//
#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<sstream>
#include<algorithm>
using namespace std;

inline double randfloat() {//generate a random float
    return rand() / (double)(RAND_MAX);
}
template<typename T1,typename T2>
inline bool operator==(const pair<T1,T2> &p1,const pair<T1,T2> &p2){
return p1.first==p2.first&&p1.second==p2.second;
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
