#include "greedy.hpp"
#include <algorithm>
#include <limits>
#include <vector>

void solveGreedy(std::vector<Task>& tasks, int max_processors) {
    if (tasks.empty() || max_processors <= 0) {
        return;
    }

    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        if (a.arrival_time != b.arrival_time) {
            return a.arrival_time < b.arrival_time;
        }

        const double a_density = static_cast<double>(a.duration) / std::max(1, a.size);
        const double b_density = static_cast<double>(b.duration) / std::max(1, b.size);

        if (a_density != b_density) {
            return a_density < b_density;
        }

        if (a.duration != b.duration) {
            return a.duration < b.duration;
        }

        if (a.size != b.size) {
            return a.size < b.size;
        }

        return a.id < b.id;
    });

    std::vector<long long> processor_free_time(max_processors, 0LL);

    for (auto& task : tasks) {
        if (task.size > max_processors) {
            task.size = max_processors;
        }

        long long current_time = task.arrival_time;
        std::vector<int> candidate_processors;
        candidate_processors.reserve(max_processors);
        for (int p = 0; p < max_processors; ++p) {
            candidate_processors.push_back(p);
        }

        std::vector<int> chosen_processors;
        chosen_processors.reserve(task.size);

        while (chosen_processors.size() < static_cast<size_t>(task.size)) {
            int best_processor = -1;
            long long best_free_time = std::numeric_limits<long long>::max();

            for (int p : candidate_processors) {
                if (processor_free_time[p] < best_free_time) {
                    best_free_time = processor_free_time[p];
                    best_processor = p;
                }
            }

            if (best_processor == -1) {
                break;
            }

            if (best_free_time > current_time) {
                current_time = best_free_time;
            }

            chosen_processors.push_back(best_processor);
            candidate_processors.erase(
                std::remove(candidate_processors.begin(), candidate_processors.end(), best_processor),
                candidate_processors.end());
        }

        task.start_time = std::max<long long>(task.arrival_time, current_time);
        task.end_time = task.start_time + task.duration;
        task.assigned_processors = chosen_processors;

        for (int p : chosen_processors) {
            processor_free_time[p] = task.end_time;
        }
    }

    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.id < b.id;
    });
}