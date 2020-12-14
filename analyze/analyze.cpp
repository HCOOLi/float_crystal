#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../cpp/room.h"
#include "../cpp/utils.h"
#define FPATH "/home/zy/data/IC_stepcoolingless/Ep2=%gEp12=%gEe2e=%g/"

using namespace std;
void analyze(double Ep, double Ep12, double Ee2e) {
  char path0[500] = {0};

  sprintf(path0, FPATH, Ep, Ep12, Ee2e);
  ofstream file(string(path0) + string("crystility"),
                std::ios::out | ios::trunc);
  ofstream lengthfile(string(path0) + string("length"),
                      std::ios::out | ios::trunc);
  if (!file) {
    cout << string(path0) + string("crystility") << endl;
    throw "create file failed";
  }

  for (auto T : myrange<double>(0.1, 10, 0.01)) {
    char path[500] = {0};
    sprintf(path, FPATH "%0.6f-0", Ep, Ep12, Ee2e, T);
    Room room;
    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    file << T << "\t" << room.get_max_nucleus() << "\n";
    lengthfile << T << "\t" << room.get_max_straight_length() << "\t"
               << room.get_average_straight_length() << "\n";
    room.save(string(path).append("labeled"));
  }
  file.close();
}
void analyze2(double Ep12, double Ee2e, double T) {
  char path0[500] = {0};
  sprintf(path0,
          "/home/hcooh/data/eccdata_ep1/2020-11-16/"
          "Ep2=0.2Ep12=%gEe2e=%gT=%g/",
          Ep12, Ee2e, T);
  ofstream file(string(path0) + string("crystility"),
                std::ios::out | ios::trunc);
  ofstream lengthfile(string(path0) + string("length"),
                      std::ios::out | ios::trunc);

  for (auto n : myrange(0, 500000, 100)) {
    char path[500] = {0};
    sprintf(path,
            "/home/hcooh/data/eccdata_ep1/2020-11-16/"
            "Ep2=0.2Ep12=%gEe2e=%gT=%g/%0.6f-%d",
            Ep12, Ee2e, T, T, n);
    Room room;

    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    cout << path << "\n";
    file << n << "\t" << room.get_max_nucleus() << "\n";
    lengthfile << n << "\t" << room.get_max_straight_length() << "\t"
               << room.get_average_straight_length() << "\n";
    room.save(string(path).append("labeled"));
  }
  file.close();
  lengthfile.close();
}
void analyze3(double Ep12, double Ee2e, double T) {
  char path0[500] = {0};
  sprintf(path0,
          "/home/hcooh/data/eccdata_ep1/2020-11-16/"
          "Ep2=0.2Ep12=%gEe2e=%gT=%g/",
          Ep12, Ee2e, T);
  // ofstream file(string(path0) + string("crystility"),
  //               std::ios::out | ios::trunc);
  // ofstream lengthfile(string(path0) + string("length"),
  //                     std::ios::out | ios::trunc);

  for (auto n : myrange(0, 500000, 100)) {
    char path[500] = {0};
    sprintf(path,
            "/home/hcooh/data/eccdata_ep1/2020-11-16/"
            "Ep2=0.2Ep12=%gEe2e=%gT=%g/%0.6f-%d",
            Ep12, Ee2e, T, T, n);
    Room room;

    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    cout << path << "\n";
    // file << n << "\t" << room.get_max_nucleus() << "\n";
    // lengthfile << n << "\t" << room.get_max_straight_length() << "\t"
    //            << room.get_average_straight_length() << "\n";
    room.transfer_to();
    room.save(string(path).append("trans"));
  }
  // file.close();
  // lengthfile.close();
}

int main(int argc, char *argv[]) {
  int thread_number;
  if (argc > 1) {
    thread_number = atoi(argv[1]);
  } else {
    thread_number = 100;
  }
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
#if 1
  for (auto Ep2 : vector<double>{0.2, 0.4}) {
    for (auto Ep12 : myrange<double>(0.4, 1.3, 0.2)) {
      for (auto Ee2e : vector<double>{4, 6, 8, 12}) {
        int pid = fork();
        if (pid == 0) {
          sem_wait(sem);
          try {
            analyze(Ep2, Ep12, Ee2e);
          } catch (char const *err) {
            cerr << err << endl;
          } catch (...) {
          }
          sem_post(sem);
          return 0;
        } else {
        }
      }
    }
  }
#else
  for (auto Ep12 : myrange<double>(0.2, 1.1, 0.2)) {
    for (auto Ee2e : myrange<double>(4, 11, 2)) {
      for (auto T : myrange<double>(0.5, 2.5, 0.2)) {
        int pid = fork();
        if (pid == 0) {
          sem_wait(sem);
          try {
            analyze3(Ep12, Ee2e, T);
          } catch (char const *err) {
            cerr << err << endl;
          } catch (...) {
          }
          sem_post(sem);
          return 0;
        } else {
        }
      }
    }
  }
#endif
  int status;
  while (wait(&status) != -1) {
    if (!WIFEXITED(status)) {
      sem_post(sem);
    }
  }
  sem_unlink(sem_str);
}
