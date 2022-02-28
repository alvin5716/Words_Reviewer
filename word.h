#ifndef WORD_H
#define WORD_H

#include "customstring.h"
#include "wordshowingmode.h"
#include <QDebug>

class Word
{
public:
    Word(CustomString english, CustomString part, CustomString meaning);
    void printAllWords() const;
    Word* getNext() const;
    Word* getLast() const;
    void setNext(Word* next);
    void setLast(Word* last);
    CustomString getWordData(WordShowingMode::Mode onlyWord=WordShowingMode::AllShowing) const;
    CustomString getEnglish() const;
    CustomString getPart() const;
    CustomString getMeaning() const;
    ~Word();
private:
    CustomString english, part, meaning;
    Word* next, *last;
};

#endif // WORD_H
