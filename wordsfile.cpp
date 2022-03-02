#include "wordsfile.h"
#include <QDebug>
#include "customstring.h"
#include <cstdio>

WordsFile::WordsFile(CustomString file_name)
    :QFile(file_name), regex("\\S+\\s+(?:\\0050\\S+\\0056\\0051)+\\s+\\S+")
{
}

void WordsFile::prepToWrite() {
    this->open(QIODevice::WriteOnly|QIODevice::Append);
}

void WordsFile::clearToWrite() {
    this->open(QIODevice::WriteOnly);
}

void WordsFile::prepToRead() {
    this->open(QIODevice::ReadOnly);
}

bool WordsFile::modifyByLine(std::function<bool(QTextStream&)> modify_method) {
    this->prepToRead();
    WordsFile temp("wordsfile_temp.txt");
    temp.prepToWrite();
    QTextStream temp_text(&temp);
    temp_text.setCodec("UTF-8");
    temp_text.setGenerateByteOrderMark(false);

    bool isSuccessful = modify_method(temp_text);

    this->close();
    temp.close();

    remove(this->fileName());
    rename("wordsfile_temp.txt",this->fileName());
    return isSuccessful;
}

bool WordsFile::deleteLine(unsigned int x) {
    bool isSuccessful = modifyByLine(
        [this, x] (QTextStream& new_file_stream) {
            CustomString old_line;
            unsigned int i=0;
            while(!this->atEnd()) {
                old_line = this->readLine();
                if(++i!=x) new_file_stream << old_line;
            }
            return i>=x;
        }
    );
    return isSuccessful;
}

bool WordsFile::editLine(unsigned int x, CustomString new_line) {
    bool isSuccessful = modifyByLine(
        [this, x, new_line] (QTextStream& new_file_stream) {
            CustomString old_line;
            unsigned int i=0;
            while(!this->atEnd()) {
                old_line = this->readLine();
                if(++i!=x) new_file_stream << old_line;
                else new_file_stream << new_line << '\n';
            }
            return i>=x;
        }
    );
    return isSuccessful;
}

bool WordsFile::insertLine(unsigned int x, CustomString new_line) {
    bool isSuccessful = modifyByLine(
        [this, x, new_line] (QTextStream& new_file_stream) {
            CustomString line;
            unsigned int i=0;
            while(!this->atEnd()) {
                if(++i!=x) {
                    line = this->readLine();
                    new_file_stream << line;
                } else {
                    new_file_stream << new_line << '\n';
                }
            }
            if(i+1==x) {
                new_file_stream << new_line << '\n';
                ++i;
            }
            return i>=x;
        }
    );
    return isSuccessful;
}

AppOption WordsFile::readOptionFile() {
    AppOption app_option;
    this->prepToRead();
    unsigned lines_count=0;
    while(!this->atEnd()) {
        ++lines_count;
        CustomString strIn = this->readLine();
        QStringList strsIn = strIn.split(' ');
        CustomString strOption = strsIn.at(0);
        strOption.chopNewLineChar();
        try {
            if(strOption=="position") {
                if(strsIn.length()<3) throw strsIn.length();
                CustomString strX = strsIn.at(1), strY = strsIn.at(2);
                app_option.position_x = strX.toInt();
                app_option.position_y = strY.toInt();
            } else if(strOption=="order") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                app_option.order = AppOption::intToOrderMethod(strX.toInt());
            } else if(strOption=="lastword") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                app_option.lastword = strX.toInt();
            } else if(strOption=="pause") {
                app_option.playing = false;
            } else if(strOption=="stayontop") {
                app_option.staying_on_top = true;
            } else if(strOption=="interval") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                app_option.interval = strX.toInt();
            }
        } catch(int args) {
            qDebug() << "Error: At File \"options\": Only got" << args << "arguments in Line" << lines_count;
        }
    }
    this->close();
    return app_option;
}

bool WordsFile::readWordFile(WordGroup* words) {
    this->prepToRead();
    unsigned lines_count=0;
    while(!this->atEnd()) {
        ++lines_count;
        //read line
        CustomString strIn = this->readLine();
        //check format of the line
        if(regex.indexIn(strIn) != 0) continue;
        //start split the line
        QStringList strsIn = strIn.split(' ');
        try {
            //check format of the splited line
            if(strsIn.size()<3) throw strsIn.size();
            //The line is in correct format. Start parse the line into word data
            CustomString english(strsIn.at(0)), part(strsIn.at(1)), meaning(strsIn.at(2));
            //chop \n and \r
            meaning.chopNewLineChar();
            //replace underline
            english.replaceUnderlineToSpace();
            part.replaceUnderlineToSpace();
            meaning.replaceUnderlineToSpace();
            //create new word and push it into data
            words->pushNewWord(english,part,meaning);
        } catch(int size) {
            qDebug() << "Error: At File \"" + this->fileName() + "\": Only got" << size << "string in Line" << lines_count;
            return false;
        }
    }
    this->close();
    return true;
}
