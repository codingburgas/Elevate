#include "homeWindow.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    HomeWindow w;
    w.show();
    return app.exec();
}