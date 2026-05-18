// ============================================================
//  mainWindow.h  –  Movie Collection Manager
//  Qt 6 Widgets  |  Code-snippet panel REMOVED
// ============================================================
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "movie.h"
#include "movieCollection.h"
#include "utils.h"
#include "reviewDialog.h"

class QLineEdit;
class QPushButton;
class QTableWidget;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onSearchChanged(const QString& text);
    void calculateTotal();
    void onAddMovie();
    void onDeleteMovie();
    void onSortMovies();
    void onReviewMovie(int movieId);

private:
    void setupUi();
    void populateTable();
    void populateTable(const std::vector<Movie>& list);

    QLineEdit* searchEdit;
    QPushButton* searchBtn;
    QTableWidget* table;
    QPushButton* calcButton;
    QLabel* resultLabel;
    // QTextEdit* codeBlock  ← removed

    std::vector<Movie> movies;
    int nextId = 1;
};

#endif // MAINWINDOW_H