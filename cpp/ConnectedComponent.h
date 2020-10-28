#pragma once

#include<iostream>
#include<queue>
#include<stack>
#include<vector>
#include<array>


using namespace std;
namespace matrix {
    typedef array<int, 2> vec2;
    typedef array<int, 3> vec3;

    inline vec2 operator+(const vec2 &v1, const vec2 &v2) {
        return vec2{v1[0] + v2[0], v1[1] + v2[1]};
    }

    inline bool operator>=(const vec2 &v1, const vec2 &v2) {
        return v1[0] >= v2[0] && v1[1] >= v2[1];
    }

    inline bool operator<(const vec2 &v1, const vec2 &v2) {
        return v1[0] < v2[0] && v1[1] < v2[1];
    }

    inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
        return vec3{v1[0] + v2[0], v1[1] + v2[1], v1[2] + v2[2]};
    }

    inline bool operator>=(const vec3 &v1, const vec3 &v2) {
        return v1[0] >= v2[0] && v1[1] >= v2[1] && v1[2] >= v2[2];
    }

    inline bool operator<(const vec3 &v1, const vec3 &v2) {
        return v1[0] < v2[0] && v1[1] < v2[1] && v1[2] < v2[2];
    }

    class Matrix3 {

        vector<vector<vector<int> > > _v;

    public:
        friend ostream &operator<<(ostream &o, Matrix3 m);

        array<int, 3> shape;

        Matrix3() : shape{0, 0, 0} {

        }

        Matrix3(int x, int y, int z) : shape{x, y, z} {
            _v.resize(x);
            for (auto &__v : _v) {
                __v.resize(y);
                for (auto &___v : __v) {
                    ___v.resize(z);
                    for (auto &x : ___v) {
                        x = 0;
                    }
                }
            }
        }

        Matrix3(array<int, 3> s) : Matrix3(s[0], s[1], s[2]) {
        }

        Matrix3(vector<vector<vector<int >>> &mat) : _v(mat) {
            shape[0] = mat.size();
            shape[1] = mat[0].size();
            shape[2] = mat[0][0].size();
        }

        Matrix3(vector<vector<vector<int >>> &&mat) : _v(mat) {
            shape[0] = mat.size();
            shape[1] = mat[0].size();
            shape[2] = mat[0][0].size();
        }

        int &operator[](vec3 x) {
            return _v[x[0]][x[1]][x[2]];
        }

        const int &operator[](vec3 x) const {
            return _v[x[0]][x[1]][x[2]];
        }

        auto operator[](int x)->decltype(_v[x]){
            return _v[x];
        }

        const auto operator[](int x) const -> const decltype(_v[x]) {
            return _v[x];
        }

    };

    class Matrix2 {

        vector<vector<int> > _v;

    public:
        array<int, 2> shape;

        Matrix2(array<int, 2> s) : Matrix2(s[0], s[1]) {
        }

        Matrix2() : shape{0, 0} {

        }

        Matrix2(int x, int y) : shape{x, y} {

            _v.resize(x);
            for (auto &__v : _v) {
                __v.resize(y);
                for (auto &x : __v) {
                    x = 0;
                }

            }
        }

        Matrix2(vector<vector<int >> &mat) : _v(mat) {
            shape[0] = mat.size();
            shape[1] = mat[0].size();
        }

        Matrix2(vector<vector<int >> &&mat) : _v(mat) {
            shape[0] = mat.size();
            shape[1] = mat[0].size();
        }

        auto operator[](int x) ->decltype(_v[x]) {
            return _v[x];
        }

        int &operator[](vec2 x) {
            return _v[x[0]][x[1]];
        }

        const int &operator[](vec2 x) const {
            return _v[x[0]][x[1]];
        }

        const auto operator[](int x)const  ->const decltype(_v[x])  {
            return _v[x];
        }

        friend ostream &operator<<(ostream &o, Matrix2 m);

    };


    inline void update_min_max(array<int, 6> &a, vec3 &next) {
        for (int i = 0; i < 3; i++) {
            a[i] = next[i] > a[i] ? next[i] : a[i];
            a[i + 3] = next[i] < a[i + 3] ? next[i] : a[i + 3];
        }
    }

    //py::list labeling_nearby(const Matrix3 & bitmap, Matrix3 & label, stack<vec3 > & que, int index);
    //py::list ConnectedComponentLabeling(const Matrix3 & bitmap);
    int ConnectedComponentLabeling(const Matrix2 &bitmap);
}