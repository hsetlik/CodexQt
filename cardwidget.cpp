#include "cardwidget.h"
#include "ui_cardwidget.h"

CardContent::CardContent(Card* c, QWidget* parent) :
    QWidget(parent),
    linkedCard(c)
{

}
//===========================================================================
NtaContent::NtaContent(NtaCard* card, QWidget* parent) :
    CardContent(card, parent),
    targetLabel(nullptr),
    nativeLabel(nullptr)
{
    targetStr = card->getFrontData().c_str();
    nativeStr = card->getFrontData().c_str();
    targetLabel = new QLabel(targetStr);
    nativeLabel = new QLabel(nativeStr);

    QVBoxLayout layout;
    layout.addWidget(targetLabel);
    layout.addWidget(nativeLabel);
    nativeLabel->setVisible(false);
    setLayout(&layout);
}
void NtaContent::flip(std::string answer)
{
    auto buttonChoice = std::stoi(answer);
    setDueDate(buttonChoice);
    nativeLabel->setVisible(true);
}
//===========================================================================
ClozeContent::ClozeContent(ClozeCard* card, QWidget* parent) :
    CardContent(card, parent),
    clozeBox(nullptr)
{
    auto allWords = stdu::matchesAsVector(card->getFullTarget(), std::regex("\\w+"));
    auto clozeWord = card->getBackData();
    int x= 5;
    int y = 5;
    for(auto word : allWords)
    {
        auto label = new QLabel(word.c_str());
        label->move(x, y);
        label->show();
        label->setAttribute(Qt::WA_DeleteOnClose);
        x += label->width() + 2;
        if(word == clozeWord)
        {
            label->setVisible(false);
            int length = label->text().length();
            clozeBox = new QLineEdit(this);
            clozeBox->move(x, y);
            clozeBox->setMaxLength(length);
            clozeBox->setMaximumWidth(label->width());
            clozeBox->show();
        }
        labels.push_back(label);
    }
}
void ClozeContent::flip(std::string answer)
{
    clozeBox->setVisible(false);
    auto card = dynamic_cast<ClozeCard*>(linkedCard);
    QString qAnswer = card->getBackData().c_str();
    for(auto l : labels)
    {
        l->setVisible(true);
    }
    auto buttonChoice = std::stoi(answer);
    setDueDate(buttonChoice);
}
//===========================================================================
FullContent::FullContent(FullCard* card, QWidget* parent) :
    CardContent(card, parent),
    fullTarget(nullptr),
    fullNative(nullptr),
    answerBox(nullptr)
{
    int y = 5;
    int x = 5;
    fullTarget = new QLabel(card->getFrontData().c_str());
    fullTarget->move(x, y);
    fullTarget->show();
    y += fullTarget->height() * 2;
    fullNative = new QLabel(card->getBackData().c_str());
    fullNative->move(x, y);
    fullNative->show();
    fullNative->setVisible(false);
    answerBox = new QLineEdit(this);
    answerBox->move(x, y);
    answerBox->setMaximumWidth(fullNative->width());
    answerBox->show();

}
void FullContent::flip(std::string answer)
{
    auto buttonChoice = std::stoi(answer);
    setDueDate(buttonChoice);
    answerBox->setVisible(false);
    fullNative->setVisible(true);
}
//===========================================================================
CardWidget::CardWidget(Deck* deck, QWidget *parent) :
    QWidget(parent),
    linkedDeck(deck),
    ui(new Ui::CardWidget)
{
    ui->setupUi(this);
    cardsDue = linkedDeck->dueToday();

}

CardWidget::~CardWidget()
{
    delete ui;
}
