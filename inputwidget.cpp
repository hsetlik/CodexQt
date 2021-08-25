#include "inputwidget.h"
#include "ui_inputform.h"



PhraseWidget::PhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    QWidget(parent),
    linkedPair(p)
{
    auto sPhrase = fullPhrase.toStdString();
    auto exp = std::regex("\\w+");
    auto words = stdu::matchesAsVector(sPhrase, exp);
    int x = 5;
    int y = 5;
    for(auto& word : words)
    {
        auto label = new WordLabel(word, linkedPair, this);
        addedLabels.push_back(label);
        label->move(x, y);
        label->show();
        label->setAttribute(Qt::WA_DeleteOnClose);

        x += label->width() + 2;
    }
    setAcceptDrops(true);
}

WordLabel* PhraseWidget::labelWidget(std::string inputWord)
{
    for(auto w : addedLabels)
    {
        auto word = dynamic_cast<WordLabel*>(w);
        if(word->text().toStdString() == inputWord)
            return word;

    }
    return nullptr;
}
bool PhraseWidget::containsWord(std::string input)
{
    for(auto w : addedLabels)
    {
        auto word = dynamic_cast<WordLabel*>(w);
        if(word->text().toStdString() == input)
            return true;

    }
    return false;
}
//TODO: fill out everything down here
NPhraseWidget::NPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    PhraseWidget(fullPhrase, p, parent)
{

}

void NPhraseWidget::mousePressEvent(QMouseEvent *event)
{

}

void NPhraseWidget::dropEvent(QDropEvent *event)
{

}
QPointF NPhraseWidget::getConnectionPointFor(std::string word)
{

}

TPhraseWidget::TPhraseWidget(const QString& fullPhrase, PhrasePair* p, QWidget *parent) :
    PhraseWidget(fullPhrase, p, parent)
{

}
void TPhraseWidget::mousePressEvent(QMouseEvent *event)
{

}

void TPhraseWidget::dropEvent(QDropEvent *event)
{

}
QPointF TPhraseWidget::getConnectionPointFor(std::string word)
{

}


PhrasePairWidget::PhrasePairWidget(PhrasePair* pair, QWidget* parent) :
    QWidget(parent),
    linkedPair(pair)
{

}

InputWidget::InputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputForm)
{
    ui->setupUi(this);


}

void InputWidget::prepareEditorsFor(std::vector<PhrasePair>& pairs)
{
    allPairs.clear();
    for(auto& pair : pairs)
        allPairs.push_back(pair);
    printf("Preparing editors. . .\n");
}
