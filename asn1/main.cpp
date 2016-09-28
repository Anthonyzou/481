#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <mutex>
#include <sstream>
#include <future>
#include "main.h"
#include <algorithm>

using namespace std;

inline void handleChunk(int idx, vec results){
    std::unique_lock<std::mutex> lk(p4[idx]);
    p4v[idx].lock();
    phase4[idx].insert(phase4[idx].end(), results.begin(), results.end());
    p4v[idx].unlock();
    threadsDone[idx]++;
    p4CV[idx].notify_one();
}

void worker(const int id, promise<vec> prom, const int from, const int end){
    //PHASE 1
    std::sort(&randomArr[from], &randomArr[end]);
    vec phase1Arr;
    for(auto i = from; i < end; i += sampleIntervals){
        phase1Arr.push_back(randomArr[i]);
    }
    prom.set_value(phase1Arr);

    //PHASE 3
    auto idx = 0;
    auto pivots = phase2Vector.get();
    auto pivot = pivots[idx];
    vec results;
    for(auto i = from; i < end; i++){
        auto k = randomArr[i];
        if(pivot < k && pivots.size() > idx){
            handleChunk(idx, results);
            results.clear();
            pivot = pivots[++idx];
        }
        results.push_back(k);
    }
    handleChunk(idx, results);

    std::unique_lock<std::mutex> lk(p4[id]);
    p4CV[id].wait(lk, [id](){return threadsDone[id] == PROCESSORS;});

    //PHASE 4 SORT
    merge_sort(phase4[id].begin(), phase4[id].end());
}

int main() {
    vector<std::thread> threads;
    vector<future<vec>> phase1Results;
    pthread_setconcurrency(PROCESSORS);
    vec results, subResults, phase4Results;

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
    for(auto &result: phase1Results){
        auto subArrays = result.get();
        results.insert(results.end(), subArrays.begin(), subArrays.end());
    }
    sort(results.begin(), results.end());

    //PHASE 2 GET PIVOT POINTS
    auto k = 0;
    for(auto i = PROCESSORS; k++ < PROCESSORS-1; i += PROCESSORS)
        subResults.push_back(results[i]);

    phase2Promise.set_value(subResults);

    //END PHASE 4
    for(auto &it : threads) it.join();
    auto end = chrono::steady_clock::now();

    //COMBINE RESULTS FROM PHASE 4
    stringstream s;
    for(auto &id : phase4){
        phase4Results.insert(phase4Results.end(), id.begin(), id.end());
//        for(auto &t : id){
//
//            s << t << " " ;
//        }
    }

//    s << endl;
    s << is_sorted(phase4Results.begin(), phase4Results.end()) << endl;

    s << "Time difference = " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << std::endl;
    cout << s.str();
    return 0;
}
