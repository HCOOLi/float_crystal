//
// Created by 1007 on 2019/9/25.
//
#pragma once
#include <algorithm>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <map>

using namespace std;
template <typename T>
vector<T> myrange(T from, T to, T step = 1) {
    vector<T> result;
    if(step==0.0){
        cerr<<"step can't be zero"<<endl;
        throw "zero";
    }
    if(step>0.0){
        for (T x = from; x < to; x += step) {
            //cout<<x<<endl;
            result.push_back(x);
        }
    }else{
        for (T x = from; x > to; x += step) {
            //cout<<x<<endl;
            result.push_back(x);
        }
    }
    return result;
}
// map<int,int> hist(vector<int> list){

// return map<int,int>();
// }

template <class First, class Second>
auto Cartesian_product(vector<First> F, vector<Second> S)
    -> vector<tuple<First, Second>> {
    vector<tuple<First, Second>> result;
    for (auto f : F) {
        for (const auto &s : S) {
            result.push_back(std::make_tuple(f, s));
        }
    }
    return result;
}

template <class First, class... Args>
auto Cartesian_product(vector<First> F, vector<Args>... rests)
    -> vector<tuple<First, Args...>> {
    vector<tuple<First, Args...>> result;
    auto rest_results = Cartesian_product(rests...);
    for (auto f : F) {
        for (const auto &r : rest_results) {
            result.push_back(tuple_cat(std::make_tuple(f), r));
        }
    }
    return result;
}

inline double randfloat() {  // generate a random float
    return rand() / (double)(RAND_MAX);
}
template <typename T>
inline bool find_in_que(const deque<T> &que, T p) {
    if (que.empty()) return false;
    auto iter = std::find(que.begin(), que.end(), p);
    return iter != que.end();
}
inline std::vector<std::string> split(const std::string &s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

