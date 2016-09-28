#pragma once

#include <iterator>
#include <algorithm>
#include <functional>

using namespace std;


typedef vector<unsigned long> vec;

vec randomArray(long size);
void worker(const int id, promise<vec> prom, const int from, const int end);
//thread::hardware_concurrency()

const int numElements = 36/3;
const int seed = 42;
const long PROCESSORS = 3;
const long totalElements = 36;
const auto sampleIntervals = totalElements/(PROCESSORS*PROCESSORS);

promise<vec> phase2Promise; shared_future<vec> phase2Vector(phase2Promise.get_future());

vector<vec> phase4(PROCESSORS);
vector<mutex> p4(PROCESSORS), p4v(PROCESSORS);
vector<condition_variable> cv(PROCESSORS);
vector<int> threadsDone(PROCESSORS, 0) ;

vec randomArray(long size){
    auto a = default_random_engine(seed);
    vec v;
    for(int i = 0; i < size; i++) {
        v.push_back(a());
    }
    return v;
}

vec randomArr = {
        16,2,17,24,33,28,30,1,0,27,9,25,34,23,19,18,11,7,
        21,13,8,35,12,29,6,3,4,14,22,15,32,10,26,31,20,5
};

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