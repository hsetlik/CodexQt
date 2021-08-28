#include "cardstructure.h"
#include <memory>
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
    //printf("Creating NTA from JSON\n");
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
    //printf("Creating Cloze from JSON\n");
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
        ntaCards.push_back(std::make_unique<NtaCard>(nta.first, nta.second, pair));
    }
    for(auto & cloze : pair->clozeWords)
    {
        clozeCards.push_back(std::make_unique<ClozeCard>(cloze, pair));
    }
    if(pair->includesFull())
        full = std::make_unique<FullCard>(pair);
}
PhrasePairCards::PhrasePairCards(QJsonObject& obj) :
    full(nullptr)
{
    pairId = obj["PhrasePairId"].toString().toStdString();
    fullNative = obj["NativePhrase"].toString().toStdString();
    fullTarget = obj["TargetPhrase"].toString().toStdString();
    auto ntaArray = obj["NtaCards"].toArray();
    for(int i = 0; i < ntaArray.size(); ++i)
    {
        //printf("Adding Nta Card\n");
        auto ntaObject = ntaArray[i].toObject();
        ntaCards.push_back(std::make_unique<NtaCard>(ntaObject));
    }
    auto clozeArray = obj["ClozeCards"].toArray();
    for(int i = 0; i < clozeArray.size(); ++i)
    {
        //printf("adding cloze card\n");
        auto clozeObject = clozeArray.at(i).toObject();
        clozeCards.push_back(std::make_unique<ClozeCard>(clozeObject));

    }
    if(obj.contains("FullCard"))
    {
        auto fullCardObject = obj["FullCard"].toObject();
        full = std::make_unique<FullCard>(fullCardObject);
    }
}
QJsonArray PhrasePairCards::getNtaJsons()
{
    QJsonArray ntaArray;
    for(auto& card : ntaCards)
    {
        auto newVal = QJsonValue(card->getJson());
        ntaArray.append(newVal);
    }
    return ntaArray;
}
QJsonArray PhrasePairCards::getClozeJsons()
{
    QJsonArray clozeArray;
    for(auto& cloze : clozeCards)
    {
        auto newVal = QJsonValue(cloze->getJson());
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
//add pointers to all the cards to the array
void PhrasePairCards::addAllToVector(std::vector<Card*>& array)
{
    for(auto& nta : ntaCards)
    {
        array.push_back(&*nta);
    }
    for(auto& cloze : clozeCards)
    {
        array.push_back(&*cloze);
    }
    if(full.get() != nullptr)
        array.push_back(&*full);
}
QJsonObject PhrasePairCards::getPairJson()
{
    QJsonObject obj
    {
        {"PhrasePairId", pairId.c_str()},
        {"NativePhrase", fullNative.c_str()},
        {"TargetPhrase", fullTarget.c_str()}
    };
    obj["NtaCards"] = getNtaJsons();
    obj["ClozeCards"] = getClozeJsons();
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
    QFileInfo info(loadFile);
    auto fullPath = info.absoluteFilePath().toStdString();
    printf("FULL FILE PATH: %s\n", fullPath.c_str());
    QByteArray deckData = loadFile.readAll();
    printf("File has size: %d bytes\n", deckData.size());
    QJsonDocument doc(QJsonDocument::fromJson(deckData));
    auto masterObject = doc.object();
    //the master deck JSON data is stored as an object w/ two properties: "DeckName" : string, and "PhrasePairs" : array of objects
    auto pairArray = masterObject["PhrasePairs"].toArray();
    printf("%d pairs found\n", pairArray.size());
    for(int i = 0; i < pairArray.size(); ++i)
    {
        auto pairObj = pairArray[i].toObject();
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
         if (allCards[i]->isDue (date))
             ++numDue;
     }
     return numDue;
}
void Deck::addPhrasePairFrom(QJsonObject obj)
{

    phrasePairs.push_back(PhrasePairCards(obj));
    phrasePairs.back().addAllToVector(allCards);
}
std::vector<Card*> Deck::dueToday()
{
    std::vector<Card*> due;
    auto date = QDateTime::currentDateTime();
    for(int i = 0; i < (int)allCards.size(); ++i)
    {
        if (allCards[i]->isDue (date))
            due.push_back (allCards[i]);
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
    //auto numPairs = obj["PhrasePairs"].toArray().size();
    //printf("Deck has %d pairs\n", numPairs);
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
void Deck::addNewPairs(QJsonArray newPairs)
{
    for(int i = 0; i < newPairs.size(); ++i)
    {
        auto pairObj = newPairs[i].toObject();
        addPhrasePairFrom(pairObj);
    }
}
void Deck::pushBackDueDates(int numDays)
{
    for(auto& card : allCards)
    {
        card->setDueIn(numDays * -1);
    }
}
