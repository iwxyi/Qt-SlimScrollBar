#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include "slimscrollbar.h"

class MyTextEdit : public QTextEdit
{
public:
    MyTextEdit(QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent *e) override;

private:
    SlimScrollBar* slim;
};

#endif // MYTEXTEDIT_H
