#pragma once

#include <iterator>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

typedef long vecType;
typedef vector<vecType> vec;

// function prototypes
vec randomArray(unsigned long size);
void worker(const int id, promise<vec> prom, const int from, const int end);
inline void handleChunk(int idx, vec results);

// Global constants
const int numElements = 10000000, seed = 42;
const auto PROCESSORS = thread::hardware_concurrency();
const unsigned long totalElements = PROCESSORS*numElements;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

// Global shared variables
// phase 2 globals
promise<vec> phase2Promise; shared_future<vec> phase2Vector(phase2Promise.get_future());

// phase 4 globals
vector<vec> phase4(PROCESSORS);
vector<mutex> p4(PROCESSORS);
vector<condition_variable> p4CV(PROCESSORS);
vector<int> threadsDone(PROCESSORS, 0);

// random array
vec randomArr = randomArray(totalElements);

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