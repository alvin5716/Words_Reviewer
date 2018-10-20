#ifndef WORDSFILE_H
#define WORDSFILE_H

#include <QFile>

class WordsFile: public QFile
{
public:
    WordsFile();
    void prepToWrite();
    void prepToRead();
};

#endif // WORDSFILE_H
