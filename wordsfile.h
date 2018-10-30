#ifndef WORDSFILE_H
#define WORDSFILE_H

#include <QFile>
#include "customstring.h"

class WordsFile: public QFile
{
public:
    WordsFile(CustomString file_name="words");
    void prepToWrite();
    void prepToRead();
    void clearToWrite();
    bool deleteLine(int x); //return true if deletion successed
};

#endif // WORDSFILE_H
