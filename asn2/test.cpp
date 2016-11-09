//
//
// Created by azou on 28/09/16.
#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <future>
#include "asn1.h"

int main(int argc, char ** argv){
    init(argc, argv);
    auto begin = std::chrono::steady_clock::now();
    vec v = randomArray(totalElements);
    merge_sort(v.begin(), v.end());
    auto end = chrono::steady_clock::now();
    cout << chrono::duration_cast<time_u>(end - begin).count() << endl;
    return 0;
}
