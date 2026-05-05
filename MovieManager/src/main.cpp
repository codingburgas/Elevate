// ════════════════════════════════════════════════════
//  Movie Collection Manager  |  main.cpp
//  Visual Studio 2022  |  C++17  |  x64
// ════════════════════════════════════════════════════

// NOMINMAX трябва да е ПРЕДИ <windows.h>
// Спира Windows да дефинира макросите min() и max()
// (те иначе чупят std::min / std::max)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <vector>

#include "Movie.h"
#include "MovieCollection.h"
#include "Utils.h"

// ─────────────────────────────────────────
//  Главно меню
// ─────────────────────────────────────────
static void printMenu() {
    printLine('=');
    std::cout << "    Movie Collection Manager\n";
    printLine('-');
    std::cout << "  1) Покажи всички филми\n"
              << "  2) Добави филм\n"
              << "  3) Изтрий филм\n"
              << "  4) Сортирай колекцията\n"
              << "  5) Търси по заглавие\n"
              << "  6) Избери филми -> обща продължителност\n"
              << "  0) Изход\n";
    printLine('=');
}

// ─────────────────────────────────────────
//  main()
// ─────────────────────────────────────────
int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   // изход → UTF-8
    SetConsoleCP(CP_UTF8);         // вход  → UTF-8

    // Шрифт с поддръжка на Unicode / кирилица
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi{};
    cfi.cbSize       = sizeof(cfi);
    cfi.dwFontSize.Y = 18;
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);

    SetConsoleTitleW(L"Movie Collection Manager");
#endif

    int nextId = 1;

    std::vector<Movie> movies = {
        {nextId++, "The Matrix",      1999, 8.7, 136, "Sci-Fi"   },
        {nextId++, "Inception",       2010, 8.8, 148, "Sci-Fi"   },
        {nextId++, "Interstellar",    2014, 8.6, 169, "Sci-Fi"   },
        {nextId++, "The Godfather",   1972, 9.2, 175, "Drama"    },
        {nextId++, "Pulp Fiction",    1994, 8.9, 154, "Thriller" },
        {nextId++, "The Dark Knight", 2008, 9.0, 152, "Action"   },
        {nextId++, "Parasite",        2019, 8.5, 132, "Thriller" },
        {nextId++, "Spirited Away",   2001, 8.6, 125, "Animation"},
        {nextId++, "Oppenheimer",     2023, 8.4, 180, "Drama"    },
        {nextId++, "Dune: Part Two",  2024, 8.5, 166, "Sci-Fi"   },
    };

    int choice = -1;
    while (choice != 0) {
        printMenu();
        choice = readInt("  Избор: ");
        switch (choice) {
        case 1:
            if (movies.empty()) { std::cout << "  Колекцията е празна.\n"; break; }
            printHeader();
            for (const auto& m : movies) printMovie(m);
            printLine('=');
            std::cout << "  Общо " << movies.size() << " филма\n";
            break;
        case 2: addMovie(movies, nextId); break;
        case 3: deleteMovie(movies);      break;
        case 4: sortMenu(movies);         break;
        case 5: {
            std::string q = readLine("\n  Търси заглавие: ");
            auto res = searchByTitle(movies, q);
            if (res.empty()) { std::cout << "  Няма резултати.\n"; break; }
            printHeader();
            for (const auto& m : res) printMovie(m);
            printLine('=');
            std::cout << "  Намерени: " << res.size() << "\n";
            break;
        }
        case 6: selectAndCalculate(movies); break;
        case 0: std::cout << "  Довиждане!\n"; break;
        default: std::cout << "  Невалиден избор.\n";
        }
        if (choice != 0) std::cout << "\n";
    }
    return 0;
}
