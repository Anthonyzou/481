#!/usr/bin/env bash


mpich(){
  pushd mpich-3.2
    ./mpich-3.2/configure --disable-fortran --prefix=$(pwd)/build
  popd
}

boost(){
  PATH=$(pwd)/mpich-3.2/build/bin:$PATH
  LD_LIBRARY_PATH=$(pwd)/mpich-3.2/build/lib:$LD_LIBRARY_PATH
  pushd boost_1_60_0
    ./bootstrap.sh
    git checkout project-config.jam
    ./b2 --with-mpi --with-serialization link=static -j 4
    ./b2 link=static install --prefix=${pwd}/build -j 4
  popd
}

make(){
  cmake -DCMAKE_BUILD_TYPE=Release -B./build -H.
}

$@
