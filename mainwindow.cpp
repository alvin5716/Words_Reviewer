#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctime>
#include <cstdlib>
#include <QDebug>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDesktopServices>
#include "windowsettings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    words_file(new WordsFile("words.txt")), options_file(new WordsFile("options.txt")),
    words(new WordGroup),
    playing(true),more_tab_opening(false), minimizing(false), staying_on_top(false),
    word_mode(WordShowingMode::AllShowing),
    dragStartPos(QPoint(0,0)),
    order_method(AppOption::OrderChronological),
    isEditing(false)
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
    connect(ui->intervalSlider,SIGNAL(actionTriggered(int)),this,SLOT(setFocus()));
    connect(ui->intervalSlider,SIGNAL(sliderPressed()),this,SLOT(setFocus()));
    //buttons
    connect(ui->closeButton,SIGNAL(clicked(bool)),this,SLOT(closeButtonClicked()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(nextButtonClicked()));
    connect(ui->nextButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(lastButtonClicked()));
    connect(ui->lastButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(playButtonClicked()));
    connect(ui->playButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->moreButton,SIGNAL(clicked(bool)),this,SLOT(moreButtonClicked()));
    connect(ui->moreButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(listButtonClicked()));
    connect(ui->listButton,SIGNAL(clicked(bool)),this,SLOT(setFocus()));
    connect(ui->wordList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(FindItemAndShowWord(QListWidgetItem*)));
    connect(ui->addNewWordButton,SIGNAL(clicked(bool)),this,SLOT(prepToAddNewWord()));
    connect(ui->backInputButton,SIGNAL(clicked(bool)),this,SLOT(backInputButtonClicked()));
    connect(ui->editWordButton,SIGNAL(clicked(bool)),this,SLOT(prepToEditNewWord()));
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
    connect(ui->refreshButton,SIGNAL(clicked(bool)),this,SLOT(refreshWords()));
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
        ui->listButton, ui->GroupButton, ui->optionsButton, ui->refreshButton,
        ui->enterInputButton, ui->clearInputButton, ui->backInputButton,
        ui->addNewWordButton, ui->deleteWordButton, ui->editWordButton, ui->searchOnNetButton, ui->backWordEditButton,
        ui->stayingOnTopButton, ui->playingOrderButton, ui->intervalSlider, ui->backOptionButton,
    });
    for(QWidget* w: pointerCursorWdgts)
        w->setCursor(Qt::PointingHandCursor);
}

void MainWindow::nextButtonClicked() {
    if(this->isEditing) return;
    showNextWord();
}

void MainWindow::lastButtonClicked() {
    if(this->isEditing) return;
    showLastWord();
}

void MainWindow::refreshWords() {
    words->deleteWholeList();
    words_file->readWordFile(words);
    showCurrentWord();
}

void MainWindow::backInputButtonClicked() {
    if(this->isEditing) leaveEditingMode();
    listButtonClicked();
}

void MainWindow::leaveEditingMode() {
    if(!this->isEditing) return;
    this->isEditing = false;
    if(playing) {
        timer->start();
        ui->playButton->setText("Playing");
    } else {
        ui->playButton->setText("Paused");
    }
    ui->playButton->setStyleSheet("");
}

void MainWindow::listButtonClicked() {
    ui->stackedWidget->setCurrentIndex(CustomEnum::WordListPage);
    listAllWords();
}

void MainWindow::listAllWords() {
    ui->wordList->clear();
    words->searchWordStart();
    const Word* searched_word = words->searchWord();
    const Word* selected_word = words->getSelectedWord();
    while(searched_word != nullptr) {
        QListWidgetItem *item= new QListWidgetItem(static_cast<QString>(searched_word->getWordData()));
        item->setFont(QFont("微軟正黑體",14));
        ui->wordList->addItem(item);
        if(searched_word == selected_word) item->setSelected(true);
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

void MainWindow::moveInScreenRange(int x, int y) {
    const QRect sc = QApplication::desktop()->screenGeometry();
    int sc_w = sc.width(), sc_h = sc.height();
    int w = this->width(), h = this->height();

    int max_x = sc_w - 2*w/3, max_y = sc_h - 2*h/3;
    int min_x = - w/3, min_y = - h/3;

    int new_x, new_y;

    if(x >= max_x) new_x = max_x;
    else if(x <= min_x) new_x = min_x;
    else new_x = x;

    if(y >= max_y) new_y = max_y;
    else if(y <= min_y) new_y = min_y;
    else new_y = y;

    this->move(new_x, new_y);
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    this->dragStartPos = e->globalPos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    const QPoint distant = e->globalPos() - this->dragStartPos;
    this->moveInScreenRange(this->x()+distant.x(), this->y()+distant.y());
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
    connect(ui->intervalSlider,SIGNAL(valueChanged(int)),this,SLOT(intervalSetAndShow(int)));
    //read options file
    AppOption option = options_file->readOptionFile();
    this->moveInScreenRange(option.position_x, option.position_y);
    this->orderMethodSetAndShow(option.order);
    words->switchToWordByIndex(option.lastword);
    if(option.staying_on_top) this->stayOnTopButtonClick();
    intervalSetAndShow(option.interval);
    if(!option.playing) this->playButtonClicked();
    //show word
    connect(words,SIGNAL(giveWordData(QString)),ui->strWord,SLOT(setText(QString)));
    showCurrentWord();
}

void MainWindow::intervalSetAndShow(int interval) {
    ui->intervalSlider->setValue(interval);
    timer->setIntervalSec(interval);
    if(playing) timer->start();
    ui->strIntervalSecond->setText(CustomString().setNum(interval)+" sec");
}

void MainWindow::stayOnTopButtonClick() {
    if(this->staying_on_top) {
        showOffTop(this);
        ui->stayingOnTopButton->setText("False");
    } else {
        showOnTop(this);
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
        this->resize(692,131);
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
        this->resize(91,51);
    }
    minimizing=!minimizing;
}

void MainWindow::deleteWordButtonClicked() {
    QString content("Delete: \""+(words->getSelectedWord()->getWordData()+"\"?"));
    bool reply = spawnConfirmingBox(content);
    if(reply) this->deleteWordConfirmed();
}

void MainWindow::deleteWordConfirmed() {
    if(words->getSelectedWord() == nullptr) words->selectCurrentWord();
    unsigned int i = words->getSelectedIndex();
    words_file->deleteLine(i+1, true);
    QListWidgetItem *removed_item = ui->wordList->takeItem(i);
    delete removed_item;
    words->deleteSelectedWord();
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
        if(this->isEditing) {
            //linked-list and set it to current word
            part.partOfSpeechSetting();
            words->editSelectedWord(english,part,meaning);
            showCurrentWord();
            //file
            unsigned int line = words->getSelectedIndex()+1;
            english.replaceSpaceToUnderline();
            meaning.replaceSpaceToUnderline();
            CustomString str = english + ' ' + part + ' ' + meaning;
            words_file->editLine(line, str, true);
            //leave from editing mode
            leaveEditingMode();
            //go back to list
            listButtonClicked();
        } else {
            //linked-list and set it to current word
            part.partOfSpeechSetting();
            if(words->isWordExisted(english,part)) throw 2;
            words->insertNewWord(english,part,meaning);
            showCurrentWord();
            //file
            unsigned int line = words->getSelectedIndex()+1;
            english.replaceSpaceToUnderline();
            meaning.replaceSpaceToUnderline();
            CustomString str = english + ' ' + part + ' ' + meaning;
            words_file->insertLine(line, str, true);
        }
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

void MainWindow::spawnWarningBox(CustomString content) {
    if(this->staying_on_top) showOffTop(this);
    QMessageBox *warning_box = new QMessageBox(QMessageBox::Icon::Critical,"Error",static_cast<QString>(content));
    warning_box->setAttribute(Qt::WA_DeleteOnClose, true);
    warning_box->exec();
    if(this->staying_on_top) showOnTop(this);
}

bool MainWindow::spawnConfirmingBox(CustomString content) {
    if(this->staying_on_top) showOffTop(this);
    QMessageBox *confirming_box = new QMessageBox(QMessageBox::Icon::Question,"Confirming",content,QMessageBox::Yes|QMessageBox::No);
    confirming_box->setAttribute(Qt::WA_DeleteOnClose, true);
    QMessageBox::StandardButton reply = static_cast<QMessageBox::StandardButton>(confirming_box->exec());
    if(this->staying_on_top) showOnTop(this);
    return (reply == QMessageBox::Yes);
}

void MainWindow::prepToAddNewWord() {
    this->isEditing = false;
    if(words->getSelectedWord() == nullptr) words->selectCurrentWord();

    ui->stackedWidget->setCurrentIndex(CustomEnum::WordInputPage);
    clearInputs();
}

void MainWindow::prepToEditNewWord() {
    if(words->getWordCount() == 0) {
        spawnWarningBox("No word exists in this group.");
        return;
    }

    this->isEditing = true;
    if(words->getSelectedWord() == nullptr) words->selectCurrentWord();
    else if(words->getSelectedWord() != words->getCurrentWord()) words->setCurrentToSelected();
    showCurrentWord();

    if(playing) timer->stop();
    ui->playButton->setText("Editing");
    ui->playButton->setStyleSheet("*{background-color: #EF0000}");

    ui->stackedWidget->setCurrentIndex(CustomEnum::WordInputPage);
    ui->englishInput->setText(words->getSelectedWord()->getEnglish());
    ui->partInput->setText(words->getSelectedWord()->getPart());
    ui->meaningInput->setText(words->getSelectedWord()->getMeaning());
}

void MainWindow::FindItemAndShowWord(QListWidgetItem* clicked_item) {
    words->switchToWordByData(clicked_item->text());
    words->selectCurrentWord();
    showCurrentWord();
}

void MainWindow::showCurrentWord() {
    words->getWordDataToUI(word_mode);
    setPreferredFontSize();
    if(playing) timer->start();
}

void MainWindow::closeButtonClicked() {
    QString content("Do you really want to close?");
    bool reply = spawnConfirmingBox(content);
    if(reply) qApp->quit();
}

void MainWindow::playButtonClicked() {
    if(this->isEditing) return; //disabled while editing

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
    this->resize(this->width(),561);
}

void MainWindow::moreTabClose() {
    ui->moreButton->setText("More");
    ui->stackedWidget->hide();
    this->resize(this->width(),131);
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
    int size=0.9*this->width()/ui->strWord->text().length();
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
