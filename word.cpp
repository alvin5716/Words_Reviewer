#include "word.h"

unsigned Word::count=0;

Word::Word(CustomString english, CustomString part, CustomString meaning):
    english(english), part(part), meaning(meaning),
    next(nullptr), last(nullptr)
{
    ++count;
}

void pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning) {
    Word* new_word = new Word(english,part,meaning);
    if(words_head==nullptr) words_tail=new_word;
    Word* ptr=words_head;
    words_head=new_word;
    new_word->next=ptr;
    if(ptr!=nullptr) ptr->last=new_word;
}

void Word::printAllWords() const{
    for(Word const* ptr=this; ptr!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}

void Word::getWordDataToUI() {
    emit giveWordData(static_cast<QString>(getWordData()));
}

CustomString Word::getWordData() const{
    return english+" "+part+" "+meaning;
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
Word* Word::getLast() const{
    return this->last;
}
unsigned Word::getCount() {
    return count;
}

Word::~Word() {
    --count;
}
