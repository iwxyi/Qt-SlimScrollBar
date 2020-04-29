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

    void paintScrollBar(QPainter& painter, QSize size);

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

signals:
    void signalRepaint();

public slots:

private slots:

private:
    QPixmap pixmap;

    QColor bg_normal_color = QColor(128, 128, 128, 64);
    QColor bg_hover_color = QColor(128, 128, 128, 128);
    QColor bg_press_color = QColor(128, 128, 128, 192);
    QColor fg_normal_color = QColor(66, 103, 124, 128);
    QColor fg_hover_color = QColor(66, 103, 124, 192);
    QColor fg_press_color = QColor(66, 103, 124, 255);

    bool hovering = false;
    bool pressing = false;
};

#endif // SLIMSCROLLBAR_H
