#include "../include/mainWindow.h"

#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QFont>
#include <QTableWidgetItem>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // Seed with your existing default movies
    movies = {
        {nextId++, "The Matrix",      1999, 8.7, 136, "Sci-Fi"   },
        {nextId++, "Inception",       2010, 8.8, 148, "Sci-Fi"   },
        {nextId++, "Interstellar",    2014, 8.6, 169, "Sci-Fi"   },
        {nextId++, "The Godfather",   1972, 9.2, 175, "Drama"    },
        {nextId++, "Pulp Fiction",    1994, 8.9, 154, "Thriller" },
        {nextId++, "The Dark Knight", 2008, 9.0, 152, "Action"   },
        {nextId++, "Parasite",        2019, 8.5, 132, "Thriller" },
        {nextId++, "Spirited Away",   2001, 8.6, 125, "Animation"},
        {nextId++, "Oppenheimer",     2023, 8.4, 180, "Drama"    },
        {nextId++, "Dune: Part Two",  2024, 8.5, 166, "Sci-Fi"   },
    };
    setupUi();
    populateTable();
}

void MainWindow::setupUi() {
    QWidget* central = new QWidget;
    setCentralWidget(central);

    const QString darkStyle = R"(
        QWidget { background: #121315; color: #cfcfcf; font-family: "Segoe UI"; }
        QLineEdit, QTableWidget, QTextEdit { background: #191a1b; border: 1px solid #2b2c2d; }
        QHeaderView::section { background: #1e1f20; color: #cfcfcf; border: 1px solid #2b2c2d; }
        QPushButton#primary { background: #2bd66b; color: #031005; border-radius: 6px; padding: 10px; font-weight: bold; }
        QPushButton#danger  { background: #d63b3b; color: #ffffff;  border-radius: 6px; padding: 10px; font-weight: bold; }
        QPushButton#icon    { background: #222324; border: 1px solid #2b2c2d; }
        QTableWidget::item  { padding: 8px; }
    )";
    qApp->setStyleSheet(darkStyle);

    QVBoxLayout* mainLay = new QVBoxLayout;
    central->setLayout(mainLay);

    QLabel* titleLabel = new QLabel("Movie Collection Manager (C++ / Qt)");
    QFont tf = titleLabel->font(); tf.setPointSize(18); tf.setBold(true);
    titleLabel->setFont(tf);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(titleLabel);

    // --- Search bar ---
    QHBoxLayout* searchLay = new QHBoxLayout;
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("Search by title...");
    searchBtn = new QPushButton(QString::fromUtf8("\U0001F50D"));
    searchBtn->setObjectName("icon");
    searchBtn->setFixedWidth(40);
    searchLay->addWidget(searchEdit);
    searchLay->addWidget(searchBtn);
    mainLay->addLayout(searchLay);
    connect(searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);

    // --- Toolbar buttons ---
    QHBoxLayout* toolLay = new QHBoxLayout;
    QPushButton* addBtn = new QPushButton("+ Add Movie");
    QPushButton* delBtn = new QPushButton("− Delete Selected");
    QPushButton* sortBtn = new QPushButton("↕ Sort");
    addBtn->setObjectName("primary");
    delBtn->setObjectName("danger");
    sortBtn->setObjectName("icon");
    toolLay->addWidget(addBtn);
    toolLay->addWidget(delBtn);
    toolLay->addWidget(sortBtn);
    toolLay->addStretch();
    mainLay->addLayout(toolLay);
    connect(addBtn, &QPushButton::clicked, this, &MainWindow::onAddMovie);
    connect(delBtn, &QPushButton::clicked, this, &MainWindow::onDeleteMovie);
    connect(sortBtn, &QPushButton::clicked, this, &MainWindow::onSortMovies);

    // --- Table ---
    table = new QTableWidget;
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({ "ID", "Title", "Year", "Rating", "Duration (min)", "Genre", "Select" });
    table->verticalHeader()->setVisible(false);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->setShowGrid(true);
    mainLay->addWidget(table, 1);

    // --- Recursive duration panel ---
    QLabel* panelTitle = new QLabel("Recursive Duration Calculation");
    QFont pf = panelTitle->font(); pf.setPointSize(12); pf.setBold(true);
    panelTitle->setFont(pf);
    mainLay->addWidget(panelTitle);

    calcButton = new QPushButton("Calculate Total Duration of Selected Movies (Recursive)");
    calcButton->setObjectName("primary");
    calcButton->setFixedHeight(44);
    mainLay->addWidget(calcButton);
    connect(calcButton, &QPushButton::clicked, this, &MainWindow::calculateTotal);

    resultLabel = new QLabel("Total Duration of (0) Selected Movies: 0 min.");
    mainLay->addWidget(resultLabel);

    // --- Code snippet ---
    codeBlock = new QTextEdit;
    codeBlock->setReadOnly(true);
    codeBlock->setFixedHeight(130);
    codeBlock->setStyleSheet("QTextEdit { background:#0f1112; color:#cfead4; border-radius:6px; border:1px solid #232425; }");
    QFont mono("Courier New"); mono.setPointSize(10);
    codeBlock->setFont(mono);
    codeBlock->setHtml(R"(
<pre style="margin:6px;">
<span style="color:#6ab0ff;">// Recursive total duration (MovieCollection.cpp)</span>
<span style="color:#ffcb6b;">int</span> <b>recursiveTotalDuration</b>(
    <span style="color:#c6b9fe;">const std::vector&lt;Movie&gt;&amp; movies</span>,
    <span style="color:#c6b9fe;">const std::vector&lt;int&gt;&amp; selectedIds</span>,
    <span style="color:#c6b9fe;">int index</span> = 0) {
    <span style="color:#6ab0ff;">if (index &gt;= selectedIds.size()) return 0;</span>  <span style="color:#6ab0ff;">// base case</span>
    <span style="color:#6ab0ff;">// find movie by id, add duration, recurse</span>
    <b>return</b> duration + recursiveTotalDuration(movies, selectedIds, index + 1);
}</pre>)");
    QHBoxLayout* bottomLay = new QHBoxLayout;
    bottomLay->addStretch();
    bottomLay->addWidget(codeBlock);
    mainLay->addLayout(bottomLay);
}

// ── populateTable (full list) ──────────────────────────
void MainWindow::populateTable() {
    populateTable(movies);
}

void MainWindow::populateTable(const std::vector<Movie>& list) {
    table->setRowCount((int)list.size());
    for (int r = 0; r < (int)list.size(); ++r) {
        const Movie& m = list[r];

        table->setItem(r, 0, new QTableWidgetItem(QString::number(m.id)));
        table->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(m.title)));
        table->setItem(r, 2, new QTableWidgetItem(QString::number(m.year)));

        // Green star rating using ratingStars() from Utils
        QString stars = QString::fromStdString(ratingStars(m.rating))
            + QString(" (%1)").arg(m.rating, 0, 'f', 1);
        QLabel* ratingLbl = new QLabel;
        ratingLbl->setText(QString("<span style='color:#2bd66b;'>%1</span>").arg(stars));
        ratingLbl->setAlignment(Qt::AlignCenter);
        table->setCellWidget(r, 3, ratingLbl);

        table->setItem(r, 4, new QTableWidgetItem(
            QString::fromStdString(formatDuration(m.duration))
            + QString(" (%1 min)").arg(m.duration)));
        table->setItem(r, 5, new QTableWidgetItem(QString::fromStdString(m.genre)));

        QCheckBox* chk = new QCheckBox;
        QWidget* chkWrap = new QWidget;
        QHBoxLayout* chkLay = new QHBoxLayout(chkWrap);
        chkLay->addWidget(chk); chkLay->setAlignment(Qt::AlignCenter); chkLay->setContentsMargins(0, 0, 0, 0);
        table->setCellWidget(r, 6, chkWrap);
    }
    table->resizeRowsToContents();
}

// ── Search — calls searchByTitle() from MovieCollection ──
void MainWindow::onSearchChanged(const QString& text) {
    std::string q = text.toStdString();
    if (q.empty()) {
        populateTable();    // restore full list
    }
    else {
        auto results = searchByTitle(movies, q);   // ← your existing function
        populateTable(results);
    }
}

// ── Add Movie — Qt dialog, then pushes into movies vector ──
void MainWindow::onAddMovie() {
    QDialog dlg(this);
    dlg.setWindowTitle("Add Movie");
    QFormLayout form(&dlg);

    QLineEdit* titleEdit = new QLineEdit;
    QSpinBox* yearSpin = new QSpinBox;  yearSpin->setRange(1888, 2100); yearSpin->setValue(2024);
    QDoubleSpinBox* ratingSpin = new QDoubleSpinBox; ratingSpin->setRange(1.0, 10.0); ratingSpin->setSingleStep(0.1); ratingSpin->setValue(7.0);
    QSpinBox* durSpin = new QSpinBox;  durSpin->setRange(1, 999); durSpin->setValue(120);
    QLineEdit* genreEdit = new QLineEdit;

    form.addRow("Title:", titleEdit);
    form.addRow("Year:", yearSpin);
    form.addRow("Rating:", ratingSpin);
    form.addRow("Duration(min):", durSpin);
    form.addRow("Genre:", genreEdit);

    QDialogButtonBox* btns = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form.addRow(btns);
    connect(btns, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btns, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted && !titleEdit->text().isEmpty()) {
        Movie m;
        m.id = nextId++;
        m.title = titleEdit->text().toStdString();
        m.year = yearSpin->value();
        m.rating = ratingSpin->value();
        m.duration = durSpin->value();
        m.genre = genreEdit->text().toStdString();
        movies.push_back(m);      // ← same as addMovie() but via GUI input
        populateTable();
    }
}

// ── Delete — uses deleteMovie() logic (find by ID) ──────
void MainWindow::onDeleteMovie() {
    bool ok;
    int id = QInputDialog::getInt(this, "Delete Movie", "Enter movie ID to delete:", 1, 1, 99999, 1, &ok);
    if (!ok) return;

    auto it = std::find_if(movies.begin(), movies.end(),
        [id](const Movie& m) { return m.id == id; });
    if (it == movies.end()) {
        QMessageBox::warning(this, "Not Found",
            QString("No movie with ID=%1 found.").arg(id));
    }
    else {
        QString removed = QString::fromStdString(it->title);
        movies.erase(it);    // ← same logic as deleteMovie()
        populateTable();
        QMessageBox::information(this, "Deleted",
            QString("Removed: %1").arg(removed));
    }
}

// ── Sort — calls your existing sortMovies() ─────────────
void MainWindow::onSortMovies() {
    QStringList fields = { "Title", "Year", "Rating", "Duration" };
    bool ok;
    QString chosen = QInputDialog::getItem(this, "Sort By", "Field:", fields, 0, false, &ok);
    if (!ok) return;

    QStringList dirs = { "Ascending", "Descending" };
    QString dir = QInputDialog::getItem(this, "Direction", "Order:", dirs, 0, false, &ok);
    if (!ok) return;

    SortField field = SortField::TITLE;
    if (chosen == "Year")     field = SortField::YEAR;
    if (chosen == "Rating")   field = SortField::RATING;
    if (chosen == "Duration") field = SortField::DURATION;

    sortMovies(movies, field, dir == "Ascending");   // ← your existing function
    populateTable();
}

// ── Calculate — calls recursiveTotalDuration() ──────────
void MainWindow::calculateTotal() {
    std::vector<int> selectedIds;

    for (int r = 0; r < table->rowCount(); ++r) {
        QWidget* wrap = table->cellWidget(r, 6);
        if (!wrap) continue;
        QCheckBox* chk = wrap->findChild<QCheckBox*>();
        if (chk && chk->isChecked()) {
            // get the ID from column 0
            int id = table->item(r, 0)->text().toInt();
            selectedIds.push_back(id);
        }
    }

    // ← calls YOUR recursive function from MovieCollection.cpp
    int total = recursiveTotalDuration(movies, selectedIds);
    QString formatted = QString::fromStdString(formatDuration(total));

    resultLabel->setText(
        QString("Total Duration of (%1) Selected Movies: %2 min  (%3)")
        .arg(selectedIds.size()).arg(total).arg(formatted));
}