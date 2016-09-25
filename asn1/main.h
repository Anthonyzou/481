#pragma once

//std::vector<unsigned long> randomArray(long size);
//void mainThread(const int from, const int end);

template<typename T>
void pop_front(std::vector<T>& vec)
{
    vec.front() = std::move(vec.back());
    vec.pop_back();
}