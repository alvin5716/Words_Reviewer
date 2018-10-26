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
signals:
    void selectLastOne();
    void selectNextOne();
    void enterInputs();
};

#endif // INPUTFORM_H
