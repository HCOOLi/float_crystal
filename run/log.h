//
// Created by 1007 on 2019/9/12.
//

#ifndef CRYSTAL_LOG_H
#define CRYSTAL_LOG_H

class logger{
    void log(){
         auto  file=fopen("test.data","w+");
        if(file == NULL){
            return 0;
        }
         fprintf(file,"%d %d %d\n",)



    }
};



#endif //CRYSTAL_LOG_H
