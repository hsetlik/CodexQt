#include "cardstructure.h"

NtaCard::NtaCard(std::string native, std::string target, PhrasePair* parent) :
    Card(parent, CardType::NTA),
    nativeWord(native),
    targetWord(target)
{
}
NtaCard::NtaCard(QJsonObject& obj) :
    Card(obj),
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
    fullTarget = parent->targetPhrase.fullPhrase;
    clozeSentence = fullTarget;
    auto clozeIdx = clozeSentence.find(answer);
    if(clozeIdx != std::string::npos)
    {
        for(int i = (int)clozeIdx; i < (int)(clozeIdx + answer.length()); ++i)
        {
            clozeSentence[i] = ' ';
        }
    }
}
ClozeCard::ClozeCard(QJsonObject& obj) :
    Card(obj),
    clozeSentence(""),
    answer("")
{
    auto qAnswer = obj["ClozeWord"].toString();
    fullTarget = obj["FullTarget"].toString().toStdString();
    clozeSentence = fullTarget;
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
    fullNative = parent->nativePhrase.fullPhrase;
    fullTarget = parent->targetPhrase.fullPhrase;
}
FullCard::FullCard(QJsonObject& obj) :
    Card(obj)
{
    fullNative = obj["NativePhrase"].toString().toStdString();
    fullTarget = obj["TargetPhrase"].toString().toStdString();
    auto qDateString = obj["DateNextDue"].toString();
    dateNextDue = QDateTime::fromString(qDateString);
}
QJsonObject NtaCard::getJson()
{
    QJsonObject obj
    {
        {"CardType", "NTA"},
        {"ParentPairId", parentPairId},
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
        {"ParentPairId", parentPairId},
        {"FullTarget", fullTarget.c_str()},
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
        {"ParentPairId", parentPairId},
        {"NativePhrase", fullNative.c_str()},
        {"TargetPhrase", fullTarget.c_str()},
        {"DateNextDue", dateNextDue.toString()}
    };
    return obj;
}
//===============================================================================
PhrasePairCards::PhrasePairCards(PhrasePair* pair) :
    full(nullptr)
{
    pairId = pair->getJsonIdString();
    fullNative = pair->nativePhrase.fullPhrase;
    fullTarget = pair->targetPhrase.fullPhrase;
    for(auto& nta : pair->ntaPairs)
    {
        ntaCards.push_back(NtaCard(nta.first, nta.second, pair));
    }
    for(auto & cloze : pair->clozeWords)
    {
        clozeCards.push_back(ClozeCard(cloze, pair));
    }
    if(pair->includesFull())
        full = new FullCard(pair);
}
PhrasePairCards::PhrasePairCards(QJsonObject& obj) :
    full(nullptr)
{
    pairId = obj["PhrasePairId"].toString().toStdString();
    fullNative = obj["NativePhrase"].toString().toStdString();
    fullTarget = obj["TargetPhrase"].toString().toStdString();
    for(auto nta : obj["NtaCards"].toObject())
    {
        auto ntaObject = nta.toObject();
        ntaCards.push_back(NtaCard(ntaObject));
    }
    for(auto cloze : obj["ClozeCards"].toObject())
    {
        auto clozeObject = cloze.toObject();
        clozeCards.push_back(ClozeCard(clozeObject));
    }
    if(obj.contains("FullCard"))
    {
        auto fullCardObject = obj["FullCard"].toObject();
        full = new FullCard(fullCardObject);
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
    if(full != nullptr)
    {
        auto fullJson = full->getJson();
        array.append(fullJson);
    }
}
void PhrasePairCards::addAllToVector(std::vector<Card>& allCards)
{
    for(auto& card : ntaCards)
        allCards.push_back(card);
    for(auto& card : clozeCards)
        allCards.push_back(card);
    if(full != nullptr)
        allCards.push_back(*full);
}
QJsonObject PhrasePairCards::getPairJson()
{
    QJsonObject obj
    {
        {"PhrasePairId", pairId.c_str()},
        {"NativePhrase", fullNative.c_str()},
        {"TargetPhrase", fullTarget.c_str()},
        {"NtaCards", getNtaJsons()},
        {"ClozeCards", getClozeJsons()}
    };
    if(full != nullptr)
        obj["FullCard"] = full->getJson();
    return obj;
}
//===============================================================================
Deck::Deck(std::string name) :
    deckName(name)
{
    //1. determine the deck file name
    auto sDeckFile = deckName + ".json";
    QString deckFileName = sDeckFile.c_str();
    //2. load file into ByteArray and by extension JSON
    QFile loadFile(deckFileName);
    if(!loadFile.open(QIODevice::ReadWrite))
        printf("File not loaded\n");
    if(loadFile.exists())
    {
        printf("File: %s exists\n", loadFile.fileName().toStdString().c_str());
    }
    QByteArray deckData = loadFile.readAll();
    printf("File has size: %d bytes\n", deckData.size());
    QJsonDocument doc(QJsonDocument::fromJson(deckData));
    auto masterObject = doc.object();
    //the master deck JSON data is stored as an object w/ two properties: "DeckName" : string, and "PhrasePairs" : array of objects
    auto pairArray = masterObject["PairArray"].toArray();
    for(auto pair : pairArray)
    {
        auto pairObj = pair.toObject();
        addPhrasePairFrom(pairObj);
    }
    loadFile.close();
}
Deck::~Deck()
{
    saveToFile();
}
void Deck::saveToFile()
{
    //1. determine the deck file name
    auto sDeckFile = deckName + ".json";
    QString deckFileName = sDeckFile.c_str();
    //2. load file into ByteArray and by extension JSON
    QFile loadFile(deckFileName);
    if(!loadFile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        printf("File not loaded\n");
    auto deckJsonDoc = QJsonDocument(getDeckAsObject());
    auto bytesWritten = loadFile.write(deckJsonDoc.toJson());
    printf("%lld bytes written to file\n", bytesWritten);
    loadFile.close();
}
int Deck::numDueToday()
{
     int numDue = 0;
     auto date = QDateTime::currentDateTime();
     for(int i = 0; i < (int)allCards.size(); ++i)
     {
         if (allCards[i].isDue (date))
             ++numDue;
     }
     return numDue;
}
void Deck::addPhrasePairFrom(QJsonObject &obj)
{
    auto newPair = PhrasePairCards(obj);
    phrasePairs.push_back(newPair);
    newPair.addAllToVector(allCards);
}
std::deque<Card*> Deck::dueToday()
{
    std::deque<Card*> due;
    auto date = QDateTime::currentDateTime();
    for(int i = 0; i < (int)allCards.size(); ++i)
    {
        if (allCards[i].isDue (date))
            due.push_back (&allCards[i]);
    }
    return due;
}
QJsonObject Deck::getDeckAsObject()
{
    QJsonObject obj
    {
        {"DeckName", deckName.c_str()},
        {"PhrasePairs", getPairJsons()}
    };
    return obj;
}
QJsonArray Deck::getPairJsons()
{
    QJsonArray arr;
    for(auto& pair : phrasePairs)
    {
        auto pairObj = pair.getPairJson();
        arr.append(pairObj);
    }
    return arr;
}
void Deck::addNewPairs(std::vector<PhrasePairCards>& newPairs)
{
    for(auto& pair : newPairs)
    {
        phrasePairs.push_back(pair);
        pair.addAllToVector(allCards);
    }
    printf("%d new pairs added to deck\n", (int)newPairs.size());
}
