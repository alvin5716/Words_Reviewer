#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "wordsfile.h"
#include "word.h"
#include "customstring.h"
#include "customtimer.h"
#include <QMessageBox>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

namespace customEnum {
    enum pageNumber{wordListPage,wordInputPage,optionPage};
    enum orderMethod{orderChronological,orderRandom};
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
    void wordListOpen();
    void wordListClose();
    void initSettings();
    void deleteWordConfirmed();
    QMessageBox* spawnWarningBox(CustomString content);
    void spawnConfirmingBox(CustomString content, YesNoFunc yesFunc, YesNoFunc noFunc=nullptr);
    ~MainWindow();
public slots:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void showWord(Word* word);
    void showNextWord();
    void showLastWord();
    void showRandomWord();
    void listWords();
    void submitInput();
    void clearInputs();
    void changeOrderMethod();
    void prepToAddNewWord();
    void showOptions();
    void playButtonClicked();
    void listButtonClicked();
    void minimizeButtonClicked();
    void deleteWordButtonClicked();
    void stayOnTopButtonClick();
    void searchListAndShow();
    void searchFinish();
    void searchStart();
    void toggleOnlyWord();
    void searchOnNet();
    void intervalShow(int interval);
    void FindItemAndShowWord(QListWidgetItem* clicked_item);
private:
    Ui::MainWindow *ui;
    WordsFile *words_file, *options_file;
    Word *words_head, *current_word, *words_tail;
    CustomTimer* timer;
    bool playing, word_list_opening, minimizing, staying_on_top;
    bool isOnlyWordShowing;
    QPoint dragStartPos;
    customEnum::orderMethod order_method;
};

#endif // MAINWINDOW_H
