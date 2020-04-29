#include "mytextedit.h"

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent)
{
    slim = new SlimScrollBar(this->parentWidget());
    setVerticalScrollBar(slim);
    connect(slim, &SlimScrollBar::signalRepaint, this, [=]() {
        // repaint(); // 不能用这句话，重绘进度条时再重绘自己，会导致递归
        update();
    });
}

void MyTextEdit::paintEvent(QPaintEvent *e)
{
    QTextEdit::paintEvent(e);

    QPainter painter(this->viewport());
    slim->paintScrollBar(painter, size());
}
