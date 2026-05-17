#pragma once
#include <string>

// ─────────────────────────────────────────
//  Movie  –  основна структура на данните
// ─────────────────────────────────────────
struct Movie {
    int         id;
    std::string title;
    int         year;
    double      rating;
    int         duration;
    std::string genre;
    std::string posterPath;  // ← add this
};