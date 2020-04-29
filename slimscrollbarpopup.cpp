#include "slimscrollbarpopup.h"

SlimScrollBarPopup::SlimScrollBarPopup(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TransparentForMouseEvents,true);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}
