#include "wordsfile.h"
#include <QDebug>

WordsFile::WordsFile()
    :QFile("words")
{
    this->open(QIODevice::ReadOnly);
    if(!this) qDebug() << "can't get words file";
}

void WordsFile::prepToWrite() {
    if(!(!this)) this->close();
    this->open(QIODevice::WriteOnly|QIODevice::Append);
}

void WordsFile::prepToRead() {
    if(!(!this)) this->close();
    this->open(QIODevice::ReadOnly);
}
