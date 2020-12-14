#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <ctime>

#include "simulator.cpp"

int main(int argc, char *argv[]) {
  int thread_number;
  double T;
  if (argc >= 2) {
    thread_number = atoi(argv[1]);
    T = atof(argv[2]);
  } else {
    thread_number = 20;
  }  //
  // process
  sem_t *sem;
  time_t now = time(0);
  pid_t pid = getpid();
  char sem_str[200];
  sprintf(sem_str, "sem-%ld-%d", now, pid);
  sem = sem_open(sem_str, O_CREAT | O_RDWR, 0744, thread_number);
  if (sem == SEM_FAILED) {
    cout << "failed" << endl;
  } else {
    sem = sem_open(sem_str, O_CREAT | O_RDWR);
    cout << "succeed" << endl;
  }
  // ThreadPool pool(thread_number);
  // UreaCrystal U;
  // auto params = U.parameters2();
  // vector<int> waitinglist;
  ExtendedChainCrystal E;
  auto params = E.parameters2();
  // int i=0;
  for (auto p : params) {
    int pid = fork();
    if (pid == 0) {
      sem_wait(sem);
      E.simulate3(p, T);

      // U.simulate2(p);
      sem_post(sem);
      return 0;
    } else {
    }
  }
  int status;
  while (wait(&status) != -1) {
    if (!WIFEXITED(status)) {
      sem_post(sem);
    }
  }
  sem_unlink(sem_str);
}
