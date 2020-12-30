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
//#define FPATH "/home/hcooh/fastdata/ureastepcool/ureastepcool/Ep2=%gEe2e=%g/"
#define FPATH "/home/hcooh/fastdata/IC_data/IC_isothermal_less/2020-12-14/Ep2=0.2Ep12=%gEe2e=%gT=%g/"

//#define FPATH "/home/hcooh/fastdata/eccdata_ep1/2020-11-16/Ep2=0.2Ep12=%gEe2e=%gT=%g/"
using namespace std;
void analyze0(double Ep2, double Ee2e) {  // ureastepheating
  char path0[500] = {0};
  sprintf(path0, FPATH, Ep2, Ee2e);
  cout << path0 << endl;
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
    sprintf(path, FPATH "%1.6f-0", Ep2, Ee2e, T);
    cout << path << endl;
    Room room;
    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    auto package = room.get_max_nucleus();
    auto count1 = get<0>(package);
    auto count2 = get<1>(package);
    file << T << "\t" << count1 << "\t" << count2 << endl;
    lengthfile << T << "\t" << room.get_max_straight_length() << "\t"
               << room.get_average_straight_length() << endl;
    // room.save(string(path).append("labeled"));
  }
  file.close();
}

void analyze(double Ep, double Ep12, double Ee2e) {  // stepheating
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
    sprintf(path, FPATH "Ep2=0.1Ee2e=0", Ep, Ep12, Ee2e, T);
    Room room;
    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    auto package = room.get_max_nucleus();
    auto count1 = get<0>(package);
    auto count2 = get<1>(package);
    file << T << "\t" << count1 << "\t" << count2 << "\n";
    lengthfile << T << "\t" << room.get_max_straight_length() << "\t"
               << room.get_average_straight_length() << "\n";
    // room.save(string(path).append("labeled"));
  }
  file.close();
}
void analyze2(double Ep12, double Ee2e, double T) {  // isothermal
  char path0[500] = {0};
  sprintf(path0,
          FPATH,
          Ep12, Ee2e, T);
  ofstream file(string(path0) + string("crystility"),
                std::ios::out | ios::trunc);
  ofstream lengthfile(string(path0) + string("length"),
                      std::ios::out | ios::trunc);

  for (auto n : myrange(0, 500000, 100)) {
    char path[500] = {0};
    sprintf(path,
            FPATH"%0.6f-%d",
            Ep12, Ee2e, T, T, n);
    Room room;

    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    cout << path << "\n";
    auto package = room.get_max_nucleus();
    auto count1 = get<0>(package);
    auto count2 = get<1>(package);
    file << n << "\t" << count1 << "\t" << count2 << "\n";
    lengthfile << n << "\t" << room.get_max_straight_length() << "\t" << room.get_average_straight_length() << "\n";}
  file.close();
  lengthfile.close();
}
void analyze3(double Ep12, double Ee2e, double T) {  // isothermal
  char path0[500] = {0};
#if 0
	sprintf(path0,
					"/home/hcooh/data/eccdata_ep1/2020-11-16/"
					"Ep2=0.2Ep12=%gEe2e=%gT=%g/",
					Ep12, Ee2e, T);
#else
  sprintf(path0,
          FPATH,Ep12, Ee2e, T);
#endif
  ofstream file(string(path0) + string("crystility"),
                std::ios::out | ios::trunc);
  ofstream lengthfile(string(path0) + string("length"),
                      std::ios::out | ios::trunc);
  auto Ep2 = 0.2;
  for (auto n : myrange(0, 500000, 1000)) {
    char path[500] = {0};

    auto T_true = 0.3 * Ee2e + 2.5 * Ep2 + T;
#if 0
		sprintf(path,
						"/home/hcooh/data/eccdata_ep1/2020-11-16/"
						"Ep2=0.2Ep12=%gEe2e=%gT=%g/%0.6f-%d",
						Ep12, Ee2e, T, T, n);
#else
    sprintf(path,
            FPATH"%0.6f-%d",
            Ep12, Ee2e, T, T_true, n);

#endif
    Room room;
    cout << path << endl;

    try {
      room.load(path);
    } catch (...) {
      continue;
    }
    cout << path << "\n";
    auto package = room.get_max_nucleus();
    auto count1 = get<0>(package);
    auto count2 = get<1>(package);
    file << n << "\t" << count1 << "\t" << count2 << "\n";
    lengthfile << n << "\t" << room.get_max_straight_length() << "\t"
               << room.get_average_straight_length() << "\n";
    room.transfer_to();
    room.save(string(path).append("trans"));
  }
  file.close();
  lengthfile.close();
}

int main(int argc, char *argv[]) {
  int thread_number;
  if (argc > 1) {
    thread_number = atoi(argv[1]);
  } else {
    thread_number = 12;
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
#if 0
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
  for (auto Ep12 : myrange<double>(0.6, 1.3, 0.2)) {
    for (auto Ee2e : myrange<double>(4, 9, 2)) {
       for (auto T : myrange<double>(0.1, 2.5, 0.1)) {
      int pid = fork();
      if (pid == 0) {
        sem_wait(sem);
        try {
          analyze3(Ep12, Ee2e,T);
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
