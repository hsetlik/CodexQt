#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QWidget>
#include "cardstructure.h"

class CardContent : public QWidget
{
    Q_OBJECT
public:
    explicit CardContent(Card* c, QWidget* parent = nullptr);
    virtual ~CardContent() {}
    Card* const linkedCard;
    virtual void flip()=0;
    virtual void setDueDate(int answerButton)
    {
        const int daysPerButton[] = {1, 3, 5, 10};
        linkedCard->setDueIn(daysPerButton[answerButton]);
    }
};
//==========================================================
class NtaContent : public CardContent
{
    Q_OBJECT
public:
    explicit NtaContent(Card* card, QWidget* parent = nullptr);
    void flip() override;
private:
    QString targetStr;
    QString nativeStr;
    QLabel* targetLabel;
    QLabel* nativeLabel;
};
//==========================================================
class ClozeContent : public CardContent
{
    Q_OBJECT
public:
    explicit ClozeContent(Card* card, QWidget* parent=nullptr);
    void flip() override;

private:
    std::vector<QLabel*> labels;
    QLineEdit* clozeBox;
};
//==========================================================
class FullContent : public CardContent
{
public:
    explicit FullContent(FullCard* card, QWidget* parent=nullptr);
    void flip() override;
private:
    QLabel* fullTarget;
    QLabel* fullNative;
    QLineEdit* answerBox;
};
//==========================================================
struct CardContentGenerator
{
    static CardContent* getContentFor(Card* card, QWidget* parent)
    {
        if(card->cardType == CardType::NTA)
            return new NtaContent(dynamic_cast<NtaCard*>(card), parent);
        else if(card->cardType == CardType::Cloze)
        {
            auto pCloze = dynamic_cast<ClozeCard*>(card);
            return new ClozeContent(pCloze, parent);
        }
        else
            return new FullContent(dynamic_cast<FullCard*>(card), parent);
    }
};
//==========================================================
namespace Ui
{
class CardWidget;
}
class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(Deck* deck, std::vector<Card*> cards, QWidget *parent = nullptr);
    Deck* const linkedDeck;
    void nextCard();
    void keyPressEvent(QKeyEvent* event) override;
    ~CardWidget();
private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();

    void on_button4_clicked();
private:
    void updateContent();
    void setButtonsVisible(bool shouldBeVisible);
    Ui::CardWidget *ui;
    std::unique_ptr<CardContent> currentContent;
    std::vector<Card*> cardsDue;
};

#endif // CARDWIDGET_H
