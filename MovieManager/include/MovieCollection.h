#pragma once
#include <vector>
#include <string>
#include "Movie.h"

// ─────────────────────────────────────────
//  Enum за полето на сортиране
// ─────────────────────────────────────────
enum class SortField { TITLE, YEAR, RATING, DURATION };

// ─────────────────────────────────────────
//  MovieCollection  –  CRUD + алгоритми
// ─────────────────────────────────────────

// Рекурсивно изчисляване на обща продължителност
int recursiveTotalDuration(const std::vector<Movie>& movies,
                            const std::vector<int>&  selectedIds,
                            int index = 0);

void sortMovies(std::vector<Movie>& movies, SortField field, bool ascending = true);

std::vector<Movie> searchByTitle(const std::vector<Movie>& movies,
                                  const std::string& query);

void addMovie(std::vector<Movie>& movies, int& nextId);
void deleteMovie(std::vector<Movie>& movies);
void sortMenu(std::vector<Movie>& movies);
void selectAndCalculate(const std::vector<Movie>& movies);
