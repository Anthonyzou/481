//
//
// Created by azou on 28/09/16.
#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <future>
#include "main.h"

int main(){
    auto begin = std::chrono::steady_clock::now();
    vec v = randomArray(numElements*PROCESSORS);
    merge_sort(v.begin(), v.end());
    auto end = chrono::steady_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - begin).count();
    return 0;
}