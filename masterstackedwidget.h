#ifndef MASTERSTACKEDWIDGET_H
#define MASTERSTACKEDWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "inputwidget.h"
#include "phraseinputform.h"
#include "deckwidget.h"
/*
 * Pages and indeces:
 * 0: phrase input
 * 1: card setup
 */

class MasterStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit MasterStackedWidget(QWidget *parent = nullptr);
private slots:
    void switchToCardEditors();
    void switchToPhraseInput();
public slots:
    void finishAddingCards(std::vector<PhrasePairCards>& newPairs);
private:
    std::unique_ptr<Deck> currentDeck;


signals:

};

#endif // MASTERSTACKEDWIDGET_H
