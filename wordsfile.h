#ifndef WORDSFILE_H
#define WORDSFILE_H

#include <QFile>
#include <QRegExp>
#include "customstring.h"
#include "wordgroup.h"
#include "appoption.h"

class WordsFile: public QFile
{
public:
    WordsFile(CustomString file_name);
    void prepToWrite();
    void prepToRead();
    void clearToWrite();
    bool deleteLine(int x); //return true if deletion successed, first line is 1 instead of 0
    bool readWordFile(WordGroup* words); // reture true if read successed
    AppOption readOptionFile();
private:
    QRegExp regex;
};

#endif // WORDSFILE_H
