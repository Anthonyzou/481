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
const long PROCESSORS = 3;//sysconf(_SC_NPROCESSORS_ONLN);
const long totalElements = 36;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

std::promise<vec> phase2Promise;
std::shared_future<vec> phase2Vector(phase2Promise.get_future());

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

void mainThread(promise<vec> p, const int from, const int end){
    std::sort(&randomArr[from], &randomArr[end]);
    vec phase1Arr;
    for(auto i = from; i < end; i+=sampleIntervals){
        phase1Arr.push_back(randomArr[i]);
    }
    p.set_value(phase1Arr);

    //PHASE 2 RESULTS
    stringstream s;
    for(auto &i: phase1Arr){
        s << i << " ";
    }
    s << endl;

    auto pivots = phase2Vector.get();
    for(auto &i: pivots){
        s<< i << " ";
    }
    s << endl;
    cout << s.str();

}

int main() {
    std::thread threads[PROCESSORS];
    cout << "NUM processors " << PROCESSORS << endl;
    auto begin = std::chrono::steady_clock::now();

    //PHASE ONE
    vector<future<vec>> phase1Results;
    for(int i = 0; i < PROCESSORS; i++) {
        promise<vec> p;
        auto f = p.get_future();
        phase1Results.push_back(move(f));
        threads[i] = std::thread(&mainThread, move(p), i * numElements, (i + 1) * numElements);
    }

    //PHASE TWO
    vec results, subResults;
    for(auto &t: phase1Results){
        auto subArrays = t.get();
        results.insert(results.end(), subArrays.begin(), subArrays.end());
    }
    sort(results.begin(), results.end());

    for(auto i = PROCESSORS; i < results.size(); i += PROCESSORS){
        subResults.push_back(results[i]);
    }
    phase2Promise.set_value(subResults);


    for(int it = 0; it < PROCESSORS; it++) {
        threads[it].join();
    }

    auto end= std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() <<std::endl;
    return 0;
}
