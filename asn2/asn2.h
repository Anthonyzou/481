// Anthony Ou
// 1248175
// cmput 481
// nov 2016 - asn 2

#pragma once

#include <iterator>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <thread>
#include <random>
#include <climits>
#include <sstream>
#include <vector>

#include <boost/mpi.hpp>
#include <boost/format.hpp>

using namespace std;

// TYPEDEFS
typedef unsigned long vecType;
typedef vector<vecType> vec;
typedef chrono::microseconds time_u;

// FUNCTION PROTOTYPES
vec randomArray(unsigned long size);

void init(int argc, char **argv);

template<typename t>
inline
void sortedMerge(vector<t> &, vector<t> &);

// GLOBAL CONSTANTS
vecType numElements, seed = 42, totalElements = 100000;
int perProcess, sampleIntervals;
vec randomArr;

// IMPLMENTATIONS
vec randomArray(unsigned long size) {
    default_random_engine generator;
    generator.seed(seed);
    uniform_int_distribution<vecType> dis(1, ULONG_MAX);

    vec v(size);
    generate(v.begin(), v.end(), bind(dis, generator));
    return v;
}

template<typename t>
inline void sortedMerge(vector<t> &toArray, vector<t> &fromArray) {
    auto size = toArray.size();
    toArray.insert(toArray.end(), fromArray.begin(), fromArray.end());
    inplace_merge(toArray.begin(), toArray.begin() + size, toArray.end());
}

// https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
class InputParser {
public:
    InputParser(int &argc, char **argv) {
        for (int i = 1; i < argc; ++i)
            this->tokens.push_back(string(argv[i]));
    }

    /// @author iain
    const string &getCmdOption(const string &option) const {
        vector<string>::const_iterator itr;
        itr = find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()) {
            return *itr;
        }
        return move(string());
    }

    /// @author iain
    bool cmdOptionExists(const string &option) const {
        return find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }

private:
    vector<string> tokens;
};

void init(int argc, char **argv) {
    InputParser input(argc, argv);

    const string &argseed = input.getCmdOption("-seed");
    if (input.cmdOptionExists("-seed"))
        seed = stoul(argseed, nullptr, 0);

    const string &filename = input.getCmdOption("-size");
    if (input.cmdOptionExists("-size"))
        totalElements = stoul(filename, nullptr, 0);
}
