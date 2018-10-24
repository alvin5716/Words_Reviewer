#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QDebug>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    file(new WordsFile),
    words_head(nullptr), current_word(nullptr), words_tail(nullptr),
    playing(true),word_list_opening(false)
{
    ui->setupUi(this);
    //widget settings
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setGeometry(QApplication::desktop()->screenGeometry().width()/2-346,0,692,131);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocus();
    wordListClose();
    ui->stackedWidget->setCurrentIndex(customEnum::wordListPage);
    //read file
    unsigned lines_count=0;
    while(!file->atEnd()) {
        ++lines_count;
        CustomString strIn = file->readLine();
        QStringList strsIn = strIn.split(' ');
        try {
            //check format of file
            if(strsIn.size()!=3) throw strsIn.size();
            CustomString english(strsIn.at(0)), part(strsIn.at(1)), meaning(strsIn.at(2));
            //chop \n and \r
            meaning.chopNewLineChar();
            //replace underline
            english.replaceUnderlineToSpace();
            part.replaceUnderlineToSpace();
            meaning.replaceUnderlineToSpace();
            //create new word and push it into data
            pushNewWord(words_head,words_tail,english,part,meaning);
            //connect
            this->connectWordToUI(words_head);
        } catch(int size) {
            qDebug() << "word file format error: got" << size << "string in Line" << lines_count << "\n";
            qApp->quit();
        }
    }
    file->close();
    //show word settings
    if(words_head!=nullptr) {
        srand(time(NULL));
        current_word = words_head->at(rand()%Word::getCount());
        current_word->getWordDataToUI();
    } else {
        ui->strWord->setText("-");
    }
    setPreferredFontSize();
    //timer
    timer = new QTimer;
    timer->setInterval(10000);
    connect(timer,&QTimer::timeout,this,&MainWindow::showNextWord);
    timer->start();
    //buttons
    connect(ui->closeButton,SIGNAL(clicked(bool)),qApp,SLOT(quit()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(showNextWord()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(showLastWord()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(playButtonClicked()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(listButtonClicked()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->wordList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(FindItemAndShowWord(QListWidgetItem*)));
    connect(ui->addNewWordButton,SIGNAL(clicked(bool)),this,SLOT(prepToAddNewWord()));
    connect(ui->backInputButton,SIGNAL(clicked(bool)),this,SLOT(listWords()));
    connect(ui->enterInputButton,SIGNAL(clicked(bool)),this,SLOT(submitInput()));
    connect(ui->clearInputButton,SIGNAL(clicked(bool)),this,SLOT(clearInputs()));
}

void MainWindow::clearInputs() {
    ui->englishInput->clear();
    ui->partInput->clear();
    ui->meaningInput->clear();
}

void MainWindow::submitInput() {
    try {
        CustomString english(ui->englishInput->text()), part(ui->partInput->text()), meaning(ui->meaningInput->text());
        if(english.lengthWithoutSpace()==0 || part.lengthWithoutSpace()==0 || meaning.lengthWithoutSpace()==0)
            throw 1;
        //linked-list
        english.replaceSpaceToUnderline();
        part.replaceSpaceToUnderline();
        meaning.replaceSpaceToUnderline();
        pushNewWord(words_head,words_tail,english,part,meaning);
        //file
        file->prepToWrite();
        QTextStream out(file);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(false);
        out << english << ' ' << part << ' ' << meaning << '\n';
        file->close();
        //clear forms
        clearInputs();
    } catch(int) {
        spawnWarningBox("You have to fill all 3 forms.");
    }
}

void MainWindow::spawnWarningBox(CustomString content) {
    QMessageBox *warning_box = new QMessageBox(QMessageBox::Icon::Critical,"Error",static_cast<QString>(content));
    warning_box->setWindowFlags(Qt::WindowStaysOnTopHint);
    warning_box->show();
}

void MainWindow::prepToAddNewWord() {
    ui->stackedWidget->setCurrentIndex(customEnum::wordInputPage);
}

void MainWindow::listWords() {
    ui->stackedWidget->setCurrentIndex(customEnum::wordListPage);
    ui->wordList->clear();
    for(Word* ptr=words_head;ptr!=nullptr;ptr=ptr->getNext()) {
        QListWidgetItem *item= new QListWidgetItem(static_cast<QString>(ptr->getWordData()));
        item->setFont(QFont("微軟正黑體",14));
        ui->wordList->addItem(item);
    }
}

void MainWindow::FindItemAndShowWord(QListWidgetItem* clicked_item) {
    for(Word* ptr=words_head;ptr!=nullptr;ptr=ptr->getNext()) {
        if(clicked_item->text()==ptr->getWordData()) {
            showWord(ptr);
            break;
        }
    }
}

void MainWindow::showWord(Word *word) {
    word->getWordDataToUI();
    current_word=word;
    setPreferredFontSize();
    if(playing) timer->start();
}

void MainWindow::playButtonClicked() {
    if(this->playing) {
        timer->stop();
        ui->playButton->setText("Play");
    } else {
        timer->start();
        ui->playButton->setText("Pause");
    }
    playing=!playing;
}

void MainWindow::listButtonClicked() {
    if(this->word_list_opening) {
        wordListClose();
    } else {
        wordListOpen();
    }
    word_list_opening=!word_list_opening;
}

void MainWindow::wordListOpen() {
    ui->listButton->setText("List Close");
    ui->listButton->setGeometry(ui->listButton->x(),ui->listButton->y(),121,ui->listButton->height());
    ui->stackedWidget->show();
    this->setGeometry(this->geometry().x(),this->geometry().y(),this->geometry().width(),561);
    listWords();
}

void MainWindow::wordListClose() {
    ui->listButton->setText("List");
    ui->listButton->setGeometry(ui->listButton->x(),ui->listButton->y(),71,ui->listButton->height());
    ui->stackedWidget->hide();
    this->setGeometry(this->geometry().x(),this->geometry().y(),this->geometry().width(),131);
    ui->wordList->clear();
}

void MainWindow::showNextWord() {
    if(current_word!=nullptr && current_word->getNext()!=nullptr) {
        showWord(current_word->getNext());
    } else if(words_head!=nullptr) {
        showWord(words_head);
    } else {
        ui->strWord->setText("-");
    }
}

void MainWindow::showLastWord() {
    if(current_word!=nullptr && current_word->getLast()!=nullptr) {
        showWord(current_word->getLast());
    } else if(words_tail!=nullptr) {
        showWord(words_tail);
    } else {
        ui->strWord->setText("-");
    }
}

void MainWindow::connectWordToUI(Word* word) {
    if(word==nullptr) return;
    connect(word,SIGNAL(giveWordData(QString)),ui->strWord,SLOT(setText(QString)));
}

void MainWindow::setPreferredFontSize() {
    int size=0.9*this->geometry().width()/ui->strWord->text().length();
    if(size<10) size=10;
    else if(size>30) size=30;
    ui->strWord->setFont(QFont("微軟正黑體",size));
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_W:
        this->setGeometry(this->geometry().x(),this->geometry().y()-5,this->geometry().width(),this->geometry().height());
        break;
    case Qt::Key_A:
        this->setGeometry(this->geometry().x()-5,this->geometry().y(),this->geometry().width(),this->geometry().height());
        break;
    case Qt::Key_S:
        this->setGeometry(this->geometry().x(),this->geometry().y()+5,this->geometry().width(),this->geometry().height());
        break;
    case Qt::Key_D:
        this->setGeometry(this->geometry().x()+5,this->geometry().y(),this->geometry().width(),this->geometry().height());
        break;
    case Qt::Key_Left:
        this->showLastWord();
        break;
    case Qt::Key_Right:
        this->showNextWord();
        break;
    case Qt::Key_C:
        qApp->quit();
        break;
    case Qt::Key_Space:
    case Qt::Key_P:
        playButtonClicked();
        break;
    case Qt::Key_L:
        listButtonClicked();
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
