#ifndef MASTERSTACKEDWIDGET_H
#define MASTERSTACKEDWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "inputwidget.h"
#include "phraseinputform.h"
#include "deckwidget.h"
#include "cardwidget.h"
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
    bool stackContains(QWidget* w)
    {
        for(int i = 0; i < count(); ++i)
        {
            if(widget(i) == w)
                return true;
        }
        return false;
    }
private slots:
    void switchToCardEditors();
    void switchToPhraseInput();
    void switchToStudyView(std::vector<Card*> cards);
public slots:
    void finishAddingCards(std::vector<PhrasePairCards> newPairs);
private:
    std::unique_ptr<Deck> currentDeck;
    std::unique_ptr<CardWidget> studyView;


signals:

};

#endif // MASTERSTACKEDWIDGET_H
