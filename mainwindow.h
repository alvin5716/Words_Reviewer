#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "wordsfile.h"
#include "word.h"
#include "wordgroup.h"
#include "wordshowingmode.h"
#include "customstring.h"
#include "customtimer.h"
#include "appoption.h"
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

namespace CustomEnum {
    enum PageNumber{MainPage,WordListPage,WordInputPage,OptionPage};
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef void (MainWindow::*YesNoFunc)();
    explicit MainWindow(QWidget *parent = 0);
    void connectWordToUI(Word* word);
    void keyPressEvent(QKeyEvent *e) override;
    void setPreferredFontSize();
    void moreTabOpen();
    void moreTabClose();
    void initSettings();
    void deleteWordConfirmed();
    void listWords();
    void orderMethodSetAndShow(AppOption::OrderMethod order_method);
    QMessageBox* spawnWarningBox(CustomString content);
    void spawnConfirmingBox(CustomString content, YesNoFunc yesFunc, YesNoFunc noFunc=nullptr);
    ~MainWindow();
public slots:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void showCurrentWord();
    void showNextWord();
    void showLastWord();
    void showRandomWord();
    void submitInput();
    void clearInputs();
    void nextOrderMethod();
    void prepToAddNewWord();
    void showOptions();
    void closeButtonClicked();
    void playButtonClicked();
    void moreButtonClicked();
    void listButtonClicked();
    void minimizeButtonClicked();
    void deleteWordButtonClicked();
    void stayOnTopButtonClick();
    void searchListAndShow();
    void searchFinish();
    void searchStart();
    void changeWordMode();
    void searchOnNet();
    void backToMainPage();
    void refreshWords();
    void listAllWords();
    void intervalSetAndShow(int interval);
    void intervalShowOnLabel(int interval);
    void FindItemAndShowWord(QListWidgetItem* clicked_item);
private:
    Ui::MainWindow *ui;
    WordsFile *words_file, *options_file;
    WordGroup *words;
    CustomTimer* timer;
    bool playing, more_tab_opening, minimizing, staying_on_top;
    WordShowingMode::Mode word_mode;
    QPoint dragStartPos;
    AppOption::OrderMethod order_method;
};

#endif // MAINWINDOW_H
