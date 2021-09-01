#ifndef CARDSTRUCTURE_H
#define CARDSTRUCTURE_H

#include "datastructures.h"
#include <QtWidgets>

/*
 * TODO LIST updated 8/29:
 * -Add comparison for text entry cards (cloze and full)
 * -Improve layout of card views
 * -Add answer history/difficulty system to the card data structure
 */
enum class CardType
{
    NTA,
    Cloze,
    Audio,
    Full
};
struct Card
{
    Card(PhrasePair* pair, CardType type) :
        cardType(type),
        timesAnswered(0),
        lastAnswer(0)
    {
        dateNextDue = QDateTime::currentDateTime();
        parentPairId = pair->getJsonIdString();
    }
    Card(QJsonObject& obj) : cardType(getCardType(obj))
    {
        auto dateString = obj["DateNextDue"].toString();
        dateNextDue = QDateTime::fromString(dateString);
        parentPairId = obj["ParentPairId"].toString();
        timesAnswered = obj["TimesAnswered"].toInt();
        lastAnswer = obj["LastAnswer"].toInt();
    }
    static CardType getCardType(QJsonObject& obj)
    {
        auto typeString = obj["CardType"].toString();
        if(typeString == "NTA")
            return CardType::NTA;
        else if(typeString == "Cloze")
            return CardType::Cloze;
        else
            return CardType::Full;
    }
    //this is the core of the SRS algorithm. figure out how to do it correctly at some point
    static int daysToDelay(int numAnswers, int lastAnswer)
    {

        if(lastAnswer == 1)
            return 1;
        if(numAnswers == 0)
            numAnswers += 1;
        return 12 * (lastAnswer / (numAnswers * 0.5f));
    }
    virtual ~Card() {}
    const CardType cardType;
    //virtual functions to be generated by the card subclasses
    //not pure virtual bc I need to be able to have template containers of type 'Card' but these are junk return values
    virtual QString getFrontData() {return "null"; }
    virtual QString getBackData() {return "null"; }
    //gives JSON object
    virtual QJsonObject getJson()
    {
        QJsonObject obj
        {
            {"Null", 0}
        };
        return obj;
    }
    virtual void updateWithAnswer(int answer)
    {
        lastAnswer = answer;
        ++timesAnswered;
        setDueIn(daysToDelay(timesAnswered, lastAnswer));
    }
    void setDueIn(int numDays)
    {
        dateNextDue = dateNextDue.addDays(numDays);
    }
    bool isDue(QDateTime current)
    {
        return current >= dateNextDue;
    }
    int getTimesAnswered() {return timesAnswered; }
    int getLastAnswer() {return lastAnswer; }
protected:
    QDateTime dateNextDue;
    QString parentPairId;
    int timesAnswered;
    int lastAnswer;
};
//=================================================================================
struct NtaCard : public Card
{
public:
    NtaCard(QString native, QString target, PhrasePair* parent);
    NtaCard(QJsonObject& obj);
    QString getFrontData() override {return nativeWord; }
    QString getBackData() override {return targetWord; }
    QJsonObject getJson() override;
private:
    QString nativeWord;
    QString targetWord;
};
//=================================================================================
struct ClozeCard : public Card
{
public:
    ClozeCard(QString toRemove, PhrasePair* parent);
    ClozeCard(QJsonObject& obj);
    QString getFrontData() override {return clozeSentence; }
    QString getBackData() override {return answer; }
    QJsonObject getJson() override;
    QString getFullTarget(){return fullTarget; }
    QString getFullNative(){return fullNative; }
private:
    QString clozeSentence;
    QString answer;
    QString fullTarget;
    QString fullNative;
};
//=================================================================================
struct FullCard : public Card
{
public:
    FullCard(PhrasePair* parent);
    FullCard(QJsonObject& obj);
    QString getFrontData() override {return fullNative; }
    QString getBackData() override {return fullTarget; }
    QJsonObject getJson() override;
private:
    QString fullNative;
    QString fullTarget;
};
//=================================================================================
struct PhrasePairCards
{
    PhrasePairCards(PhrasePair* parent);
    PhrasePairCards(QJsonObject& obj);
    //store all the cards as unique_ptrs and only instantiate card subclasses inside the PhrasePairCards class
    //Outside of this class, all access to and manipulation of card objects should be done via pointer to object of this class
    std::vector<std::unique_ptr<NtaCard>> ntaCards;
    std::vector<std::unique_ptr<ClozeCard>> clozeCards;
    std::unique_ptr<FullCard> full;
    QJsonObject getPairJson();
    void addAllToVector(std::vector<Card*>& allCards);
private:
    //adds all the cards to an external array for storing a deck of multiple phrase pairs
    void appendToDeckArray(QJsonArray& array);
    QJsonArray getNtaJsons();
    QJsonArray getClozeJsons();
    QString pairId;
    QString fullNative;
    QString fullTarget;
};
//================================================================================
//Full deck data structure, includes functionality for storing as file JSON
class Deck
{
public:
    Deck(QString name = "default_deck");
    ~Deck();
    void addPhrasePairFrom(QJsonObject obj);
    std::vector<PhrasePairCards> phrasePairs;
    std::vector<Card*> allCards;
    std::vector<Card*> dueToday();
    int numDueToday();
    void addNewPairs(QJsonArray newPairs);
    void saveToFile();
    QString getName() {return deckName; }
    void pushBackDueDates(int numDays);
    QLocale getNativeLocale() {return nativeLocale; }
    QLocale getTargetLocale() {return targetLocale; }
private:
    QString deckName;
    QJsonObject getDeckAsObject();
    QJsonArray getPairJsons();
    QLocale nativeLocale;
    QLocale targetLocale;

};
#endif // CARDSTRUCTURE_H
