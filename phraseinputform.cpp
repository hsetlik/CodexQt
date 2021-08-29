#include "phraseinputform.h"
#include "ui_phraseinputform.h"

PhraseInputForm::PhraseInputForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PhraseInputForm)
{
    ui->setupUi(this);
    auto parentSize = parent->size();
    setMinimumSize(parentSize);
}

PhraseInputForm::~PhraseInputForm()
{
    delete ui;
}

void PhraseInputForm::on_addButton_clicked()
{
    auto newPair = newPairFromFields();
    addedPairs.push_back(newPair);
    ui->nativeLineEdit->clear();
    ui->targetLineEdit->clear();
}

PhrasePair PhraseInputForm::newPairFromFields()
{
    auto sNative = ui->nativeLineEdit->text().toStdString();
    auto sTarget = ui->targetLineEdit->text().toStdString();
    return PhrasePair(sNative, sTarget);
}
void PhraseInputForm::on_backButton_clicked()
{
    if(addedPairs.size() < 1)
    {
        return;
    }
    auto currentPhrase = addedPairs.back();
    addedPairs.pop_back();
    QString sNative = currentPhrase.nativePhrase.fullPhrase.c_str();
    QString sTarget = currentPhrase.targetPhrase.fullPhrase.c_str();
    ui->nativeLineEdit->setText(sNative);
    ui->targetLineEdit->setText(sTarget);
}

void PhraseInputForm::addDummyPairs(int pairsToAdd)
{
    for(int i = 0; i < pairsToAdd; ++i)
    {
        std::string sNative = "Dummy Native Phrase Number " + std::to_string(i);
        std::string sTarget = "Dummy Target Phrase Number " + std::to_string(i);
        addedPairs.push_back(PhrasePair(sNative, sTarget));
    }
}

void PhraseInputForm::on_createSetupButton_clicked()
{
    //addDummyPairs();
    emit getPairList(addedPairs);
}

