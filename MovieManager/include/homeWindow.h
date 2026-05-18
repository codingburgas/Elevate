// ============================================================
//  homeWindow.h  –  Personal Movie Library  /  Home Page
//  Qt 6 Widgets  |  Dark Theme  |  Cinema Gold accent
// ============================================================
#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QFrame>
#include <vector>
#include "movie.h"
#include "movieCollection.h"
#include "utils.h"
#include "reviewDialog.h"   // ← review feature

// ─────────────────────────────────────────────────────────────
//  MovieCardWidget  –  individual poster card in the grid
// ─────────────────────────────────────────────────────────────
class MovieCardWidget : public QFrame {
    Q_OBJECT
public:
    explicit MovieCardWidget(const Movie& movie, QWidget* parent = nullptr);

signals:
    void movieCardClicked(int movieId);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void enterEvent(QEnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    int     m_id;
    QLabel* m_posterLabel;
    QLabel* m_titleLabel;
    QLabel* m_ratingLabel;
    QLabel* m_metaLabel;

    void buildCard(const Movie& m);
    static QPixmap generatePosterPixmap(const Movie& m);
    static QColor  genreColor(const std::string& genre);
};

// ─────────────────────────────────────────────────────────────
//  HomeWindow  –  the application's main / home page
// ─────────────────────────────────────────────────────────────
class HomeWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit HomeWindow(QWidget* parent = nullptr);

private slots:
    // Wired to search bar
    void on_search_textChanged(const QString& text);

    // Wired to each MovieCardWidget – now opens ReviewDialog
    void on_movieCard_clicked(int movieId);

    // Sidebar navigation
    void on_navItem_clicked(QListWidgetItem* item);

    // Header buttons
    void on_addMovie_clicked();
    void on_filter_clicked();

private:
    // ── Layout setup ────────────────────────────────────────
    void setupUi();
    void setupSidebar();
    void setupHeader();
    void setupContentArea();
    void setupStatusBar();
    void applyStyleSheet();

    // ── Grid management ─────────────────────────────────────
    void populateGrid(const std::vector<Movie>& list);
    void clearGrid();
    void refreshStats();

    // ── Widgets ─────────────────────────────────────────────
    QWidget* m_centralWidget;

    // Sidebar
    QListWidget* m_navList;

    // Header
    QLineEdit* m_searchEdit;
    QPushButton* m_filterBtn;
    QPushButton* m_addMovieBtn;
    QLabel* m_activeViewLabel;

    // Content
    QScrollArea* m_scrollArea;
    QWidget* m_gridContainer;
    QGridLayout* m_gridLayout;

    // ── State ────────────────────────────────────────────────
    std::vector<Movie> m_movies;      // full library
    int                m_nextId = 1;
    QString            m_activeView = "Library";
    static constexpr int GRID_COLUMNS = 4;   // cards per row
};

#endif // HOMEWINDOW_H