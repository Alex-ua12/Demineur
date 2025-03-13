#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QIcon>
#include <QDockWidget>
#include <QDialog>
#include <QGroupBox>
#include <filesystem>
#include <iostream>
#include <fstream>

#include "mainwindow.hh"

mainwindow::mainwindow(QWidget * parent)
    : QMainWindow(parent) {
        this -> setWindowTitle("Démineur");
        this -> setWindowIcon(QIcon("../images/drapeau.png"));

        create_dockWidget();
        create_dialog();
        create_actions();
        create_menu();

        this -> setFixedSize(260,80);
}

void mainwindow::create_dockWidget() {
    // création du dockWidget
    dockWidget = new QDockWidget(tr("Head"), this);
    // le dockWidget ne peut être que sur la gauche ou la droite
    dockWidget -> setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    // le dockWidget est fixe
    widget = new QWidget(dockWidget);
    // le layout du dockWidget
    layout = new QHBoxLayout(widget);
}

void mainwindow::create_dialog() {
    // création des boutons radio pour choisir la difficulté
    easy = new QRadioButton(tr("Easy - 10x10 with 10 bomb"), this);
    medium = new QRadioButton(tr("Medium - 16x16 with 16 bomb"), this);
    hard = new QRadioButton(tr("Hard - 32x16 with 64 bomb"), this);

    // Créer un QDialog
    dialog = new QDialog(this);
    dialog -> setWindowTitle("Choose your difficulty");
    dialog -> resize(150,100);

    // Ajouter les QRadioButton à un QVBoxLayout
    QVBoxLayout * vbox = new QVBoxLayout;
    vbox -> addWidget(easy);
    vbox -> addWidget(medium);
    vbox -> addWidget(hard);

    // Ajouter un bouton pour valider la sélection
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, dialog);
    connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

    // // Ajouter les boutons radio et le bouton de validation au layout principal du QDialog
    vbox -> addWidget(buttonBox);
    dialog -> setLayout(vbox);
}

void mainwindow::create_actions() {
    // action pour créer une nouvelle partie
    _load_game = new QAction(tr("Create a new game"), this);
    connect(_load_game, &QAction::triggered, this, [this]() { view_possebilities_game(); });

    // action pour quitter l'application
    _quit = new QAction(tr("Exit"), this);
    _quit -> setShortcut(QKeySequence::Quit);
    _quit -> setStatusTip(tr("Close the application"));
    connect(_quit, &QAction::triggered, this, &QApplication::quit);

    // action pour choisir les difficultés
    view_scores_easy = new QAction(tr("Easy"), this);
    view_scores_medium = new QAction(tr("Medium"), this);
    view_scores_hard = new QAction(tr("Hard"), this);

    // action pour afficher les scores
    connect(view_scores_easy, &QAction::triggered, this, [this]() { view_scores("easy"); });
    connect(view_scores_medium, &QAction::triggered, this, [this]() { view_scores("medium"); });
    connect(view_scores_hard, &QAction::triggered, this, [this]() { view_scores("hard"); });

    // action pour afficher les informations sur le jeu
    about_qt = new QAction(tr("About Qt"), this);
    about_game = new QAction(tr("About the game"), this);
    about_author = new QAction(tr("About the author"), this);

    // action pour afficher les informations sur Qt
    connect(about_qt, &QAction::triggered, this, [this]() { load_about_qt(); });

    // action pour afficher les informations sur le jeu
    connect(about_game, &QAction::triggered, this, [this]() { load_about_game(); });

    // action pour afficher les informations sur l'auteur
    connect(about_author, &QAction::triggered, this, [this]() { load_about_author(); });

    // création du timer et du nombre de mines restantes
    timer = new QLCDNumber(widget);
    timer -> setSegmentStyle(QLCDNumber::Flat);
    timer -> setFixedSize(100, 50);
    timer -> display("000");
    timer -> setDigitCount(3);

    mines = new QLCDNumber(widget);
    mines -> setSegmentStyle(QLCDNumber::Flat);
    mines -> setFixedSize(100, 50);
    mines -> display("000");
    mines -> setDigitCount(3);

    // création de l'image
    imageLabel = new QLabel(widget);
    imageLabel -> setPixmap(QPixmap("../images/neutre.png"));
    imageLabel -> setAlignment(Qt::AlignCenter);

}

void mainwindow::create_menu() {
    // menu pour les actions
    QMenu * menu = menuBar() -> addMenu(tr("&File"));
    menu -> addAction(_load_game);
    menu -> addSeparator();
    menu -> addAction(_quit);

    // menu pour les scores
    QMenu * scores = menuBar() -> addMenu(tr("&Scores"));
    scores -> addAction(view_scores_easy);
    scores -> addAction(view_scores_medium);
    scores -> addAction(view_scores_hard);

    // menu a propos
    QMenu * about = menuBar() -> addMenu(tr("&About"));
    about -> addAction(about_qt);
    about -> addAction(about_game);
    about -> addAction(about_author);

    // ajouter les Timer et l'image dans le dockWidget
    layout -> addWidget(timer);
    layout -> addWidget(imageLabel);
    layout -> addWidget(mines);

    // ajouter le dockWidget dans la fenêtre principale
    widget -> setLayout(layout);
    dockWidget -> setWidget(widget);
}

void mainwindow::view_scores(QString const & difficulty) {
    QString filename;

    if(!std::filesystem::exists("scores")) {
        std::filesystem::create_directory("scores");
    }

    // choisir le fichier en fonction de la difficulté
    if (difficulty == "easy") {
        filename = "scores/facile.txt";
    } else if (difficulty == "medium") {
        filename = "scores/moyen.txt";
    } else if (difficulty == "hard") {
        filename = "scores/difficile.txt";
    }

    // ouvrir le fichier
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Le fichier n'existe pas, essayez de le créer
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            // Le fichier a été créé avec succès
            file.close();
        } else {
            // Gérer l'erreur de création du fichier
            qWarning("Impossible de créer le fichier.");
        }
    } else {
        // Le fichier existe et a été ouvert en mode lecture
        file.close();
    }

    // lire le contenu du fichier
    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    // afficher le contenu du fichier
    if (difficulty == "easy") {
        QMessageBox::information(this, tr("Score-board Easy"), content);
    } else if (difficulty == "medium") {
        QMessageBox::information(this, tr("Score-board Medium"), content);
    } else if (difficulty == "hard") {
        QMessageBox::information(this, tr("Score-board Hard"), content);
    }
}

void mainwindow::load_about_qt() {
    QMessageBox::aboutQt(nullptr, tr("About Qt"));
}

void mainwindow::load_about_game() {
    QString filename = tr("../about/about_game_en.txt");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QMessageBox::information(nullptr, tr("About the game"), content);
}

void mainwindow::load_about_author() {
    QString filename = tr("../about/about_author_en.txt");

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QMessageBox::information(nullptr, tr("About the author"), content);
}

void mainwindow::view_possebilities_game() {
    // Afficher le QDialog
    if (dialog -> exec() == QDialog::Accepted) {
        // Gérer la sélection ici
        if (easy -> isChecked()) {
            // Option 1 sélectionnée
            easy -> setChecked(false);
        } else if (medium -> isChecked()) {
            // Option 2 sélectionnée
            medium -> setChecked(false);
        } else if (hard -> isChecked()) {
            // Option 3 sélectionnée
            hard -> setChecked(false);
        }
    }
}