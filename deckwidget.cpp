#include "deckwidget.h"
#include "ui_deckwidget.h"

DeckWidget::DeckWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckWidget)
{
    ui->setupUi(this);
}

DeckWidget::~DeckWidget()
{
    delete ui;
}

void DeckWidget::on_addButton_clicked()
{
    emit goToInputScreen();
}


void DeckWidget::on_studyButton_clicked()
{

}

