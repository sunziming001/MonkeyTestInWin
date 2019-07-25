#include "CaptureFrame.h"
#include "QScreen"
#include "QGuiApplication"
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QRect>
#include <Windows.h>
#if _WIN32
    #include <windows.h>
#endif

CaptureFrame::CaptureFrame(QWidget* parent)
    :QFrame(parent),status_(CaptureFrame::Idel)
{
    this->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground);

}


void CaptureFrame::appendClickedPos(const QPoint& globalPos)
{
    //QPoint localPos = mapFromGlobal(globalPos);
    while(clickedPosRecd_.size()>MaxClickedPosCnt)
    {
        clickedPosRecd_.dequeue();
    }
    QPoint paintPos = this->mapFromGlobal(globalPos);

    clickedPosRecd_.enqueue(paintPos);
    repaint();
}

void CaptureFrame::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);
    QPainter painter(this);
    qDebug()<<status_;
    if(status_ == CaptureFrame::Capturing)
    {
        painter.fillRect(0,0,this->width(),this->height(),QColor(1,1,1,1));
        if(!captureArea_.isEmpty())
        {
             painter.fillRect(captureArea_,QColor(125,125,125,125));
        }
    }else if(status_ == CaptureFrame::Display)
    {
        //QRect area = this->geometry();
        //QRect regin =event->region().boundingRect();
        painter.fillRect(0,
                         0,
                         event->region().boundingRect().width(),
                         event->region().boundingRect().height(),
                         QColor(125,125,125,125));
        int colorStep = 255;
        if(clickedPosRecd_.size()>0)
        {
            colorStep = 255/clickedPosRecd_.size();
        }

        int alpha = 255;
        int gb = 255;
        for(auto iter = clickedPosRecd_.begin(); iter != clickedPosRecd_.end(); iter++)
        {
            QColor color(255,gb,gb,alpha);
            painter.setBrush(color);

            painter.drawEllipse(*iter,3,3);
            gb-=colorStep;
        }
    }

}

void CaptureFrame::startCapture()
{
    status_ = CaptureFrame::Capturing;
    this->setFixedSize( QGuiApplication::primaryScreen()->geometry().size());
    this->move(0,0);
    this->show();

    this->setAttribute(Qt::WA_TransparentForMouseEvents,false);
    //make win32 top level window does not transparent mouse event
#if _WIN32
    HWND hwnd = (HWND) winId();
    LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, styles & (~WS_EX_TRANSPARENT));
#endif

    this->repaint();
}

void CaptureFrame::startDisplay()
{
    status_ = CaptureFrame::Display;
    this->setFixedSize(captureArea_.size());
    this->move(captureArea_.topLeft());
    this->setAttribute(Qt::WA_TransparentForMouseEvents,true);
    clickedPosRecd_.clear();
     //make win32 top level window transparent mouse event
#if _WIN32
    HWND hwnd = (HWND) winId();
    LONG styles = GetWindowLong(hwnd, GWL_EXSTYLE);
    SetWindowLong(hwnd, GWL_EXSTYLE, styles | WS_EX_TRANSPARENT);
#endif

    this->repaint();
}


void CaptureFrame::clearArea()
{
    captureArea_ = QRect();
}

void CaptureFrame::mousePressEvent(QMouseEvent *event)
{
    __super::mousePressEvent(event);

    if(status_ == CaptureFrame::Capturing)
        pressPos_ = event->pos();
}



void CaptureFrame::mouseMoveEvent(QMouseEvent *event)
{
    __super::mouseMoveEvent(event);

    if(status_ == CaptureFrame::Capturing)
    {
        movePos_ = event->pos();
        captureArea_ = QRect(pressPos_,movePos_);
        this->repaint();
    }
}



void CaptureFrame::mouseReleaseEvent(QMouseEvent *event)
{
    __super::mouseReleaseEvent(event);

    if(status_ == CaptureFrame::Capturing)
        emit sigAreaCaptured(captureArea_);
}
