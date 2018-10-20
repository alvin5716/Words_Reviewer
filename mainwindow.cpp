#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QString>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    file(new WordsFile),
    words_head(nullptr), current_word(nullptr)
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
        this->connectWordToUI(words_head);
        } catch(int size) {
            qDebug() << "word file format error: only got " << size << " string in Line " << lines_count << ".\n";
        }
    }
    if(words_head!=nullptr) {
        words_head->printAllWords();
        srand(time(NULL));
        current_word = words_head->at(rand()%Word::getCount());
        current_word->giveWordData();
    } else {
        ui->english->setText("-");
        ui->part->setText("-");
        ui->meaning->setText("-");
    }
    timer = new QTimer;
    timer->setInterval(30000);
    connect(timer,&QTimer::timeout,this,&MainWindow::showNextWord);
    timer->start();
}

void MainWindow::showNextWord() {
    if(current_word!=nullptr && current_word->getNext()!=nullptr) current_word=current_word->getNext();
    else if(words_head!=nullptr) current_word=words_head;
    else {
        ui->english->setText("-");
        ui->part->setText("-");
        ui->meaning->setText("-");
    }
    current_word->giveWordData();
}

void MainWindow::connectWordToUI(Word* word) {
    if(word==nullptr) return;
    connect(word,SIGNAL(giveEnglish(QString)),ui->english,SLOT(setText(QString)));
    connect(word,SIGNAL(givePart(QString)),ui->part,SLOT(setText(QString)));
    connect(word,SIGNAL(giveMeaning(QString)),ui->meaning,SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}
