// ============================================================
//  reviewDialog.h  –  Personal Movie Review  (1–5 ★ + text)
//  Qt 6 Widgets  |  Dark Theme  |  Cinema Gold accent
// ============================================================
#pragma once

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QSize>
#include "movie.h"

// ─────────────────────────────────────────────────────────────
//  StarRatingWidget  –  interactive clickable 1–5 star picker
// ─────────────────────────────────────────────────────────────
class StarRatingWidget : public QWidget {
    Q_OBJECT
public:
    explicit StarRatingWidget(QWidget* parent = nullptr);

    int  rating()     const { return m_rating; }
    void setRating(int r) { m_rating = qBound(0, r, MAX_STARS); m_hovered = 0; update(); }

    QSize sizeHint() const override;

signals:
    void ratingChanged(int rating);

protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e)  override;
    void leaveEvent(QEvent* e)           override;
    void paintEvent(QPaintEvent* e)      override;

private:
    int starAtPos(int x) const;

    int m_rating = 0;
    int m_hovered = 0;

    static constexpr int MAX_STARS = 5;
    static constexpr int STAR_SIZE = 34;
    static constexpr int STAR_GAP = 8;
};

// ─────────────────────────────────────────────────────────────
//  ReviewDialog  –  movie detail header + personal review form
// ─────────────────────────────────────────────────────────────
class ReviewDialog : public QDialog {
    Q_OBJECT
public:
    // movie is taken by non-const ref; data is written on Save.
    explicit ReviewDialog(Movie& movie, QWidget* parent = nullptr);

private:
    void buildHeader();
    void buildReviewForm();

    Movie& m_movie;
    StarRatingWidget* m_stars = nullptr;
    QTextEdit* m_edit = nullptr;
    QLabel* m_charLbl = nullptr;

    static constexpr int MAX_CHARS = 500;
};