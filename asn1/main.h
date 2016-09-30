// Anthony Ou
// 1248175
// cmput 481
// Oct 5, 2016

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
#include <algorithm>
#include <mutex>
#include <future>
#include <climits>

using namespace std;

typedef long vecType;
typedef vector<vecType> vec;

// function prototypes
vec randomArray(unsigned long size);
void worker(const int id, promise<vec> prom, const int from, const int end);
inline void handleChunk(int idx, vec results);
void init(int argc, char ** argv);

// Global constants
unsigned long numElements = 100000, seed = 42, PROCESSORS = thread::hardware_concurrency(), totalElements, sampleIntervals;

// Global shared variables
// phase 2 globals
promise<vec> phase2Promise; shared_future<vec> phase2Vector(phase2Promise.get_future());

// phase 4 globals
vector<vec> phase4;
vector<mutex> p4;
vector<condition_variable> p4CV;
vector<int> threadsDone;

// random array
vec randomArr;

vec randomArray(unsigned long size) {
    default_random_engine generator;
    generator.seed(seed);
    uniform_int_distribution<vecType> dis(1, LONG_MAX);

    vec v(size);
    for(auto i = 0; i < size; i++)
        v[i] = dis(generator);
    return v;
}

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Merge_sort
template <typename BidirIt, typename Compare = std::less<vecType>>
void merge_sort(BidirIt first, BidirIt last, Compare cmp = Compare {}) {
    const auto n = std::distance(first, last);

    if (n > 1) {
        const auto middle = std::next(first, n / 2);

        merge_sort(first, middle, cmp);
        merge_sort(middle, last, cmp);

        std::inplace_merge(first, middle, last, cmp);
    }
}

// http://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
class InputParser{
public:
    InputParser (int &argc, char **argv){
        for (int i=1; i < argc; ++i)
            this->tokens.push_back(std::string(argv[i]));
    }
    /// @author iain
    const std::string& getCmdOption(const std::string &option) const{
        std::vector<std::string>::const_iterator itr;
        itr =  std::find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        return std::string();
    }
    /// @author iain
    bool cmdOptionExists(const std::string &option) const{
        return std::find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }
private:
    std::vector <std::string> tokens;
};

void init(int argc, char ** argv){
    InputParser input(argc, argv);
    const std::string & argseed = input.getCmdOption("-seed");
    if(input.cmdOptionExists("-seed")){
        seed = stoul(argseed,nullptr,0);
    }
    const std::string &filename = input.getCmdOption("-size");
    if (input.cmdOptionExists("-size")){
        numElements = stoul(filename,nullptr,0);
    }
    const std::string &argthreads = input.getCmdOption("-threads");
    if (input.cmdOptionExists("-threads")){
        PROCESSORS = stoul(argthreads,nullptr,0);
    }

    phase4.resize(PROCESSORS);
    threadsDone.resize(PROCESSORS,0);
    vector<mutex> a(PROCESSORS);
    p4.swap(a);
    vector<condition_variable> b(PROCESSORS);
    p4CV.swap(b);

    totalElements = PROCESSORS*numElements;
    sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

    randomArr = randomArray(totalElements);
}