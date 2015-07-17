#include "mainwindow.h"
#include <QApplication>
#include "cv.h"
#include "highgui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
