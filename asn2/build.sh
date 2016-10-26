#!/usr/bin/env bash


PATH=$PATH:$(pwd)/mpich-3.2/build/bin
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/mpich-3.2/build/include
# cd boost
# ./bootstrap.sh
# git checkout project-config.jam
# ./b2 --with-mpi --with-serialization link=static -j 4
# ./b2 install --prefix=${pwd}/build -j 4
# cd ..
cmake -DCMAKE_BUILD_TYPE=Release -B./build -H.
