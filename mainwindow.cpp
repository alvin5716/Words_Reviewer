#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QDebug>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    words_file(new WordsFile("words.txt")), options_file(new WordsFile("options.txt")),
    words_head(nullptr), current_word(nullptr), words_tail(nullptr),
    playing(true),word_list_opening(false), minimizing(false), staying_on_top(false),
    isOnlyWordShowing(false),
    dragStartPos(QPoint(0,0)),
    order_method(customEnum::orderChronological)
{
    ui->setupUi(this);
    //widget settings
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QApplication::desktop()->screenGeometry().width()/2-346,0,692,131);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocus();
    wordListClose();
    ui->stackedWidget->setCurrentIndex(customEnum::wordListPage);
    //sliders
    ui->intervalSlider->setRange(1,60);
    ui->intervalSlider->setValue(20);
    connect(ui->intervalSlider,SIGNAL(valueChanged(int)),this,SLOT(intervalShow(int)));
    connect(ui->intervalSlider,SIGNAL(actionTriggered(int)),this,SLOT(setFocus()));
    connect(ui->intervalSlider,SIGNAL(sliderPressed()),this,SLOT(setFocus()));
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
    connect(ui->deleteWordButton,SIGNAL(clicked(bool)),this,SLOT(deleteWordButtonClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked(bool)),this,SLOT(minimizeButtonClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->onlyWordButton,SIGNAL(clicked(bool)),this,SLOT(toggleOnlyWord()));
    connect(ui->searchOnNetButton,SIGNAL(clicked(bool)),this,SLOT(searchOnNet()));
    connect(ui->optionsButton,SIGNAL(clicked(bool)),this,SLOT(showOptions()));
    connect(ui->backOptionButton,SIGNAL(clicked(bool)),this,SLOT(listWords()));
    connect(ui->playingOrderButton,SIGNAL(clicked(bool)),this,SLOT(changeOrderMethod()));
    connect(ui->stayingOnTopButton,SIGNAL(clicked(bool)),this,SLOT(stayOnTopButtonClick()));
    //forms
    connect(ui->englishInput,SIGNAL(selectNextOne()),ui->partInput,SLOT(setFocus()));
    connect(ui->partInput,SIGNAL(selectNextOne()),ui->meaningInput,SLOT(setFocus()));
    connect(ui->meaningInput,SIGNAL(selectNextOne()),ui->englishInput,SLOT(setFocus()));
    connect(ui->meaningInput,SIGNAL(selectLastOne()),ui->partInput,SLOT(setFocus()));
    connect(ui->partInput,SIGNAL(selectLastOne()),ui->englishInput,SLOT(setFocus()));
    connect(ui->englishInput,SIGNAL(selectLastOne()),ui->meaningInput,SLOT(setFocus()));
    connect(ui->meaningInput,SIGNAL(enterInputs()),ui->enterInputButton,SIGNAL(clicked()));
    connect(ui->searchInput,SIGNAL(textEdited(QString)),this,SLOT(searchListAndShow()));
    connect(ui->searchInput,SIGNAL(pressed()),this,SLOT(searchStart()));
    connect(ui->searchInput,SIGNAL(editingFinished()),this,SLOT(searchFinish()));
    initSettings();
    //cursor
    QVector<QWidget*> pointerCursorWdgts({
        ui->searchOnNetButton, ui->onlyWordButton, ui->closeButton, ui->minimizeButton,
        ui->nextButton, ui->lastButton, ui->playButton,
        ui->listButton, ui->enterInputButton, ui->clearInputButton,
        ui->backInputButton, ui->addNewWordButton, ui->deleteWordButton, ui->optionsButton,
        ui->backOptionButton, ui->stayingOnTopButton, ui->playingOrderButton, ui->intervalSlider
    });
    for(QWidget* w: pointerCursorWdgts)
        w->setCursor(Qt::PointingHandCursor);
}

void MainWindow::searchOnNet() {
    if(!this->current_word) return;
    QString url_str("https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/"
                    +this->current_word->getEnglish()
    );
    QUrl url(url_str);
    QDesktopServices::openUrl(url);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    this->dragStartPos = e->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint distant = e->globalPos() - this->dragStartPos;
    move(this->x()+distant.x(), this->y()+distant.y());
    this->dragStartPos = e->globalPos();
}


void MainWindow::toggleOnlyWord() {
    isOnlyWordShowing = !isOnlyWordShowing;
    ui->onlyWordButton->setText(this->isOnlyWordShowing?"Word":"Meaning");
    if(!this->current_word) return;
    showWord(this->current_word);
}

void MainWindow::initSettings() {
    //regex for checking format later
    QRegExp regex("\\S+\\s+(?:\\0050\\S+\\0056\\0051)+\\s+\\S+");
    //read word file
    unsigned lines_count=0;
    while(!words_file->atEnd()) {
        ++lines_count;
        //read line
        CustomString strIn = words_file->readLine();
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
            Word* new_word = pushNewWord(words_head,words_tail,english,part,meaning);
            //connect
            this->connectWordToUI(new_word);
        } catch(int size) {
            qDebug() << "Error: At File \"" + words_file->fileName() + "\": Only got" << size << "string in Line" << lines_count;
            qApp->quit();
        }
    }
    words_file->close();
    //show word settings
    if(words_head!=nullptr) {
        qsrand(time(NULL));
        current_word = words_head->at(qrand()%Word::getCount());
        current_word->getWordDataToUI();
    } else {
        ui->strWord->setText("-");
    }
    setPreferredFontSize();
    //timer
    timer = new CustomTimer;
    timer->setInterval(20000);
    connect(timer,&CustomTimer::timeout,this,&MainWindow::showNextWord);
    timer->start();
    connect(ui->intervalSlider,SIGNAL(valueChanged(int)),timer,SLOT(restartWithInterval(int)));
    //read options file
    lines_count=0;
    while(!this->options_file->atEnd()) {
        ++lines_count;
        CustomString strIn = this->options_file->readLine();
        QStringList strsIn = strIn.split(' ');
        CustomString strOption = strsIn.at(0);
        strOption.chopNewLineChar();
        try {
            if(strOption=="position") {
                if(strsIn.length()<3) throw strsIn.length();
                CustomString strX = strsIn.at(1), strY = strsIn.at(2);
                int x=strX.toInt(), y=strY.toInt();
                this->setGeometry(x,y,this->width(),this->height());
            } else if(strOption=="order") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                int x=strX.toInt();
                for(int i=0;i<x;++i) this->changeOrderMethod();
            } else if(strOption=="lastword") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                int x=strX.toInt();
                showWord(words_head->at(x));
            } else if(strOption=="pause") {
                this->playButtonClicked();
            } else if(strOption=="stayontop") {
                this->stayOnTopButtonClick();
            } else if(strOption=="interval") {
                if(strsIn.length()<2) throw strsIn.length();
                CustomString strX = strsIn.at(1);
                int x=strX.toInt();
                ui->intervalSlider->setValue(x);
                timer->restartWithInterval(x);
                this->intervalShow(x);
            }
        } catch(int args) {
            qDebug() << "Error: At File \"options\": Only got" << args << "arguments in Line" << lines_count;
        }
    }
    options_file->close();
}

void MainWindow::intervalShow(int interval) {
    ui->strIntervalSecond->setText(CustomString().setNum(interval)+" sec");
}

void MainWindow::stayOnTopButtonClick() {
    if(this->staying_on_top) {
        this->setWindowFlags(Qt::FramelessWindowHint);
        this->show();
        ui->stayingOnTopButton->setText("False");
    } else {
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
        ui->stayingOnTopButton->setText("True");
    }
    staying_on_top=!staying_on_top;
}

void MainWindow::searchFinish() {
    if(ui->searchInput->text()=="") ui->searchInput->setText("Search...");
}

void MainWindow::searchStart() {
    if(ui->searchInput->text()=="Search...") ui->searchInput->setText("");
}

void MainWindow::showOptions() {
    ui->stackedWidget->setCurrentIndex(customEnum::optionPage);
}

void MainWindow::searchListAndShow() {
    CustomString searchWord = ui->searchInput->text();
    if(searchWord=="") {
        this->listWords();
        return;
    }
    if(searchWord=="Search...") {
        ui->searchInput->setText("");
    }
    ui->stackedWidget->setCurrentIndex(customEnum::wordListPage);
    ui->wordList->clear();
    for(Word* ptr=words_head;ptr!=nullptr;ptr=ptr->getNext()) {
        if(CustomString(ptr->getEnglish()).startsWith(static_cast<QString>(searchWord)) ||
                CustomString(ptr->getMeaning()).startsWith(static_cast<QString>(searchWord))) {
            QListWidgetItem *item= new QListWidgetItem(static_cast<QString>(ptr->getWordData()));
            item->setFont(QFont("微軟正黑體",14));
            ui->wordList->addItem(item);
        }
    }
}

void MainWindow::changeOrderMethod() {
    switch(order_method) {
    case customEnum::orderChronological:
        order_method=customEnum::orderRandom;
        ui->playingOrderButton->setText("Random");
        timer->disconnect();
        connect(timer,SIGNAL(timeout()),this,SLOT(showRandomWord()));
        break;
    case customEnum::orderRandom:
        order_method=customEnum::orderChronological;
        ui->playingOrderButton->setText("Chronological");
        timer->disconnect();
        connect(timer,SIGNAL(timeout()),this,SLOT(showNextWord()));
        break;
    }
}

void MainWindow::minimizeButtonClicked() {
    if(minimizing) {
        ui->listButton->show();
        ui->playButton->show();
        ui->strWord->show();
        ui->nextButton->show();
        ui->lastButton->show();
        ui->minimizeButton->setGeometry(610,90,ui->minimizeButton->width(),ui->minimizeButton->height());
        ui->closeButton->setGeometry(650,90,ui->closeButton->width(),ui->closeButton->height());
        this->setGeometry(this->x(),this->y(),692,131);
        setPreferredFontSize();
    } else {
        if(word_list_opening) listButtonClicked();
        ui->listButton->hide();
        ui->playButton->hide();
        ui->strWord->hide();
        ui->nextButton->hide();
        ui->lastButton->hide();
        ui->minimizeButton->setGeometry(10,10,ui->minimizeButton->width(),ui->minimizeButton->height());
        ui->closeButton->setGeometry(50,10,ui->closeButton->width(),ui->closeButton->height());
        this->setGeometry(this->x(),this->y(),91,51);
    }
    minimizing=!minimizing;
}

void MainWindow::deleteWordButtonClicked() {
    QString content("Delete: \""+(current_word->getWordData()+"\"?"));
    if(playing) timer->stop();
    QMessageBox::StandardButton confirming_box = QMessageBox::question(nullptr,"Confirming",content,QMessageBox::Yes|QMessageBox::No);
    if(confirming_box==QMessageBox::Yes) this->deleteWordConfirmed();
    if(playing) timer->start();
}

void MainWindow::deleteWordConfirmed() {
    for(int i=0;i<ui->wordList->count();++i) {
        if(ui->wordList->item(i)->text()==current_word->getWordData()) {
            words_file->deleteLine(ui->wordList->count()-i);
            QListWidgetItem *removed_item = ui->wordList->takeItem(i);
            delete removed_item;
            break;
        }
    }
    deleteCurrentWord(words_head,words_tail,current_word);
    showWord(current_word);
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
        //linked-list and set it to current word
        part.partOfSpeechSetting();
        for(Word* ptr=words_head;ptr!=nullptr;ptr=ptr->getNext())
            if(ptr->getEnglish()==english && ptr->getPart()==part)
                throw 2;
        current_word = pushNewWord(words_head,words_tail,english,part,meaning);
        this->connectWordToUI(current_word);
        showWord(current_word);
        //file
        english.replaceSpaceToUnderline();
        meaning.replaceSpaceToUnderline();
        words_file->prepToWrite();
        QTextStream out(words_file);
        out.setCodec("UTF-8");
        out.setGenerateByteOrderMark(false);
        out << english << ' ' << part << ' ' << meaning << '\n';
        words_file->close();
        //clear forms
        clearInputs();
    } catch(int x) {
        switch (x) {
        case 1:
            spawnWarningBox("You have to fill all 3 forms.");
            break;
        case 2:
            spawnWarningBox("This word with the same part of speech has existed.");
            break;
        default:
            break;
        }
    }
}

QMessageBox* MainWindow::spawnWarningBox(CustomString content) {
    QMessageBox *warning_box = new QMessageBox(QMessageBox::Icon::Critical,"Error",static_cast<QString>(content));
    warning_box->setWindowFlags(Qt::WindowStaysOnTopHint);
    warning_box->show();
    return warning_box;
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
    if(word==nullptr) {
        ui->strWord->setText("-");
        return;
    }
    word->getWordDataToUI(isOnlyWordShowing);
    current_word=word;
    setPreferredFontSize();
    if(playing) timer->start();
    //qDebug() << current_word->getWordData();
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
        setPreferredFontSize();
    }
}

void MainWindow::showLastWord() {
    if(current_word!=nullptr && current_word->getLast()!=nullptr) {
        showWord(current_word->getLast());
    } else if(words_tail!=nullptr) {
        showWord(words_tail);
    } else {
        ui->strWord->setText("-");
        setPreferredFontSize();
    }
}

void MainWindow::showRandomWord() {
    if(words_head==nullptr) {
        ui->strWord->setText("-");
        setPreferredFontSize();
        return;
    }
    Word* new_current_word;
    do {
        new_current_word = words_head->at(qrand()%Word::getCount());
    } while(current_word==new_current_word);
    current_word = new_current_word;
    showWord(current_word);
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
    case Qt::Key_X:
        qApp->quit();
        break;
    case Qt::Key_Space:
    case Qt::Key_P:
        playButtonClicked();
        break;
    case Qt::Key_L:
        listButtonClicked();
        break;
    case Qt::Key_Minus:
    case Qt::Key_M:
        minimizeButtonClicked();
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    options_file->clearToWrite();
    QTextStream temp(options_file);
    temp << "position" << ' ' << this->x() << ' ' << this->y() <<endl;
    temp << "order" << ' ' << this->order_method <<endl;
    temp << "interval" << ' ' << ui->intervalSlider->value() <<endl;
    if(!playing) temp << "pause" <<endl;
    if(staying_on_top) temp << "stayontop" <<endl;
    unsigned lastword;
    bool found=false;
    for(unsigned i=0;i<Word::getCount();++i) {
        if(words_head->at(i)==current_word) {
            lastword=i;
            found=true;
            break;
        }
    }
    if(found) temp << "lastword" << ' ' << lastword <<endl;
    options_file->close();
}
