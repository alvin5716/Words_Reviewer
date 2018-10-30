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
    friend Word* pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning);
    friend void deleteCurrentWord(Word*& words_head, Word*& words_tail, Word*& current_word);
    CustomString getEnglish() const;
    CustomString getPart() const;
    CustomString getMeaning() const;
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

Word* pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning);
void deleteCurrentWord(Word*& words_head, Word*& words_tail, Word*& current_word);

#endif // WORD_H
