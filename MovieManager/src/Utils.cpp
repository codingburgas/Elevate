#include "../include/utils.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <limits>

// ─────────────────────────────────────────
//  Форматиране
// ─────────────────────────────────────────
std::string formatDuration(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    std::ostringstream ss;
    if (h > 0) ss << h << "ч ";
    ss << m << "м";
    return ss.str();
}

std::string ratingStars(double r) {
    int full = static_cast<int>(std::round(r / 2.0));
    std::string s;
    for (int i = 1; i <= 5; ++i)
        s += (i <= full) ? "\u2605" : "\u2606";   // ★ / ☆
    return s;
}

void printLine(char c, int len) {
    std::cout << std::string(len, c) << "\n";
}

void printHeader() {
    printLine('=');
    std::cout << std::left
              << std::setw(4)  << "ID"
              << std::setw(28) << "Заглавие"
              << std::setw(6)  << "Год."
              << std::setw(14) << "Рейтинг"
              << std::setw(7)  << "Мин"
              << std::setw(12) << "Жанр"
              << "\n";
    printLine('-');
}

void printMovie(const Movie& m) {
    std::string stars = ratingStars(m.rating)
                      + " " + std::to_string(m.rating).substr(0, 3);
    std::cout << std::left
              << std::setw(4)  << m.id
              << std::setw(28) << m.title.substr(0, 26)
              << std::setw(6)  << m.year
              << std::setw(14) << stars
              << std::setw(7)  << m.duration
              << std::setw(12) << m.genre
              << "\n";
}

// ─────────────────────────────────────────
//  I/O helpers
// ─────────────────────────────────────────
void clearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) { clearCin(); return v; }
        clearCin();
        std::cout << "  >> Невалиден вход.\n";
    }
}

double readDouble(const std::string& prompt) {
    double v;
    while (true) {
        std::cout << prompt;
        if (std::cin >> v) { clearCin(); return v; }
        clearCin();
        std::cout << "  >> Невалиден вход.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    std::getline(std::cin, s);
    return s;
}
