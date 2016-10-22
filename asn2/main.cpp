// Anthony Ou
// 1248175
// cmput 481
// nov 2016 - asn 2

#include "main.h"

#include <boost/mpi.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>

#include <list>

namespace mpi = boost::mpi;
using namespace mpi;
using namespace boost;
using namespace std;
using namespace chrono;
stringstream s;

void phase1(const int from, const int end, vec * phase1Results){
    sort(randomArr.begin()+from, randomArr.begin()+end);
    for(auto i = from; i < end; i += sampleIntervals)
        phase1Results->push_back(randomArr[i]);
}

void phase2(const communicator world, vec * phase1Results, vec * pivots){
    vec temp;
    vector<vec> all_samples;
    if (world.rank() == 0) {
        // PHASE2
        gather(world, *phase1Results, all_samples, 0);
        for (auto &proc : all_samples) {
            sortedMerge(&temp, &proc);
        }
        const int j = floor(world.size()/2);
        for(auto i = world.size()+j-1, k = 0; k++ < world.size()-1; i += world.size()+j)
            pivots->push_back(temp[i]);

        // Broadcast begins phase 3
        broadcast(world, *pivots, 0);
    }
    else
        gather(world, *phase1Results, 0);
}

void phase3(const int from, const int end, communicator world, vec * pivots, vec * result){
    vec temp;
    // Recieve a broadcast of the pivots
    broadcast(world, *pivots, 0);

    // Build partitions and send them to the other processes
    auto idx = 0;
    auto movingIt = randomArr.begin()+from;
    for(auto &pivot : *pivots){
        auto nextPoint = partition(movingIt, randomArr.begin()+end, [pivot](vecType em){ return em <= pivot; });
        world.isend(idx++, 0, vec(movingIt, nextPoint));
        movingIt = nextPoint;
    }
    world.isend(idx, 0, vec(movingIt, randomArr.begin()+end));

    // recieve the partitions and send concat them.
    for(int messages = 0; messages < world.size(); messages++){
        auto msg = world.probe(messages, 0);
        world.recv(msg.source(), msg.tag(), temp);
        sortedMerge(result, &temp);
    }
}

void phase4(communicator world, vec * result, vec * finalResults){
    if(world.rank() == 0){
        vector<vec> all_numbers;
        gather(world, *result, all_numbers, 0);
        for(auto &nums : all_numbers) sortedMerge(finalResults, &nums);
    }
    else
        gather(world, *result, 0);
}

int main(int argc, char ** argv) {

    init(argc, argv);
    environment env(argc, argv);
    communicator world;

    if (world.rank() == 0)
//        randomArr = randomArray(totalElements);
        randomArr = {16,2,17,24,33,28,30,1,0,27,9,25,34,23,19,18,11,7, 21,13,8,35,12,29,6,3,4,14,22,15,32,10,26,31,20,5};

    // Give the array to everyone
    broadcast(world, randomArr, 0);

    // KEEP THIS BELOW THE BROADCAST
    perProcess = randomArr.size()/world.size();
    sampleIntervals = floor(randomArr.size()/(world.size()*world.size()));

    const int from = world.rank()*perProcess;
    int end = from+perProcess;
    if(world.rank() == world.size()-1)
        end = randomArr.size();

    auto start = std::chrono::steady_clock::now();

    // PHASE 1
    vec phase1Results;
    phase1(from, end, &phase1Results);

    // PHASE 2
    vec pivots, temp;
    phase2(world, &phase1Results, &pivots);

    // PHASE 3
    vec result, finalResults;
    phase3(from, end, world, &pivots, &result);

    // PHASE 4
    phase4(world, &result, &finalResults);

    if(world.rank() == 0){

        auto sorted = is_sorted(finalResults.begin(), finalResults.end());
        cout << format("%1%\nsorted: %2%\noriginalsize: %3%\nfinalsize: %4%\n") %
                duration_cast<time_u>(steady_clock::now() - start).count() % (sorted?"true":"false") %
                randomArr.size() % finalResults.size();
        return sorted ? 0 : 1;
    }
    else
        return 0;
}
