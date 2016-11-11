// Anthony Ou
// CMPUT 481
// Oct 5, 2016

// Created by azou on 28/09/16.

#include "asn1.h"

using namespace std;

atomic<long> phase3, phase4_t;

inline void handleChunk(int idx, vec results) {
    unique_lock<mutex> lk(p4[idx]);
    auto size = phase4[idx].size();
    phase4[idx].insert(phase4[idx].end(), results.begin(), results.end());
    inplace_merge(phase4[idx].begin(), phase4[idx].begin() + size, phase4[idx].end());
}

void worker(const int id, promise<vec> prom, const int from, const int end) {
    //PHASE 1
    sort(randomArr.begin(), randomArr.end());
    vec phase1Arr;
    for (auto i = from; i < end; i += sampleIntervals) {
        phase1Arr.push_back(randomArr[i]);
    }
    prom.set_value(phase1Arr);

    //PHASE 3 partition and assign
    auto idx = 0;
    auto pivots = phase2Vector.get();
    auto PHASE3START = chrono::steady_clock::now();
    auto movingIt = randomArr.begin() + from;
    auto endPoint = randomArr.begin() + end;
    for (auto &pivot : pivots) {
        auto nextPoint = partition(movingIt, endPoint, [pivot](vecType em) { return em <= pivot; });
        handleChunk(idx, vec(movingIt, nextPoint));
        movingIt = nextPoint;
        idx++;
    }
    handleChunk(idx, vec(movingIt, endPoint));

    phase3 += chrono::duration_cast<time_u>(chrono::steady_clock::now() - PHASE3START).count();
}

int main(int argc, char **argv) {
    init(argc, argv);

    vector<thread> threads;
    vector<future<vec>> phase1Results;
    vec results, subResults, phase4Results;

//    cout << "NUM processors " << PROCESSORS << endl << "totalElements " << totalElements << endl;

    auto begin = chrono::steady_clock::now();
    //CREATE THREADS
    for (int i = 0; i < PROCESSORS; i++) {
        promise<vec> phase1Prom;
        auto f = phase1Prom.get_future();
        phase1Results.push_back(move(f));
        threads.push_back(thread(&worker, i, move(phase1Prom), i * numElements, (i + 1) * numElements));
    }

    //PHASE 2
    for (auto &result: phase1Results) {
        auto subArrays = result.get();
        results.insert(results.end(), subArrays.begin(), subArrays.end());
    }
    cout << chrono::duration_cast<time_u>(chrono::steady_clock::now() - begin).count() << ",";

    auto PHASE2START = chrono::steady_clock::now();
    sort(results.begin(), results.end());

    //PHASE 2 GET PIVOT POINTS
    auto k = 1;
    const int p = (int) floor(PROCESSORS/2);
    for (auto i = PROCESSORS+p; k++ < PROCESSORS; i = (PROCESSORS*k)+p)
        subResults.push_back(results[i]);

    phase2Promise.set_value(subResults);
    cout << chrono::duration_cast<time_u>(chrono::steady_clock::now() - PHASE2START).count() << ",";

    //END PHASE 4
    for (auto &it : threads) it.join();
    auto end = chrono::steady_clock::now();

    cout << phase3 / PROCESSORS << ",";

    auto PHASE4START = chrono::steady_clock::now();

    //COMBINE RESULTS FROM PHASE 4
    for (auto &id : phase4)
        phase4Results.insert(phase4Results.end(), id.begin(), id.end());
    cout << chrono::duration_cast<time_u>(chrono::steady_clock::now() - PHASE4START).count() << ",";


    cout << chrono::duration_cast<time_u>(end - begin).count() << endl;
    return ((is_sorted(phase4Results.begin(), phase4Results.end()) == 1) ? 0 : 1);
}
