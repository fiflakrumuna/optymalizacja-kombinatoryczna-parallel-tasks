#ifndef DATA_SCRAPER_HPP
#define DATA_SCRAPER_HPP

#include <string>
#include <vector>

struct HeaderInfo {
    int max_jobs = -1;
    int max_nodes = -1;
    int max_procs = -1;
};


HeaderInfo parseHeader(const std::string& file_path);
std::vector<std::vector<int>> scrap(const std::string& file_path, int& proc_limit);

#endif