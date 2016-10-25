#!/usr/bin/env bash

if [ ! -f "boost/bootstrap.sh" ]; then
  version="boost_1_60_0"
  tar -jxf "${version}.tar.bz2"
  mv "${version}/*" boost
  rm -rf ${version}
fi

cd boost
./bootstrap.sh
git checkout project-config.jam
./b2 --with-mpi --with-serialization -j 4
./b2 install --prefix=${pwd}/build -j 4
cd ..
cmake -DCMAKE_BUILD_TYPE=Release -B./build -H.
