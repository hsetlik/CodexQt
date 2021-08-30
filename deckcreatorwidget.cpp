#include "deckcreatorwidget.h"
#include "ui_deckcreatorwidget.h"

DeckCreatorWidget::DeckCreatorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckCreatorWidget)
{
    ui->setupUi(this);
    auto localeValues = CodexLang::langEnumsAndNames();
    for(auto& l : localeValues)
    {
        QString name = l.first.c_str();
        ui->nativeCB->addItem(name);
        ui->targetQB->addItem(name);
    }
}

DeckCreatorWidget::~DeckCreatorWidget()
{
    delete ui;
}

void DeckCreatorWidget::on_createButton_clicked()
{

}


void DeckCreatorWidget::on_cancelButton_clicked()
{

}

