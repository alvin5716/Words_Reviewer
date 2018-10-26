#include "customstring.h"
#include <QStringList>

CustomString::CustomString()
{

}


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
    CustomString temp(*this);
    QStringList temp2 = temp.split(",");
    CustomString temp3[temp2.length()];
    for(int i=0;i<temp2.length();++i) {
        temp3[i] = temp2.at(i);
        //left parenthesis
        if(!temp3[i].startsWith("(")) {
            temp3[i].prepend("(");
        }
        //right parenthesis and dot
        if(temp3[i].endsWith(".")) {
            temp3[i].append(")");
        } else if(temp3[i].endsWith(".)")) {
            ;
        } else if(temp3[i].endsWith(")")) {
            temp3[i].insert(temp3[i].length()-1,".");
        } else {
            temp3[i].append(".)");
        }
    }
    this->clear();
    for(int i=0;i<temp2.length();++i) {
        this->append(temp3[i]);
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
