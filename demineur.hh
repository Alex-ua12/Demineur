#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

class demineur : public QWidget {
    Q_OBJECT

    public:
        demineur(QWidget *parent = nullptr);
        ~demineur();

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private slots:
        void handleButtonClick(int row, int col);
        void handleRightClick(int row, int col);

    private:
        void setupBoard();
        void revealCell(int row, int col);
        void checkWinCondition();
        void revealAdjacentCells(int row, int col);
        QIcon createFlagIcon();
        QIcon createMineIcon();
        void revealAllMines();
        void initializeButtons();

        int rows;
        int cols;
        int mines;
        int revealedCells;
        std::vector<std::vector<int>> board;
        std::vector<std::vector<QPushButton*>> buttons;
        QGridLayout *gridLayout;
};