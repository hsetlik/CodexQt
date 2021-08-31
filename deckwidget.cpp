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
    QString name = linkedDeck->getName();
    QString sNumDue = QString::number(linkedDeck->numDueToday());
    ui->cardsDueValue->setText(sNumDue);
    ui->nameValue->setText(name);
}
void DeckWidget::on_studyButton_clicked()
{
    emit studyScreenWith();
}

