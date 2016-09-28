#pragma once

#include <iterator>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>

using namespace std;


typedef vector<unsigned long> vec;

vec randomArray(long size);
void worker(const int id, promise<vec> prom, const int from, const int end);
inline void handleChunk(int idx, vec results);

//thread::hardware_concurrency()

const int numElements = 2000000;
const int seed = 42;
const auto PROCESSORS = thread::hardware_concurrency();
const long totalElements = PROCESSORS*numElements;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

promise<vec> phase2Promise; shared_future<vec> phase2Vector(phase2Promise.get_future());

vector<vec> phase4(PROCESSORS);
vector<mutex> p4(PROCESSORS), p4v(PROCESSORS);
vector<condition_variable> p4CV(PROCESSORS);
vector<int> threadsDone(PROCESSORS, 0) ;
vec randomArr = randomArray(totalElements);

vec randomArray(long size){
//    auto a = default_random_engine(seed);
    vec v((unsigned long) size);
    std::srand(seed);
    for(int i = 0; i < size; i++)
        v[i]= (std::rand());
    return v;
}

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Merge_sort
template <typename BidirIt, typename Compare = std::less<unsigned long>>
void merge_sort(BidirIt first, BidirIt last, Compare cmp = Compare {})
{
    const auto n = std::distance(first, last);

    if (n > 1) {
        const auto middle = std::next(first, n / 2);

        merge_sort(first, middle, cmp);
        merge_sort(middle, last, cmp);

        std::inplace_merge(first, middle, last, cmp);
    }
}