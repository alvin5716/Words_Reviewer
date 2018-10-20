#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "wordsfile.h"
#include "word.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void connectWordToUI(Word* word);
    ~MainWindow();
public slots:
    void showNextWord();
private:
    Ui::MainWindow *ui;
    WordsFile *file;
    Word *words_head, *current_word;
    QTimer* timer;
};

#endif // MAINWINDOW_H
