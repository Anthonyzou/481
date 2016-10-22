#include "main.h"

#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/join.hpp>
#include <stdlib.h>

#include <list>

namespace mpi = boost::mpi;
using namespace mpi;
using namespace boost;
using namespace std;

template <typename t>
inline void sortedMerge(vector<t> * result, vector<t> * a){
    auto size = result->size();
    result->insert(result->end(), a->begin(), a->end());
    inplace_merge(result->begin(), result->begin()+size, result->end());
}

int main(int argc, char ** argv) {
    auto start = chrono::steady_clock::now();

    init(argc, argv);
    environment env(argc, argv);
    communicator world;

    vec mainArray;

    if (world.rank() == 0) {
        // mainArray = randomArray(totalElements);
        mainArray = {
            16,2,17,24,33,28,30,1,0,27,9,25,34,23,19,18,11,7,
            21,13,8,35,12,29,6,3,4,14,22,15,32,10,26,31,20,5
        };
    }

    broadcast(world, mainArray, 0);

    int perProcess = 12;
    totalElements = mainArray.size();
    auto sampleIntervals = floor(totalElements/(world.size()*world.size()));

    const auto from = world.rank()*perProcess;
    const auto end = from+perProcess;

    // PHASE 1
    sort(mainArray.begin()+from, mainArray.begin()+end);
    vec phase1Results;
    for(auto i = from; i < end; i += sampleIntervals)
        phase1Results.push_back(mainArray[i]);

    vec pivots, temp;
    stringstream s;
    if (world.rank() == 0) {
        // PHASE2
        vector<vec> all_numbers;
        gather(world, phase1Results, all_numbers, 0);
        for (auto &proc : all_numbers)
            sortedMerge(&temp, &proc);

        for(auto i = world.size(), k = 0; k++ < world.size()-1; i += world.size())
            pivots.push_back(temp[i]);

        broadcast(world, pivots, 0);
    }
    else {
        gather(world, phase1Results, 0);
    }

    // phase 3
    broadcast(world, pivots, 0);

    auto idx = 0;
    vec::iterator a = mainArray.begin()+from;
    for(auto &pivot : pivots){
        auto b = partition(a, mainArray.begin()+end, [pivot](vecType em){ return em <= pivot; });
        vec l(a, b);
        (world.isend(idx++,0,l));
        s.str("");
        a = b;
    }
    auto b = partition(a, mainArray.begin()+end, [](vecType em){ return true;});
    vec l(a, b);
    (world.isend(idx,0,l));

    s.str("");
    cout << endl;
    vec  result;
    int messages = 0;
    s << format("PHASE 4 :: RANK %1% :: SOURCE %2% :: ")  % world.rank() % 0;
    while(messages < world.size()){
        auto msg = world.probe(messages++, 0);
        vec a;
        world.recv(msg.source(), msg.tag(), a);
        sortedMerge(&result, &a);
    }
    std::copy(result.begin(), result.end(), std::ostream_iterator<int>(s, " "));
    s << endl;
    cout << s.str();
    return 0;
}
