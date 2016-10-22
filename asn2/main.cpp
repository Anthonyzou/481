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
    int perProcess = 3;
    totalElements = perProcess * world.size();

    if (world.rank() == 0) {
        mainArray = randomArray(totalElements);
    }
    broadcast(world, mainArray, 0);

    auto begin = mainArray.begin()+world.rank()*perProcess;
    auto end = begin+perProcess;
    sort(begin, end);

    if (world.rank() == 0) {
        vector<vec> all_numbers;
        gather(world, mainArray, all_numbers, 0);
        stringstream s;
        for (int proc = 0; proc < world.size(); ++proc) {
            for(auto &i : all_numbers[proc]){
                s << i << endl;
            }
            s << endl;
        }
        cout << s.str();
    }
    else {
        gather(world, mainArray, 0);
    }


    return 0;
}