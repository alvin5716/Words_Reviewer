#ifndef WORD_H
#define WORD_H

#include "customstring.h"
#include <QObject>
#include <QDebug>

class Word : public QObject
{
    Q_OBJECT
public:
    Word(CustomString english, CustomString part, CustomString meaning);
    void printAllWords() const;
    Word* at(unsigned index);
    Word* getNext() const;
    Word* getLast() const;
    CustomString getWordData() const;
    static unsigned getCount();
    friend void pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning);
    ~Word();
public slots:
    void getWordDataToUI();
signals:
    void giveWordData(QString);
private:
    static unsigned count;
    CustomString english, part, meaning;
    Word* next, *last;
};

void pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning);

#endif // WORD_H
