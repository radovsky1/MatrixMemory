#ifndef MATRIXWIDGET_H
#define MATRIXWIDGET_H

#include <QWidget>
#include <QVector>
#include <QGridLayout>
#include "card.h"

namespace Ui {
class MatrixWidget;
}

class MatrixWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixWidget(int size = 3, QWidget *parent = nullptr);
    ~MatrixWidget();

    void setLevel(int level) {
        currentLevel = level;
    }

    int getLevel() { return currentLevel; };

    int getLevelBoardSize(int level) {
        if (level >= 0 && level < (int)level_settings.size()) {
            return std::get<2>(level_settings[level]);
        } else if (level < 0) {
            return std::get<2>(level_settings[0]);
        } else {
            return std::get<2>(level_settings.back());
        }
    }

    int getLevelBoardSizeByDelta(int delta) {
        int level = currentLevel + delta;
        return getLevelBoardSize(level);
    }

    void restartGame(int delta, QString welcomeText="");

signals:
    void gameOver();
    void gameWin();
    void gameRefused();

protected slots:
    void onCardSelected(int x, int y);

protected:
    void showLevelBeginningMenu();

private:
    int currentLevel = 0;
    QVector<std::pair<int, int>> points;
    QVector<std::tuple<int, int, int>> level_settings;

    int boardSize;
    int userPos;

    bool wait_for_input;

    QGridLayout *Layout;
    Ui::MatrixWidget *ui;
    QVector<QVector<Card*>> cards;

    QVector<QIcon> icons;

    void sleept(unsigned int ms);

    void initLayout(int size);

    QVector<std::pair<int, int>> getRandomPoints(const int &n,
                                                 const int &size);

    void generateLevelSettings();

    void userMove();
};

#endif // MATRIXWIDGET_H
