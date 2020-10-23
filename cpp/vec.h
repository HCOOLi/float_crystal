#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <array>
using namespace std;
typedef array<int, 3> vec;
// some useful functions and operators;
inline vec operator+(const vec &a, const vec &b)
{
	vec v;
	for (int i = 0; i < 3; i++)
	{
		v[i] = a[i] + b[i];
	}
	return v;
}
inline vec operator-(const vec &a, const vec &b)
{
	vec v;
	for (int i = 0; i < 3; i++)
	{
		v[i] = a[i] - b[i];
	}
	return v;
}
inline vec operator/(const vec &a, const int b)
{
	return vec{a[0] / b, a[1] / b, a[2] / b};
}
inline bool operator>=(const vec &a, const vec &b) // any 
{
	for (int i = 0; i < 3; i++)
	{
		if (a[i] >= b[i])
		{
			return true;
		}
	}
	return false;
}
inline bool operator<(const vec &a, const vec &b) //any
{
	/*
	any_of(a,b,[](int a,int b){
		return a<b;

	});
	*/
	for (int i = 0; i < 3; i++)
	{
		if (a[i] < b[i])
		{
			return true;
		}
	}
	return false;
}
inline int operator*(const vec &a, const vec &b)
{
	int sum = 0;
	for (int i = 0; i < 3; i++)
	{
		sum += a[i] * b[i];
	}
	return sum;
}

inline vec operator*(int a, const vec &b)
{
	vec result;
	for (int i = 0; i < 3; i++)
	{
		result[i] = a * b[i];
	}
	return result;
}

inline vector<double> mean(const vec &a, const vec &b)
{
	return vector<double> {(a[0]+b[0])/2.0,(a[1]+b[1])/2.0,(a[2]+b[2])/2.0};
}
inline double mean(const vec &a)
{
	return 0;
}
inline ostream &operator<<(ostream &o, const vec &a)
{
	o << '[' << a[0] << "," << a[1] << ',' << a[2] << ']';
	return o;
}
inline bool if_opposite(const vec &a, const vec &b)
{
	for (int i = 0; i < 3; i++)
	{
		if (a[i] + b[i] != 0)
		{
			return false;
		}
	}
	return true;
}
inline vec operator%(const vec &a, const vec &b)
{
	vec v;
	for (int i = 0; i < 3; i++)
	{
		if (a[i] >= b[i])
		{
			v[i] = a[i] % b[i];
		}
		else if (a[i] < 0)
		{
			v[i] = a[i] + b[i];
		}
		else
		{
			v[i] = a[i];
		}
	}
	return v;
}
inline bool operator==(const vec &a, const vec &b) //all
{
	for (int i = 0; i < 3; i++)
	{
		if (a[i] != b[i])
		{
			return false;
		}
	}
	return true;
}

class Matrix3
{

	vector<vector<vector<int>>> _v;

public:
	Matrix3() = default;

	Matrix3(int x, int y, int z)
	{
		_v.resize(x);
		for (auto &__v : _v)
		{
			__v.resize(y);
			for (auto &___v : __v)
			{
				___v.resize(z);
				for (auto &value : ___v)
				{
					value = 0;
				}
			}
		}
	}
	vector<vector<int>> &operator[](int x)
	{
		return _v[x];
	}
	const vector<vector<int>> &operator[](int x) const
	{
		return _v[x];
	}
};