// ============================================================
//  reviewDialog.cpp  –  Personal Movie Review
//  Qt 6 Widgets  |  Dark Theme  |  Cinema Gold accent
// ============================================================
#include "../include/reviewDialog.h"

#include <cmath>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QFont>
#include <QFontMetrics>

// ════════════════════════════════════════════════════════════
//   StarRatingWidget
// ════════════════════════════════════════════════════════════

StarRatingWidget::StarRatingWidget(QWidget* parent) : QWidget(parent) {
    setMouseTracking(true);
    setFixedSize(sizeHint());
    setCursor(Qt::PointingHandCursor);
}

QSize StarRatingWidget::sizeHint() const {
    int w = MAX_STARS * STAR_SIZE + (MAX_STARS - 1) * STAR_GAP;
    return { w, STAR_SIZE };
}

int StarRatingWidget::starAtPos(int x) const {
    for (int i = 0; i < MAX_STARS; ++i) {
        int left = i * (STAR_SIZE + STAR_GAP);
        if (x >= left && x < left + STAR_SIZE)
            return i + 1;
    }
    return 0;
}

void StarRatingWidget::mousePressEvent(QMouseEvent* e) {
    int s = starAtPos(static_cast<int>(e->position().x()));
    if (s > 0) {
        m_rating = (s == m_rating) ? 0 : s;   // click same star → clear
        emit ratingChanged(m_rating);
        update();
    }
}

void StarRatingWidget::mouseMoveEvent(QMouseEvent* e) {
    int s = starAtPos(static_cast<int>(e->position().x()));
    if (s != m_hovered) { m_hovered = s; update(); }
}

void StarRatingWidget::leaveEvent(QEvent*) {
    m_hovered = 0;
    update();
}

void StarRatingWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    static constexpr double PI = 3.14159265358979;
    int display = (m_hovered > 0) ? m_hovered : m_rating;

    for (int i = 0; i < MAX_STARS; ++i) {
        int    x = i * (STAR_SIZE + STAR_GAP);
        QRectF rect(x, 1, STAR_SIZE - 1, STAR_SIZE - 1);
        bool   filled = (i < display);

        QColor fill = filled ? QColor("#FFD700") : QColor("#2E2E2E");
        QColor border = filled ? QColor("#FFD700") : QColor("#4A4A4A");

        // Build 5-point star polygon
        QPolygonF star;
        double cx = rect.center().x();
        double cy = rect.center().y();
        double outer = rect.width() / 2.0;
        double inner = outer * 0.40;

        for (int j = 0; j < 10; ++j) {
            double angle = (j * 36.0 - 90.0) * PI / 180.0;
            double r = (j % 2 == 0) ? outer : inner;
            star << QPointF(cx + r * std::cos(angle), cy + r * std::sin(angle));
        }

        p.setPen(QPen(border, 1.2));
        p.setBrush(fill);
        p.drawPolygon(star);
    }
}

// ════════════════════════════════════════════════════════════
//   ReviewDialog
// ════════════════════════════════════════════════════════════

ReviewDialog::ReviewDialog(Movie& movie, QWidget* parent)
    : QDialog(parent), m_movie(movie)
{
    setWindowTitle("Review — " + QString::fromStdString(movie.title));
    setMinimumWidth(500);
    setMaximumWidth(580);
    setModal(true);

    // Dark background that works under both HomeWindow and MainWindow themes
    setStyleSheet("QDialog { background-color: #181818; }");

    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    buildHeader();
    buildReviewForm();
}

// ── Header band (movie info) ──────────────────────────────────
void ReviewDialog::buildHeader() {
    QFrame* header = new QFrame;
    header->setFixedHeight(116);
    header->setStyleSheet(
        "QFrame { background: qlineargradient("
        "  x1:0, y1:0, x2:1, y2:1,"
        "  stop:0 #1A1A2E, stop:1 #0F1020"
        "); border-bottom: 1px solid #2A2A2A; }");

    QVBoxLayout* hLay = new QVBoxLayout(header);
    hLay->setContentsMargins(28, 18, 28, 14);
    hLay->setSpacing(6);

    // ── Genre pill ────────────────────────────────────────────
    QLabel* genrePill = new QLabel(QString::fromStdString(m_movie.genre).toUpper());
    genrePill->setFixedHeight(20);
    genrePill->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    genrePill->setStyleSheet(
        "QLabel { color: #888888; font-size: 10px; font-weight: 700; "
        "letter-spacing: 2px; }");
    hLay->addWidget(genrePill);

    // ── Movie title ───────────────────────────────────────────
    QLabel* titleLbl = new QLabel(QString::fromStdString(m_movie.title));
    QFont tf = titleLbl->font();
    tf.setPointSize(16); tf.setBold(true);
    titleLbl->setFont(tf);
    titleLbl->setStyleSheet("QLabel { color: #FFD700; }");
    hLay->addWidget(titleLbl);

    // ── Sub-info row ─────────────────────────────────────────
    int hrs = m_movie.duration / 60;
    int mins = m_movie.duration % 60;
    QString sub = QString("%1   ·   ★ %2 / 10   ·   %3h %4m")
        .arg(m_movie.year)
        .arg(m_movie.rating, 0, 'f', 1)
        .arg(hrs)
        .arg(mins, 2, 10, QChar('0'));
    QLabel* subLbl = new QLabel(sub);
    subLbl->setStyleSheet("QLabel { color: #666666; font-size: 12px; }");
    hLay->addWidget(subLbl);

    static_cast<QVBoxLayout*>(layout())->addWidget(header);
}

// ── Review form body ─────────────────────────────────────────
void ReviewDialog::buildReviewForm() {
    QWidget* body = new QWidget;
    body->setStyleSheet("QWidget { background-color: #181818; }");

    QVBoxLayout* bLay = new QVBoxLayout(body);
    bLay->setContentsMargins(28, 24, 28, 24);
    bLay->setSpacing(16);

    // ── Section: Your Rating ─────────────────────────────────
    QLabel* ratingSectionLbl = new QLabel("Your Rating");
    ratingSectionLbl->setStyleSheet(
        "QLabel { color: #E0E0E0; font-size: 13px; font-weight: 700; }");
    bLay->addWidget(ratingSectionLbl);

    QHBoxLayout* starsRow = new QHBoxLayout;
    starsRow->setSpacing(14);

    m_stars = new StarRatingWidget;
    m_stars->setRating(m_movie.userRating);
    starsRow->addWidget(m_stars);

    QLabel* starHint = new QLabel("(click a star again to clear)");
    starHint->setStyleSheet("QLabel { color: #444444; font-size: 11px; }");
    starsRow->addWidget(starHint);
    starsRow->addStretch();
    bLay->addLayout(starsRow);

    // ── Divider ───────────────────────────────────────────────
    QFrame* div = new QFrame;
    div->setFrameShape(QFrame::HLine);
    div->setStyleSheet("QFrame { background: #2A2A2A; border: none; max-height: 1px; }");
    bLay->addWidget(div);

    // ── Section: Your Review ─────────────────────────────────
    QHBoxLayout* reviewHeader = new QHBoxLayout;
    QLabel* reviewSectionLbl = new QLabel("Your Review");
    reviewSectionLbl->setStyleSheet(
        "QLabel { color: #E0E0E0; font-size: 13px; font-weight: 700; }");
    reviewHeader->addWidget(reviewSectionLbl);
    reviewHeader->addStretch();

    m_charLbl = new QLabel("0 / 500");
    m_charLbl->setStyleSheet("QLabel { color: #444444; font-size: 11px; }");
    reviewHeader->addWidget(m_charLbl);
    bLay->addLayout(reviewHeader);

    m_edit = new QTextEdit;
    m_edit->setPlaceholderText("Write your thoughts about this movie…");
    m_edit->setMinimumHeight(130);
    m_edit->setMaximumHeight(180);
    m_edit->setStyleSheet(
        "QTextEdit {"
        "  background: #222222;"
        "  border: 1px solid #2E2E2E;"
        "  border-radius: 8px;"
        "  padding: 10px 12px;"
        "  color: #E0E0E0;"
        "  font-size: 13px;"
        "  line-height: 1.5;"
        "}"
        "QTextEdit:focus { border-color: #FFD700; }");

    if (!m_movie.review.empty()) {
        m_edit->setPlainText(QString::fromStdString(m_movie.review));
        m_charLbl->setText(QString("%1 / 500").arg(m_movie.review.size()));
    }
    bLay->addWidget(m_edit);

    // Live character counter
    connect(m_edit, &QTextEdit::textChanged, this, [this]() {
        QString txt = m_edit->toPlainText();
        if (txt.length() > MAX_CHARS) {
            // Enforce hard cap
            QTextCursor cur = m_edit->textCursor();
            int pos = cur.position();
            m_edit->blockSignals(true);
            m_edit->setPlainText(txt.left(MAX_CHARS));
            m_edit->blockSignals(false);
            QTextCursor nc = m_edit->textCursor();
            nc.setPosition(qMin(pos, MAX_CHARS));
            m_edit->setTextCursor(nc);
            txt = m_edit->toPlainText();
        }
        int len = txt.length();
        m_charLbl->setText(QString("%1 / 500").arg(len));
        bool nearLimit = (len >= MAX_CHARS - 20);
        m_charLbl->setStyleSheet(nearLimit
            ? "QLabel { color: #D65B3B; font-size: 11px; }"
            : "QLabel { color: #444444; font-size: 11px; }");
        });

    // ── Action buttons ────────────────────────────────────────
    QHBoxLayout* btnRow = new QHBoxLayout;
    btnRow->setSpacing(10);

    QPushButton* cancelBtn = new QPushButton("Cancel");
    cancelBtn->setFixedHeight(42);
    cancelBtn->setMinimumWidth(100);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "  background: #242424;"
        "  color: #BDBDBD;"
        "  border: 1px solid #333333;"
        "  border-radius: 8px;"
        "  font-size: 13px;"
        "}"
        "QPushButton:hover { background: #2E2E2E; color: #FFFFFF; border-color: #555555; }"
        "QPushButton:pressed { background: #1A1A1A; }");

    QPushButton* saveBtn = new QPushButton("💾   Save Review");
    saveBtn->setFixedHeight(42);
    saveBtn->setMinimumWidth(160);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "  background: #FFD700;"
        "  color: #0D0D0D;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-size: 13px;"
        "  font-weight: 700;"
        "}"
        "QPushButton:hover { background: #FFE033; }"
        "QPushButton:pressed { background: #E6C200; }");

    btnRow->addWidget(cancelBtn);
    btnRow->addStretch();
    btnRow->addWidget(saveBtn);
    bLay->addLayout(btnRow);

    static_cast<QVBoxLayout*>(layout())->addWidget(body);

    // ── Connections ───────────────────────────────────────────
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(saveBtn, &QPushButton::clicked, this, [this]() {
        m_movie.userRating = m_stars->rating();
        m_movie.review = m_edit->toPlainText().toStdString();
        accept();
        });
}