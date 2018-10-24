#include "customstring.h"

CustomString::CustomString(const char *ch)
    :QString(ch)
{

}

CustomString::CustomString(QString str)
    :QString(str)
{

}

CustomString::CustomString(QByteArray str)
    :QString(str)
{

}

void CustomString::chopNewLineChar() {
    if(this->endsWith("\r\n")) {
        this->chop(2);
    } else if(this->endsWith("\n")) {
        this->chop(1);
    }
}

void CustomString::replaceUnderlineToSpace() {
    while(this->indexOf("_")!=-1) {
        this->replace(this->indexOf("_"),1," ");
    }
}

void CustomString::replaceSpaceToUnderline() {
    while (this->indexOf(" ")!=-1) {
        this->replace(this->indexOf(" "),1,"_");
    }
}

void CustomString::partOfSpeechSetting() {
    if(!this->startsWith("(")) {
        this->prepend("(");
    }
    if(this->endsWith(".")) {
        this->prepend(")");
    } else if(this->endsWith(".)")) {
        ;
    } else if(this->endsWith(")")) {
        this->insert(this->length()-1,".");
    } else {
        this->prepend(".)");
    }
}

int CustomString::lengthWithoutSpace() {
    CustomString temp(*this);
    int space_count=0;
    while (temp.indexOf(" ")!=-1) {
        temp.replace(temp.indexOf(" "),1,"_");
        ++space_count;
    }
    return this->length()-space_count;
}
