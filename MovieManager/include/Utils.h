#pragma once
#include <string>
#include "Movie.h"

// ─────────────────────────────────────────
//  Помощни функции (форматиране, I/O)
// ─────────────────────────────────────────

std::string formatDuration(int minutes);
std::string ratingStars(double r);
void        printLine(char c = '-', int len = 76);
void        printHeader();
void        printMovie(const Movie& m);

// I/O helpers
void        clearCin();
int         readInt(const std::string& prompt);
double      readDouble(const std::string& prompt);
std::string readLine(const std::string& prompt);
