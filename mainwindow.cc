#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>

#include "mainwindow.hh"

mainwindow::mainwindow(QWidget * parent)
    : QMainWindow(parent) {
        create_actions();
        create_menu();
}

void mainwindow::create_actions() {
    // action pour créer une nouvelle partie
    _load_game = new QAction(tr("Create a new game"), this);

    // action pour quitter l'application
    _quit = new QAction(tr("Exit"), this);
    _quit -> setShortcut(QKeySequence::Quit);
    _quit -> setStatusTip(tr("Close the application"));
    connect(_quit, &QAction::triggered, this, &QApplication::quit);
}

void mainwindow::create_menu() {
    // menu pour les actions
    QMenu * menu = menuBar() -> addMenu(tr("File"));
    menu -> addAction(_load_game);
    menu -> addSeparator();
    menu -> addAction(_quit);

    // menu pour les scores
    QMenu * scores = menuBar() -> addMenu(tr("Scores"));

    // menu a propos
    QMenu * about = menuBar() -> addMenu(tr("About"));
}