#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QObject>
#include <QDebug>

class Word : public QObject
{
    Q_OBJECT
public:
    Word(QString english, QString part, QString meaning);
    void printAllWords() const;
    Word* at(unsigned index);
    Word* getNext() const;
    static unsigned getCount();
    friend void pushNewWord(Word*& words_head, QString english, QString part, QString meaning);
    ~Word();
public slots:
    void giveWordData();
signals:
    void giveEnglish(QString);
    void givePart(QString);
    void giveMeaning(QString);
private:
    static unsigned count;
    QString english, part, meaning;
    Word* next;
};

void pushNewWord(Word*& words_head, QString english, QString part, QString meaning);

#endif // WORD_H
