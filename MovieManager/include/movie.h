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
    std::string posterPath;

    // ── Personal review data ─────────────────────
    int         userRating = 0;   // 0 = not reviewed, 1–5 personal stars
    std::string review;           // free-text personal notes
};