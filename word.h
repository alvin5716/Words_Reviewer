#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QDebug>

class Word
{
public:
    Word(QString english, QString part, QString meaning);
    friend void pushNewWord(Word*& words_head, QString english, QString part, QString meaning);
    friend void printAllWords(Word* words_head);
private:
    QString english, part, meaning;
    Word* next;
};

void pushNewWord(Word*& words_head, QString english, QString part, QString meaning);
void printAllWords(Word* words_head);

#endif // WORD_H
