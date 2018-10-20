#include "word.h"

Word::Word(QString english, QString part, QString meaning):
    english(english), part(part), meaning(meaning),
    next(nullptr)
{

}

void pushNewWord(Word*& words_head, QString english, QString part, QString meaning) {
    Word* new_word = new Word(english,part,meaning);
    Word* ptr=words_head;
    words_head=new_word;
    new_word->next=ptr;
}

void printAllWords(Word* words_head) {
    for(Word* ptr=words_head; ptr!=nullptr&&ptr->next!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}
