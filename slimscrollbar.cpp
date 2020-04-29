#include "slimscrollbar.h"

SlimScrollBar::SlimScrollBar(QWidget *parent) : QScrollBar(parent)
{
    setStyleSheet("\
            QScrollBar:vertical\
            {\
                width:12px;\
                background:rgba(0,0,0,0%);\
                margin:0px,0px,0px,0px;\
                /*留出9px给上面和下面的箭头*/\
                padding-top:0px;\
                padding-bottom:0px;\
            }\
            QScrollBar::handle:vertical\
            {\
                width:12px;\
                background:rgba(0,0,0,0%);\
                /*滚动条两端变成椭圆*/\
                border-radius:0px;\
                min-height:20;\
            }\
            QScrollBar::handle:vertical:hover\
            {\
                width:8px;\
                /*鼠标放到滚动条上的时候，颜色变深*/\
                background:rgba(0,0,0,0%);\
                border-radius:0px;\
                min-height:20;\
            }\
            /*这个应该是设置下箭头的，3.png就是箭头*/\
            QScrollBar::add-line:vertical\
            {\
                height:0px;width:0px;\
                border-image:url(:/images/a/3.png);\
                subcontrol-position:bottom;\
            }\
            /*设置上箭头*/\
            QScrollBar::sub-line:vertical\
            {\
                height:0px;width:0px;\
                border-image:url(:/images/a/1.png);\
                subcontrol-position:top;\
            }\
            /*当鼠标放到下箭头上的时候*/\
            QScrollBar::add-line:vertical:hover\
            {\
                height:0px;width:0px;\
                border-image:url(:/images/a/4.png);\
                subcontrol-position:bottom;\
            }\
            /*当鼠标放到下箭头上的时候*/\
            QScrollBar::sub-line:vertical:hover\
            {\
                height:0px;width:0px;\
                border-image:url(:/images/a/2.png);\
                subcontrol-position:top;\
            }\
            /*当滚动条滚动的时候，上面的部分和下面的部分*/\
            QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical\
            {\
                background:rgba(0,0,0,0%);\
                border-radius:0px;\
            }");
}

void SlimScrollBar::enterEvent(QEvent *e)
{
    DEB_EVENT << "enterEvent" << e;
    QScrollBar::enterEvent(e);
}

void SlimScrollBar::leaveEvent(QEvent *e)
{
    DEB_EVENT << "leaveEvent" << e;
    QScrollBar::leaveEvent(e);
}

void SlimScrollBar::mousePressEvent(QMouseEvent *e)
{
    DEB_EVENT << "mousePressEvent" << e;
    QScrollBar::mousePressEvent(e);
}

void SlimScrollBar::mouseMoveEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseMoveEvent" << e;
    QScrollBar::mouseMoveEvent(e);
}

void SlimScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseReleaseEvent" << e;
    QScrollBar::mouseReleaseEvent(e);
}

void SlimScrollBar::wheelEvent(QWheelEvent *e)
{
    DEB_EVENT << "wheelEvent" << e;
    QScrollBar::wheelEvent(e);
}

void SlimScrollBar::paintEvent(QPaintEvent *e)
{
    // 这里是透明度，必须得绘制（不然是纯黑色）
    QScrollBar::paintEvent(e);

    // 绘制自己的控件

    emit signalRepaint(&pixmap);
}

/**
 * 开始加载、调整大小的时候都会触发
 * - 0 范围改变
 * - 1 方向改变
 * - 2 步长改变
 * - 3 进度改变
 */
void SlimScrollBar::sliderChange(QAbstractSlider::SliderChange change)
{
    DEB_EVENT << "sliderChange" << change;
    QScrollBar::sliderChange(change);
}

void SlimScrollBar::contextMenuEvent(QContextMenuEvent *e)
{
    DEB_EVENT << "contextMenuEvent" << e;
    QScrollBar::contextMenuEvent(e);
}

void SlimScrollBar::hideEvent(QHideEvent *e)
{
    DEB_EVENT << "hideEvent" << e;
    QScrollBar::hideEvent(e);
}

void SlimScrollBar::resizeEvent(QResizeEvent *e)
{
    DEB_EVENT << "resizeEvent" << e;
    QScrollBar::resizeEvent(e);

    // 重新设置滚动条图片的大小
    pixmap = QPixmap(e->size());
    qDebug() << "size:" << e->size();
}
