#ifndef TASK_HPP
#define TASK_HPP

#include <cstdint>
#include <vector>

struct Task {
    int id = 0;
    long long arrival_time = 0;
    long long duration = 0;
    int size = 0;
    long long start_time = 0;
    long long end_time = 0;
    std::vector<int> assigned_processors;
};

#endif
