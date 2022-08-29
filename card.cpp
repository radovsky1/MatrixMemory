#include "card.h"
#include "ui_card.h"

#include <QPixmap>
#include <QDebug>

Card::Card(QWidget *parent) :
    QWidget(parent)
{
}

Card::~Card()
{
    delete m_button;
}


Card::Card(int i, int j, QIcon icon) : x(i), y(j) {
    defaultIcon = icon;
    m_button = new QPushButton(this);
    m_button->setFixedSize(QSize(100, 100));
    hide();
    connect(m_button, SIGNAL(clicked()), this, SLOT(onClicked()));
}

void Card::onClicked() {
    emit selected(x, y);
}

void Card::show(QIcon newIcon) {
    m_button->setIcon(newIcon);
    m_button->setStyleSheet("background-color: #e1ecfc; border: none; border-radius: 7px;");
    m_button->setIconSize(m_button->size());
    showedIcon = newIcon;
}

void Card::hide() {
    m_button->setIcon(defaultIcon);
    m_button->setStyleSheet("background-color: #4472c7; border: none; border-radius: 7px;");
    m_button->setIconSize(m_button->size());
}
