#include <iostream>
#include <string>
#include "data_scraper.hpp"

int main() {
    std::string file_path;
    std::cout << "Podaj sciezke do pliku: ";
    std::getline(std::cin, file_path);

    if (!file_path.empty() && (file_path.front() == '"' || file_path.front() == '\'')) {
        file_path.erase(0, 1);
    }
    if (!file_path.empty() && (file_path.back() == '"' || file_path.back() == '\'')) {
        file_path.pop_back();
    }

    int proc_limit;

    std::cout << "Podaj ilosc procesow do przerobienia: ";

    std::cin >> proc_limit;

    // Wywołujemy wczytywanie
    HeaderInfo header = parseHeader(file_path);
    std::vector<std::vector<int>> data = scrap(file_path, proc_limit);

    std::cout << "\nMaxJobs: " << header.max_jobs << std::endl;
    std::cout << "MaxNodes: " << header.max_nodes << std::endl;
    std::cout << "Wczytane wiersze danych: " << data.size() << std::endl;

    return 0;
}