//
// Created by azou on 28/09/16.
//
#include <iostream>
#include <thread>
#include <random>
#include <algorithm>
#include <mutex>
#include <sstream>
#include <future>
#include "main.h"
#include <algorithm>

int main(){
    auto begin = std::chrono::steady_clock::now();
    vec v = randomArray(8000000);
    sort(v.begin(), v.end());
    auto end = chrono::steady_clock::now();
    cout << "Time difference = " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << endl;
    return 0;
}