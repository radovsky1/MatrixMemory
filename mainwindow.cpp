#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bcrypt.h"

#include <QInputDialog>
#include <QFileDialog>
#include <QLabel>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QSlider>
#include <QPushButton>
#include <QDebug>
#include <QSpacerItem>
#include <QTextStream>
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const int boardSize = 2;
    setFixedSize(QSize((boardSize + 3) * 100, (boardSize + 3) * 100));
    widget = new MatrixWidget(boardSize, this);
    statsWindow = new StatsWindow(this);
    connect(widget, &MatrixWidget::gameOver, this, &MainWindow::showGameOverMessage);
    connect(widget, &MatrixWidget::gameWin, this, &MainWindow::showGameWinMessage);
    setCentralWidget(widget);

    connect(ui->actionLoad, SIGNAL(triggered()), this, SLOT(onLoadTriggered()));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(onSaveTriggered()));
    connect(ui->actionRegister, SIGNAL(triggered()), this, SLOT(onRegisterTriggered()));
    connect(ui->actionLogin, SIGNAL(triggered()), this, SLOT(onLoginTriggered()));
    connect(ui->actionStats, SIGNAL(triggered()), this, SLOT(onStatsTriggered()));

    //connect(widget, &MatrixWidget::gameRefused, this, &MainWindow::showStartMenu);
    //showStartMenu();
    //setFixedSize(QSize((boardSize + 1 + 3) * 100, (boardSize + 1 + 3) * 100));
    this->show();
    widget->restartGame(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStartMenu() {
    QDialog dialog;
    QFormLayout form(&dialog);
    dialog.setFixedHeight(400);
    dialog.setFixedWidth(600);
    QLabel *label = new QLabel();
    label->setAlignment(Qt::AlignCenter);
    QFont font("Times", 30);
    label->setFont(font);
    label->setText("Matrix Memory");
    form.addRow(label);
    QSpacerItem *spacer = new QSpacerItem(600, 100);
    form.addItem(spacer);
    QPushButton *ll = new QPushButton("Load previous game", this);
    connect(ll, SIGNAL(clicked()), SLOT(onLoadTriggered()));
    form.addWidget(ll);
    form.addItem(spacer);
    QPushButton *playButton = new QPushButton("PLAY", this);
    playButton->setFixedHeight(50);
    form.addWidget(playButton);
    connect(playButton, SIGNAL(clicked()), &dialog, SLOT(accept()));
    if (dialog.exec() == QDialog::Accepted) {
        widget->restartGame(0);
    } else {
        this->close();
    }
}

void MainWindow::showGameOverMessage() {
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("You are wrong!");
    msgBox.setInformativeText("Would you like to play again?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch(ret) {
        case QMessageBox::Yes:
            widget->restartGame(-1);
            break;
        case QMessageBox::No:
            this->hide();
            showStartMenu();
            break;
    }
}

void MainWindow::showGameWinMessage() {
    QMessageBox msgBox;
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText("Absolutely correct. You win!");
    msgBox.setInformativeText("Would you like to play next level?");
    msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    int newBoardSize = widget->getLevelBoardSizeByDelta(+1);
    switch(ret) {
        case QMessageBox::Yes:
            setFixedSize(QSize((newBoardSize + 3) * 100, (newBoardSize + 3) * 100));
            widget->restartGame(+1);
            break;
        case QMessageBox::No:
            this->hide();
            showStartMenu();
            break;
    }
}

void MainWindow::showWarningMessage(QString msg) {
    QMessageBox::warning(this, tr("Warning"), msg, QMessageBox::Yes);
}

void MainWindow::showOkMessage(QString msg) {
    QMessageBox::information(this, tr("OK"), msg, QMessageBox::Yes);
}

void MainWindow::onLoadTriggered() {
    if (!verified) {
        showWarningMessage("You must login or register to do this action!");
        return;
    }

    QString filename = dbPath;
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QTextStream in(&f);

    std::pair<QString, QString> userData = user.getUserData();

    QVector<std::pair<User, int>> usersData = getUsersData();
    for (std::pair<User, int> u : usersData) {
        std::pair<QString, QString> checkedUserData = u.first.getUserData();
        if (checkedUserData.first == userData.first) {
            int currentLevel = u.second;
            widget->setLevel(currentLevel);
            int newBoardSize = widget->getLevelBoardSize(currentLevel);
            setFixedSize(QSize((newBoardSize + 3) * 100, (newBoardSize + 3) * 100));
            this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);
            widget->restartGame(0);
            break;
        }
    }

    f.close();
}

void MainWindow::onSaveTriggered() {
    if (!verified) {
        showWarningMessage("You must login or register to do this action!");
        return;
    }

    QString filename = dbPath;
    QFile f(filename);
    f.open(QIODevice::Append);
    QTextStream out(&f);

    std::pair<QString, QString> userData = user.getUserData();

    out << userData.first << " " << userData.second << " " << widget->getLevel() << '\n';

    f.close();

    showOkMessage("Saved successfully!");
}

void MainWindow::onRegisterTriggered() {
    if (verified) {
        showWarningMessage("You are already registred!");
        return;
    }

    QDialog dialog(this);

    QFormLayout form(&dialog);
    form.addRow(new QLabel("Enter data:"));

    QLineEdit *usernameLineEdit = new QLineEdit(&dialog);
    form.addRow("Username:", usernameLineEdit);
    QLineEdit *passLineEdit = new QLineEdit(&dialog);
    passLineEdit->setEchoMode(QLineEdit::Password);
    form.addRow("Password:", passLineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        bool exist = false;
        User newUser = User(usernameLineEdit->text(), passLineEdit->text(), false);
        QVector<std::pair<User, int>> users = getUsersData();
        for (auto u : users) {
            auto userData = u.first.getUserData();
            if (userData.first == newUser.getUserData().first) {
                exist = true;
            }
        }
        if (exist) {
            showWarningMessage("This name is already taken!");
        } else {
            user = newUser;
            verified = true;
            onSaveTriggered();
            showOkMessage("Registred successfully!");
        }
    }

}

void MainWindow::onLoginTriggered() {
    if (verified) {
        showWarningMessage("You are already loggined!");
        return;
    }

    QDialog dialog(this);

    QFormLayout form(&dialog);
    form.addRow(new QLabel("Enter data:"));

    QLineEdit *usernameLineEdit = new QLineEdit(&dialog);
    form.addRow("Username:", usernameLineEdit);
    QLineEdit *passLineEdit = new QLineEdit(&dialog);
    passLineEdit->setEchoMode(QLineEdit::Password);
    form.addRow("Password:", passLineEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
    QObject::connect(&buttonBox, SIGNAL(rejected()), &dialog, SLOT(reject()));

    if (dialog.exec() == QDialog::Accepted) {
        QString username = usernameLineEdit->text();
        QString pass = passLineEdit->text();

        QVector<std::pair<User, int>> usersData = getUsersData();
        int userLevel = -1;
        for (std::pair<User, int> u : usersData) {
            std::pair<QString, QString> checkedUserData = u.first.getUserData();
            if (checkedUserData.first == username && bcrypt::validatePassword(pass.toStdString(), checkedUserData.second.toStdString())) {
                verified = true;
                userLevel = u.second;
                user = u.first;
                break;
            }
        }
        if (verified) {
            widget->setLevel(userLevel);
            int newBoardSize = widget->getLevelBoardSize(userLevel);
            setFixedSize(QSize((newBoardSize + 3) * 100, (newBoardSize + 3) * 100));
            this->move((screenWidth - this->width()) / 2, (screenHeight - this->height()) / 2);
            widget->restartGame(0);
        } else {
            showWarningMessage("Wrong name or password. Please retry!");
        }
    }

}

QVector<std::pair<User, int>> MainWindow::getUsersData() {
    QString filename = dbPath;
    QFile f(filename);
    f.open(QIODevice::ReadOnly);
    QTextStream in(&f);

    QVector<std::pair<User, int>> users;

    while (!in.atEnd()) {
        QString name;
        QString hashedPassword;
        int currentLevel;
        in >> name >> hashedPassword >> currentLevel;
        users.append({User(name, hashedPassword, true), currentLevel});
    }

    f.close();

    std::reverse(users.begin(), users.end());
    return users;
}

void MainWindow::onStatsTriggered() {
    if (!verified) {
        showWarningMessage("You must login or register to do this action!");
        return;
    }
    auto usersData = getUsersData();
    QVector<int> data;
    for (auto u : usersData) {
        if (u.first.getUserData().first == user.getUserData().first) {
            data.append(u.second);
        }
    }
    std::reverse(data.begin(), data.end());

    statsWindow->createUI(user, usersData);
    statsWindow->show();
}
