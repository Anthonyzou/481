#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <mutex>
#include <sstream>
#include <future>
#include "main.h"

using namespace std;

inline void handleChunk(int idx, vec results){
    std::unique_lock<std::mutex> lk(p4[idx]);
    p4v[idx].lock();
    phase4[idx].insert(phase4[idx].end(), results.begin(), results.end());
    p4v[idx].unlock();
    threadsDone[idx]++;
    cv[idx].notify_one();
}

void worker(const int id, promise<vec> prom, const int from, const int end){
    //PHASE 1
    std::sort(&randomArr[from], &randomArr[end]);
    vec phase1Arr;
    for(auto i = from; i < end; i+=sampleIntervals){
        phase1Arr.push_back(randomArr[i]);
    }
    prom.set_value(phase1Arr);

    //PHASE 3
    auto pivots = phase2Vector.get();
    auto idx = 0;
    auto pivot = pivots[idx];
    vec results;
    for(auto i = from; i < end; i++){
        auto k = randomArr[i];
        if(pivot < k && pivots.size() > idx){
            handleChunk(idx, results);
            results.clear();
            pivot = pivots[idx++];
        }
        results.push_back(k);
    }
    handleChunk(idx, results);

    std::unique_lock<std::mutex> lk(p4[id]);
    cv[id].wait(lk, [id](){return threadsDone[id] == PROCESSORS;});

    //PHASE 4 SORT
    sort(phase4[id].begin(), phase4[id].end());
}

int main() {
    vector<std::thread> threads;
    vector<future<vec>> phase1Results;

    cout << "NUM processors " << thread::hardware_concurrency() << endl;
    auto begin = std::chrono::steady_clock::now();

    //CREATE THREADS
    for(int i = 0; i < PROCESSORS; i++) {
        promise<vec> phase1Prom;
        auto f = phase1Prom.get_future();
        phase1Results.push_back(move(f));
        threads.push_back(std::thread(&worker, i, move(phase1Prom), i * numElements, (i + 1) * numElements));
    }

    //PHASE 2
    vec results, subResults;
    for(auto &result: phase1Results){
        auto subArrays = result.get();
        results.insert(results.end(), subArrays.begin(), subArrays.end());
    }
    sort(results.begin(), results.end());

    //PHASE 2 GET PIVOT POINTS
    for(auto i = PROCESSORS; i < results.size(); i += PROCESSORS)
        subResults.push_back(results[i]);

    phase2Promise.set_value(subResults);

    //END PHASE 4
    for(auto &it : threads) it.join();
    auto end = chrono::steady_clock::now();

    //COMBINE RESULTS FROM PHASE 4
    for(auto &id : phase4)
        for(auto &t : id)
            cout << t << " " ;

    cout << endl;

    std::cout << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << std::endl;
    return 0;
}
