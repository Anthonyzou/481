#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <mutex>
#include <sstream>
#include <atomic>
#include "main.h"

using namespace std;

const int numElements = 36/3;
const int seed = 42;
const long PROCESSORS = 3;//sysconf(_SC_NPROCESSORS_ONLN);
const long totalElements = 36;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

atomic<bool> PHASE1_A[PROCESSORS];
atomic<vector<unsigned int>*> PHASE2_A;

vector<unsigned int> PHASE1_D;

mutex phase1_m;

vector<unsigned int> randomArr = {
        16,2,17,24,33,28,30,1,0,27,9,25,34,23,19,18,11,7,
        21,13,8,35,12,29,6,3,4,14,22,15,32,10,26,31,20,5
};

void mainThread(const int id, const int from, const int end){
    std::sort(&randomArr[from], &randomArr[end]);
    vector<unsigned int> phase1Arr;
    for(auto i = from; i < end; i+=sampleIntervals){
        phase1Arr.push_back(randomArr[i]);
    }
    phase1_m.lock();
    PHASE1_D.insert(PHASE1_D.end(), phase1Arr.begin(), phase1Arr.end());
    phase1_m.unlock();

    // declare phase 1 done for this thread
    atomic_thread_fence(std::memory_order_release);
    PHASE1_A[id].store(true, std::memory_order_relaxed);

    // await phase 2 from main thread
    vector<unsigned int> * i;
    do{
        i = PHASE2_A.load(std::memory_order_relaxed);
    }while(i == NULL);
    atomic_thread_fence(std::memory_order_acquire);
    cout << i->size() << endl;
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
    vector<std::thread> threads;
    cout << "NUM processors " << PROCESSORS << endl;
    // spawn threads
    for(int i = 0; i < PROCESSORS; i++) {
        threads.push_back(std::thread(&mainThread, i, i * numElements, (i + 1) * numElements));
    }

    const auto begin = std::chrono::steady_clock::now();
    // AWAIT PHASE 1
    for(int i = 0; i < PROCESSORS; i++)
        while(!PHASE1_A[i].load(std::memory_order_relaxed));
    std::atomic_thread_fence(std::memory_order_acquire);

    // BEGIN PHASE 2
    sort(PHASE1_D.begin(), PHASE1_D.end());
    for(auto &t: PHASE1_D) cout << t << " "; cout << endl;

    vector<unsigned int> * PHASE2_D = new vector<unsigned int>();
    for(auto i = PROCESSORS; i < PHASE1_D.size(); i += PROCESSORS){
        PHASE2_D->push_back(PHASE1_D[i]);
    }
    for(auto &t: *PHASE2_D) cout << t << " "; cout << endl;
    atomic_thread_fence(std::memory_order_release);
    PHASE2_A.store(PHASE2_D, std::memory_order_relaxed);

    for(auto &t: threads){
        t.join();
    }

    const auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    delete PHASE2_D;
    return 0;
}
