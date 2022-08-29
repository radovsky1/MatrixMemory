#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QInputDialog>

#include "matrixwidget.h"
#include "user.h"
#include "statswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    int screenHeight;
    int screenWidth;
    ~MainWindow();

protected slots:
    void showGameOverMessage();
    void showGameWinMessage();

    void onLoadTriggered();
    void onSaveTriggered();
    void onRegisterTriggered();
    void onLoginTriggered();
    void onStatsTriggered();

protected:
    void showStartMenu();
    void showWarningMessage(QString msg);
    void showOkMessage(QString msg);

private:
    User user;
    bool verified = false;
    MatrixWidget *widget;
    Ui::MainWindow *ui;
    StatsWindow *statsWindow;

    const QString dbPath = "D://famcs/qt_labs/db.txt";

    QVector<std::pair<User, int>> getUsersData();
};
#endif // MAINWINDOW_H
