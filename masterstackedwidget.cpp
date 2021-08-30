#include "masterstackedwidget.h"

MasterStackedWidget::MasterStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    currentDeck(nullptr)
{
    deckMenuScreen = new DeckListWidget(this);
    addWidget(deckMenuScreen);
    setCurrentWidget(deckMenuScreen);
    connect(deckMenuScreen, &DeckListWidget::openDeck, this, &MasterStackedWidget::openDeckWithName);
    connect(deckMenuScreen, &DeckListWidget::launchNewDeckDialog, this, &MasterStackedWidget::switchToDeckCreatorView);
}

void MasterStackedWidget::switchToCardEditors()
{
    setCurrentIndex(2);
}
void MasterStackedWidget::switchToStudyView()
{
    studyScreen = new CardWidget(&*currentDeck, this);
    connect(studyScreen, &CardWidget::dueCardsFinished, this, &MasterStackedWidget::switchToDeckView);
    addWidget(studyScreen);
    setCurrentWidget(studyScreen);
}
void MasterStackedWidget::finishAddingCards(QJsonArray pairs)
{
    if(currentDeck == nullptr)
        currentDeck.reset(new Deck());
    currentDeck->addNewPairs(pairs);
    currentDeck->saveToFile();
    setCurrentIndex(0); //go back to the deck view
}

void MasterStackedWidget::switchToPhraseInput()
{
    setCurrentIndex(1);
}

void MasterStackedWidget::openDeckWithName(QString name)
{
    currentDeck.reset(new Deck(name.toStdString()));
    //! note this is only here for debuging
    currentDeck->pushBackDueDates(3);
    auto cardsDue = currentDeck->numDueToday();
    printf("%d cards due today\n", cardsDue);
    deckScreen = new DeckWidget(&*currentDeck, this);
    addWidget(deckScreen);
    setCurrentWidget(deckScreen);
    phraseScreen = new PhraseInputForm(this);
    editorScreen = new InputWidget(this);
    addWidget(editorScreen);
    connect(deckScreen, &DeckWidget::goToInputScreen, this, &MasterStackedWidget::switchToPhraseInput);
    connect(phraseScreen, &PhraseInputForm::getPairList, editorScreen, &InputWidget::prepareEditorsFor);
    connect(phraseScreen, &PhraseInputForm::getPairList, this, &MasterStackedWidget::switchToCardEditors);
    connect(editorScreen, &InputWidget::returnNewPairCards, this, &MasterStackedWidget::finishAddingCards);
    connect(deckScreen, &DeckWidget::studyScreenWith, this, &MasterStackedWidget::switchToStudyView);

}
