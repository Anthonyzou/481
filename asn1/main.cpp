#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

using namespace std;



std::vector<int> randomArray(int size);
void call_from_thread(int * i);
void call_from_thread(int * i){
    auto it = (*i).begin();
    while (it != (*i).end()){
        std::cout << *it << std::endl;
        it++;
    }
}

std::vector<int> randomArray(int size){
    std::vector<int> vec;
    for(int i = 0; i < size; i++) {
        vec.push_back(i);
    }
    return vec;
}

const long PROCESSORS = sysconf(_SC_NPROCESSORS_ONLN);

int main() {
    std::thread threads[PROCESSORS];
    cout << PROCESSORS << endl;

    auto ki = randomArray(20);
    for(int i = 0; i < PROCESSORS; i++) {
        threads[i] = std::thread(&call_from_thread, &ki);

    }

    for(long it = 0; it < PROCESSORS; it++) {
        threads[it].join();
    }

    return 0;
}
