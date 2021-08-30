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
    auto langs = CodexLang::langEnumsAndNames();
    auto sNative = ui->nativeCB->currentText().toStdString();
    auto sTarget = ui->targetQB->currentText().toStdString();
    auto nLocale = QLocale((QLocale::Language)langs[sNative]);
    auto tLocale = QLocale((QLocale::Language)langs[sTarget]);
    auto name = ui->deckNameEdit->text().toStdString();
    printf("New deck is called: %s\n", name.c_str());
    emit newDeck(nLocale, tLocale, name);
}
void DeckCreatorWidget::on_cancelButton_clicked()
{
    emit abortNewDeck();
}

