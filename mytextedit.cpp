#include "mytextedit.h"

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{
    slim = new SlimScrollBar(this->parentWidget());
    setVerticalScrollBar(slim);
}

void MyTextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);
}
