#include "word.h"

Word::Word(CustomString english, CustomString part, CustomString meaning):
    english(english), part(part), meaning(meaning),
    next(nullptr), last(nullptr)
{

}

void Word::printAllWords() const{
    for(Word const* ptr=this; ptr!=nullptr; ptr=ptr->next) {
        qDebug() << ptr->english << ptr->part << ptr->meaning;
    }
}

CustomString Word::getEnglish() const{
    return english;
}

CustomString Word::getPart() const{
    return part;
}

CustomString Word::getMeaning() const{
    return meaning;
}

void Word::setEnglish(CustomString english) {
    this->english = english;
}

void Word::setPart(CustomString part) {
    this->part = part;
}

void Word::setMeaning(CustomString meaning) {
    this->meaning = meaning;
}

CustomString Word::getWordData(WordShowingMode::Mode onlyWord) const{
    switch (onlyWord) {
    case WordShowingMode::AllShowing:
        return english+" "+part+" "+meaning;
    case WordShowingMode::WordShowing:
        return english;
    case WordShowingMode::MeaningNotShowing:
        return english+" "+part;
    case WordShowingMode::MeaningShowing:
        return meaning;
    case WordShowingMode::WordNotShowing:
        return part+" "+meaning;
    case WordShowingMode::PartNotShowing:
        return english+" "+meaning;
    default:
        qDebug() << "No such mode, nextMode() failed.";
        return english+" "+part+" "+meaning;
    }
}

Word* Word::getNext() const{
    return this->next;
}
Word* Word::getLast() const{
    return this->last;
}
void Word::setNext(Word* next) {
    this->next = next;
}
void Word::setLast(Word* last) {
    this->last = last;
}

Word::~Word() {

}
