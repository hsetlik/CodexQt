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
    virtual void flip(std::string answer="null")=0;
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
    explicit NtaContent(NtaCard* card, QWidget* parent = nullptr);
    void flip(std::string answer="null") override;
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
    explicit ClozeContent(ClozeCard* card, QWidget* parent=nullptr);
    void flip(std::string answer="null") override;
private:
    std::vector<QLabel*> labels;
    QLineEdit* clozeBox;
};
//==========================================================
class FullContent : public CardContent
{
public:
    explicit FullContent(FullCard* card, QWidget* parent=nullptr);
    void flip(std::string answer="null") override;
private:
    QLabel* fullTarget;
    QLabel* fullNative;
    QLineEdit* answerBox;
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
    explicit CardWidget(Deck* deck,QWidget *parent = nullptr);
    Deck* const linkedDeck;
    void viewCard(Card* card);
    ~CardWidget();
private:
    Ui::CardWidget *ui;
    std::deque<Card> cardsDue;
};

#endif // CARDWIDGET_H
