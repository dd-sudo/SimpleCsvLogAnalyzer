#include "SimpleCsvLogAnalyzer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SimpleCsvLogAnalyzer w;
    w.show();
    return a.exec();
}
