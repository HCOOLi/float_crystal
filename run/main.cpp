#include "simulator.cpp"
#include<unistd.h>
#include<semaphore.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include<ctime>
#include <unistd.h>
#include<string.h>




int main(int argc, char *argv[]) {
    int thread_number;
    if (argc > 1) {
        thread_number = atoi(argv[1]);
    } else {
        thread_number = 12;
    }//
    //process
    sem_t *sem;
    time_t now= time(0);
    pid_t pid=getpid();
    char sem_str[200];
    sprintf(sem_str,"sem-%ld-%d",now,pid);
    sem=sem_open(sem_str,O_CREAT|O_RDWR,0744,thread_number);
    if(sem==SEM_FAILED){
        cout<<"failed"<<endl;
    }else{
        sem=sem_open(sem_str,O_CREAT|O_RDWR);
        cout<<"succeed"<<endl;
    }


    
    //ThreadPool pool(thread_number);
    UreaCrystal U;
    auto params = U.parameters2();
    //vector<int> waitinglist;
    for (auto p : params) {
        //process
        sem_wait(sem);
        sem_post(sem);
        int pid=fork();
        //cout<<pid<<endl;
        if(pid==0){
            sem_wait(sem);
            U.simulate2(p);
            sem_post(sem);
            return 0;
        }else{
            //waitinglist.push_back(pid);
        }
        //pool.enqueue(bind(&ExtendedChainCrystal::simulate, E, p));
    }
    int status;
    while(wait(&status)!=-1);
    sem_unlink(sem_str);
    // for(auto id:waitinglist){
    //     wait();
    // }
}
