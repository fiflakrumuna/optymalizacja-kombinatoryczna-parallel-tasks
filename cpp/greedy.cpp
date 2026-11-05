#include "greedy.hpp"
#include <algorithm>
#include <iostream>

void solveGreedy(std::vector<Task>& tasks, int max_processors) {
    if (tasks.empty()) return;

    // 1. Krok opcjonalny, ale zalecany: sortujemy zadania po momencie gotowości (r_j),
    // żeby symulacja „płynęła” zgodnie z czasem pojawiania się zadań.
    // (Jeżeli wczytane zadania mają zachować oryginalną kolejność wypisywania, 
    // musimy pamiętać ich oryginalne indeksy/ID).
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.arrival_time < b.arrival_time;
    });

    // Tablica przechowująca czas, do którego każdy procesor (od 0 do M-1) jest zajęty
    std::vector<long long> processor_free_time(max_processors, 0);

    // Symulacja dla każdego zadania
    for (auto& task : tasks) {
        // Zadanie nie może zacząć się wcześniej niż jego arrival_time (r_j)
        long long current_time = task.arrival_time;

        // Potrzebujemy znaleźć moment, w którym zbierzemy 'size' wolnych procesorów.
        // Ponieważ robimy przydział NIEPRZYLEGŁY, wystarczy nam DOWOLNYCH size procesorów,
        // które będą wolne w tym samym momencie.
        std::vector<int> chosen_processors;
        
        while (true) {
            chosen_processors.clear();
            long long max_free_among_chosen = current_time;

            // Szukamy wolnych procesorów w puli
            for (int p = 0; p < max_processors; ++p) {
                if (processor_free_time[p] <= current_time) {
                    chosen_processors.push_back(p);
                    if (chosen_processors.size() == static_cast<size_t>(task.size)) {
                        break;
                    }
                }
            }

            // Jeśli znaleźliśmy wystarczającą liczbę procesorów wolnych w 'current_time', przerywamy
            if (chosen_processors.size() == static_cast<size_t>(task.size)) {
                break;
            }

            // Jeśli nie, musimy przesunąć czas w przyszłość do momentu, 
            // aż zwolni się jakiś kolejny procesor, który mógłby nam pomóc.
            // Szukamy najwcześniejszego czasu zwolnienia procesora spośród tych, które jeszcze nie są wolne.
            long long next_free_time = -1;
            for (int p = 0; p < max_processors; ++p) {
                if (processor_free_time[p] > current_time) {
                    if (next_free_time == -1 || processor_free_time[p] < next_free_time) {
                        next_free_time = processor_free_time[p];
                    }
                }
            }

            if (next_free_time == -1) {
                // Zabezpieczenie (nie powinno wystąpić, chyba że zadanie żąda więcej procesorów niż ma klaster)
                break;
            }
            current_time = next_free_time;
        }

        // Ustalamy moment startu i zakończenia zadania
        task.start_time = current_time;
        task.end_time = current_time + task.duration;
        task.assigned_processors = chosen_processors;

        // Aktualizujemy czas zajętości dla przydzielonych procesorów
        for (int p : chosen_processors) {
            processor_free_time[p] = task.end_time;
        }
    }

    // 3. Ważne: Sprawdzarka wymaga, aby zadania w pliku wynikowym były w takiej kolejności, 
    // w jakiej występowały w oryginalnym pliku wejściowym (nie wolno zmieniać ID ani kolejności wyjściowej).
    // Jeśli posortowaliśmy je wyżej po r_j, teraz musimy je z powrotem posortować rosnąco po ID!
    std::sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
        return a.id < b.id;
    });
}