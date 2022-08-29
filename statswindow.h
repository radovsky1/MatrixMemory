#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QGridLayout>

#include "user.h"

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StatsWindow(QWidget *parent = nullptr);
    void createUI(User user, QVector<std::pair<User, int>> usersData);
    ~StatsWindow();

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::StatsWindow *ui;
    QGridLayout *gridLayout;
};

#endif // STATSWINDOW_H
