#include "masterstackedwidget.h"

MasterStackedWidget::MasterStackedWidget(QWidget *parent) :
    QStackedWidget(parent),
    currentDeck(nullptr)
{
    currentDeck.reset(new Deck());
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
}

void MasterStackedWidget::switchToCardEditors()
{
    setCurrentIndex(2);
}

void MasterStackedWidget::finishAddingCards(std::vector<PhrasePairCards>& pairs)
{
    printf("master widget received cards\n");
    //do the logic to actually add cards to the deck somewhere in here
    setCurrentIndex(0); //go back to the deck view
    if(currentDeck == nullptr)
        currentDeck.reset(new Deck());
    currentDeck->addNewPairs(pairs);
}

void MasterStackedWidget::switchToPhraseInput()
{
    setCurrentIndex(1);
}
