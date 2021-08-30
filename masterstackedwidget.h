#ifndef MASTERSTACKEDWIDGET_H
#define MASTERSTACKEDWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "decklistwidget.h"
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
    void switchToStudyView();
    void switchToDeckView()
    {
        setCurrentIndex(0);
        deckScreen->updateLabels();
    }
public slots:
    void finishAddingCards(QJsonArray pairs);
    void openDeckWithName(QString name);
private:
    std::unique_ptr<Deck> currentDeck;
    DeckListWidget* deckMenuScreen;
    DeckWidget* deckScreen;
    CardWidget* studyScreen;
    PhraseInputForm* phraseScreen;
    InputWidget* editorScreen;


signals:

};

#endif // MASTERSTACKEDWIDGET_H
