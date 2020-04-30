#include "slimscrollbar.h"

SlimScrollBar::SlimScrollBar(QWidget *parent) : QScrollBar(parent)
{
    /*popup = new SlimScrollBarPopup(nullptr);
    popup->pixmap = &pixmap;
    popup->show();
    popup->setGeometry(-2, -2, 0, 0);*/

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
        mouse_pos = press_pos = e->pos();
        setOffsetPoss();
    }
}

void SlimScrollBar::mouseMoveEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseMoveEvent" << e;
    QScrollBar::mouseMoveEvent(e);

    if(pressing)
    {
        mouse_pos = e->pos();
        setOffsetPoss();

        // 判断拖拽的位置，调整可视区域大小
        // 已经显示弹窗，则应该实时调整
        // 如果没有，那么当开始拖拽的时候，就应该开始调整
        if (popuping || qAbs(e->pos().x() - press_pos.x()) >= width())
        {
            // 拖动，重新设置大小
            calcPixmapSize();

            if (!popuping)
            {
                startPopup();
            }
        }
    }
}

void SlimScrollBar::mouseReleaseEvent(QMouseEvent *e)
{
    DEB_EVENT << "mouseReleaseEvent" << e;
    QScrollBar::mouseReleaseEvent(e);

    if (pressing && e->button() == Qt::LeftButton)
    {
        pressing = false;

        target_pos = QPoint(0, 0);
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

    // 绘制到pixmap
    paintPixmap();

    if (!popuping) // 如果没有显示弹窗，绘制到自己
    {
        QPainter painter(this);
        painter.drawPixmap(QRect(0,0,width(),height()), pixmap, QRect(pixmap.width()-width(), 0, width(), height()));
    }
    else // 绘制到弹窗
    {
        repaintPopup();
    }
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
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (!popuping) // 直接画矩形
    {
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
        int height = qMax(this->height() * step / range - 10, 16);
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
    else
    {
        QPainterPath bg_path;
        QPainterPath fg_path;
        int ctrl_offset = 32;
        QPoint offset(popup_offset + QPoint(width()/2, 0));
        QPoint ctrl_t_p(0, -ctrl_offset);
        QPoint top_p(0, 0);
        QPoint bulge(anchor_pos);
        QPoint btm_p(0, height());
        QPoint ctrl_b_p(0, height()+ctrl_offset);
        ctrl_t_p += offset;
        top_p += offset;
        bulge += offset;
        btm_p += offset;
        ctrl_b_p += offset;

        bg_path.moveTo(top_p);
        bg_path.cubicTo(top_p, bulge, btm_p);
        bg_path.lineTo(btm_p);

        int pen_w = width();
        pen_w -= quick_sqrt(qMax(qAbs(popup_offset.x()), qAbs(popup_offset.y())));
        pen_w = qMax(1, pen_w);
        painter.setPen(QPen(bg_normal_color, pen_w));
        painter.drawPath(bg_path);

        int range = maximum() - minimum();
        int step = this->pageStep();
        int height = qMax(this->height() * step / range - 10, 16);
        int top = (this->height() - height) * sliderPosition() / range;

        fg_path.addRect(0, top, pixmap.width(), height);
        painter.setClipPath(fg_path, Qt::IntersectClip);
        painter.setPen(QPen(fg_normal_color, width()));
        painter.drawPath(bg_path);
        if (hover_prop)
        {
            QColor c = fg_hover_color;
            c.setAlpha(c.alpha() * hover_prop / 100);
            painter.setPen(QPen(c, width()));
            painter.drawPath(bg_path);
        }
        if (press_prop)
        {
            QColor c = fg_press_color;
            c.setAlpha(c.alpha() * press_prop / 100);
            painter.setPen(QPen(c, width()));
            painter.drawPath(bg_path);
        }
    }
}

void SlimScrollBar::activeTimer()
{
    if (!event_timer->isActive())
        event_timer->start();
}

void SlimScrollBar::setOffsetPoss()
{
    target_pos.setX(mouse_pos.x() - width()/2);
    /*if (mouse_pos.y() <= 0)
        target_pos.setY(mouse_pos.y());
    else if (mouse_pos.y() > height())
        target_pos.setY(mouse_pos.y() - height());
    else
        target_pos.setY(0);*/
    target_pos.setY(mouse_pos.y());
}

/**
 * 实时刷新界面
 */
void SlimScrollBar::eventTimer()
{
    // 判断比例
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

    // 判断移动
//    QPoint sqrt_pos(quick_sqrt(target_pos.x()), quick_sqrt(target_pos.y())); // 开方再开方
    if (anchor_pos != target_pos)
    {
        int delta_x = anchor_pos.x() - target_pos.x();
        int delta_y = anchor_pos.y() - target_pos.y();

        // 锚点靠近目标点
        anchor_pos.setX( anchor_pos.x() - quick_sqrt(delta_x) );
        anchor_pos.setY( anchor_pos.y() - quick_sqrt(delta_y) );

        // 偏差点是锚点 开方
        effect_pos.setX( quick_sqrt(static_cast<long>(anchor_pos.x())) );
        effect_pos.setY( quick_sqrt(static_cast<long>(anchor_pos.y())) );
    }

    // 更新图片
    if (popuping)
    {
        calcPixmapSize();
    }

    // 自动暂停
    if(!hovering && !pressing && hover_prop == 0 && press_prop == 0 && effect_pos == QPoint(0,0))
    {
        event_timer->stop();
        if (popuping)
        {
            popuping = false;
            popup->setGeometry(-2, -2, 0, 0);
            // 删除popup
            popup->deleteLater();
            popup = nullptr;
        }
    }

    update();
}

void SlimScrollBar::enable()
{
    enabling = true;
}

void SlimScrollBar::disable()
{
    enabling = false;
}

/**
 * 速度极快的开方算法，效率未知，原理未知
 * @param  X 待开方的数字
 * @return   平方根
 */
int SlimScrollBar::quick_sqrt(long X) const
{
    bool fu = false;
    if (X < 0)
    {
        fu = true;
        X = -X;
    }
#if !defined(Q_OS_WIN)
    X = qSqrt(X);
    return fu ? -X : X;
#endif
    unsigned long M = static_cast<unsigned long>(X);
    unsigned int N, i;
    unsigned long tmp, ttp; // 结果、循环计数
    if (M == 0) // 被开方数，开方结果也为0
        return 0;
    N = 0;
    tmp = (M >> 30); // 获取最高位：B[m-1]
    M <<= 2;
    if (tmp > 1) // 最高位为1
    {
        N ++; // 结果当前位为1，否则为默认的0
        tmp -= N;
    }
    for (i = 15; i > 0; i--) // 求剩余的15位
    {
        N <<= 1; // 左移一位
        tmp <<= 2;
        tmp += (M >> 30); // 假设
        ttp = N;
        ttp = (ttp << 1) + 1;
        M <<= 2;
        if (tmp >= ttp) // 假设成立
        {
            tmp -= ttp;
            N ++;
        }
    }
    return (fu ? -1 : 1) * static_cast<int>(N); // 不知道为什么计算出来的结果是反过来的
}

void SlimScrollBar::calcPixmapSize()
{
    if (!popup)
    {
        pixmap = QPixmap(width(), height());
        popup_offset = QPoint(0, 0);
        return ;
    }

    int leftest = qMin(qMin(anchor_pos.x(), 0), effect_pos.x());
    int rightest = qMax(qMax(anchor_pos.x(), width()), effect_pos.x()+width());
    int wi = rightest - leftest;
    int topest = qMin(qMin(anchor_pos.y(), 0), effect_pos.x());
    int bottomest = qMax(qMax(anchor_pos.y(), height()), effect_pos.x()+height());
    int he = bottomest - topest;
    pixmap = QPixmap(QSize(wi, he));
    popup_offset.setX(0-leftest);
    popup_offset.setY(0-topest);
}

/**
 * 切换至浮窗界面
 * 初始化所有数据
 */
void SlimScrollBar::startPopup()
{
    popuping = true;
    if (!popup)
    {
        popup = new SlimScrollBarPopup(nullptr);
        popup->scrollbar_pixmap = pixmap;
        popup->show();
    }

    paintPixmap();
    repaintPopup();
}

/**
 * 自己更新界面，重绘弹窗
 */
void SlimScrollBar::repaintPopup()
{
    // 设置大小
    popup->setGeometry(QRect(mapToGlobal(pos())-popup_offset, pixmap.size()));
    // 设置绘制位置
    popup->offset = popup_offset;
    popup->scrollbar_pixmap = pixmap;
    // 绘制图片
    popup->update();
}
