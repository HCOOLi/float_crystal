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

using namespace std;
void analyze(double Ep, double Ee2e) {
    char path0[500] = {0};

    sprintf(path0,
            "/home/hcooh/projects/float_crystal/data/30datastepheat/"
            "2020-11-8/Ep2=%1gEe2e=%1.0f/",
            Ep, Ee2e);
    ofstream file(string(path0) + string("crystility"),
                  std::ios::out | ios::trunc);
    if (!file) {
        throw "create file failed";
    }

    for (auto T : myrange<double>(0.1, 5.1, 0.1)) {
        char path[500] = {0};
        sprintf(path,
                "/home/hcooh/projects/float_crystal/data/30datastepheat/"
                "2020-11-8/Ep2=%1gEe2e=%1.0f/%0.6f-4",
                Ep, Ee2e, T);
        Room room;
        cout << path << endl;
        room.load(path);
        room.Ep_matrix[0][0] = room.Ep_matrix[2][2];
        room.Ep_matrix[0][2] = room.Ep_matrix[2][2];
        room.Ep_matrix[2][0] = room.Ep_matrix[2][2];

        file << T << "\t" << room.get_max_nucleus() << "\n";
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
        } catch(...) {
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
        } catch(...) {
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
#if 0
    for (auto Ep2 : myrange<double>(0.2, 1.1, 0.2)) {
        for (auto Ee2e : myrange<double>(4, 11, 2)) {
            int pid = fork();
            if (pid == 0) {
                sem_wait(sem);
                try {
                    analyze(Ep2, Ee2e);
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
