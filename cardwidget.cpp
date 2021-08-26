#include "cardwidget.h"
#include "ui_cardwidget.h"

CardWidget::CardWidget(Deck* deck, QWidget *parent) :
    QWidget(parent),
    linkedDeck(deck),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    cardsDue = linkedDeck->dueToday();

}

CardWidget::~CardWidget()
{
    delete ui;
}
