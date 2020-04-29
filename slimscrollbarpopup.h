#ifndef SLIMSCROLLBARPOPUP_H
#define SLIMSCROLLBARPOPUP_H

#include <QObject>
#include <QWidget>

class SlimScrollBarPopup : public QWidget
{
    Q_OBJECT
    friend class SlimScrollBar;
public:
    SlimScrollBarPopup(QWidget *parent = nullptr);

signals:

public slots:

private:
    QPixmap* pixmap = nullptr;
    QPoint offset;
};

#endif // SLIMSCROLLBARPOPUP_H
