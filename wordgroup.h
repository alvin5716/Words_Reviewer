#ifndef WORDGROUP_H
#define WORDGROUP_H

#include "word.h"
#include <QObject>

class WordGroup : public QObject
{
    Q_OBJECT
public:
    WordGroup();
    void pushNewWord(CustomString english, CustomString part, CustomString meaning);
    void deleteWholeList();
    void deleteCurrentWord();
    unsigned int getWordCount() const;
    const Word* getCurrentWord() const;
    unsigned int getCurrentIndex() const;
    void switchToNextWord();
    void switchToLastWord();
    void switchToWordByIndex(unsigned int index);
    void switchToWordByData(CustomString word_data);
    bool isWordExisted(CustomString english, CustomString part) const;
    void searchWordStart(CustomString keyword = "") const;
    const Word* searchWord() const;
    ~WordGroup();
public slots:
    void getWordDataToUI(WordShowingMode::Mode onlyWord=WordShowingMode::AllShowing);
signals:
    void giveWordData(QString);
private:
    Word *at(unsigned int index) const;
    Word* words_head, *words_tail, *current_word;
    unsigned int current_index;
    mutable CustomString keyword;
    mutable Word* searched_word;
    unsigned int word_count;
};

#endif // WORDGROUP_H
