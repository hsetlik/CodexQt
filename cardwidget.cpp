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
CardWidget::CardWidget(Deck* deck, std::deque<Card*>& cards, QWidget *parent) :
    QWidget(parent),
    linkedDeck(deck),
    ui(new Ui::CardWidget),
    currentContent(nullptr),
    cardsDue(cards)
{
    ui->setupUi(this);
    //ui->contentVBox->addWidget(&*currentContent);
}

void CardWidget::nextCard()
{
    auto nextCard = cardsDue.front();
    cardsDue.pop_front();
    if(currentContent != nullptr)
    {
        currentContent.reset(CardContentGenerator::getContentFor(nextCard, this));
        ui->contentVBox->addWidget(&*currentContent);
    }
    else
        currentContent.reset(CardContentGenerator::getContentFor(nextCard, this));
    currentContent->setVisible(true);
    ui->nextButton->setVisible(false);
}
CardWidget::~CardWidget()
{
    delete ui;
}
void CardWidget::on_button1_clicked()
{
    if(currentContent !=nullptr)
    {
        auto toRepeat = currentContent->linkedCard;
        cardsDue.push_back(toRepeat);
        currentContent->flip("0");
        ui->nextButton->setVisible(true);
    }
}
void CardWidget::on_button2_clicked()
{
    currentContent->flip("1");
    ui->nextButton->setVisible(true);
}
void CardWidget::on_button3_clicked()
{
    currentContent->flip("2");
    ui->nextButton->setVisible(true);
}
void CardWidget::on_button4_clicked()
{
    currentContent->flip("3");
    ui->nextButton->setVisible(true);
}
void CardWidget::on_nextButton_clicked()
{
    nextCard();
}

