#include "matrixwidget.h"
#include "ui_matrixwidget.h"


#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QString>

#include <algorithm>

MatrixWidget::MatrixWidget(int size, QWidget *parent) :
    QWidget(parent),
    boardSize(size),
    ui(new Ui::MatrixWidget)
{
    ui->setupUi(this);

    // add level settings (delay in secs, amount, board size)
    level_settings.push_back({1000, 1, 2});
    level_settings.push_back({1000, 1, 3});
    level_settings.push_back({1000, 2, 3});
    level_settings.push_back({950, 3, 3});
    level_settings.push_back({850, 4, 3});
    level_settings.push_back({750, 4, 3});
    level_settings.push_back({900, 4, 4});
    level_settings.push_back({750, 4, 4});

    //add icons
    QIcon defaultIcon = QIcon("D:/famcs/qt_labs/build-MatrixMemory-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/resources/icon0.PNG");
    QIcon icon1 = QIcon("D:/famcs/qt_labs/build-MatrixMemory-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/resources/icon1.PNG");
    QIcon icon2 = QIcon("D:/famcs/qt_labs/build-MatrixMemory-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/resources/icon2.PNG");
    QIcon icon3 = QIcon("D:/famcs/qt_labs/build-MatrixMemory-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/resources/icon3.PNG");
    icons.push_back(defaultIcon);
    icons.push_back(icon1);
    icons.push_back(icon2);
    icons.push_back(icon3);

    Layout = new QGridLayout(this);
    Layout->setContentsMargins(100, 100, 100, 100);
    setLayout(Layout);

    generateLevelSettings();
}

MatrixWidget::~MatrixWidget()
{
    delete ui;
}

void MatrixWidget::onCardSelected(int x, int y) {
    if (x != points[userPos].first || y != points[userPos].second) {
        emit gameOver();
        return;
    }
    cards[x][y]->show(cards[x][y]->getShowedIcon());
    userPos++;
    if (userPos == (int)points.size()) {
        emit gameWin();
    }
}

void MatrixWidget::initLayout(int size) {
    for (int i = 0; i < Layout->count(); i++) {
        Layout->itemAt(i)->widget()->deleteLater();
    }
    cards.clear();
    cards.resize(size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            Card *card = new Card(i, j, icons[0]);
            connect(card, SIGNAL(selected(int, int)), this, SLOT(onCardSelected(int, int)));
            Layout->addWidget(card, i, j);
            cards[i].push_back(card);
        }
    }
}

void MatrixWidget::sleept(unsigned int ms) {
    QTime dieTime = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

QVector<std::pair<int, int>> MatrixWidget::getRandomPoints(const int &n, const int &size) {
    QVector<std::pair<int, int>> field;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            field.push_back({i, j});
        }
    }
    random_shuffle(field.begin(), field.end());
    return QVector(field.begin(), field.begin() + n);
}

void MatrixWidget::restartGame(int delta, QString welcomeText) {

    if (currentLevel + delta >= 0 && currentLevel + delta < (int)level_settings.size()) {
        if (std::get<2>(level_settings[currentLevel + delta]) < std::get<2>(level_settings[currentLevel])) {
            if (std::get<1>(level_settings[currentLevel + delta]) > 1) {
                std::get<1>(level_settings[currentLevel + delta])--;
            }
            std::get<0>(level_settings[currentLevel + delta]) += 100;
            //
        } else {
            currentLevel += delta;
        }
    }

    const int delay = std::get<0>(level_settings[currentLevel]);
    const int amount = std::get<1>(level_settings[currentLevel]);
    const int boardSize = std::get<2>(level_settings[currentLevel]);

    initLayout(boardSize);
    update();
    sleept(200);

    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Now you are playing on level: " + QString::number(currentLevel));
    msgBox.setInformativeText("Would you like to start level?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch(ret) {
        case QMessageBox::Yes:
            break;
        case QMessageBox::No:
            this->hide();
            emit gameRefused();
            return;
    }

    points = getRandomPoints(amount, boardSize);

    for (int i = 0; i < (int)points.size(); i++) {
        int p = rand() % 3 + 1;
        cards[points[i].first][points[i].second]->show(icons[p]);
        sleept(delay);
        cards[points[i].first][points[i].second]->hide();
    }

    userPos = 0;
}


void MatrixWidget::generateLevelSettings() {
    int delay = std::get<0>(level_settings.back());
    int amount = std::get<1>(level_settings.back());
    int boardSize = std::get<2>(level_settings.back());
    int iter = 0;
    while (delay >= 100) {
        if (iter % 3 == 0) {
            delay -= 50;
        } else if (iter % 3 == 1) {
            amount += 1;
        } else if (iter % 3 == 2 && boardSize < 8) {
            boardSize += 1;
        }
        iter++;
        level_settings.push_back({delay, amount, boardSize});
    }
}
