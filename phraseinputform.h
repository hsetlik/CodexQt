#ifndef PHRASEINPUTFORM_H
#define PHRASEINPUTFORM_H

#include <QWidget>
#include "datastructures.h"

namespace Ui {
class PhraseInputForm;
}

class PhraseInputForm : public QWidget
{
    Q_OBJECT

public:
    explicit PhraseInputForm(QWidget *parent = nullptr);
    ~PhraseInputForm();
    PhrasePair newPairFromFields();
public slots:
    void on_addButton_clicked();

    void on_backButton_clicked();

    void on_createSetupButton_clicked();
signals:
    void getPairList(std::vector<PhrasePair>& pairs);

private:
    Ui::PhraseInputForm *ui;
    std::vector<PhrasePair> addedPairs;
    void addDummyPairs(int pairsToAdd = 10);

};

#endif // PHRASEINPUTFORM_H
