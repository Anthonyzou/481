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
#include <mutex>
#include <future>
#include <climits>
#include <sstream>

using namespace std;

// TYPEDEFS
typedef unsigned long vecType;
typedef vector<vecType> vec;
typedef chrono::microseconds time_u;

// FUNCTION PROTOTYPES
vec         randomArray(unsigned long size);
void        worker(const int id, promise<vec> prom, const int from, const int end);
void        init(int argc, char ** argv);
template <typename t> inline
void        sortedMerge(vector<t> * result, vector<t> * a);

// GLOBAL CONSTANTS
vecType numElements, seed = 42, totalElements = 100000;
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

template <typename t>
inline void sortedMerge(vector<t> * result, vector<t> * a){
    auto size = result->size();
    result->insert(result->end(), a->begin(), a->end());
    inplace_merge(result->begin(), result->begin()+size, result->end());
}

// https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Merge_sort
template <typename BidirIt, typename Compare = less<vecType>>
void merge_sort(BidirIt first, BidirIt last, Compare cmp = Compare {}) {
    const auto n = distance(first, last);

    if (n > 1) {
        const auto middle = next(first, n / 2);

        merge_sort(first, middle, cmp);
        merge_sort(middle, last, cmp);

        inplace_merge(first, middle, last, cmp);
    }
}

// https://stackoverflow.com/questions/865668/how-to-parse-command-line-arguments-in-c
class InputParser{
public:
    InputParser (int &argc, char **argv){
        for (int i=1; i < argc; ++i)
            this->tokens.push_back(string(argv[i]));
    }
    /// @author iain
    const string& getCmdOption(const string &option) const{
        vector<string>::const_iterator itr;
        itr =  find(this->tokens.begin(), this->tokens.end(), option);
        if (itr != this->tokens.end() && ++itr != this->tokens.end()){
            return *itr;
        }
        return move(string());
    }
    /// @author iain
    bool cmdOptionExists(const string &option) const{
        return find(this->tokens.begin(), this->tokens.end(), option)
               != this->tokens.end();
    }
private:
    vector <string> tokens;
};

void init(int argc, char ** argv){
    InputParser input(argc, argv);

    const string & argseed = input.getCmdOption("-seed");
    if(input.cmdOptionExists("-seed"))
        seed = stoul(argseed,nullptr,0);

    const string &filename = input.getCmdOption("-size");
    if (input.cmdOptionExists("-size"))
        totalElements = stoul(filename,nullptr,0);
}