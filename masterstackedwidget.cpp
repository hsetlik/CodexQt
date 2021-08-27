#include "masterstackedwidget.h"

MasterStackedWidget::MasterStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    currentDeck(nullptr),
    studyView(nullptr)
{
    currentDeck.reset(new Deck());
    //! note this is only here for debuging
    currentDeck->pushBackDueDates(3);
    auto cardsDue = currentDeck->numDueToday();
    printf("%d cards due today\n", cardsDue);
    auto deckScreen = new DeckWidget(&*currentDeck, this);
    addWidget(deckScreen);
    auto phraseInputForm = new PhraseInputForm(this);
    auto inputFormIdx = addWidget(phraseInputForm);
    setCurrentIndex(inputFormIdx);
    setCurrentIndex(0);
    auto editorWidget = new InputWidget(this);
    addWidget(editorWidget);
    QObject::connect(deckScreen, &DeckWidget::goToInputScreen, this, &MasterStackedWidget::switchToPhraseInput);
    QObject::connect(phraseInputForm, &PhraseInputForm::getPairList, editorWidget, &InputWidget::prepareEditorsFor);
    QObject::connect(phraseInputForm, &PhraseInputForm::getPairList, this, &MasterStackedWidget::switchToCardEditors);
    QObject::connect(editorWidget, &InputWidget::returnNewPairCards, this, &MasterStackedWidget::finishAddingCards);
    QObject::connect(deckScreen, &DeckWidget::studyScreenWith, this, &MasterStackedWidget::switchToStudyView);
}

void MasterStackedWidget::switchToCardEditors()
{
    setCurrentIndex(2);
}
void MasterStackedWidget::switchToStudyView()
{
    studyView.reset(new CardWidget(&*currentDeck, this));
    if(!stackContains(&*studyView))
        addWidget(&*studyView);
    setCurrentWidget(&*studyView);
}
void MasterStackedWidget::finishAddingCards(QJsonArray pairs)
{
    if(currentDeck == nullptr)
        currentDeck.reset(new Deck());
    currentDeck->addNewPairs(pairs);
    setCurrentIndex(0); //go back to the deck view
}

void MasterStackedWidget::switchToPhraseInput()
{
    setCurrentIndex(1);
}
