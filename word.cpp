#include "word.h"

unsigned Word::count=0;

Word::Word(QString english, QString part, QString meaning):
    english(english), part(part), meaning(meaning),
    next(nullptr)
{
    ++count;
}

void pushNewWord(Word*& words_head, QString english, QString part, QString meaning) {
    Word* new_word = new Word(english,part,meaning);
    Word* ptr=words_head;
    words_head=new_word;
    new_word->next=ptr;
}

void Word::printAllWords() const{
    for(Word const* ptr=this; ptr!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}

void Word::giveWordData() {
    emit giveEnglish(english);
    emit givePart(part);
    emit giveMeaning(meaning);
}

Word* Word::at(unsigned index) {
    unsigned i=0;
    Word* ptr=this;
    for(; ptr!=nullptr && i<index; ptr=ptr->next) ++i;
    return ptr;
}
Word* Word::getNext() const{
    return this->next;
}
unsigned Word::getCount() {
    return count;
}

Word::~Word() {
    --count;
}
