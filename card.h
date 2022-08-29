#ifndef CARD_H
#define CARD_H

#include <QWidget>
#include <QPushButton>
#include <QColor>
#include <QIcon>
#include <QVector>

namespace Ui {
class Card;
}

class Card : public QWidget
{
    Q_OBJECT

public:
    explicit Card(QWidget *parent = nullptr);
    Card(int i, int j, QIcon icon);
    ~Card();

    QIcon getShowedIcon() { return showedIcon; };

    void show(QIcon newIcon);
    void hide();

protected slots:
    void onClicked();

signals:
    void selected(int i, int j);

private:
    int x, y;
    QPushButton *m_button;

    QIcon showedIcon;
    QIcon defaultIcon;
};

#endif // CARD_H
