#include "cardstructure.h"

NtaCard::NtaCard(std::string native, std::string target, PhrasePair* parent) :
    Card(parent, CardType::NTA),
    nativeWord(native),
    targetWord(target)
{
}

NtaCard::NtaCard(QJsonObject& obj, PhrasePair* parent) :
    Card(parent, CardType::NTA),
    nativeWord(""),
    targetWord("")
{
    auto qNative = obj["NativeWord"].toString();
    nativeWord = qNative.toStdString();
    auto qTarget = obj["TargetWord"].toString();
    targetWord = qTarget.toStdString();
    auto qDateString = obj["DateNextDue"].toString();
    dateNextDue = QDateTime::fromString(qDateString);
}

ClozeCard::ClozeCard(std::string toRemove, PhrasePair* parent) :
    Card(parent, CardType::Cloze),
    clozeSentence(" "),
    answer(toRemove)
{
    clozeSentence = parentPair->targetPhrase.fullPhrase;
    auto clozeIdx = clozeSentence.find(answer);
    if(clozeIdx != std::string::npos)
    {
        for(int i = (int)clozeIdx; i < (int)(clozeIdx + answer.length()); ++i)
        {
            clozeSentence[i] = ' ';
        }
    }
}

ClozeCard::ClozeCard(QJsonObject& obj, PhrasePair* parent) :
    Card(parent, CardType::Cloze),
    clozeSentence(""),
    answer("")
{
    auto qAnswer = obj["ClozeWord"].toString();
    answer = qAnswer.toStdString();
    clozeSentence = parentPair->targetPhrase.fullPhrase;
    auto clozeIdx = clozeSentence.find(answer);
    if(clozeIdx != std::string::npos)
    {
        for(int i = (int)clozeIdx; i < (int)(clozeIdx + answer.length()); ++i)
        {
            clozeSentence[i] = ' ';
        }
    }
    auto qDateString = obj["DateNextDue"].toString();
    dateNextDue = QDateTime::fromString(qDateString);
}
FullCard::FullCard(PhrasePair* parent) :
    Card(parent, CardType::Full)
{
    question = parent->nativePhrase.fullPhrase;
    answer = parent->targetPhrase.fullPhrase;
}

FullCard::FullCard(QJsonObject& obj, PhrasePair* parent) :
    Card(parent, CardType::Full)
{
    question = obj["NativePhrase"].toString().toStdString();
    answer = obj["TargetPhrase"].toString().toStdString();
    auto qDateString = obj["DateNextDue"].toString();
    dateNextDue = QDateTime::fromString(qDateString);
}

QJsonObject NtaCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "NTA"},
        {"ParentPairId", parentPair->getJsonIdString().c_str()},
        {"NativeWord", nativeWord.c_str()},
        {"TargetWord", targetWord.c_str()},
        {"DateNextDue", dateNextDue.toString()}
    };
    return obj;
}

QJsonObject ClozeCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "Cloze"},
        {"ParentPairId", parentPair->getJsonIdString().c_str()},
        {"ClozeWord", answer.c_str()},
        {"DateNextDue", dateNextDue.toString()}
    };
    return obj;
}

QJsonObject FullCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "Full"},
        {"ParentPairId", parentPair->getJsonIdString().c_str()},
        {"NativePhrase", parentPair->nativePhrase.fullPhrase.c_str()},
        {"TargetPhrase", parentPair->targetPhrase.fullPhrase.c_str()},
        {"DateNextDue", dateNextDue.toString()}
    };
    return obj;
}
PhrasePairCards::PhrasePairCards(PhrasePair* pair) :
    linkedPair(pair),
    full(nullptr)
{
    for(auto& nta : linkedPair->ntaPairs)
    {
        ntaCards.push_back(NtaCard(nta.first, nta.second, linkedPair));
    }
    for(auto & cloze : linkedPair->clozeWords)
    {
        clozeCards.push_back(ClozeCard(cloze, linkedPair));
    }
    if(linkedPair->includesFull())
        full = new FullCard(linkedPair);

}

QJsonArray PhrasePairCards::getNtaJsons()
{
    QJsonArray ntaArray;
    for(auto card : ntaCards)
    {
        auto newVal = QJsonValue(card.getJson());
        ntaArray.append(newVal);
    }
    return ntaArray;
}

QJsonArray PhrasePairCards::getClozeJsons()
{
    QJsonArray clozeArray;
    for(auto cloze : clozeCards)
    {
        auto newVal = QJsonValue(cloze.getJson());
        clozeArray.append(newVal);
    }
    return clozeArray;
}

void PhrasePairCards::appendToDeckArray(QJsonArray &array)
{
    auto ntas = getNtaJsons();
    auto clozes = getClozeJsons();
    for(int i = 0; i < ntas.size(); ++i)
    {
        array.append(ntas[i]);
    }
    for(int i = 0; i < clozes.size(); ++i)
    {
        array.append(clozes[i]);
    }
    if(full != nullptr)
    {
        auto fullJson = full->getJson();
        array.append(fullJson);
    }
}

Deck::Deck(std::string name) :
    deckName(name)
{

}

std::vector<Card> Deck::dueToday()
{
    std::vector<Card> due;
    auto date = QDateTime::currentDateTime();
    for(int i = 0; i < (int)allCards.size(); ++i)
    {
        if(allCards[i].isDue(date))
            due.push_back(allCards[i]);
    }
    return due;
}
