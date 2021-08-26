#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include "cardstructure.h"

namespace Ui {
class CardWidget;
}

class CardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CardWidget(Deck* deck,QWidget *parent = nullptr);
    Deck* const linkedDeck;
    void viewCard(Card* card);
    ~CardWidget();

private:
    Ui::CardWidget *ui;
    std::deque<Card> cardsDue;
};

#endif // CARDWIDGET_H
