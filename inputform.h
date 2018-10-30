#ifndef INPUTFORM_H
#define INPUTFORM_H

#include <QLineEdit>
#include <QKeyEvent>

class InputForm : public QLineEdit
{
    Q_OBJECT
public:
    InputForm(QWidget *parent=nullptr);
    void keyPressEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
signals:
    void selectLastOne();
    void selectNextOne();
    void enterInputs();
    void pressed();
};

#endif // INPUTFORM_H
