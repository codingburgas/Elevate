#ifndef NOMINMAX
#define NOMINMAX
#endif

#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
#include <windows.h>
#endif

#include <QApplication>
#include "../include/mainWindow.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(980, 720);
    w.setWindowTitle("Movie Collection Manager");
    w.show();
    return a.exec();
}