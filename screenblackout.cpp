#include "screenblackout.h"
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>

ScreenBlackOut::ScreenBlackOut(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);
    this->setStyleSheet("background:black;");
    isFull = false;
    ms = Idle;
    QDesktopWidget dw;
    sw = dw.availableGeometry(dw.primaryScreen()).width();
    sh = dw.availableGeometry(dw.primaryScreen()).height();
    
    panelwidth = dw.screenGeometry(dw.primaryScreen()).width() -sw;
    int widgetw = sw/2;
    int widgeth = sh/2;
    
    anime = new QPropertyAnimation(this, "geometry");
    anime->setDuration(250);
    anime->setStartValue(QRect(0,widgeth ,widgetw,0));
    anime->setEndValue(QRect(panelwidth+sw/4,sh/4,widgetw, widgeth));
    anime->start();
    
    
    down = new QPushButton(this);
    up = new QPushButton(this);
    left = new QPushButton(this);
    right = new QPushButton(this);
    whole = new QPushButton(this);
    cl = new QPushButton(this);
    
    upUp = new QPushButton(this);
    upDown = new QPushButton(this);
    downUp = new QPushButton(this);
    downDown = new QPushButton(this);
    leftLeft = new QPushButton(this);
    leftRight = new QPushButton(this);
    rightRight = new QPushButton(this);
    rightLeft = new QPushButton(this);
    
    upUp->setFlat(true);
    upDown->setFlat(true);
    downUp->setFlat(true);
    downDown->setFlat(true);
    leftLeft->setFlat(true);
    leftRight->setFlat(true);
    rightRight->setFlat(true);
    rightLeft->setFlat(true);
    
    upUp->setIconSize(QSize(50,50));
    upDown->setIconSize(QSize(50,50));
    downUp->setIconSize(QSize(50,50));
    downDown->setIconSize(QSize(50,50));
    leftLeft->setIconSize(QSize(50,50));
    leftRight->setIconSize(QSize(50,50));
    rightRight->setIconSize(QSize(50,50));
    rightLeft->setIconSize(QSize(50,50));
    
    upUp->setVisible(false);
    upDown->setVisible(false);
    downUp->setVisible(false);
    downDown->setVisible(false);
    leftLeft->setVisible(false);
    leftRight->setVisible(false);
    rightRight->setVisible(false);
    rightLeft->setVisible(false);
    
    layout = new QGridLayout();


    
    up->setFlat(true);
    up->setIcon(QIcon(":images/perde_up.svg"));
    up->setIconSize(QSize(50,50));
    
    
    down->setFlat(true);
    down->setIcon(QIcon(":images/perde_down.svg"));
    down->setIconSize(QSize(50,50));
    
    
    left->setFlat(true);
    left->setIcon(QIcon(":images/perde_left.svg"));
    left->setIconSize(QSize(50,50));
    

    right->setFlat(true);
    right->setIcon(QIcon(":images/perde_right.svg"));
    right->setIconSize(QSize(50,50));
    
    
    whole->setFlat(true);
    whole->setIcon(QIcon(":images/full.svg"));
    whole->setIconSize(QSize(50,50));
    
    
    cl->setFlat(true);
    cl->setIcon(QIcon(":images/close.svg"));
    cl->setIconSize(QSize(50,50));
    
    
    layout->addWidget(up,0,2);
    layout->addWidget(down,2,2);
    layout->addWidget(whole,1,2);
    layout->addWidget(left,1,1);
    layout->addWidget(right,1,3);
    layout->addWidget(cl,2,0);
    
    layout->setGeometry(QRect(0,widgeth-150,200,150));

    
    connect(up,SIGNAL(clicked()),this,SLOT(upAnimation()));
    connect(down,SIGNAL(clicked()),this,SLOT(downAnimation()));
    connect(left,SIGNAL(clicked()),this,SLOT(leftAnimation()));
    connect(right,SIGNAL(clicked()),this,SLOT(rightAnimation()));
    connect(whole,SIGNAL(clicked()),this,SLOT(wholeAnimation()));
    connect(cl,SIGNAL(clicked()),this,SLOT(close()));
    
    connect(upUp,SIGNAL(clicked()),this,SLOT(upUpSlot()));
    connect(upDown,SIGNAL(clicked()),this,SLOT(upDownSlot()));
    connect(downDown,SIGNAL(clicked()),this,SLOT(downDownSlot()));
    connect(downUp,SIGNAL(clicked()),this,SLOT(downUpSlot()));
    
    connect(leftLeft,SIGNAL(clicked()),this,SLOT(leftLeftSlot()));
    connect(leftRight,SIGNAL(clicked()),this,SLOT(leftRightSlot()));
    connect(rightRight,SIGNAL(clicked()),this,SLOT(rightRightSlot()));
    connect(rightLeft,SIGNAL(clicked()),this,SLOT(rightLeftSlot()));

}

ScreenBlackOut::~ScreenBlackOut()
{

}

void ScreenBlackOut::mousePressEvent(QMouseEvent *event)
{
    m_MouseX = event->x();
    m_MouseY = event->y();

    QRect current = this->geometry();

    m_WidgetHeight = current.height();
    m_WidgetWidth = current.width();

    int endPointHor = current.x() + current.width();
    int startPointHor = endPointHor - 50;

    int endPointVer = current.y() + current.height();
    int startPointVer = endPointVer - 50;

    if(startPointHor < event->globalX() && endPointHor > event->globalX() &&
            startPointVer < event->globalY() && endPointVer > event->globalY()) {
        ms = Both;
    } else if (startPointHor < event->globalX() && endPointHor > event->globalX()) {
        ms = Horizontal;
    } else if(startPointVer < event->globalY() && endPointVer > event->globalY()) {
        ms = Vertical;
    } else {
        ms = Idle;
    }
}

void ScreenBlackOut::mouseReleaseEvent(QMouseEvent *event)
{
    event->accept();
}

void ScreenBlackOut::mouseMoveEvent(QMouseEvent *event)
{
    QRect current = this->geometry();
    int mouseDistanceHorizontal = m_WidgetWidth - m_MouseX;
    int mouseDistanceVertical = m_WidgetHeight - m_MouseY;

    int nextHeight = mouseDistanceVertical + event->globalY() - current.y();
    int nextWidth = mouseDistanceHorizontal + event->globalX() - current.x();

    switch (ms) {
    case Idle:
        this->setGeometry(event->globalX()-m_MouseX, event->globalY()-m_MouseY,
                          current.width(), current.height());
        break;
    case Horizontal:

        this->setGeometry(current.x(),
                          current.y(),
                          nextWidth <= 200 ? 200 : nextWidth,
                          current.height());
        break;
    case Vertical:

        this->setGeometry(current.x(),
                          current.y(),
                          current.width(),
                          nextHeight <= 150 ? 150 : nextHeight);

        break;
    case Both:
        this->setGeometry(current.x(),
                          current.y(),
                          nextWidth <= 200 ? 200 : nextWidth,
                          nextHeight <= 150 ? 150 : nextHeight);
        break;
    default:
        this->setGeometry(event->globalX()-m_MouseX, event->globalY()-m_MouseY,
                          current.width(), current.height());
        break;
    }
    if (leftLeft->isVisible()) {
        leftLeft->setGeometry(current.width() - 50,
                              current.height() / 2 - 50,
                              50,
                              50);
    }
    if (leftRight->isVisible()) {
        leftRight->setGeometry(current.width() - 50,
                               current.height() / 2,
                               50,
                               50);
    }
    if (upUp->isVisible()) {
        upUp->setGeometry(current.width() / 2 - 50,
                          current.height() - 50,
                          50,
                          50);
    }
    if (upDown->isVisible()) {
        upDown->setGeometry(current.width() / 2 - 100,
                            current.height() - 50,
                            50,
                            50);
    }

    if (rightLeft->isVisible()) {
        rightLeft->setGeometry(0,
                               current.height() / 2 - 50,
                               50,
                               50);
    }
    if (rightRight->isVisible()) {
        rightRight->setGeometry(0,
                               current.height() / 2,
                               50,
                               50);
    }

    if (downDown->isVisible()) {
        downDown->setGeometry(current.width() / 2 - 50,
                               0,
                               50,
                               50);
    }

    if (downUp->isVisible()) {
        downUp->setGeometry(current.width() / 2,
                               0,
                               50,
                               50);
    }


    layout->setGeometry(QRect(0,current.height()-150,200,150));

}

int ScreenBlackOut::getsh()
{
    return sh;
}
int ScreenBlackOut::getsw()
{
    return sw;
}

void ScreenBlackOut::rightLeftSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    
    
    if(tmp.x() > panelwidth+200)
    {

        tmp.setX(tmp.x()-sw/10);


    }
    anime->setEndValue(tmp);
    anime->start();
}

void ScreenBlackOut::rightRightSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();

    if(tmp.x() < sw)
    {

        tmp.setX(tmp.x()+sw/10);

    }
    anime->setEndValue(tmp);
    anime->start();
}

void ScreenBlackOut::leftLeftSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect leftRighttmp = leftRight->geometry();
    QRect leftLefttmp = leftLeft->geometry();
    
    
    
    if(tmp.width() > panelwidth)
    {
        tmp.setWidth(tmp.width() - sw/10);
        leftLefttmp.setX(leftLefttmp.x()-sw/10);
        leftLefttmp.setWidth(50);
        leftRighttmp.setX(leftRighttmp.x()-sw/10);
        leftRighttmp.setWidth(50);

        leftLeft->setGeometry(leftLefttmp);
        leftRight->setGeometry(leftRighttmp);

    }
    anime->setEndValue(tmp);
    anime->start();
}
void ScreenBlackOut::leftRightSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect leftRighttmp = leftRight->geometry();
    QRect leftLefttmp = leftLeft->geometry();
    
    
    
    if(tmp.width() < sw-panelwidth)
    {
        tmp.setWidth(tmp.width() + sw/10);
        leftLefttmp.setX(leftLefttmp.x()+sw/10);
        leftLefttmp.setWidth(50);
        leftRighttmp.setX(leftRighttmp.x()+sw/10);
        leftRighttmp.setWidth(50);

        leftLeft->setGeometry(leftLefttmp);
        leftRight->setGeometry(leftRighttmp);

    }
    anime->setEndValue(tmp);
    anime->start();
}

void ScreenBlackOut::downDownSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect layouttmp = tmp;
    
    
    if(tmp.y() < sh-150)
    {

        tmp.setY(tmp.y()+sh/10);
        layouttmp.setX(layouttmp.x()-panelwidth);
        layouttmp.setY(layouttmp.height()-250);
        layouttmp.setHeight(150);
        layouttmp.setWidth(200);
        layout->setGeometry(layouttmp);
    }
    anime->setEndValue(tmp);
    anime->start();
}
void ScreenBlackOut::downUpSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect layouttmp = tmp;
    
    qDebug() << "downUP called";
    
    if(tmp.y() > 150)
    {

        tmp.setY(tmp.y()-sh/10);
        layouttmp.setX(layouttmp.x()-panelwidth);
        layouttmp.setY(layouttmp.height()-50);
        layouttmp.setHeight(150);
        layouttmp.setWidth(200);
        layout->setGeometry(layouttmp);
    }
    anime->setEndValue(tmp);
    anime->start();
}

void ScreenBlackOut::upUpSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect upUptmp = upUp->geometry();
    QRect upDowntmp = upDown->geometry();
    QRect layouttmp = tmp;
    
    
    if(tmp.height() > 150)
    {
        tmp.setHeight(tmp.height() - sh/10);
        upDowntmp.setY(upDowntmp.y()-sh/10);
        upDowntmp.setHeight(50);
        upUptmp.setY(upUptmp.y()-sh/10);
        upUptmp.setHeight(50);
        layouttmp.setX(layouttmp.x()-panelwidth);
        layouttmp.setY(layouttmp.height()-150-sh/10);
        layouttmp.setHeight(150);
        layouttmp.setWidth(200);
        upDown->setGeometry(upDowntmp);
        upUp->setGeometry(upUptmp);
        layout->setGeometry(layouttmp);
    }
    anime->setEndValue(tmp);
    anime->start();

}

void ScreenBlackOut::upDownSlot()
{
    anime->setStartValue(this->geometry());
    QRect tmp = this->geometry();
    QRect upUptmp = upUp->geometry();
    QRect upDowntmp = upDown->geometry();
    QRect layouttmp = tmp;
    
    
    if(tmp.height() < sh-150)
    {
        tmp.setHeight(tmp.height() + sh/10);
        upDowntmp.setY(upDowntmp.y()+sh/10);
        upDowntmp.setHeight(50);
        upUptmp.setY(upUptmp.y()+sh/10);
        upUptmp.setHeight(50);
        layouttmp.setX(layouttmp.x()-panelwidth);
        layouttmp.setY(layouttmp.height()-150+sh/10);
        layouttmp.setHeight(150);
        layouttmp.setWidth(200);
        upDown->setGeometry(upDowntmp);
        upUp->setGeometry(upUptmp);
        layout->setGeometry(layouttmp);
    }
    anime->setEndValue(tmp);
    anime->start();

    
    

}
void ScreenBlackOut::downAnimation()
{

    
    anime->setStartValue(this->geometry());
    anime->setEndValue(QRect(panelwidth,sh/2,sw,sh/2));
    anime->start();
    layout->setGeometry(QRect(0,sh/2-150,200,150));
    
    downUp->setGeometry(QRect(sw/2-50,0,50,50));
    downUp->setVisible(true);
    downUp->setIcon(QIcon(":images/up.svg"));
    downDown->setGeometry(QRect(sw/2-100,0,50,50));
    downDown->setVisible(true);
    downDown->setIcon(QIcon(":images/down.svg"));
    
    upUp->setVisible(false);
    upDown->setVisible(false);
    leftLeft->setVisible(false);
    leftRight->setVisible(false);
    rightLeft->setVisible(false);
    rightRight->setVisible(false);

}

void ScreenBlackOut::upAnimation()
{
    
    anime->setStartValue(this->geometry());
    anime->setEndValue(QRect(panelwidth,0,sw,sh/2));
    anime->start();
    layout->setGeometry(QRect(0,sh/2-150,200,150));
    
    upUp->setGeometry(QRect(sw/2-50,sh/2-50,50,50));
    upUp->setVisible(true);
    upUp->setIcon(QIcon(":images/up.svg"));
    upDown->setGeometry(QRect(sw/2-100,sh/2-50,50,50));
    upDown->setVisible(true);
    upDown->setIcon(QIcon(":images/down.svg"));
    
    downDown->setVisible(false);
    downUp->setVisible(false);
    leftLeft->setVisible(false);
    leftRight->setVisible(false);
    rightLeft->setVisible(false);
    rightRight->setVisible(false);
}

void ScreenBlackOut::leftAnimation()
{

    anime->setStartValue(this->geometry());
    anime->setEndValue(QRect(panelwidth,0,sw/2,sh));
    anime->start();
    layout->setGeometry(QRect(0,sh-150,200,150));
    
    leftLeft->setGeometry(QRect(sw/2-50,sh/2-50,50,50));
    leftLeft->setVisible(true);
    leftLeft->setIcon(QIcon(":images/left.svg"));
    leftRight->setGeometry(QRect(sw/2-50,sh/2,50,50));
    leftRight->setVisible(true);
    leftRight->setIcon(QIcon(":images/right.svg"));
    
    downDown->setVisible(false);
    downUp->setVisible(false);
    upUp->setVisible(false);
    upDown->setVisible(false);
    rightLeft->setVisible(false);
    rightRight->setVisible(false);
}

void ScreenBlackOut::rightAnimation()
{
    
    anime->setStartValue(this->geometry());
    anime->setEndValue(QRect(sw/2+panelwidth,0,sw/2,sh));
    anime->start();
    layout->setGeometry(QRect(0,sh-150,200,150));
    
    
    rightLeft->setGeometry(QRect(0,sh/2-50,50,50));
    rightLeft->setVisible(true);
    rightLeft->setIcon(QIcon(":images/left.svg"));
    rightRight->setGeometry(QRect(0,sh/2,50,50));
    rightRight->setVisible(true);
    rightRight->setIcon(QIcon(":images/right.svg"));
    
    downDown->setVisible(false);
    downUp->setVisible(false);
    upUp->setVisible(false);
    upDown->setVisible(false);
    leftLeft->setVisible(false);
    leftRight->setVisible(false);

}

void ScreenBlackOut::wholeAnimation()
{
    if(isFull) {
        isFull = false;
        anime->setStartValue(this->geometry());
        anime->setEndValue(QRect(panelwidth+sw/4,sh/4,sw/2,sh/2));
        anime->start();
        layout->setGeometry(QRect(0,sh/2-150,200,150));

        whole->setIcon(QIcon(":images/full.svg"));
    } else {
        isFull = true;
        anime->setStartValue(this->geometry());
        anime->setEndValue(QRect(0,0,sw+panelwidth,sh));
        anime->start();
        layout->setGeometry(QRect(0,sh-150,200,150));

        whole->setIcon(QIcon(":images/restore.svg"));
    }

    
    downUp->setVisible(false);
    downDown->setVisible(false);
    upUp->setVisible(false);
    upDown->setVisible(false);
    leftLeft->setVisible(false);
    leftRight->setVisible(false);
    rightLeft->setVisible(false);
    rightRight->setVisible(false);
}


