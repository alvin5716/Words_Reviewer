#ifndef CUSTOMSTRING_H
#define CUSTOMSTRING_H

#include <QString>

class CustomString: public QString
{
public:
    CustomString();
    CustomString(QString str);
    CustomString(QByteArray str);
    CustomString(const char *ch);
    void chopNewLineChar();
    void replaceUnderlineToSpace();
    void replaceSpaceToUnderline();
    int lengthWithoutSpace();
    void partOfSpeechSetting();
};

#endif // CUSTOMSTRING_H
