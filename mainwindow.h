#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "wordsfile.h"
#include "word.h"
#include "customstring.h"
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

namespace customEnum {
    enum pageNumber{wordListPage,wordInputPage,optionPage};
}
enum class OrderMethod{chronological,random};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectWordToUI(Word* word);
    void keyPressEvent(QKeyEvent *e);
    void setPreferredFontSize();
    void wordListOpen();
    void wordListClose();
    void spawnWarningBox(CustomString content);
    void initSettings();
    ~MainWindow();
public slots:
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
    void FindItemAndShowWord(QListWidgetItem* clicked_item);
private:
    Ui::MainWindow *ui;
    WordsFile *file;
    Word *words_head, *current_word, *words_tail;
    QTimer* timer;
    bool playing, word_list_opening, minimizing;
    OrderMethod order_method;
};

#endif // MAINWINDOW_H
