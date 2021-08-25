#include "masterstackedwidget.h"

MasterStackedWidget::MasterStackedWidget(QWidget *parent) : QStackedWidget(parent)
{
    auto phraseInputForm = new PhraseInputForm(this);
    auto inputFormIdx = addWidget(phraseInputForm);
    setCurrentIndex(inputFormIdx);
    auto editorWidget = new InputWidget(this);
    addWidget(editorWidget);
    QObject::connect(phraseInputForm, &PhraseInputForm::getPairList, editorWidget, &InputWidget::prepareEditorsFor);
    QObject::connect(phraseInputForm, &PhraseInputForm::getPairList, this, &MasterStackedWidget::switchToCardEditors);

}

void MasterStackedWidget::switchToCardEditors()
{
    setCurrentIndex(1);
}
