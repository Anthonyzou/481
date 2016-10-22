#include "main.h"

#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/format.hpp>

#include <stdlib.h>

#include <list>

namespace mpi = boost::mpi;
using namespace mpi;
using namespace boost;
using namespace std;

inline void handleChunk(int idx, vec results){
}

int main(int argc, char* argv[])
{
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
    for(auto i = from; i < end; i += sampleIntervals){
        phase1Results.push_back(mainArray[i]);
    }

    vec pivots, temp;
    stringstream s;
    if (world.rank() == 0) {
        // PHASE2
        vector<vec> all_numbers;
        gather(world, phase1Results, all_numbers, 0);
        for (auto &proc : all_numbers) {
            auto size = temp.size();
            temp.insert(temp.end(), proc.begin(), proc.end());
            inplace_merge(temp.begin(), temp.begin()+size, temp.end());
        }

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
    vec results;
    if(pivots.size() > 0){
        auto pivot = pivots[idx];
        for(auto i = from; i < end; i++){
            auto k = mainArray[i];
            if(pivot < k && pivots.size() > idx){
                world.isend(idx,0,results);
                results.clear();
                ++idx;
                if(idx < pivots.size())
                    pivot = pivots[idx];
            }
            results.push_back(k);
        }
        world.send(idx,0,results);
    }

    s.str("");
    vec a;
    int messages = 0;
    while(messages < world.size()){
        auto msg = world.probe(any_source, any_tag);
        s << format("RANK %1% :: SOURCE %2% :: ")  % world.rank() % msg.source();
        world.recv(msg.source(), msg.tag(), a);
        for(auto &i :a ){
            s << i << " ";
        }
        s << endl;
        messages++;
    }
    cout << s.str();
    return 0;
}
