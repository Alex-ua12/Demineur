#include "demineur.hh"

#include <QMessageBox>
#include <ctime>
#include <cstdlib>
#include <QApplication>
#include <QMouseEvent>
#include <QPainter>

demineur::demineur(QWidget *parent)
    : QWidget(parent), rows(10), cols(10), mines(10), revealedCells(0) {
    QWidget *centralWidget = new QWidget(this);
    gridLayout = new QGridLayout(centralWidget);
    setCentralWidget(centralWidget);

    setupBoard();
}

demineur::~demineur() {}

void demineur::setupBoard() {
    srand(time(0));
    board.resize(rows);
    buttons.resize(rows);
    for (int i = 0; i < rows; ++i) {
        board[i].resize(cols);
        buttons[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            buttons[i][j] = new QPushButton(this);
            buttons[i][j]->setFixedSize(40, 40);
            gridLayout->addWidget(buttons[i][j], i, j);
            buttons[i][j]->installEventFilter(this);

            // Set checkerboard pattern
            if ((i + j) % 2 == 0) {
                buttons[i][j]->setStyleSheet("background-color: lightgreen;");
            } else {
                buttons[i][j]->setStyleSheet("background-color: green;");
            }

            board[i][j] = 0;
        }
    }

    // Remove spacing between cells
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);

    // Place mines
    for (int i = 0; i < mines; ++i) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (board[row][col] == -1) {
            --i;
        } else {
            board[row][col] = -1;
        }
    }

    // Calculate adjacent mines
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == -1) continue;
            int mineCount = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    int ni = i + x;
                    int nj = j + y;
                    if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && board[ni][nj] == -1) {
                        ++mineCount;
                    }
                }
            }
            board[i][j] = mineCount;
        }
    }
}

bool demineur::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (buttons[i][j] == obj) {
                        handleRightClick(i, j);
                        return true;
                    }
                }
            }
        } else if (mouseEvent->button() == Qt::LeftButton) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (buttons[i][j] == obj) {
                        handleButtonClick(i, j);
                        return true;
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

void demineur::handleButtonClick(int row, int col) {
    revealCell(row, col);
}

void demineur::handleRightClick(int row, int col) {
    if (buttons[row][col]->isEnabled()) { // Only allow flagging if the cell is not revealed
        if (buttons[row][col]->icon().isNull()) {
            buttons[row][col]->setIcon(createFlagIcon());
        } else {
            buttons[row][col]->setIcon(QIcon());
        }
    }
}

void demineur::revealAllMines() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (board[i][j] == -1) {
                buttons[i][j]->setIcon(createMineIcon());
                buttons[i][j]->setEnabled(false);
            }
        }
    }
}

void demineur::revealCell(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols || buttons[row][col]->isEnabled() == false) {
        return;
    }

    // Remove the flag icon if it exists
    if (!buttons[row][col]->icon().isNull()) {
        buttons[row][col]->setIcon(QIcon());
    }

    buttons[row][col]->setEnabled(false);
    buttons[row][col]->setContextMenuPolicy(Qt::NoContextMenu); // Disable right-click context menu

    if (board[row][col] == -1) {
        buttons[row][col]->setIcon(createMineIcon());
        revealAllMines();
        QMessageBox::information(this, "Game Over", "You clicked on a mine!");
        QApplication::quit();
    } else {
        QString styleSheet;
        if ((row + col) % 2 == 0) {
            styleSheet = "background-color: beige;";
        } else {
            styleSheet = "background-color: #d2b48c;"; // darker beige
        }

        if (board[row][col] == 0) {
            buttons[row][col]->setText("");
            revealAdjacentCells(row, col);
        } else {
            buttons[row][col]->setText(QString::number(board[row][col]));
            switch (board[row][col]) {
                case 1:
                    styleSheet += "color: blue;";
                    break;
                case 2:
                    styleSheet += "color: green;";
                    break;
                case 3:
                    styleSheet += "color: red;";
                    break;
                case 4:
                    styleSheet += "color: purple;";
                    break;
                case 5:
                    styleSheet += "color: yellow;";
                    break;
                default:
                    styleSheet += "color: black;";
                    break;
            }
        }
        buttons[row][col]->setStyleSheet(styleSheet);
    }

    revealedCells++;
    checkWinCondition();
}

void demineur::revealAdjacentCells(int row, int col) {
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            int ni = row + x;
            int nj = col + y;
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && buttons[ni][nj]->isEnabled()) {
                revealCell(ni, nj);
            }
        }
    }
}

void demineur::checkWinCondition() {
    if (revealedCells == rows * cols - mines) {
        QMessageBox::information(this, "Congratulations", "You have won the game!");
        QApplication::quit();
    }
}

QIcon demineur::createFlagIcon() {
    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(Qt::red);
    painter.drawRect(10, 10, 20, 20);
    painter.setBrush(Qt::black);
    painter.drawRect(15, 30, 10, 10);
    return QIcon(pixmap);
}

QIcon demineur::createMineIcon() {
    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setBrush(Qt::black);
    painter.drawEllipse(10, 10, 20, 20);
    return QIcon(pixmap);
}

void demineur::initializeButtons() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            buttons[i][j] = new QPushButton(this);
            buttons[i][j]->setFixedSize(40, 40);
            buttons[i][j]->setStyleSheet(
                "QPushButton {"
                "background-color: lightgray;"
                "}"
                "QPushButton:hover {"
                "background-color: rgba(0, 0, 0, 1);" // Darker color with less transparency
                "}"
            );
            connect(buttons[i][j], &QPushButton::clicked, [this, i, j]() { handleButtonClick(i, j); });
            buttons[i][j]->installEventFilter(this);
            gridLayout->addWidget(buttons[i][j], i, j);
        }
    }
}