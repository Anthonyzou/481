#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <mutex>
#include <sstream>
#include "main.h"

using namespace std;

const int numElements = 2;
const int seed = 42;
const long PROCESSORS = sysconf(_SC_NPROCESSORS_ONLN);
const long totalElements = PROCESSORS*numElements;
auto randomArr = randomArray(totalElements);

std::mutex mtx;


void call_from_thread(const int from, const int end){
    mtx.lock();
    static int tid = 0;
    cout << tid << endl;
    tid++;
    mtx.unlock();

    std::sort(&randomArr[from], &randomArr[end]);
    std::stringstream msg;

    for(int i = from; i < end; i++){
        msg << randomArr[i] << "\n";
    }
    cout << msg.str();
}

std::vector<unsigned long> randomArray(long size){
    auto a = default_random_engine(seed);
    std::vector<unsigned long> vec;
    for(int i = 0; i < size; i++) {
        vec.push_back(a());
    }
    return vec;
}

int main() {
    std::thread threads[PROCESSORS];
    cout << "NUM processors " << PROCESSORS << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for(int i = 0; i < PROCESSORS; i++) {
        threads[i] = std::thread(&call_from_thread, i*numElements, (i+1)*numElements);
    }

    for(int it = 0; it < PROCESSORS; it++) {
        threads[it].join();
    }

    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;
}
