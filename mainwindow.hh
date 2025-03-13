#pragma once

#include <QMainWindow>
#include <QLCDNumber>
#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QDialogButtonBox>

class mainwindow : public QMainWindow {
    Q_OBJECT

    public :
        mainwindow(QWidget * parent = nullptr);
        ~mainwindow(){}

    private :
        void create_dockWidget();
        void create_dialog();
        void create_actions();
        void create_menu();

        QAction * _load_game;
        QAction * _quit;

        QAction * view_scores_easy;
        QAction * view_scores_medium;
        QAction * view_scores_hard;

        QAction * about_qt;
        QAction * about_game;
        QAction * about_author;

        QDockWidget * dockWidget;
        QWidget * widget;
        QHBoxLayout * layout;

        QLCDNumber * timer;
        // quand tu mets un drapeau sur une case, le nombre de mines restantes diminue
        QLCDNumber * mines;
        QLabel * imageLabel;

        QRadioButton * easy;
        QRadioButton * medium;
        QRadioButton * hard;

        QDialog * dialog;

    private slots:
        void view_scores(QString const & difficulty);
        void load_about_qt();
        void load_about_game();
        void load_about_author();
        void view_possebilities_game();
};