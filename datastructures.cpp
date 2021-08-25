#include "datastructures.h"
PhrasePair::PhrasePair(std::string native, std::string target) :
    nativePhrase(native, this),
    targetPhrase(target, this),
    includeFull(false),
    totalNumCards(0)
{

}

PhrasePair::PhrasePair(const PhrasePair& other) :
    nativePhrase(other.nativePhrase.fullPhrase, this),
    targetPhrase(other.targetPhrase.fullPhrase, this)
{
}

void PhrasePair::addNtaPair(std::string nativeString, std::string targetString)
{
    printf("Adding pair: %s, %s\n", nativeString.c_str(), targetString.c_str());
    ntaPairs[nativeString] = targetString;
    totalNumCards++;
    pingListeners();
}

void PhrasePair::removeNtaPairByNative(std::string nativeWord)
{
   for(auto it = ntaPairs.begin(); it != ntaPairs.end(); it++)
   {
       if(it->first == nativeWord)
       {
           ntaPairs.erase(it);
           totalNumCards--;
           pingListeners();
           return;
       }
   }

}
void PhrasePair::removeNtaPairByTarget(std::string targetWord)
{
   for(auto it = ntaPairs.begin(); it != ntaPairs.end(); it++)
   {
       if(it->second == targetWord)
       {
           ntaPairs.erase(it);
           totalNumCards--;
           pingListeners();
           return;
       }
   }
}
void PhrasePair::removeCloze(std::string word)
{
    for(auto it = clozeWords.begin(); it != clozeWords.end(); it++)
    {
        if(*it == word)
        {
            totalNumCards--;
            clozeWords.erase(it);
            pingListeners();
            return;
        }
    }
}
bool PhrasePair::hasCloze(std::string word)
{
    for(auto& cWord : clozeWords)
    {
        if(word == cWord)
            return true;
    }
    return false;
}
void PhrasePair::toggleIncludeFull()
{
    includeFull = !includeFull;
    pingListeners();
}
//make sure this always gets called at the end of functions which edit card/pairing information
void PhrasePair::pingListeners()
{
    for(auto list : listeners)
        list->phraseChanged(this);
}

std::string PhrasePair::getJsonIdString()
{
     return nativePhrase.fullPhrase + targetPhrase.fullPhrase;
}
