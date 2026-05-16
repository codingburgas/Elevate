// ============================================================
//  homeWindow.cpp  –  Personal Movie Library  /  Home Page
//  Qt 6 Widgets  |  Dark Theme  |  Cinema Gold accent
// ============================================================
#include "../include/homeWindow.h"

#include <map>
#include <QApplication>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QStatusBar>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QEnterEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QScrollBar>
#include <QFont>
#include <algorithm>
#include <numeric>
#include <cmath>

// ════════════════════════════════════════════════════════════
//   QSS  –  complete dark theme stylesheet
// ════════════════════════════════════════════════════════════
static const char* DARK_QSS = R"(
/* ── Base ───────────────────────────────────────────────── */
QWidget {
    background-color: #121212;
    color: #E0E0E0;
    font-family: "Segoe UI", "Inter", sans-serif;
    font-size: 13px;
}

/* ── Main window chrome ──────────────────────────────────── */
QMainWindow {
    background-color: #0D0D0D;
}

/* ── Sidebar / navigation rail ──────────────────────────── */
QListWidget#navList {
    background-color: #1A1A1A;
    border: none;
    border-right: 1px solid #2A2A2A;
    padding-top: 8px;
    outline: 0;
}
QListWidget#navList::item {
    color: #9E9E9E;
    padding: 12px 16px;
    border-radius: 8px;
    margin: 2px 8px;
    font-size: 13px;
    font-weight: 500;
}
QListWidget#navList::item:hover {
    background-color: #252525;
    color: #E0E0E0;
}
QListWidget#navList::item:selected {
    background-color: #1E1E1E;
    color: #FFD700;
    border-left: 3px solid #FFD700;
    padding-left: 13px;
}

/* ── App logo label in sidebar ───────────────────────────── */
QLabel#logoLabel {
    color: #FFD700;
    font-size: 17px;
    font-weight: 700;
    padding: 20px 16px 12px 16px;
    border-bottom: 1px solid #2A2A2A;
    letter-spacing: 1px;
}

/* ── Header bar ─────────────────────────────────────────── */
QWidget#headerBar {
    background-color: #181818;
    border-bottom: 1px solid #2A2A2A;
}

QLabel#viewTitleLabel {
    font-size: 20px;
    font-weight: 700;
    color: #FFFFFF;
    padding-left: 4px;
}

/* ── Search input ───────────────────────────────────────── */
QLineEdit#searchEdit {
    background-color: #252525;
    border: 1px solid #333333;
    border-radius: 20px;
    padding: 8px 16px 8px 38px;
    color: #E0E0E0;
    font-size: 13px;
    selection-background-color: #FFD700;
    selection-color: #000000;
    min-width: 240px;
}
QLineEdit#searchEdit:focus {
    border: 1px solid #FFD700;
    background-color: #2A2A2A;
}
QLineEdit#searchEdit::placeholder {
    color: #555555;
}

/* ── Filter button ──────────────────────────────────────── */
QPushButton#filterBtn {
    background-color: #252525;
    color: #BDBDBD;
    border: 1px solid #333333;
    border-radius: 20px;
    padding: 8px 18px;
    font-size: 13px;
    font-weight: 500;
}
QPushButton#filterBtn:hover {
    background-color: #2E2E2E;
    color: #FFFFFF;
    border-color: #555555;
}
QPushButton#filterBtn:pressed {
    background-color: #1E1E1E;
}

/* ── Add Movie button (primary accent) ──────────────────── */
QPushButton#addMovieBtn {
    background-color: #FFD700;
    color: #0D0D0D;
    border: none;
    border-radius: 20px;
    padding: 8px 20px;
    font-size: 13px;
    font-weight: 700;
}
QPushButton#addMovieBtn:hover {
    background-color: #FFE033;
}
QPushButton#addMovieBtn:pressed {
    background-color: #E6C200;
}

/* ── Content scroll area ────────────────────────────────── */
QScrollArea#contentArea {
    background-color: #121212;
    border: none;
}
QScrollArea#contentArea > QWidget > QWidget {
    background-color: #121212;
}
QScrollBar:vertical {
    background: #1A1A1A;
    width: 8px;
    border-radius: 4px;
}
QScrollBar::handle:vertical {
    background: #3A3A3A;
    border-radius: 4px;
    min-height: 30px;
}
QScrollBar::handle:vertical:hover {
    background: #555555;
}
QScrollBar::add-line:vertical,
QScrollBar::sub-line:vertical {
    height: 0;
}

/* ── Movie Card ─────────────────────────────────────────── */
QFrame#movieCard {
    background-color: #1E1E1E;
    border: 1px solid #2A2A2A;
    border-radius: 12px;
}
QFrame#movieCard:hover {
    border: 1px solid #FFD700;
    background-color: #242424;
}

QLabel#cardTitle {
    color: #FFFFFF;
    font-size: 12px;
    font-weight: 600;
}
QLabel#cardRating {
    color: #FFD700;
    font-size: 11px;
    font-weight: 700;
}
QLabel#cardMeta {
    color: #777777;
    font-size: 10px;
}

/* ── Status bar ─────────────────────────────────────────── */
QStatusBar {
    background-color: #181818;
    color: #757575;
    border-top: 1px solid #2A2A2A;
    font-size: 12px;
    padding: 0 12px;
}
QStatusBar::item {
    border: none;
}

/* ── Dialogs ─────────────────────────────────────────────── */
QDialog {
    background-color: #1E1E1E;
}
QDialog QLabel {
    color: #BDBDBD;
}
QDialog QLineEdit,
QDialog QSpinBox,
QDialog QDoubleSpinBox {
    background-color: #252525;
    border: 1px solid #333333;
    border-radius: 6px;
    padding: 6px 10px;
    color: #E0E0E0;
}
QDialog QLineEdit:focus,
QDialog QSpinBox:focus,
QDialog QDoubleSpinBox:focus {
    border-color: #FFD700;
}
QDialogButtonBox QPushButton {
    background-color: #FFD700;
    color: #0D0D0D;
    border: none;
    border-radius: 6px;
    padding: 7px 18px;
    font-weight: 700;
}
QDialogButtonBox QPushButton:hover {
    background-color: #FFE033;
}
QDialogButtonBox QPushButton[text="Cancel"],
QDialogButtonBox QPushButton[text="Отказ"] {
    background-color: #2E2E2E;
    color: #BDBDBD;
}
)";

// ════════════════════════════════════════════════════════════
//   MovieCardWidget  implementation
// ════════════════════════════════════════════════════════════

// Map genre → vivid gradient base colour
QColor MovieCardWidget::genreColor(const std::string& genre) {
    static const std::map<std::string, QColor> palette = {
        {"Sci-Fi",     QColor(0,  120, 212)},   // Electric Blue
        {"Drama",      QColor(180,  60,  60)},   // Deep Red
        {"Thriller",   QColor(100,  40, 160)},   // Deep Purple
        {"Action",     QColor(220,  80,   0)},   // Burnt Orange
        {"Animation",  QColor(20, 160, 120)},   // Teal
        {"Horror",     QColor(60,  20,  20)},   // Near-black red
        {"Comedy",     QColor(200, 160,   0)},   // Warm Gold
        {"Romance",    QColor(200,  60, 110)},   // Rose
        {"Crime",      QColor(40,  80,  40)},   // Dark Green
        {"Documentary",QColor(80,  80,  80)},   // Slate
    };
    auto it = palette.find(genre);
    return (it != palette.end()) ? it->second : QColor(50, 50, 80);
}

// Generate a 180×260 poster placeholder using QPainter
QPixmap MovieCardWidget::generatePosterPixmap(const Movie& m) {

    if (!m.posterPath.empty()) {
        QPixmap pm(QString::fromStdString(m.posterPath));
        if (!pm.isNull())
            return pm.scaled(180, 260, Qt::KeepAspectRatioByExpanding,
                Qt::SmoothTransformation);
    }

    const int W = 180, H = 260;
    QPixmap pm(W, H);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);

    // Rounded-rect clip
    QPainterPath clip;
    clip.addRoundedRect(0, 0, W, H, 10, 10);
    p.setClipPath(clip);

    // Background gradient
    QColor base = genreColor(m.genre);
    QLinearGradient bg(0, 0, W, H);
    bg.setColorAt(0.0, base.lighter(140));
    bg.setColorAt(1.0, base.darker(180));
    p.fillPath(clip, bg);

    // Subtle film-grain texture dots
    p.setPen(Qt::NoPen);
    QColor grain(255, 255, 255, 8);
    p.setBrush(grain);
    for (int i = 0; i < 120; ++i) {
        int gx = (qHash(i * 31) % W);
        int gy = (qHash(i * 97) % H);
        p.drawEllipse(gx, gy, 2, 2);
    }

    // Large faded genre initial as watermark
    QFont wf("Segoe UI", 80, QFont::Black);
    p.setFont(wf);
    p.setPen(QColor(255, 255, 255, 22));
    QChar initial = m.genre.empty() ? '?' : QChar(m.genre[0]);
    p.drawText(QRect(-10, 40, W + 10, H - 40), Qt::AlignCenter, QString(initial));

    // Bottom overlay scrim for text legibility
    QLinearGradient scrim(0, H - 90, 0, H);
    scrim.setColorAt(0, QColor(0, 0, 0, 0));
    scrim.setColorAt(1, QColor(0, 0, 0, 200));
    p.fillRect(0, H - 90, W, 90, scrim);

    // Year badge (top-right)
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0, 0, 0, 140));
    p.drawRoundedRect(W - 52, 10, 42, 22, 11, 11);
    QFont yf("Segoe UI", 8, QFont::Bold);
    p.setFont(yf);
    p.setPen(QColor(255, 215, 0));
    p.drawText(QRect(W - 52, 10, 42, 22), Qt::AlignCenter,
        QString::number(m.year));

    // Genre badge (bottom-left)
    p.setPen(Qt::NoPen);
    QColor badgeCol = base.lighter(110);
    badgeCol.setAlpha(200);
    p.setBrush(badgeCol);
    QString genreStr = QString::fromStdString(m.genre);
    QFont gf("Segoe UI", 8, QFont::DemiBold);
    p.setFont(gf);
    QFontMetrics gfm(gf);
    int bw = gfm.horizontalAdvance(genreStr) + 16;
    p.drawRoundedRect(8, H - 32, bw, 20, 10, 10);
    p.setPen(Qt::white);
    p.drawText(QRect(8, H - 32, bw, 20), Qt::AlignCenter, genreStr);

    p.end();
    return pm;
}

MovieCardWidget::MovieCardWidget(const Movie& movie, QWidget* parent)
    : QFrame(parent), m_id(movie.id)
{
    setObjectName("movieCard");
    setFixedWidth(200);
    setCursor(Qt::PointingHandCursor);
    buildCard(movie);
}

void MovieCardWidget::buildCard(const Movie& m) {
    QVBoxLayout* lay = new QVBoxLayout(this);
    lay->setContentsMargins(0, 0, 0, 10);
    lay->setSpacing(4);

    // Poster image
    m_posterLabel = new QLabel;
    m_posterLabel->setPixmap(generatePosterPixmap(m));
    m_posterLabel->setFixedSize(200, 260);
    m_posterLabel->setAlignment(Qt::AlignCenter);
    m_posterLabel->setScaledContents(true);
    lay->addWidget(m_posterLabel);

    // Text section
    QVBoxLayout* textLay = new QVBoxLayout;
    textLay->setContentsMargins(10, 4, 10, 0);
    textLay->setSpacing(2);

    m_titleLabel = new QLabel(QString::fromStdString(m.title));
    m_titleLabel->setObjectName("cardTitle");
    m_titleLabel->setWordWrap(true);
    textLay->addWidget(m_titleLabel);

    // Rating row: star icon + numeric score
    QString ratingStr = QString("★  %1 / 10").arg(m.rating, 0, 'f', 1);
    m_ratingLabel = new QLabel(ratingStr);
    m_ratingLabel->setObjectName("cardRating");
    textLay->addWidget(m_ratingLabel);

    // Meta: year · duration
    int hrs = m.duration / 60, mins = m.duration % 60;
    QString meta = QString("%1  ·  %2h %3m")
        .arg(m.year)
        .arg(hrs)
        .arg(mins, 2, 10, QChar('0'));
    m_metaLabel = new QLabel(meta);
    m_metaLabel->setObjectName("cardMeta");
    textLay->addWidget(m_metaLabel);

    lay->addLayout(textLay);
    setLayout(lay);
}

void MovieCardWidget::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    emit movieCardClicked(m_id);
}
void MovieCardWidget::enterEvent(QEnterEvent* event) {
    Q_UNUSED(event);
    // Slight lift: handled entirely by QSS hover selector
    update();
}
void MovieCardWidget::leaveEvent(QEvent* event) {
    Q_UNUSED(event);
    update();
}

// ════════════════════════════════════════════════════════════
//   HomeWindow  implementation
// ════════════════════════════════════════════════════════════

HomeWindow::HomeWindow(QWidget* parent) : QMainWindow(parent) {
    // ── Seed library ────────────────────────────────────────
    m_movies = {
    {m_nextId++, "The Matrix",        1999, 8.7, 136, "Sci-Fi",    ":/posters/matrix.jpg"},
    {m_nextId++, "Inception",          2010, 8.8, 148, "Sci-Fi",    ":/posters/inception.jpg"},
    {m_nextId++, "Interstellar",       2014, 8.6, 169, "Sci-Fi",    ":/posters/interstellar.jpg"},
    {m_nextId++, "The Godfather",      1972, 9.2, 175, "Drama",     ":/posters/godfather.jpg"},
    {m_nextId++, "Pulp Fiction",       1994, 8.9, 154, "Thriller",  ":/posters/pulp_fiction.jpg"},
    {m_nextId++, "The Dark Knight",    2008, 9.0, 152, "Action",    ":/posters/dark_knight.jpg"},
    {m_nextId++, "Parasite",           2019, 8.5, 132, "Thriller",  ":/posters/parasite.jpg"},
    {m_nextId++, "Spirited Away",      2001, 8.6, 125, "Animation", ":/posters/spirited_away.jpg"},
    {m_nextId++, "Oppenheimer",        2023, 8.4, 180, "Drama",     ":/posters/oppenheimer.jpg"},
    {m_nextId++, "Dune: Part Two",     2024, 8.5, 166, "Sci-Fi",    ":/posters/dune2.jpg"},
    {m_nextId++, "Blade Runner 2049",  2017, 8.0, 164, "Sci-Fi",    ":/posters/blade_runner.jpg"},
    {m_nextId++, "Get Out",            2017, 7.7, 104, "Thriller",  ":/posters/get_out.jpg"},
    };

    setWindowTitle("Personal Movie Library");
    setMinimumSize(1100, 720);
    resize(1280, 800);

    setupUi();
    applyStyleSheet();
    populateGrid(m_movies);
    refreshStats();
}

// ─────────────────────────────────────────────────────────────
//  setupUi  –  assemble the full window hierarchy
// ─────────────────────────────────────────────────────────────
void HomeWindow::setupUi() {
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);

    // Root: sidebar | right-column
    QHBoxLayout* rootLay = new QHBoxLayout(m_centralWidget);
    rootLay->setContentsMargins(0, 0, 0, 0);
    rootLay->setSpacing(0);

    // ── LEFT: sidebar ────────────────────────────────────────
    QWidget* sidebar = new QWidget;
    sidebar->setFixedWidth(200);
    sidebar->setObjectName("sidebar");

    QVBoxLayout* sidebarLay = new QVBoxLayout(sidebar);
    sidebarLay->setContentsMargins(0, 0, 0, 0);
    sidebarLay->setSpacing(0);

    QLabel* logoLabel = new QLabel("🎬 ELEVATE");
    logoLabel->setObjectName("logoLabel");
    sidebarLay->addWidget(logoLabel);

    m_navList = new QListWidget;
    m_navList->setObjectName("navList");
    m_navList->setFocusPolicy(Qt::NoFocus);

    // Navigation items with Unicode icons
    auto addNavItem = [&](const QString& icon, const QString& label) {
        QListWidgetItem* item = new QListWidgetItem(icon + "   " + label);
        item->setData(Qt::UserRole, label);   // store plain label for routing
        m_navList->addItem(item);
        };
    addNavItem("📚", "Library");
    addNavItem("❤️", "Favourites");
    addNavItem("🕐", "Watchlist");
    addNavItem("🎭", "Genres");

    m_navList->setCurrentRow(0);
    sidebarLay->addWidget(m_navList, 1);
    sidebarLay->addStretch();

    // Sidebar footer: app version
    QLabel* verLabel = new QLabel("v1.0  –  Qt 6 Widgets");
    verLabel->setAlignment(Qt::AlignCenter);
    verLabel->setStyleSheet("color:#3A3A3A; font-size:10px; padding:8px;");
    sidebarLay->addWidget(verLabel);

    rootLay->addWidget(sidebar);

    // ── RIGHT: header + content ──────────────────────────────
    QWidget* rightCol = new QWidget;
    QVBoxLayout* rightLay = new QVBoxLayout(rightCol);
    rightLay->setContentsMargins(0, 0, 0, 0);
    rightLay->setSpacing(0);

    setupHeader();

    // We build the header in-place
    QWidget* headerBar = new QWidget;
    headerBar->setObjectName("headerBar");
    headerBar->setFixedHeight(72);

    QHBoxLayout* headerLay = new QHBoxLayout(headerBar);
    headerLay->setContentsMargins(24, 0, 24, 0);
    headerLay->setSpacing(12);

    // View title
    m_activeViewLabel = new QLabel("Library");
    m_activeViewLabel->setObjectName("viewTitleLabel");
    headerLay->addWidget(m_activeViewLabel);
    headerLay->addStretch();

    // Search bar (with magnifying glass via QSS padding trick)
    m_searchEdit = new QLineEdit;
    m_searchEdit->setObjectName("searchEdit");
    m_searchEdit->setPlaceholderText("🔍  Search movies…");
    headerLay->addWidget(m_searchEdit);

    // Filter button
    m_filterBtn = new QPushButton("⚙  Filter");
    m_filterBtn->setObjectName("filterBtn");
    headerLay->addWidget(m_filterBtn);

    // Add Movie button (gold CTA)
    m_addMovieBtn = new QPushButton("＋  Add Movie");
    m_addMovieBtn->setObjectName("addMovieBtn");
    m_addMovieBtn->setFixedHeight(38);
    headerLay->addWidget(m_addMovieBtn);

    rightLay->addWidget(headerBar);

    // ── Content scroll area ──────────────────────────────────
    m_scrollArea = new QScrollArea;
    m_scrollArea->setObjectName("contentArea");
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_gridContainer = new QWidget;
    m_gridContainer->setObjectName("gridContainer");

    m_gridLayout = new QGridLayout(m_gridContainer);
    m_gridLayout->setContentsMargins(24, 20, 24, 20);
    m_gridLayout->setSpacing(18);
    m_gridLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    m_scrollArea->setWidget(m_gridContainer);
    rightLay->addWidget(m_scrollArea, 1);

    rootLay->addWidget(rightCol, 1);

    // ── Status bar ───────────────────────────────────────────
    statusBar()->setObjectName("statusBar");

    // ── Connections ──────────────────────────────────────────
    connect(m_searchEdit, &QLineEdit::textChanged,
        this, &HomeWindow::on_search_textChanged);
    connect(m_navList, &QListWidget::itemClicked,
        this, &HomeWindow::on_navItem_clicked);
    connect(m_addMovieBtn, &QPushButton::clicked,
        this, &HomeWindow::on_addMovie_clicked);
    connect(m_filterBtn, &QPushButton::clicked,
        this, &HomeWindow::on_filter_clicked);
}

// stubs – not used when building inline header widget
void HomeWindow::setupSidebar() {}
void HomeWindow::setupHeader() {}
void HomeWindow::setupContentArea() {}
void HomeWindow::setupStatusBar() {}

// ─────────────────────────────────────────────────────────────
//  applyStyleSheet  –  apply the full QSS to the application
// ─────────────────────────────────────────────────────────────
void HomeWindow::applyStyleSheet() {
    qApp->setStyleSheet(DARK_QSS);
}

// ─────────────────────────────────────────────────────────────
//  populateGrid  –  fill the grid with MovieCardWidgets
// ─────────────────────────────────────────────────────────────
void HomeWindow::populateGrid(const std::vector<Movie>& list) {
    clearGrid();

    // ✅ Reset column stretches FIRST, before anything is added
    for (int c = 0; c < GRID_COLUMNS; ++c)
        m_gridLayout->setColumnStretch(c, 0);

    int col = 0, row = 0;
    for (const Movie& m : list) {
        MovieCardWidget* card = new MovieCardWidget(m, m_gridContainer);
        connect(card, &MovieCardWidget::movieCardClicked,
            this, &HomeWindow::on_movieCard_clicked);
        m_gridLayout->addWidget(card, row, col);
        ++col;
        if (col >= GRID_COLUMNS) { col = 0; ++row; }
    }

    // Push last incomplete row to the left
    if (col > 0) {
        m_gridLayout->setColumnStretch(col, 1);
    }
}

void HomeWindow::clearGrid() {
    // Remove all widgets from the grid layout
    QLayoutItem* item;
    while ((item = m_gridLayout->takeAt(0)) != nullptr) {
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
}

// ─────────────────────────────────────────────────────────────
//  refreshStats  –  update the status bar
// ─────────────────────────────────────────────────────────────
void HomeWindow::refreshStats() {
    int total = static_cast<int>(m_movies.size());
    int totalMins = 0;
    for (const Movie& m : m_movies) totalMins += m.duration;
    int totalHrs = totalMins / 60;

    statusBar()->showMessage(
        QString("📂  Total Movies: %1   |   ⏱  Total Runtime: %2h   |   ⭐  Avg. Rating: %3")
        .arg(total)
        .arg(totalHrs)
        .arg(total > 0
            ? QString::number(
                std::accumulate(m_movies.begin(), m_movies.end(), 0.0,
                    [](double s, const Movie& m) { return s + m.rating; })
                / total, 'f', 1)
            : "N/A")
    );
}

// ════════════════════════════════════════════════════════════
//   Slots
// ════════════════════════════════════════════════════════════

// ── Search – live filtering by title ────────────────────────
void HomeWindow::on_search_textChanged(const QString& text) {
    if (text.isEmpty()) {
        populateGrid(m_movies);
    }
    else {
        std::string q = text.toLower().toStdString();
        std::vector<Movie> results;
        std::copy_if(m_movies.begin(), m_movies.end(),
            std::back_inserter(results),
            [&q](const Movie& m) {
                std::string t = m.title;
                std::transform(t.begin(), t.end(), t.begin(), ::tolower);
                return t.find(q) != std::string::npos;
            });
        populateGrid(results);
    }
}

// ── Movie card clicked – show detail dialog ──────────────────
void HomeWindow::on_movieCard_clicked(int movieId) {
    auto it = std::find_if(m_movies.begin(), m_movies.end(),
        [movieId](const Movie& m) { return m.id == movieId; });
    if (it == m_movies.end()) return;
    const Movie& m = *it;

    int hrs = m.duration / 60;
    int mins = m.duration % 60;
    QString details =
        QString("<b style='font-size:16px; color:#FFD700;'>%1</b><br><br>"
            "<b>Year:</b> %2<br>"
            "<b>Genre:</b> %3<br>"
            "<b>IMDb Rating:</b> ★ %4 / 10<br>"
            "<b>Duration:</b> %5h %6m<br>")
        .arg(QString::fromStdString(m.title))
        .arg(m.year)
        .arg(QString::fromStdString(m.genre))
        .arg(m.rating, 0, 'f', 1)
        .arg(hrs).arg(mins, 2, 10, QChar('0'));

    QMessageBox mb(this);
    mb.setWindowTitle("Movie Details");
    mb.setTextFormat(Qt::RichText);
    mb.setText(details);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}

// ── Sidebar navigation ────────────────────────────────────────
void HomeWindow::on_navItem_clicked(QListWidgetItem* item) {
    QString view = item->data(Qt::UserRole).toString();
    m_activeView = view;
    m_activeViewLabel->setText(view);
    m_searchEdit->clear();

    if (view == "Library") {
        populateGrid(m_movies);
    }
    else if (view == "Favourites") {
        // Placeholder: show top-rated movies (rating >= 8.8)
        std::vector<Movie> favs;
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(favs),
            [](const Movie& m) { return m.rating >= 8.8; });
        populateGrid(favs);
    }
    else if (view == "Watchlist") {
        // Placeholder: show recent (year >= 2019)
        std::vector<Movie> wl;
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(wl),
            [](const Movie& m) { return m.year >= 2019; });
        populateGrid(wl);
    }
    else if (view == "Genres") {
        // Show genre picker via sort
        QStringList genres;
        for (const Movie& m : m_movies) {
            QString g = QString::fromStdString(m.genre);
            if (!genres.contains(g)) genres << g;
        }
        genres.sort();
        bool ok;
        QString picked = QInputDialog::getItem(this, "Browse by Genre",
            "Select genre:", genres, 0, false, &ok);
        if (ok) {
            std::string sel = picked.toStdString();
            std::vector<Movie> filtered;
            std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
                [&sel](const Movie& m) { return m.genre == sel; });
            populateGrid(filtered);
            m_activeViewLabel->setText("Genres  ›  " + picked);
        }
    }
}

// ── Add Movie ─────────────────────────────────────────────────
void HomeWindow::on_addMovie_clicked() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add New Movie");
    dlg.setMinimumWidth(340);
    QFormLayout form(&dlg);
    form.setContentsMargins(20, 16, 20, 16);
    form.setSpacing(10);

    QLineEdit* titleEdit = new QLineEdit;
    QSpinBox* yearSpin = new QSpinBox;
    QDoubleSpinBox* ratingSpin = new QDoubleSpinBox;
    QSpinBox* durSpin = new QSpinBox;
    QLineEdit* genreEdit = new QLineEdit;

    yearSpin->setRange(1888, 2100);   yearSpin->setValue(2024);
    ratingSpin->setRange(1.0, 10.0);  ratingSpin->setSingleStep(0.1); ratingSpin->setValue(7.0);
    durSpin->setRange(1, 999);        durSpin->setValue(120);

    form.addRow("Title:", titleEdit);
    form.addRow("Year:", yearSpin);
    form.addRow("IMDb Rating:", ratingSpin);
    form.addRow("Duration (min):", durSpin);
    form.addRow("Genre:", genreEdit);

    QDialogButtonBox* btns = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(btns);
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    QLineEdit* posterEdit = new QLineEdit;
    posterEdit->setPlaceholderText("No image selected...");
    posterEdit->setReadOnly(true);
    QPushButton* browseBtn = new QPushButton("Browse…");
    QHBoxLayout* posterRow = new QHBoxLayout;
    posterRow->addWidget(posterEdit);
    posterRow->addWidget(browseBtn);
    form.addRow("Poster:", posterRow);
    connect(browseBtn, &QPushButton::clicked, [&]() {
        QString path = QFileDialog::getOpenFileName(
            &dlg, "Select Poster Image", "",
            "Images (*.png *.jpg *.jpeg *.webp)");
        if (!path.isEmpty())
            posterEdit->setText(path);
        });

    if (dlg.exec() == QDialog::Accepted && !titleEdit->text().trimmed().isEmpty()) {
        Movie m;
        m.id = m_nextId++;
        m.title = titleEdit->text().trimmed().toStdString();
        m.year = yearSpin->value();
        m.rating = ratingSpin->value();
        m.duration = durSpin->value();
        m.genre = genreEdit->text().trimmed().toStdString();
        m.posterPath = posterEdit->text().toStdString();
        m_movies.push_back(m);
        populateGrid(m_movies);
        refreshStats();
    }

}

// ── Filter (placeholder stub) ─────────────────────────────────
void HomeWindow::on_filter_clicked() {
    QStringList options = { "All", "Rating ≥ 8.0", "Rating ≥ 9.0",
                            "Before 2000", "2000–2010", "After 2010" };
    bool ok;
    QString choice = QInputDialog::getItem(this, "Filter Library",
        "Select filter:", options, 0, false, &ok);
    if (!ok || choice == "All") { populateGrid(m_movies); return; }

    std::vector<Movie> filtered;
    if (choice == "Rating ≥ 8.0")
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
            [](const Movie& m) { return m.rating >= 8.0; });
    else if (choice == "Rating ≥ 9.0")
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
            [](const Movie& m) { return m.rating >= 9.0; });
    else if (choice == "Before 2000")
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
            [](const Movie& m) { return m.year < 2000; });
    else if (choice == "2000–2010")
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
            [](const Movie& m) { return m.year >= 2000 && m.year <= 2010; });
    else if (choice == "After 2010")
        std::copy_if(m_movies.begin(), m_movies.end(), std::back_inserter(filtered),
            [](const Movie& m) { return m.year > 2010; });

    populateGrid(filtered);
}