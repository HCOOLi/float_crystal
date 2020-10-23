//
// Created by 1007 on 2019/9/25.
//
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include<tuple>
#include<algorithm>

using namespace std;

template<class First, class Second>
auto Cartesian_product(vector<First> F, vector<Second> S) -> vector<tuple<First, Second >> {
    vector<tuple<First, Second >> result;
    for (auto f:F) {
        for (const auto &s:S) {
            result.push_back(std::make_tuple(f, s));
        }
    }
    return result;
}

template<class First, class... Args>
auto Cartesian_product(vector<First> F, vector<Args> ... rests) -> vector<tuple<First, Args ... >> {
    vector<tuple<First, Args ... >> result;
    auto rest_results = Cartesian_product(rests...);
    for (auto f:F) {
        for (const auto &r:rest_results) {
            result.push_back(tuple_cat(std::make_tuple(f), r));
        }
    }
    return result;

}


inline double randfloat()
{ //generate a random float
    return rand() / (double)(RAND_MAX);
}
template <typename T>
inline bool find_in_que(const deque<T> &que, T p)
{
    if (que.empty())
        return false;
    auto iter = std::find(que.begin(), que.end(), p);
    return iter != que.end();
}
inline std::vector<std::string> split(const std::string &s, char delimiter)
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

inline vector<double> range(double from, double to, double step)
{ //generate a random float
    vector<double> temp;
    for (double i = from; i < to; i += step)
    {
        temp.emplace_back(i);
    }
    return temp;
}
