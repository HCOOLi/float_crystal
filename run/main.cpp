#include "simulator.cpp"

int main(int argc, char *argv[]) {
    int thread_number;
    if (argc > 1) {
        thread_number = atoi(argv[1]);
    } else {
        thread_number = 12;
    }
    ThreadPool pool(thread_number);
    ExtendedChainCrystal E;
    auto params = E.parameters();
    for (auto p : params) {
        pool.enqueue(bind(&ExtendedChainCrystal::simulate, E, p));
    }
}
