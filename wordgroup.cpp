#include "wordgroup.h"
#include <climits>

WordGroup::WordGroup():
    words_head(nullptr), words_tail(nullptr), current_word(nullptr),
    current_index(0),
    word_count(0)
{

}

void WordGroup::getWordDataToUI(WordShowingMode::Mode onlyWord) {
    if(current_word == nullptr) {
        emit giveWordData("-");
    } else {
        emit giveWordData(static_cast<QString>(current_word->getWordData(onlyWord)));
    }
}

void WordGroup::deleteWholeList() {
    if(words_head == nullptr) return;
    for(Word* ptr=words_head; ptr!=nullptr; ) {
        Word* next = ptr->getNext();
        delete ptr;
        ptr = next;
    }
    words_head = words_tail = current_word = nullptr;
    word_count = current_index = 0;
}

void WordGroup::pushNewWord(CustomString english, CustomString part, CustomString meaning) {
    if(word_count == UINT_MAX) {
        qDebug() << "word group reach its maximum";
        return;
    }
    Word* new_word = new Word(english,part,meaning);
    Word* ptr=words_tail;
    words_tail=new_word;
    new_word->setLast(ptr);
    if(ptr!=nullptr) ptr->setNext(new_word);
    else words_head=new_word;
    ++word_count;
    current_word = new_word;
    current_index = word_count-1;
}

void WordGroup::deleteCurrentWord() {
    if(word_count == 0) {
        qDebug() << "no word can be deleted";
        return;
    }
    if(current_word==nullptr) return;
    Word* temp=current_word;
    if(words_head==words_tail) words_head=words_tail=current_word=nullptr;
    else if(current_word==words_tail) {
        words_tail = current_word->getLast();
        current_word->getLast()->setNext(nullptr);
        current_word = words_head;
        current_index = 0;
    } else if(current_word==words_head) {
        words_head = current_word->getNext();
        current_word->getNext()->setLast(nullptr);
        current_word = current_word->getNext();
    } else {
        current_word->getLast()->setNext(current_word->getNext());
        current_word->getNext()->setLast(current_word->getLast());
        current_word=current_word->getNext();
    }
    delete temp;
    --word_count;
}

unsigned int WordGroup::getWordCount() const {
    return word_count;
}

const Word* WordGroup::getCurrentWord() const {
    return this->current_word;
}

unsigned int WordGroup::getCurrentIndex() const {
    return this->current_index;
}

void WordGroup::switchToNextWord() {
    if(this->current_word == nullptr) return;
    if(this->current_word->getNext() == nullptr) {
        this->current_word = this->words_head;
        this->current_index = 0;
    }
    else {
        this->current_word = this->current_word->getNext();
        ++current_index;
    }
}

void WordGroup::switchToLastWord() {
    if(this->current_word == nullptr) return;
    if(this->current_word->getLast() == nullptr) {
        this->current_word = this->words_tail;
        this->current_index = this->word_count-1;
    } else {
        this->current_word = this->current_word->getLast();
        --current_index;
    }
}

void WordGroup::switchToWordByIndex(unsigned int index) {
    Word* ptr = this->at(index);
    if(ptr == nullptr) return;
    this->current_word = ptr;
    this->current_index = index;
}

void WordGroup::switchToWordByData(CustomString word_data) {
    unsigned int i = 0;
    for(Word* ptr=words_head; ptr!=nullptr; ptr=ptr->getNext()) {
        if(word_data==ptr->getWordData()) {
            this->current_word = ptr;
            this->current_index = i;
            return;
        }
        ++i;
    }
}

void WordGroup::searchWordStart(CustomString keyword) const{
    this->keyword = keyword;
    this->searched_word = nullptr;
}

const Word* WordGroup::searchWord() const{
    if(this->searched_word == nullptr) searched_word = this->words_head;
    else searched_word = searched_word->getNext();
    if(keyword == "") return searched_word;
    while(searched_word != nullptr) {
        if(CustomString(searched_word->getEnglish()).startsWith(keyword) ||
                        CustomString(searched_word->getMeaning()).startsWith(keyword)) {
            return searched_word;
        }
        searched_word = searched_word->getNext();
    }
    return nullptr;
}

bool WordGroup::isWordExisted(CustomString english, CustomString part) const{
    for(Word* ptr=words_head; ptr!=nullptr; ptr=ptr->getNext())
        if(ptr->getEnglish()==english && ptr->getPart()==part)
            return true;
    return false;
}

Word *WordGroup::at(unsigned int index) const{
    Word* ptr = words_head;
    for(unsigned int i=0; ptr!=nullptr && i<index; ptr=ptr->getNext(), ++i) ;
    return ptr;
}

WordGroup::~WordGroup() {
    deleteWholeList();
}
