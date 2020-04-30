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
#include "slimscrollbarpopup.h"

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
    void setOffsetPoss();
    int quick_sqrt(long X) const;
    void calcPixmapSize();
    void startPopup();
    void repaintPopup();

signals:

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
    QPoint press_pos;  // 按下位置
    QPoint mouse_pos;  // 鼠标位置
    QPoint target_pos; // 实时随队鼠标的目标点（相对竖直中心）
    QPoint anchor_pos; // 逐步靠近目标点的锚点
    QPoint effect_pos; // 偏差（相对于左上角），逐步靠近锚点根号位置

    bool popuping = false;
    SlimScrollBarPopup* popup = nullptr;
    QPoint popup_offset; // 弹窗和自己的左上角的绝对位置差
};

#endif // SLIMSCROLLBAR_H
