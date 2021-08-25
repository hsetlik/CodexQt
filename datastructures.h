#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H
#include "stlutility.h"

class PhrasePair;


struct Word
{
    Word (std::string _text, PhrasePair* _parent) :
        text(_text),
        parent(_parent)
    {
    }
    Word (const Word& orig) :
        text(orig.text),
        parent(orig.parent)
    {

    }
    std::string text;
    PhrasePair* parent;
    bool operator > (Word& other) { return (text > other.text); }
    bool operator < (Word& other) { return (text < other.text); }
    bool operator <= (Word& other) { return (text <= other.text); }
    bool operator >= (Word& other) { return (text >= other.text); }
    bool operator == (Word& other) { return (text == other.text); }
};

struct Phrase
{
    Phrase(std::string full, PhrasePair* _parent) :
        fullPhrase(full),
        parent(_parent)
    {
        auto exp = std::regex("\\w+");
        auto wordStrings = stdu::matchesAsVector(fullPhrase, exp);
        for(auto str : wordStrings)
        {
            words.push_back(Word(str, parent));
        }
    }
    Word withText(std::string txt)
    {
        for(auto w : words)
        {
            if(w.text == txt)
                return w;
        }
        return words.back();
    }
    const std::string fullPhrase;
    PhrasePair* const parent;
    std::vector<Word> words;
};
//inherit from this for a callback function for every time a set of connections changes in a PhrasePair
struct PhrasePairListener
{
    virtual ~PhrasePairListener() {}
    virtual void phraseChanged(PhrasePair* changedPair)=0;
};

class PhrasePair
{
public:
    PhrasePair(std::string nativeFull, std::string targetFull);
    PhrasePair(const PhrasePair& other);
    Phrase nativePhrase;
    Phrase targetPhrase;
    void addListener(PhrasePairListener* list) {listeners.push_back(list); }
    void addNtaPair(std::string nativeWord, std::string targetString);
    void removeNtaPairByNative(std::string nativeWord);
    void removeNtaPairByTarget(std::string targetWord);
    void addCloze(std::string word) {clozeWords.push_back(word); }
    void removeCloze(std::string word);
    std::unordered_map<std::string, std::string> ntaPairs;
    std::vector<std::string> clozeWords;
private:
    void pingListeners();
    std::vector<PhrasePairListener*> listeners;
};







#endif // DATASTRUCTURES_H


