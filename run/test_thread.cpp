//
// Created by 1007 on 2019/8/27.
//

#include <iostream>
#include "threadpool.h"
using namespace std;



int main(){
    ThreadPool pool(8);
    for(int i=0;i<10;i++){
        pool.enqueue([i]{cout<<i<<endl;});
    }
}



