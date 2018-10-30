#include "inputform.h"

InputForm::InputForm(QWidget *parent)
    :QLineEdit(parent)
{

}

void InputForm::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Up:
        emit selectLastOne();
        break;
    case Qt::Key_Return:
        emit enterInputs();
    case Qt::Key_Down:
        emit selectNextOne();
        break;
    default:
        QLineEdit::keyPressEvent(e);
        break;
    }
}

void InputForm::mousePressEvent(QMouseEvent *e) {
    emit pressed();
    QLineEdit::mousePressEvent(e);
}
