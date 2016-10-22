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

//    cout << boost::format("writing %1%,  x=%2% : %3%-th try\n") % "toto" % 40.23 % 50;
// prints "writing toto,  x=40.230 : 50-th try"

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

    const auto begin = world.rank()*perProcess;
    const auto end = begin+perProcess;

    sort(mainArray.begin()+begin, mainArray.begin()+end);
    vec phase1Results;
    for(auto i = begin; i < end; i += sampleIntervals){
        phase1Results.push_back(mainArray[i]);
    }

    cout << format("rank %1%: %2% %3%\n") % world.rank() % begin % end ;

    if (world.rank() == 0) {
        cout << sampleIntervals << endl;
        vector<vec> all_numbers;
        gather(world, phase1Results, all_numbers, 0);
        stringstream s;
        for (auto &proc : all_numbers) {
            for(auto &i : proc){
                s << format("%1% ") %i;
            }
            s << format(" size: %1% \n") % proc.size();
        }
        cout << s.str();
    }
    else {
        gather(world, phase1Results, 0);
    }

    return 0;
}
