#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <unistd.h>
#include <mutex>
#include <sstream>
#include <future>
#include "main.h"

using namespace std;

typedef vector<unsigned long> vec;

const int numElements = 36/3;
const int seed = 42;
const long PROCESSORS = 3;
//const unsigned procs = thread::hardware_concurrency();
const long totalElements = 36;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

promise<vec> phase2Promise;
shared_future<vec> phase2Vector(phase2Promise.get_future());

vec phase4[PROCESSORS];
std::mutex p4, p4v[PROCESSORS];
std::condition_variable cv;
int threadsDone = 0 ;

vec randomArray(long size){
    auto a = default_random_engine(seed);
    vec v;
    for(int i = 0; i < size; i++) {
        v.push_back(a());
    }
    return v;
}

vec randomArr = {
        16,2,17,24,33,28,30,1,0,27,9,25,34,23,19,18,11,7,
        21,13,8,35,12,29,6,3,4,14,22,15,32,10,26,31,20,5
};

void mainThread(const int id, promise<vec> prom,const int from, const int end){
    //PHASE 1
    std::sort(&randomArr[from], &randomArr[end]);
    vec phase1Arr;
    for(auto i = from; i < end; i+=sampleIntervals){
        phase1Arr.push_back(randomArr[i]);
    }
    prom.set_value(phase1Arr);

    //PHASE 3
    auto pivots = phase2Vector.get();
    auto pivot = (pivots.front());
    auto idx = 0;
    vec results;
    for(auto i = from; i < end; i++){
        auto k = randomArr[i];
        if(pivot < k && pivots.size() > 0){
            pop_front(pivots);
            pivot = (pivots.front());

            p4v[idx].lock();
            phase4[idx].insert(phase4[idx].end(), results.begin(), results.end());
            p4v[idx].unlock();

            results.clear();
            idx++;
        }
        results.push_back(k);
    }
    p4v[idx].lock();
    phase4[idx].insert(phase4[idx].end(), results.begin(), results.end());
    p4v[idx].unlock();

    {
        std::unique_lock<std::mutex> lk(p4);
        threadsDone++;
        cv.notify_all();
    }

    std::unique_lock<std::mutex> lk(p4);
    cv.wait(lk, [](){return threadsDone == PROCESSORS;});

    //PHASE 4 SORT
    sort(phase4[id].begin(), phase4[id].end());
}

int main() {
    vector<std::thread> threads;
    vector<future<vec>> phase1Results;

    cout << "NUM processors " << PROCESSORS << endl;
    auto begin = std::chrono::steady_clock::now();

    //CREATE THREADS
    for(int i = 0; i < PROCESSORS; i++) {
        promise<vec> phase1Prom;
        auto f = phase1Prom.get_future();
        phase1Results.push_back(move(f));
        threads.push_back(std::thread(&mainThread, i, move(phase1Prom), i * numElements, (i + 1) * numElements));
    }

    //PHASE 2
    vec results, subResults;
    for(auto &result: phase1Results){
        auto subArrays = result.get();
        results.insert(results.end(), subArrays.begin(), subArrays.end());
    }
    sort(results.begin(), results.end());

    //PHASE 2 GET PIVOT POINTS
    for(auto i = PROCESSORS; i < results.size(); i += PROCESSORS){
        subResults.push_back(results[i]);
    }
    phase2Promise.set_value(subResults);

    //END PHASE 4
    for(auto &it : threads) {
        it.join();
    }

    //COMBINE RESULTS FROM PHASE 4
    for(int id = 0; id < PROCESSORS; id++)
        for(auto &t : phase4[id]){
            cout << t << " " ;
        }
    cout << endl;

    auto end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;
}
