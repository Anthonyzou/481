#include "main.h"

#include <boost/mpi.hpp>
#include <iostream>
#include <string>
#include <boost/serialization/string.hpp>
namespace mpi = boost::mpi;
using namespace mpi;

int main(int argc, char* argv[])
{
    auto start = chrono::steady_clock::now();

    environment env(argc, argv);
    communicator world;
    if (world.rank() == 0) {
        request reqs[2];
        std::string msg, out_msg = "Hello";
        reqs[0] = world.isend(1, 0, out_msg);
        reqs[1] = world.irecv(1, 1, msg);
        wait_all(reqs, reqs + 2);
        std::cout << msg << "!";
    } else {
        request reqs[2];
        std::string msg, out_msg = "world";
        reqs[0] = world.isend(0, 1, out_msg);
        reqs[1] = world.irecv(0, 0, msg);
        wait_all(reqs, reqs + 2);
        std::cout << msg << ", ";
    }

//    std::cout<< endl;
    std::string value;

    if (world.rank() == 0) {
        value = "Hello, World!";
    }

    broadcast(world, value, 0);

    std::cout << "Process #" << world.rank() << " says " << value
              << std::endl;

    return 0;
}