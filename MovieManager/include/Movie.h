#pragma once
#include <string>

// ─────────────────────────────────────────
//  Movie  –  основна структура на данните
// ─────────────────────────────────────────
struct Movie {
    int         id;
    std::string title;
    int         year;
    double      rating;    // 1.0 – 10.0
    int         duration;  // минути
    std::string genre;
};
