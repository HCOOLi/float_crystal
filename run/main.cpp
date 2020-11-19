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
    // UreaCrystal U;
    // auto params = U.parameters2();
    //vector<int> waitinglist;
    ExtendedChainCrystal E;
    auto params = E.parameters();
    //int i=0;
    for (auto p : params) {      
        int pid=fork();
        if(pid==0){
            sem_wait(sem);
            E.simulate2(p);

            // U.simulate2(p);
            sem_post(sem);
            return 0;
        }else{
           
        }
       
    }
    int status;
    while(wait(&status)!=-1){
        if(!WIFEXITED(status) ){
            sem_post(sem);
        }
    }
    sem_unlink(sem_str);
}
