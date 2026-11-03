# Szeregowanie zadań równoległych (*Parallel Tasks*) z wykorzystaniem danych z *Parallel Workload Archives*

## 1. Opis problemu
Celem zadania jest znalezienie uszeregowania zadań równoległych o **najmniejszym średnim czasie przepływu**, co jest równoważne **minimalizacji sumy czasów zakończenia zadań** ($\sum C_j$) dla zadanego zbioru zadań niepodzielnych.

Program wczytuje dane wejściowe w tekstowym formacie **SWF** (*Standard Workload Format*). Z pliku wejściowego należy uwzględnić następujące parametry:
* **Kolumna 1:** `nr_zadania` – identyfikator zadania,
* **Kolumna 2:** $r_j$ – moment gotowości / przybycia zadania (*submit time*),
* **Kolumna 4:** $p_j$ – czas trwania zadania (*run time*),
* **Kolumna 5:** $size_j$ – liczba żądanych / przydzielonych procesorów (*allocated processors*).

---

## 2. Format wyjściowy
Wyniki należy wyprowadzić w postaci listy linii opisujących uszeregowanie każdego z przetworzonych zadań w ścisłym formacie:

```text
nr_zadania momentstartu momentzakończenia numeryprzydzielonychprocesorów
```

### Zasady formatowania wyjścia:
1. **Separator:** Wyłącznie pojedyncza spacja między wartościami.
2. **Czystość linii:** Niedozwolone jest umieszczanie jakichkolwiek innych wyrazów czy znaków w linii opisującej pojedyncze zadanie.
3. Każde uszeregowane zadanie kończy się znakiem nowej linii `\n`.

---

## 3. Dodatkowe założenia i zastrzeżenia

### Obsługa niepoprawnych danych i pomijanie zadań:
* **Zadania niekompletne** (np. występujące w pliku `DAS2-fs0-2003-1.swf.gz`) oraz **zadania o zerowym czasie trwania** ($p_j = 0$, np. w pliku `NASA-iPSC-1993-2.1-cln.swf.gz`) należy **zignorować (pominąć)**.
* Identyfikatory zignorowanych zadań **nie mogą pojawić się** w wyjściowym sprawozdaniu/pliku wynikowym.
* **Brak przenumerowywania:** Nie wolno zmieniać oryginalnych identyfikatorów zadań w pliku wyjściowym (zadania, które zostały przetworzone, zachowują swoje unikalne `nr_zadania` z pliku SWF).
* Sprawdzarka zakłada, że identyfikatory zadań spełniają warunek $\text{ID} \le MaxJobs$.

### Parametry nagłówka i przetwarzania:
* Należy przyjąć reguły: 
  * $MaxNodes = MaxProcs$
  * $MaxJobs = MaxRecords$
* Umożliwić konstrukcję uszeregowania tylko dla podzbioru $N$ pierwszych poprawnych zadań z pliku SWF, gdzie wartość $N$ jest zadawana w momencie uruchomienia programu.
* Kolejność zadań w wejściowym pliku SWF nie musi być zgodna z czasem przybycia $r_j$.

### Sposób przydziału procesorów:
Należy podjąć decyzję dotyczącą typu przydziału procesorów:
* **Przydział nieprzyległy (*noncontiguous*):** Procesory mogą mieć dowolne, niekolejne numery (np. dla $size_j = 5$: procesory `2, 3, 7, 19, 43`). Jest to rozwiązanie zalecane i prostsze w implementacji.
* **Przydział przyległy (*contiguous*):** Zadaniu przydzielane jest $size_j$ kolejnych numerów procesorów (np. dla $size_j = 5$: procesory `3, 4, 5, 6, 7`).
* *Uwaga:* Wybrany wariant przydziału należy wyraźnie zaznaczyć i opisać w sprawozdaniu.

---

## 4. Wymagania efektywnościowe i czasowe

* Czas wykonania algorytmu zależy od liczby zadań. Program musi być zoptymalizowany pod kątem wydajności.
* W ustalonym limicie czasu **5 minut** program musi być w stanie znaleźć rozwiązanie dla **co najmniej 10 000 zadań**.

---

## 5. Zestawy danych i testy

1. **Testy poprawności działania:**
   * `test1`, `test2`, `test3`

2. **Testy efektywnościowe:**
   * `LANL-CM5-1994-3.1-cln.swf.gz`
   * `SDSC-SP2-1998-3.1-cln.swf.gz`
   * `DAS2-fs0-2003-1.swf.gz`
   * *Procedura:* Dla każdego pliku należy uruchamiać algorytm dla kolejnych wartości podzbioru zadań: $N \in \{1, 2, 5, 10, 20, 50, 100, 200, \dots\}$. Maksymalne $N$ wynika z limitu czasu (5 min). W sprawozdaniu można przedstawić również wyniki z dłuższym limitem czasu.

3. **Test jakościowy (suma czasów zakończenia zadań):**
   * Plik: `test-n10k-m128-r0-0.zip` (wszystkie zadania są gotowe jednocześnie, $r_j = 0$).
   * Limit 1. kwartyla dla drugiego rozwiązania: **723823274**.