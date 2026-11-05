#include <iostream>
#include <string>
#include <vector>
#include "data_scraper.hpp"
#include "greedy.hpp"

static std::string trimQuotes(std::string value) {
    if (value.size() >= 2 && ((value.front() == '"' && value.back() == '"') ||
                              (value.front() == '\'' && value.back() == '\''))) {
        value = value.substr(1, value.size() - 2);
    }
    return value;
}

int main(int argc, char* argv[]) {
    std::string file_path;
    int task_limit = 0;

    if (argc >= 3) {
        file_path = argv[1];
        task_limit = std::stoi(argv[2]);
    } else {
        std::cout << "Podaj sciezke do pliku: ";
        std::getline(std::cin, file_path);
        file_path = trimQuotes(file_path);

        std::cout << "Podaj liczbe zadan do przetworzenia: ";
        std::cin >> task_limit;
    }

    if (task_limit <= 0) {
        return 0;
    }

    file_path = trimQuotes(file_path);

    HeaderInfo header = parseHeader(file_path);
    int remaining_tasks = task_limit;
    std::vector<std::vector<int>> raw_data = scrap(file_path, remaining_tasks);

    std::vector<Task> tasks;
    tasks.reserve(raw_data.size());

    for (const auto& row : raw_data) {
        if (row.size() < 4) {
            continue;
        }

        Task task;
        task.id = row[0];
        task.arrival_time = row[1];
        task.duration = row[2];
        task.size = row[3];

        if (task.duration <= 0 || task.size <= 0) {
            continue;
        }

        tasks.push_back(task);
    }

    int max_processors = header.max_procs > 0 ? header.max_procs :
                         (header.max_nodes > 0 ? header.max_nodes : 1);
    if (!tasks.empty()) {
        solveGreedy(tasks, max_processors);
    }

    for (const auto& task : tasks) {
        std::cout << task.id << ' ' << task.start_time << ' ' << task.end_time;
        for (int processor : task.assigned_processors) {
            std::cout << ' ' << processor;
        }
        std::cout << '\n';
    }

    return 0;
}
