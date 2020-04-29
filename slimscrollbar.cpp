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

    event_timer = new QTimer(this);
    event_timer->setInterval(30);
    connect(event_timer, SIGNAL(timeout()), this, SLOT(eventTimer()));
}

void SlimScrollBar::enterEvent(QEvent *e)
{
    DEB_EVENT << "enterEvent" << e;
    QScrollBar::enterEvent(e);

    activeTimer();

    hovering = true;
}

void SlimScrollBar::leaveEvent(QEvent *e)
{
    DEB_EVENT << "leaveEvent" << e;
    QScrollBar::leaveEvent(e);

    hovering = false;
}

void SlimScrollBar::mousePressEvent(QMouseEvent *e)
{
    DEB_EVENT << "mousePressEvent" << e;
    QScrollBar::mousePressEvent(e);

    activeTimer();
    if (e->button() == Qt::LeftButton)
    {
        pressing = true;
    }
}

void SlimScrollBar::mouseMoveEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseMoveEvent" << e;
    QScrollBar::mouseMoveEvent(e);

    if(pressing)
    {
        // 判断拖拽的位置，调整可视区域大小

    }
}

void SlimScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseReleaseEvent" << e;
    QScrollBar::mouseReleaseEvent(e);

    if (pressing && e->button() == Qt::LeftButton)
    {
        pressing = false;
    }
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
    if (!enabling)
        return ;

    // TODO: 判断需不需要重绘

    // 绘制自己的控件
    paintPixmap();

    QPainter painter(this);
    painter.drawPixmap(QRect(0,0,width(),height()), pixmap, QRect(pixmap.width()-width(), 0, width(), height()));

    emit signalRepaint();
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
    // 应该会自动update吧
    // update();
}

/**
 * 绘制自身控件至pixmap；自己绘制进度条内的部分
 * 如果大小改变，则调整pixmap的大小
 */
void SlimScrollBar::paintPixmap()
{
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPainterPath bg_path;
    QPainterPath fg_path;

    // 绘制背景
    QRect bg_rect(0,0,width(),height());
    painter.fillRect(bg_rect,bg_normal_color);
    if (hover_prop)
    {
        QColor c = bg_hover_color;
        c.setAlpha(c.alpha() * hover_prop / 100);
        painter.fillRect(bg_rect, c);
    }
    if (press_prop)
    {
        QColor c = bg_press_color;
        c.setAlpha(c.alpha() * press_prop / 100);
        painter.fillRect(bg_rect, c);
    }

    // 绘制前景
    int range = maximum() - minimum();
    int step = this->pageStep();
    int height = this->height() * step / range;
    int top = (this->height() - height) * sliderPosition() / range;
    QRect fg_rect(0, top, width(), height);
    painter.fillRect(fg_rect, fg_normal_color);
    if (hover_prop)
    {
        QColor c = fg_hover_color;
        c.setAlpha(c.alpha() * hover_prop / 100);
        painter.fillRect(fg_rect, c);
    }
    if (press_prop)
    {
        QColor c = fg_press_color;
        c.setAlpha(c.alpha() * press_prop / 100);
        painter.fillRect(fg_rect, c);
    }
}

void SlimScrollBar::activeTimer()
{
    if (!event_timer->isActive())
        event_timer->start();
}

/**
 * 实时刷新界面
 */
void SlimScrollBar::eventTimer()
{
    if (hovering)
    {
        if (hover_prop < 100)
        {
            hover_prop += 10;
            if (hover_prop >= 100) // 结束
            {
                hover_prop = 100;
            }
        }
    }
    else
    {
        if (hover_prop > 0)
        {
            hover_prop -= 10;
            if (hover_prop <= 0) // 结束
            {
                hover_prop = 0;
            }
        }
    }

    if (pressing)
    {
        if (press_prop < 100)
        {
            press_prop += 10;
            if (press_prop >= 100) // 结束
            {
                press_prop = 100;
            }
        }
    }
    else
    {
        if (press_prop > 0)
        {
            press_prop -= 10;
            if (press_prop <= 0) // 结束
            {
                press_prop = 0;
            }
        }
    }

    if(!hovering && !pressing && hover_prop == 0 && press_prop == 0)
    {
        event_timer->stop();
    }
}

/**
 * 父类绘制进度条超出的部分
 */
void SlimScrollBar::paintScrollBar(QPainter &painter, QSize size)
{
    QRect rect(size.width()+this->width()-pixmap.width(), 0, pixmap.width(), pixmap.height());
    painter.drawPixmap(rect, pixmap);
}

void SlimScrollBar::enable()
{
    enabling = true;
}

void SlimScrollBar::disable()
{
    enabling = false;
}
