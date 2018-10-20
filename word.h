#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QDebug>

class Word
{
public:
    Word(QString english, QString meaning, QString part);
    friend void pushNewWord(Word*& words_head, QString english, QString meaning, QString part);
    friend void printAllWords(Word* words_head);
private:
    QString english, meaning, part;
    Word* next;
};

void pushNewWord(Word*& words_head, QString english, QString meaning, QString part);
void printAllWords(Word* words_head);

#endif // WORD_H
