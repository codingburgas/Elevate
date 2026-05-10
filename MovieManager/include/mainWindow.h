#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include "movie.h"          // ← your existing struct
#include "movieCollection.h"
#include "utils.h"

class QLineEdit;
class QPushButton;
class QTableWidget;
class QLabel;
class QTextEdit;

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

private:
    void setupUi();
    void populateTable();
    void populateTable(const std::vector<Movie>& list); // overload for search results

    QLineEdit* searchEdit;
    QPushButton* searchBtn;
    QTableWidget* table;
    QPushButton* calcButton;
    QLabel* resultLabel;
    QTextEdit* codeBlock;

    // ← your existing data, NOT a Qt-internal Movie struct
    std::vector<Movie> movies;
    int nextId = 1;
};

#endif // MAINWINDOW_H