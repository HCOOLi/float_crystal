#include "simulator.cpp"
#include<unistd.h>
#include<semaphore.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>


int main(int argc, char *argv[]) {
    int thread_number;
    if (argc > 1) {
        thread_number = atoi(argv[1]);
    } else {
        thread_number = 40;
    }//
    //process
    sem_t *sem;
    sem_unlink("myipc");
    sem=sem_open("myipc",O_CREAT|O_RDWR,0744,30);
    if(sem==SEM_FAILED){
        cout<<"failed"<<endl;
    }else{
        sem=sem_open("myipc",O_CREAT|O_RDWR);
        cout<<"succeed"<<endl;
    }


    
    //ThreadPool pool(thread_number);
    ExtendedChainCrystal E;
    auto params = E.parameters();
    //vector<int> waitinglist;
    for (auto p : params) {
        //process
        int pid=fork();
        //cout<<pid<<endl;
        if(pid==0){
            sem_wait(sem);
            E.simulate(p);
            sem_post(sem);
            return 0;
        }else{
            //waitinglist.push_back(pid);
        }
        //pool.enqueue(bind(&ExtendedChainCrystal::simulate, E, p));
    }
    int status;
    while(wait(&status)!=-1);
    sem_unlink("myipc");
    // for(auto id:waitinglist){
    //     wait();
    // }
}
