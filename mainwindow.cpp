#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    file(new WordsFile)
{
    ui->setupUi(this);
    unsigned lines_count=0;
    while(!file->atEnd()) {
        ++lines_count;
        QString strIn = file->readLine();
        QStringList strsIn = strIn.split(' ');
        try {
            //check format of file
            if(strsIn.size()!=3) throw strsIn.size();
            QString english(strsIn.at(0)), part(strsIn.at(1)), meaning(strsIn.at(2));
            //chop \n and \r
            if(meaning.endsWith("\r\n")) {
                meaning.chop(2);
            } else if(meaning.endsWith("\n")) {
                meaning.chop(1);
            }
        pushNewWord(words_head,english,part,meaning);
        } catch(int size) {
            qDebug() << "word file format error: only got " << size << " string in Line " << lines_count << ".\n";
        }
    }
    printAllWords(words_head);
}

MainWindow::~MainWindow()
{
    delete ui;
}
