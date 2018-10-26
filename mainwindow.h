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
    enum pageNumber{wordListPage,wordInputPage};
}

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
    void showNextWord();
    void showLastWord();
    void showWord(Word* word);
    void listWords();
    void submitInput();
    void clearInputs();
    void prepToAddNewWord();
    void playButtonClicked();
    void listButtonClicked();
    void deleteWordButtonClicked();
    void FindItemAndShowWord(QListWidgetItem* clicked_item);
private:
    Ui::MainWindow *ui;
    WordsFile *file;
    Word *words_head, *current_word, *words_tail;
    QTimer* timer;
    bool playing, word_list_opening;
};

#endif // MAINWINDOW_H
