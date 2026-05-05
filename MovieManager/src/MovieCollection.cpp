#include "MovieCollection.h"
#include "Utils.h"

#include <iostream>
#include <algorithm>

// ─────────────────────────────────────────
//  Рекурсия: обща продължителност
// ─────────────────────────────────────────
int recursiveTotalDuration(const std::vector<Movie>& movies,
                            const std::vector<int>&  selectedIds,
                            int index)
{
    if (index >= static_cast<int>(selectedIds.size()))
        return 0;                               // base case

    for (const auto& m : movies)
        if (m.id == selectedIds[index])
            return m.duration
                 + recursiveTotalDuration(movies, selectedIds, index + 1);

    return recursiveTotalDuration(movies, selectedIds, index + 1);
}

// ─────────────────────────────────────────
//  Сортиране
// ─────────────────────────────────────────
void sortMovies(std::vector<Movie>& movies, SortField field, bool ascending) {
    std::stable_sort(movies.begin(), movies.end(),
        [&](const Movie& a, const Movie& b) {
            bool r = false;
            switch (field) {
                case SortField::TITLE:    r = a.title    < b.title;    break;
                case SortField::YEAR:     r = a.year     < b.year;     break;
                case SortField::RATING:   r = a.rating   < b.rating;   break;
                case SortField::DURATION: r = a.duration < b.duration; break;
            }
            return ascending ? r : !r;
        });
}

// ─────────────────────────────────────────
//  Търсене (ASCII регистър-нечувствително)
// ─────────────────────────────────────────
static std::string toLower(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

std::vector<Movie> searchByTitle(const std::vector<Movie>& movies,
                                  const std::string& query)
{
    std::vector<Movie> res;
    std::string q = toLower(query);
    for (const auto& m : movies)
        if (toLower(m.title).find(q) != std::string::npos)
            res.push_back(m);
    return res;
}

// ─────────────────────────────────────────
//  Добавяне / изтриване
// ─────────────────────────────────────────
void addMovie(std::vector<Movie>& movies, int& nextId) {
    Movie m;
    m.id = nextId++;
    std::cout << "\n--- Добавяне на нов филм ---\n";
    m.title    = readLine  ("  Заглавие             : ");
    m.year     = readInt   ("  Година               : ");
    m.rating   = readDouble("  Рейтинг (1-10)       : ");
    // Clamp без std::min/std::max (заобикаляме Windows макросите)
    if (m.rating < 1.0)  m.rating = 1.0;
    if (m.rating > 10.0) m.rating = 10.0;
    m.duration = readInt   ("  Продължителност (мин): ");
    m.genre    = readLine  ("  Жанр                 : ");
    movies.push_back(m);
    std::cout << "  [OK] Добавен с ID=" << m.id << "\n";
}

void deleteMovie(std::vector<Movie>& movies) {
    int id = readInt("\n  ID за изтриване: ");
    auto it = std::find_if(movies.begin(), movies.end(),
                           [id](const Movie& m) { return m.id == id; });
    if (it == movies.end())
        std::cout << "  [!!] Не е намерен филм с ID=" << id << "\n";
    else {
        std::cout << "  Изтрит: " << it->title << "\n";
        movies.erase(it);
    }
}

// ─────────────────────────────────────────
//  Меню сортиране
// ─────────────────────────────────────────
void sortMenu(std::vector<Movie>& movies) {
    std::cout << "\n  Сортирай по:\n"
              << "    1) Заглавие\n"
              << "    2) Година\n"
              << "    3) Рейтинг\n"
              << "    4) Продължителност\n";
    int choice = readInt("  Избор: ");
    int dir    = readInt("  1=Възходящо  2=Низходящо: ");

    SortField field = SortField::TITLE;
    switch (choice) {
        case 2: field = SortField::YEAR;     break;
        case 3: field = SortField::RATING;   break;
        case 4: field = SortField::DURATION; break;
    }
    sortMovies(movies, field, dir != 2);
    std::cout << "  [OK] Сортирано.\n";
}

// ─────────────────────────────────────────
//  Избери → рекурсивна продължителност
// ─────────────────────────────────────────
void selectAndCalculate(const std::vector<Movie>& movies) {
    if (movies.empty()) { std::cout << "  Няма филми.\n"; return; }

    printHeader();
    for (const auto& m : movies) printMovie(m);
    printLine('=');

    std::cout << "\n  Въведи ID-та (интервал между тях, 0 за край):\n  > ";
    std::vector<int> sel;
    int id;
    while (std::cin >> id && id != 0) sel.push_back(id);
    clearCin();

    if (sel.empty()) { std::cout << "  Няма избрани.\n"; return; }

    std::cout << "\n  Избрани филми:\n";
    for (int sid : sel) {
        auto it = std::find_if(movies.begin(), movies.end(),
                               [sid](const Movie& m) { return m.id == sid; });
        if (it != movies.end())
            std::cout << "    [" << it->id << "] "
                      << it->title << "  -  " << it->duration << " мин\n";
        else
            std::cout << "    [" << sid << "] не е намерен\n";
    }

    int total = recursiveTotalDuration(movies, sel);
    std::cout << "\n  == Обща продължителност (рекурсия) ==\n"
              << "     " << total << " мин  =  "
              << formatDuration(total) << "\n";
}
