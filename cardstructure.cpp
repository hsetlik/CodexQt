#include "cardstructure.h"

NtaCard::NtaCard(std::string native, std::string target, PhrasePair* parent) :
    Card(parent, CardType::NTA),
    nativeWord(native),
    targetWord(target)
{
}

ClozeCard::ClozeCard(std::string toRemove, PhrasePair* parent) :
    Card(parent, CardType::Cloze),
    clozeSentence(" "),
    answer(toRemove)
{
    clozeSentence = parentPair->nativePhrase.fullPhrase;
    auto clozeIdx = clozeSentence.find(answer);
    if(clozeIdx != std::string::npos)
    {
        for(int i = (int)clozeIdx; i < (int)(clozeIdx + answer.length()); ++i)
        {
            clozeSentence[i] = ' ';
        }
    }
}

FullCard::FullCard(PhrasePair* parent) :
    Card(parent, CardType::Full)
{
    question = parent->nativePhrase.fullPhrase;
    answer = parent->targetPhrase.fullPhrase;
}

QJsonObject NtaCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "NTA"},
        {"NativePhrase", parentPair->nativePhrase.fullPhrase.c_str()},
        {"TargetPhrase", parentPair->targetPhrase.fullPhrase.c_str()},
        {"NativeWord", nativeWord.c_str()},
        {"TargetWord", targetWord.c_str()}
    };
    return obj;
}

QJsonObject ClozeCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "Cloze"},
        {"NativePhrase", parentPair->nativePhrase.fullPhrase.c_str()},
        {"TargetPhrase", parentPair->targetPhrase.fullPhrase.c_str()},
        {"ClozeWord", answer.c_str()}
    };
    return obj;
}

QJsonObject FullCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "Full"},
        {"NativePhrase", parentPair->nativePhrase.fullPhrase.c_str()},
        {"TargetPhrase", parentPair->targetPhrase.fullPhrase.c_str()}
    };
    return obj;
}
PhrasePairCards::PhrasePairCards(PhrasePair* pair) :
    linkedPair(pair)
{
    for(auto& nta : linkedPair->ntaPairs)
    {
        ntaCards.push_back(NtaCard(nta.first, nta.second, linkedPair));
    }
    for(auto & cloze : linkedPair->clozeWords)
    {
        clozeCards.push_back(ClozeCard(cloze, linkedPair));
    }
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
}
