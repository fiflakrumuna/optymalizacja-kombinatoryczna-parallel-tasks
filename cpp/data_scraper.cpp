#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::vector<int>> scrap(const std::string& file_path, int& proc_limit) {
    std::ifstream file(file_path);
    std::vector<std::vector<int>> data_ready;

    if (!file.is_open()) {
        std::cerr << "Blad: Nie mozna otworzyc pliku: " << file_path << std::endl;
        return data_ready;
    }

    std::string line;
    while (std::getline(file, line) && proc_limit>0) {
        size_t first_char = line.find_first_not_of(" \t\r\n");

        if (first_char == std::string::npos || line[first_char] == ';') {
            continue;
        }

        std::stringstream ss(line);
        std::vector<int> row;
        int value;
        int column_index = 0;
        int col3_value = 0;

        while (ss >> value) {
            if (column_index == 3) {
                col3_value = value; // Zapamiętujemy wartość 4. kolumny
            }

            // Zachowujemy tylko kolumny o indeksach 0, 1, 3, 4
            if (column_index == 0 || column_index == 1 || column_index == 3 || column_index == 4) {
                row.push_back(value);
            }
            column_index++;
        }

        if (col3_value == 0 || col3_value == -1) {
            continue;
        }

        if (!row.empty()) {
            data_ready.push_back(row);
        }
        --proc_limit;
    }
    
    file.close();
    return data_ready;
}

int main(void) {
    std::string file_path;

    std::cout << "Podaj sciezke do pliku: ";

    std::getline(std::cin, file_path);

    int proc_limit;

    std::cout << "Podaj ilosc procesow do przerobienia: ";

    std::cin >> proc_limit;
    
    
    if (!file_path.empty() && (file_path.front() == '"' || file_path.front() == '\'')) {
        file_path.erase(0, 1);
    }
    if (!file_path.empty() && (file_path.back() == '"' || file_path.back() == '\'')) {
        file_path.pop_back();
    }

    std::vector<std::vector<int>> data = scrap(file_path, proc_limit);

    if (data.empty()) {
        std::cout << "Brak danych do wyswietlenia." << std::endl;
        return 0;
    }

    std::cout << "Pobrano " << data.size() << " wierszy danych:\n\n";

    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << "Wiersz " << (i + 1) << ": ";
        for (int value : data[i]) {
            std::cout << value << "\t";
        }
        std::cout << "\n";
    }

    return 0;
}