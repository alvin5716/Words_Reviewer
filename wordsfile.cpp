#include "wordsfile.h"
#include <QDebug>
#include "customstring.h"
#include <cstdio>

WordsFile::WordsFile(CustomString file_name)
    :QFile(file_name)
{
    this->open(QIODevice::ReadOnly);
    if(!this) qDebug() << "can't get words file";
}

void WordsFile::prepToWrite() {
    if(!(!this)) this->close();
    this->open(QIODevice::WriteOnly|QIODevice::Append);
}

void WordsFile::clearToWrite() {
    if(!(!this)) this->close();
    this->open(QIODevice::WriteOnly);
}

void WordsFile::prepToRead() {
    if(!(!this)) this->close();
    this->open(QIODevice::ReadOnly);
}

bool WordsFile::deleteLine(int x) {
    CustomString line;
    this->prepToRead();
    WordsFile temp("wordsfile_temp.txt");
    temp.prepToWrite();
    QTextStream temp_text(&temp);
    temp_text.setCodec("UTF-8");
    temp_text.setGenerateByteOrderMark(false);
    int i=0;
    while(!this->atEnd()) {
        line = this->readLine();
        if(++i!=x) temp_text << line;
    }
    this->close();
    temp.close();

    remove(this->fileName());
    rename("wordsfile_temp.txt",this->fileName());
    return i>=x;
}
