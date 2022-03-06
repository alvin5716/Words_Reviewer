#ifndef WORDSFILE_H
#define WORDSFILE_H

#include <QFile>
#include <QRegExp>
#include "customstring.h"
#include "wordgroup.h"
#include "appoption.h"
#include <functional>

class WordsFile: public QFile
{
public:
    WordsFile(CustomString file_name);
    void prepToWrite();
    void prepToRead();
    void clearToWrite();
    bool deleteLine(unsigned int x, bool isWordLineOnly=false); //return true if deletion successed, first line is 1 instead of 0
    bool editLine(unsigned int x, CustomString str, bool isWordLineOnly=false); //return true if edit successed, first line is 1 instead of 0
    bool insertLine(unsigned int x, CustomString str, bool isWordLineOnly=false); //return true if insertion successed, first line is 1 instead of 0
    bool readWordFile(WordGroup* words); // reture true if read successed
    AppOption readOptionFile();
private:
    bool modifyByLine(std::function<bool(QTextStream &)> modify_method);
    QRegExp wordLineRegex;
};

#endif // WORDSFILE_H
