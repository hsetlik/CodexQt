#include "masterstackedwidget.h"

MasterStackedWidget::MasterStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    currentDeck(nullptr)
{
    currentDeck.reset(new Deck());
    //! note this is only here for debuging
    //currentDeck->pushBackDueDates(3);
    auto cardsDue = currentDeck->numDueToday();
    printf("%d cards due today\n", cardsDue);
    deckScreen = new DeckWidget(&*currentDeck, this);
    addWidget(deckScreen);
    phraseScreen = new PhraseInputForm(this);
    auto inputFormIdx = addWidget(phraseScreen);
    setCurrentIndex(inputFormIdx);
    setCurrentIndex(0);
    editorScreen = new InputWidget(this);
    addWidget(editorScreen);
    QObject::connect(deckScreen, &DeckWidget::goToInputScreen, this, &MasterStackedWidget::switchToPhraseInput);
    QObject::connect(phraseScreen, &PhraseInputForm::getPairList, editorScreen, &InputWidget::prepareEditorsFor);
    QObject::connect(phraseScreen, &PhraseInputForm::getPairList, this, &MasterStackedWidget::switchToCardEditors);
    QObject::connect(editorScreen, &InputWidget::returnNewPairCards, this, &MasterStackedWidget::finishAddingCards);
    QObject::connect(deckScreen, &DeckWidget::studyScreenWith, this, &MasterStackedWidget::switchToStudyView);
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
