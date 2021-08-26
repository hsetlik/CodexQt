#include "deckwidget.h"
#include "ui_deckwidget.h"

DeckWidget::DeckWidget(Deck* deck, QWidget *parent) :
    QWidget(parent),
    linkedDeck(deck),
    ui(new Ui::DeckWidget)
{
    ui->setupUi(this);
    updateLabels();
}

DeckWidget::~DeckWidget()
{
    delete ui;
}

void DeckWidget::on_addButton_clicked()
{
    emit goToInputScreen();
}

void DeckWidget::updateLabels()
{
    QString name = linkedDeck->getName().c_str();
    QString sNumDue = std::to_string(linkedDeck->numDueToday()).c_str();
    ui->cardsDueValue->setText(sNumDue);
    ui->nameValue->setText(name);
}
void DeckWidget::on_studyButton_clicked()
{
    auto cardsDue = linkedDeck->dueToday();
    emit studyScreenWith(cardsDue);
}

