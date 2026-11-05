#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include "data_scraper.hpp"
#include "greedy.hpp"

int main(int argc,char* argv[]) {

    std::string file_path;
    int task_limit = 0;
    if(argc>=2){
        file_path = argv[1];
        if(argc>=3){
            task_limit = std::atoi(argv[2]);
        }
    }
    else{
        std::cout << "Podaj sciezke do pliku: ";
        std::getline(std::cin, file_path);

        std::cout << "Podaj liczbe zadan do przetworzenia: ";
        std::cin >> task_limit;
        }
        if (task_limit <= 0) {
            std::cout << "Liczba zadan musi byc dodatnia." << std::endl;
            return 0;
    }

    
    if (!file_path.empty() && (file_path.front() == '"' || file_path.front() == '\'')) {
        file_path.erase(0, 1);
    }
    if (!file_path.empty() && (file_path.back() == '"' || file_path.back() == '\'')) {
        file_path.pop_back();
    }

    HeaderInfo header = parseHeader(file_path);
    if ((task_limit >= header.max_jobs)){
        task_limit = header.max_jobs;
    }
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

    int max_processors = header.max_nodes > 0 ? header.max_nodes : 1;
    if (!tasks.empty()) {
        solveGreedy(tasks, max_processors);
    }

    std::cout << "\nMaxJobs: " << header.max_jobs << std::endl;
    std::cout << "MaxNodes: " << header.max_nodes << std::endl;
    std::cout << "Wczytane zadania: " << tasks.size() << std::endl;
    std::ofstream res_file("resfile.txt");
    for (const auto& task : tasks) {
        res_file << task.id << " " << task.start_time << " " << task.end_time << " ";
        if (!task.assigned_processors.empty()) {
            for (size_t i = 0; i < task.assigned_processors.size(); ++i) {
                if (i > 0) {
                    res_file << " ";
                }
                res_file << task.assigned_processors[i];
            }
        } else {
            res_file << 0;
        }
        res_file << '\n';
    }

    return 0;
}
