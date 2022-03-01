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
    words(new WordGroup),
    playing(true),more_tab_opening(false), minimizing(false), staying_on_top(false),
    word_mode(WordShowingMode::AllShowing),
    dragStartPos(QPoint(0,0)),
    order_method(AppOption::OrderChronological)
{
    ui->setupUi(this);
    //widget settings
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(QApplication::desktop()->screenGeometry().width()/2-346,0,692,131);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFocus();
    moreTabClose();
    ui->stackedWidget->setCurrentIndex(CustomEnum::MainPage);
    //sliders
    ui->intervalSlider->setRange(1,60);
    ui->intervalSlider->setValue(20);
    connect(ui->intervalSlider,SIGNAL(valueChanged(int)),this,SLOT(intervalShowOnLabel(int)));
    connect(ui->intervalSlider,SIGNAL(actionTriggered(int)),this,SLOT(setFocus()));
    connect(ui->intervalSlider,SIGNAL(sliderPressed()),this,SLOT(setFocus()));
    //buttons
    connect(ui->closeButton,SIGNAL(clicked(bool)),this,SLOT(closeButtonClicked()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(showNextWord()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(showLastWord()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(playButtonClicked()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->moreButton,SIGNAL(clicked(bool)),this,SLOT(moreButtonClicked()));
    connect(ui->moreButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(listButtonClicked()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->wordList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(FindItemAndShowWord(QListWidgetItem*)));
    connect(ui->addNewWordButton,SIGNAL(clicked(bool)),this,SLOT(prepToAddNewWord()));
    connect(ui->backInputButton,SIGNAL(clicked(bool)),this,SLOT(listButtonClicked()));
    connect(ui->enterInputButton,SIGNAL(clicked(bool)),this,SLOT(submitInput()));
    connect(ui->clearInputButton,SIGNAL(clicked(bool)),this,SLOT(clearInputs()));
    connect(ui->deleteWordButton,SIGNAL(clicked(bool)),this,SLOT(deleteWordButtonClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked(bool)),this,SLOT(minimizeButtonClicked()));
    connect(ui->minimizeButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->showingModeButton,SIGNAL(clicked(bool)),this,SLOT(changeWordMode()));
    connect(ui->searchOnNetButton,SIGNAL(clicked(bool)),this,SLOT(searchOnNet()));
    connect(ui->optionsButton,SIGNAL(clicked(bool)),this,SLOT(showOptions()));
    connect(ui->backOptionButton,SIGNAL(clicked(bool)),this,SLOT(backToMainPage()));
    connect(ui->backWordEditButton,SIGNAL(clicked(bool)),this,SLOT(backToMainPage()));
    connect(ui->playingOrderButton,SIGNAL(clicked(bool)),this,SLOT(nextOrderMethod()));
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
        ui->moreButton, ui->showingModeButton, ui->closeButton, ui->minimizeButton,
        ui->nextButton, ui->lastButton, ui->playButton,
        ui->listButton, ui->searchOnNetButton, ui->optionsButton,
        ui->enterInputButton, ui->clearInputButton, ui->backInputButton,
        ui->addNewWordButton, ui->deleteWordButton, ui->backWordEditButton,
        ui->stayingOnTopButton, ui->playingOrderButton, ui->intervalSlider, ui->backOptionButton,

    });
    for(QWidget* w: pointerCursorWdgts)
        w->setCursor(Qt::PointingHandCursor);
}

void MainWindow::listButtonClicked() {
    ui->stackedWidget->setCurrentIndex(CustomEnum::WordListPage);
    ui->wordList->clear();
    words->searchWordStart();
    const Word* searched_word = words->searchWord();
    while(searched_word != nullptr) {
        QListWidgetItem *item= new QListWidgetItem(static_cast<QString>(searched_word->getWordData()));
        item->setFont(QFont("微軟正黑體",14));
        ui->wordList->addItem(item);
        searched_word = words->searchWord();
    }
}

void MainWindow::searchOnNet() {
    const Word* current_word = words->getCurrentWord();
    if(!current_word) return;
    QString url_str("https://dictionary.cambridge.org/zht/%E8%A9%9E%E5%85%B8/%E8%8B%B1%E8%AA%9E-%E6%BC%A2%E8%AA%9E-%E7%B9%81%E9%AB%94/"
                    +current_word->getEnglish()
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


void MainWindow::changeWordMode() {
    this->word_mode = WordShowingMode::nextMode(word_mode);
    showCurrentWord();
}

void MainWindow::initSettings() {
    //read word file
    bool isReadSuccessed = words_file->readWordFile(words);
    if(!isReadSuccessed) qApp->quit();
    //timer
    timer = new CustomTimer;
    timer->setInterval(20000);
    connect(timer,&CustomTimer::timeout,this,&MainWindow::showNextWord);
    timer->start();
    connect(ui->intervalSlider,SIGNAL(valueChanged(int)),timer,SLOT(restartWithInterval(int)));
    //read options file
    AppOption option = options_file->readOptionFile();
    this->setGeometry(option.position_x, option.position_y, this->width(), this->height());
    this->orderMethodSetAndShow(option.order);
    words->switchToWordByIndex(option.lastword);
    if(!option.playing) this->playButtonClicked();
    if(option.staying_on_top) this->stayOnTopButtonClick();
    intervalSetAndShow(option.interval);
    //show word
    connect(words,SIGNAL(giveWordData(QString)),ui->strWord,SLOT(setText(QString)));
    showCurrentWord();
}

void MainWindow::intervalSetAndShow(int interval) {
    ui->intervalSlider->setValue(interval);
    timer->restartWithInterval(interval);
    this->intervalShowOnLabel(interval);
}

void MainWindow::intervalShowOnLabel(int interval) {
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
    ui->stackedWidget->setCurrentIndex(CustomEnum::OptionPage);
}

void MainWindow::searchListAndShow() {
    CustomString searchWord = ui->searchInput->text();
    ui->stackedWidget->setCurrentIndex(CustomEnum::WordListPage);
    ui->wordList->clear();
    words->searchWordStart(searchWord);
    const Word* searched_word = words->searchWord();
    while(searched_word != nullptr) {
        QListWidgetItem *item= new QListWidgetItem(static_cast<QString>(searched_word->getWordData()));
        item->setFont(QFont("微軟正黑體",14));
        ui->wordList->addItem(item);
        searched_word = words->searchWord();
    }
}

void MainWindow::nextOrderMethod() {
    AppOption::OrderMethod new_method = AppOption::nextMethod(this->order_method);
    orderMethodSetAndShow(new_method);
}

void MainWindow::orderMethodSetAndShow(AppOption::OrderMethod order_method) {
    this->order_method = order_method;
    switch(order_method) {
    case AppOption::OrderChronological:
        ui->playingOrderButton->setText("Chronological");
        timer->disconnect();
        connect(timer,SIGNAL(timeout()),this,SLOT(showNextWord()));
        break;
    case AppOption::OrderRandom:
        ui->playingOrderButton->setText("Random");
        timer->disconnect();
        connect(timer,SIGNAL(timeout()),this,SLOT(showRandomWord()));
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
        if(more_tab_opening) moreButtonClicked();
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
    QString content("Delete: \""+(words->getCurrentWord()->getWordData()+"\"?"));
    if(playing) timer->stop();
    QMessageBox::StandardButton confirming_box = QMessageBox::question(nullptr,"Confirming",content,QMessageBox::Yes|QMessageBox::No);
    if(confirming_box==QMessageBox::Yes) this->deleteWordConfirmed();
    if(playing) timer->start();
}

void MainWindow::deleteWordConfirmed() {
    unsigned int i = words->getCurrentIndex();
    words_file->deleteLine(i+1);
    QListWidgetItem *removed_item = ui->wordList->takeItem(i);
    delete removed_item;
    words->deleteCurrentWord();
    showCurrentWord();
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
        if(words->isWordExisted(english,part)) throw 2;
        words->pushNewWord(english,part,meaning);
        showCurrentWord();
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
    ui->stackedWidget->setCurrentIndex(CustomEnum::WordInputPage);
}

void MainWindow::FindItemAndShowWord(QListWidgetItem* clicked_item) {
    words->switchToWordByData(clicked_item->text());
    showCurrentWord();
}

void MainWindow::showCurrentWord() {
    words->getWordDataToUI(word_mode);
    setPreferredFontSize();
    if(playing) timer->start();
}

void MainWindow::closeButtonClicked() {
    QString content("Do you really want to close?");
    QMessageBox::StandardButton confirming_box = QMessageBox::question(nullptr,"Confirming",content,QMessageBox::Yes|QMessageBox::No);
    if(confirming_box==QMessageBox::Yes) qApp->quit();
}

void MainWindow::playButtonClicked() {
    if(this->playing) {
        timer->stop();
        ui->playButton->setText("Paused");
    } else {
        timer->start();
        ui->playButton->setText("Playing");
    }
    playing=!playing;
}

void MainWindow::moreButtonClicked() {
    if(this->more_tab_opening) {
        moreTabClose();
    } else {
        moreTabOpen();
    }
    more_tab_opening=!more_tab_opening;
}

void MainWindow::backToMainPage() {
    ui->stackedWidget->setCurrentIndex(CustomEnum::MainPage);
}

void MainWindow::moreTabOpen() {
    ui->moreButton->setText("Close");
    ui->stackedWidget->show();
    this->setGeometry(this->geometry().x(),this->geometry().y(),this->geometry().width(),561);
}

void MainWindow::moreTabClose() {
    ui->moreButton->setText("More");
    ui->stackedWidget->hide();
    this->setGeometry(this->geometry().x(),this->geometry().y(),this->geometry().width(),131);
}

void MainWindow::showNextWord() {
    words->switchToNextWord();
    showCurrentWord();
}

void MainWindow::showLastWord() {
    words->switchToLastWord();
    showCurrentWord();
}

void MainWindow::showRandomWord() {
    unsigned int num = words->getWordCount();
    unsigned int current_index = words->getCurrentIndex();
    unsigned int new_index;
    if(num <= 1) return;
    new_index = qrand()%(num-1);
    if(new_index >= current_index) ++new_index;
    words->switchToWordByIndex(new_index);
    showCurrentWord();
}

void MainWindow::setPreferredFontSize() {
    int size=0.9*this->geometry().width()/ui->strWord->text().length();
    if(size<10) size=10;
    else if(size>30) size=30;
    ui->strWord->setFont(QFont("微軟正黑體",size));
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Left:
        this->showLastWord();
        break;
    case Qt::Key_Right:
        this->showNextWord();
        break;
    case Qt::Key_Space:
        playButtonClicked();
        break;
    default:
        break;
    }
}

MainWindow::~MainWindow()
{
    options_file->clearToWrite();
    QTextStream temp(options_file);
    temp << "position" << ' ' << this->x() << ' ' << this->y() <<endl;
    temp << "order" << ' ' << static_cast<int>(this->order_method) <<endl;
    temp << "interval" << ' ' << ui->intervalSlider->value() <<endl;
    if(!playing) temp << "pause" <<endl;
    if(staying_on_top) temp << "stayontop" <<endl;
    temp << "lastword" << ' ' << words->getCurrentIndex() <<endl;
    options_file->close();
    delete options_file;

    words_file->close();
    delete words_file;
    delete words;
    delete timer;
    delete ui;
}
