#include"ConnectedComponent.h"

namespace matrix {
    ostream &operator<<(ostream &o, Matrix3 m) {
        for (int i = 0; i < m.shape[0]; i++) {
            for (int j = 0; j < m.shape[1]; j++) {
                for (int k = 0; k < m.shape[2]; k++) {
                    o << m[i][j][k] << ',';
                }
                o << endl;
            }
            o << endl;
        }
        return o;
    }

    ostream &operator<<(ostream &o, Matrix2 m) {
        for (int i = 0; i < m.shape[0]; i++) {
            for (int j = 0; j < m.shape[1]; j++) {
                o << m[i][j] << ',';
            }
            o << endl;
        }
        return o;
    }

    void labeling_nearby(const Matrix3 &bitmap, Matrix3 &label, stack<vec3> &que, int index) {
        static vector<vec3> direction = {{-1, 0,  0},
                                         {1,  0,  0},
                                         {0,  -1, 0},
                                         {0,  1,  0},
                                         {0,  0,  1}};
        array<int, 6> a{0, 0, 0, bitmap.shape[0], bitmap.shape[1], bitmap.shape[2]};
        while (!que.empty()) {
            vec3 seed = que.top();
            que.pop();
            update_min_max(a, seed);
            for (auto &dir : direction) {
                vec3 next = seed + dir;
                if (next >= vec3{0, 0, 0} && next < bitmap.shape) {
                    if (label[next] == 0) {
                        if (bitmap[next] == 1) {
                            label[next] = index;
                            update_min_max(a, next);
                            que.emplace(next);
                        }
                            /*else if(bitmap[next] == -1){
                                if (!que.empty()) {
                                    stack<vec3> emptystack;
                                    que.swap(emptystack);
                                }

                                return  *(new py::list);

                            }*/
                        else {
                            label[next] = -1;
                        }
                    }
                }
            }
        }
        //py::list * results = new py::list;
        //for (int i = 0; i < 6; i++)
        //	 results->append(a[i]) ;
        //return *results;

    }

    void prelabeling_nearby(const Matrix3 &bitmap, Matrix3 &label, stack<vec3> &que, int index) {
        static vector<vec3> direction = {{-1, 0,  0},
                                         {1,  0,  0},
                                         {0,  -1, 0},
                                         {0,  1,  0}};
        array<int, 6> a{0, 0, 0, bitmap.shape[0], bitmap.shape[1], bitmap.shape[2]};
        while (!que.empty()) {
            vec3 seed = que.top();
            que.pop();
            update_min_max(a, seed);
            for (auto &dir : direction) {
                vec3 next = seed + dir;
                if (next >= vec3{0, 0, 0} && next < bitmap.shape) {
                    if (label[next] == 0) {
                        if (bitmap[next] == 1) {
                            label[next] = -1;
                            que.emplace(next);
                        }
                            /*else if(bitmap[next] == -1){
                                if (!que.empty()) {
                                    stack<vec3> emptystack;
                                    que.swap(emptystack);
                                }

                                return  *(new py::list);

                            }*/
                        else {
                            label[next] = -1;
                        }
                    }
                }
            }
        }
    }

    void ConnectedComponentLabeling(const Matrix3 &bitmap) {
        Matrix3 label(bitmap.shape);
        stack<vec3> que;

        int label_index = 0;
        for (int k = 0; k < bitmap.shape[2]; k++) {
            int count_1 = 0;
            for (int i = 0; i < bitmap.shape[0]; i++) {
                for (int j = 0; j < bitmap.shape[1]; j++) {
                    if (label[i][j][k] == 0) {
                        if (bitmap[i][j][k] == -1) {
                            count_1++;
                            if (count_1 > 10) {
                                for (int i = 0; i < bitmap.shape[0]; i++) {
                                    for (int j = 0; j < bitmap.shape[1]; j++) {
                                        label[i][j][k] = -1;
                                    }
                                }
                                k++;
                                count_1 = 0;
                            }


                        }
                    }
                }
            }
        }


        for (int k = 0; k < bitmap.shape[2]; k++) {
            for (int i = 0; i < bitmap.shape[0]; i++) {
                for (int j = 0; j < bitmap.shape[1]; j++) {
                    if (label[i][j][k] == 0) {
                        if (bitmap[i][j][k] == 1) {

                            label_index++;
                            label[i][j][k] = label_index;
                            que.emplace(vec3{i, j, k});
                            //results->append(labeling_nearby(bitmap, label, que, label_index));
                        } else {
                            label[i][j][k] = -1;
                        }
                    }
                }

            }
        }
        //return *results;
    }

    int labeling_nearby(const Matrix2 &bitmap, Matrix2 &label, stack<vec2> &que, int index) {
//	    cout<<__FUNCTION__<<endl;
        static vector<vec2> direction = {{-1, 0},
                                         {1,  0},
                                         {0,  -1},
                                         {0,  1}};
        int count = 0;
        while (!que.empty()) {

            vec2 seed = que.top();
            que.pop();
            for (auto &dir : direction) {
                vec2 next = seed + dir;
                if (next >= vec2{0, 0} && next < bitmap.shape) {
                    if (label[next] == 0) {
                        if (bitmap[next] == 1) {
                            label[next] = index;
                            count++;
                            que.emplace(next);
                        } else {
                            label[next] = -1;
                        }
                    }
                }
            }
        }
        //py::list * results = new py::list;
        //for (int i = 0; i < 6; i++)
        //	 results->append(a[i]) ;
        return count;

    }

    void prelabeling_nearby(const Matrix2 &bitmap, Matrix2 &label, stack<vec2> &que, int index) {
        static const vector<vec2> direction = {{-1, 0},
                                               {1,  0},
                                               {0,  -1},
                                               {0,  1}};
        while (!que.empty()) {
            vec2 seed = que.top();
            que.pop();

            for (auto &dir : direction) {
                vec2 next = seed + dir;
                if (next >= vec2{0, 0} && next < bitmap.shape) {
                    if (label[next] == 0) {
                        if (bitmap[next] == 1) {
                            label[next] = -1;
                            que.emplace(next);
                        } else {
                            label[next] = -1;
                        }
                    }
                }
            }
        }
    }

    int ConnectedComponentLabeling(const Matrix2 &bitmap) {
//	    cout<<"labeling"<<endl;
        Matrix2 label(bitmap.shape);
        stack<vec2> que;

        int label_index = 0;
        int max_count = 0;
        for (int i = 0; i < bitmap.shape[0]; i++) {
            for (int j = 0; j < bitmap.shape[1]; j++) {
                if (label[i][j] == 0) {
                    if (bitmap[i][j] == 1) {

                        label_index++;
                        label[i][j] = label_index;
                        que.emplace(vec2{i, j});
                        int count = labeling_nearby(bitmap, label, que, label_index);
                        if (count > max_count) {
                            max_count = count;
                        }
                        //results->append(labeling_nearby(bitmap, label, que, label_index));
                    } else {
                        label[i][j] = -1;
                    }
                }
            }
        }
        return max_count;
    }
}