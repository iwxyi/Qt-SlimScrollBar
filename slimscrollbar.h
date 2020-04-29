#ifndef SLIMSCROLLBAR_H
#define SLIMSCROLLBAR_H

#include <QObject>
#include <QScrollBar>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPainterPath>
#include <QDebug>

#define DEB_EVENT if (0) qDebug()

class SlimScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    SlimScrollBar(QWidget* parent = nullptr);

protected:
    void enterEvent(QEvent* e) override;
    void leaveEvent(QEvent* e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void wheelEvent(QWheelEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    void sliderChange(SliderChange change) override;
    void contextMenuEvent(QContextMenuEvent *e) override;
    void hideEvent(QHideEvent *e) override;
    void resizeEvent(QResizeEvent *e) override;

private:

signals:
    void signalRepaint(QPixmap* pixmap);

public slots:

private slots:

private:
    QPixmap pixmap;

};

#endif // SLIMSCROLLBAR_H
