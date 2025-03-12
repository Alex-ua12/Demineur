#pragma once

#include <QMainWindow>

class mainwindow : public QMainWindow {
    Q_OBJECT

    public :
        mainwindow(QWidget * parent = nullptr);
        ~mainwindow() = default;

    private :
        void create_actions();
        void create_menu();

        QAction * _load_game;
        QAction * _quit;
};