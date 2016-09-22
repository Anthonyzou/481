#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <unistd.h>
#include "main.h"

using namespace std;

const int numElements = 200;
const int seed = 42;
const long PROCESSORS = sysconf(_SC_NPROCESSORS_ONLN);
const long totalElements = PROCESSORS*numElements;
auto randomArr = randomArray(totalElements);


void call_from_thread(const int from, const int end){
    std::sort(&randomArr[from], &randomArr[end]);
    for(int i = from; i < end; i++){
        std::cout << randomArr[i] << std::endl;
    }
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
    time_t start, end;
    time(&start);
    for(int i = 0; i < PROCESSORS; i++) {
        threads[i] = std::thread(&call_from_thread, i*numElements, (i+1)*numElements-1);
    }

    for(int it = 0; it < PROCESSORS; it++) {
        threads[it].join();
    }

    time(&end);
    std::cout << difftime(end, start) << " seconds" << std::endl;
    return 0;
}
