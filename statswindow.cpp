#include "statswindow.h"
#include "ui_statswindow.h"
#include <QMap>
#include <QDebug>

StatsWindow::StatsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StatsWindow)
{
    ui->setupUi(this);
    gridLayout = new QGridLayout(ui->widget);
    this->setWindowTitle("Your stats");
}

StatsWindow::~StatsWindow()
{
    delete ui;
}

void StatsWindow::createUI(User user, QVector<std::pair<User, int>> usersData) {
    ui->listWidget->clear();
    ui->listWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QListWidgetItem *title = new QListWidgetItem("TOP PLAYERS");
    title->setTextAlignment(Qt::AlignCenter);
    ui->listWidget->addItem(title);
    QListWidgetItem *spacer = new QListWidgetItem("");
    ui->listWidget->addItem(spacer);

    QVector<int> data;
    QMap<QString, int> cnt;

    for (auto u : usersData) {
        QString name = u.first.getUserData().first;
        if (name == "") {
            continue;
        }
        cnt[name] = std::max(cnt[name], u.second);
        if (name == user.getUserData().first) {
            data.append(u.second);
        }
    }

    std::reverse(data.begin(), data.end());
    ui->widget->setData(data);

    auto comp = [&](const std::pair<int, QString> &p1, const std::pair<int, QString> &p2) {
        return p1.first < p2.first;
    };

    QVector<std::pair<int, QString>> players;
    for (auto u : cnt.keys()) {
        players.push_back({int(cnt[u]), u});
    }

    std::sort(players.begin(), players.end(), comp);
    std::reverse(players.begin(), players.end());

    for (int i = 0; i < (int)players.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem('\n' + players[i].second + " - " + QString::number(players[i].first) + " levels passed\n");
        item->setTextAlignment(Qt::AlignCenter);
        if (i == 0) {
            item->setBackground(Qt::yellow);
        } else if (i == 1) {
            item->setBackground(Qt::gray);
        } else if (i == 2) {
            item->setBackground(QColorConstants::Svg::brown);
        }
//        if (players[i].second == user.getUserData().first) {
//            item->setBackground(Qt::green);
//        }
        ui->listWidget->addItem(item);
    }

    ui->widget->makePlot();
    gridLayout->addWidget(ui->widget->chartView, 0, 0);
}


void StatsWindow::resizeEvent(QResizeEvent *) {
    ui->widget->move(this->width() / 2, 0);
    ui->widget->resize(this->width() / 2, this->height() - 40);
    ui->listWidget->resize(this->width() / 2, this->height() - 40);
}
