#ifndef SLIMSCROLLBAR_H
#define SLIMSCROLLBAR_H

#include <QObject>
#include <QScrollBar>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QDebug>

#define DEB_EVENT if (0) qDebug()

class SlimScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    SlimScrollBar(QWidget* parent = nullptr);

    void paintScrollBar(QPainter& painter, QSize size);

    void enable();
    void disable();

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
    void paintPixmap();
    void activeTimer();

signals:
    void signalRepaint();

public slots:

private slots:
    void eventTimer();

private:
    QPixmap pixmap;
    QTimer* event_timer;

    QColor bg_normal_color = QColor(128, 128, 128, 64);
    QColor bg_hover_color = QColor(128, 128, 128, 0);
    QColor bg_press_color = QColor(128, 128, 128, 32);
    QColor fg_normal_color = QColor(66, 103, 124, 64);
    QColor fg_hover_color = QColor(66, 103, 124, 128);
    QColor fg_press_color = QColor(66, 103, 124, 192);

    bool enabling = true;
    bool hovering = false;
    bool pressing = false;
    int hover_prop = 0;
    int press_prop = 0;
    QPoint press_pos;
    QPoint cursor_pos;
    QPoint offset_pos;
};

#endif // SLIMSCROLLBAR_H
