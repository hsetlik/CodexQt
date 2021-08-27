#include "cardwidget.h"
#include "ui_cardwidget.h"

CardContent::CardContent(Card* c, QWidget* parent) :
    QWidget(parent),
    linkedCard(c)
{
    printf("Creating card content\n");
}
//===========================================================================
NtaContent::NtaContent(Card* card, QWidget* parent) :
    CardContent(card, parent),
    targetLabel(nullptr),
    nativeLabel(nullptr)
{
    printf("Creating NTA content. . .");
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
void NtaContent::flip()
{
    nativeLabel->setVisible(true);
}
//===========================================================================
ClozeContent::ClozeContent(Card* _card, QWidget* parent) :
    CardContent(_card, parent),
    clozeBox(nullptr)
{
    auto card = dynamic_cast<ClozeCard*>(_card);
    printf("Creating cloze content. . . \n");
    auto allWords = stdu::matchesAsVector(card->getFullTarget(), std::regex("\\w+"));
    auto clozeWord = card->getBackData();
    int x= 5;
    int y = 5;
    for(auto& word : allWords)
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
void ClozeContent::flip()
{
    clozeBox->setVisible(false);
    for(auto l : labels)
    {
        l->setVisible(true);
    }
}
//===========================================================================
FullContent::FullContent(Card* card, QWidget* parent) :
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
void FullContent::flip()
{
    answerBox->setVisible(false);
    fullNative->setVisible(true);
}
//===========================================================================
CardViewer::CardViewer(std::vector<Card*>& cards, QWidget *parent) :
    QWidget(parent),
    allCards(cards),
    cardIdx(0),
    currentWidget(nullptr),
    newWidget(nullptr)
{
    layout = new QVBoxLayout;
    newWidget = CardContentGenerator::getContentFor(allCards[cardIdx], this);
    layout->addWidget(newWidget);
    currentWidget = newWidget;
    setLayout(layout);
}

void CardViewer::nextCard()
{
    ++cardIdx;
    if(cardIdx >= (int)allCards.size())
        return; //TODO: emit a signal here to finish study mode
    newWidget = CardContentGenerator::getContentFor(allCards[cardIdx], this);
    layout->replaceWidget(currentWidget, newWidget);
    delete currentWidget;
    currentWidget = newWidget;
}
//===========================================================================
CardWidget::CardWidget(Deck* deck, std::vector<Card*> cards, QWidget *parent) :
    QWidget(parent),
    linkedDeck(deck),
    ui(new Ui::CardWidget),
   viewer(nullptr),
    cardsDue(cards)
{
    viewer = new CardViewer(cards, this);
    ui->setupUi(this);
    ui->contentVBox->addWidget(viewer);
    grabKeyboard();
}

void CardWidget::nextCard()
{
    viewer->nextCard();
    setButtonsVisible(false);
}
CardWidget::~CardWidget()
{
    delete ui;
}
void CardWidget::on_button1_clicked()
{
    nextCard();
}
void CardWidget::on_button2_clicked()
{
    nextCard();
}
void CardWidget::on_button3_clicked()
{
    nextCard();
}
void CardWidget::on_button4_clicked()
{
    nextCard();
}

void CardWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        printf("Enter pressed\n");
        setButtonsVisible(true);
    }
    else
        printf("Other key pressed\n");
}
void CardWidget::setButtonsVisible(bool shouldBeVisible)
{
    ui->button1->setVisible(shouldBeVisible);
    ui->button2->setVisible(shouldBeVisible);
    ui->button3->setVisible(shouldBeVisible);
    ui->button4->setVisible(shouldBeVisible);
}
