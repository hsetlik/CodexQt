#include "datastructures.h"
PhrasePair::PhrasePair(std::string native, std::string target) :
    nativePhrase(native, this),
    targetPhrase(target, this)
{

}

void PhrasePair::addNtaPair(std::string nativeString, std::string targetString)
{
    ntaPairs[nativeString] = targetString;
    pingListeners();
}

void PhrasePair::removeNtaPairByNative(std::string nativeWord)
{
   for(auto it = ntaPairs.begin(); it != ntaPairs.end(); it++)
   {
       if(it->first == nativeWord)
       {
           ntaPairs.erase(it);
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
            clozeWords.erase(it);
            pingListeners();
            return;
        }
    }
}
//make sure this always gets called at the end of functions which edit card/pairing information
void PhrasePair::pingListeners()
{
    for(auto list : listeners)
        list->phraseChanged(this);
}
