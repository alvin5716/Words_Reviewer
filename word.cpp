#include "word.h"

unsigned Word::count=0;

Word::Word(CustomString english, CustomString part, CustomString meaning):
    english(english), part(part), meaning(meaning),
    next(nullptr), last(nullptr)
{
    ++count;
}

Word* pushNewWord(Word*& words_head, Word*& words_tail, CustomString english, CustomString part, CustomString meaning) {
    Word* new_word = new Word(english,part,meaning);
    if(words_head==nullptr) words_tail=new_word;
    Word* ptr=words_head;
    words_head=new_word;
    new_word->next=ptr;
    if(ptr!=nullptr) ptr->last=new_word;
    return new_word;
}

void deleteCurrentWord(Word*& words_head, Word*& words_tail, Word*& current_word) {
    if(current_word==nullptr) return;
    Word* temp=current_word;
    if(words_head==words_tail) words_head=words_tail=current_word=nullptr;
    else if(current_word==words_tail) {
        words_tail=current_word->last;
        current_word->last->next=nullptr;
        current_word=words_head;
    } else if(current_word==words_head) {
        words_head=current_word->next;
        current_word->next->last=nullptr;
        current_word=current_word->next;
    } else {
        current_word->last->next=current_word->next;
        current_word->next->last=current_word->last;
        current_word=current_word->next;
    }
    delete temp;
}

void Word::printAllWords() const{
    for(Word const* ptr=this; ptr!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}

void Word::getWordDataToUI() {
    emit giveWordData(static_cast<QString>(getWordData()));
}

CustomString Word::getEnglish() const{
    return english;
}

CustomString Word::getPart() const{
    return part;
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
