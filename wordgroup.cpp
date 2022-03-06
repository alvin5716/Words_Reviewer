#include "wordgroup.h"
#include <climits>

WordGroup::WordGroup():
    words_head(nullptr), words_tail(nullptr), current_word(nullptr), selected_word(nullptr),
    current_index(0), selected_index(0),
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
    words_head = words_tail = current_word = selected_word = nullptr;
    word_count = current_index = selected_index = 0;
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

void WordGroup::editSelectedWord(CustomString english, CustomString part, CustomString meaning) {
    selected_word->setEnglish(english);
    selected_word->setPart(part);
    selected_word->setMeaning(meaning);
}

void WordGroup::insertNewWord(CustomString english, CustomString part, CustomString meaning) {
    if(word_count == UINT_MAX) {
        qDebug() << "word group reach its maximum";
        return;
    }
    if(selected_word == nullptr) {
        this->pushNewWord(english, part, meaning);
        return;
    }
    Word* new_word = new Word(english,part,meaning);
    if(selected_word == words_tail) words_tail=new_word;
    new_word->setLast(selected_word);
    new_word->setNext(selected_word->getNext());
    if(selected_word->getNext() != nullptr) selected_word->getNext()->setLast(new_word);
    selected_word->setNext(new_word);
    if(words_head == nullptr) words_head=new_word;

    ++word_count;
    selected_word = new_word;
    ++selected_index;
    this->setCurrentToSelected();
}

void WordGroup::deleteSelectedWord() {
    if(word_count == 0) {
        qDebug() << "no word can be deleted";
        return;
    }
    if(current_word==nullptr) return;
    Word* temp=selected_word;
    if(words_head==words_tail) { //selected word is the only one word
        words_head = words_tail = selected_word = current_word = nullptr;
        selected_index = current_index = 0; //null
    } else if(selected_word==words_tail) { //selected word is tail
        words_tail = selected_word->getLast();
        selected_word->getLast()->setNext(nullptr);
        selected_word = words_head;
        selected_index = 0; //head
        if(current_word == temp) this->setCurrentToSelected();
    } else if(selected_word==words_head) { //selected word is head
        words_head = selected_word->getNext();
        selected_word->getNext()->setLast(nullptr);
        selected_word = selected_word->getNext();
        if(current_word == temp) this->setCurrentToSelected();
    } else {
        selected_word->getLast()->setNext(selected_word->getNext());
        selected_word->getNext()->setLast(selected_word->getLast());
        selected_word=selected_word->getNext();
        if(current_word == temp) this->setCurrentToSelected();
    }
    delete temp;
    --word_count;
}

unsigned int WordGroup::getWordCount() const {
    return word_count;
}

const Word* WordGroup::getSelectedWord() const {
    return this->selected_word;
}

unsigned int WordGroup::getSelectedIndex() const {
    return this->selected_index;
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

void WordGroup::selectCurrentWord() {
    this->selected_word = this->current_word;
    this->selected_index = this->current_index;
}

void WordGroup::setCurrentToSelected() {
    this->current_word = this->selected_word;
    this->current_index = this->selected_index;
}

WordGroup::~WordGroup() {
    deleteWholeList();
}
