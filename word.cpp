#include "word.h"

Word::Word(QString english, QString meaning, QString part):
    english(english), meaning(meaning), part(part),
    next(nullptr)
{

}

void pushNewWord(Word*& words_head, QString english, QString meaning, QString part) {
    Word* new_word = new Word(english,meaning,part);
    Word* ptr=words_head;
    words_head=new_word;
    new_word->next=ptr;
}

void printAllWords(Word* words_head) {
    for(Word* ptr=words_head; ptr!=nullptr&&ptr->next!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}
