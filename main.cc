#include <QApplication>
#include <QTranslator>

#include "mainwindow.hh"

int main(int argc, char *argv[]) {
    QApplication a (argc, argv);

    QTranslator translator;
    if (translator.load(QLocale::system(), "demineur", "_", a.applicationDirPath(), ".qm")) {
        a.installTranslator(&translator);
    }

    mainwindow w;
    w.show();

    return a.exec();
}