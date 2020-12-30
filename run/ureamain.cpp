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
#define UREA
int main(int argc, char *argv[]) {
  int thread_number;

#ifdef UREA
  double Ep2;
  if (argc >= 2) {
    thread_number = atoi(argv[1]);
    Ep2 = atof(argv[2]);
  } else {
    thread_number = 12;
  }
  UreaCrystal simulator;

#else
  double T;
  if (argc >= 2) {
    thread_number = atoi(argv[1]);
    T = atof(argv[2]);
  } else {
    thread_number = 12;
  }
  ExtendedChainCrystal simulator;
#endif  //
  sem_t *sem;
  time_t now = time(0);
  pid_t pid = getpid();
  char sem_str[200];

  auto params = simulator.parameters3();
  for (auto p : params) {
    int pid = fork();
    if (pid == 0) {
      // sem_wait(sem);
      simulator.simulate2(p, Ep2);

      // U.simulate2(p);
      // sem_post(sem);
      return 0;
    } else {
    }
  }
  int status;
  while (wait(&status) != -1) {
    if (!WIFEXITED(status)) {
      // sem_post(sem);
    }
  }
  // sem_unlink(sem_str);
}