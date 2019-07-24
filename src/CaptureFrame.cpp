#include "CaptureFrame.h"
#include "QScreen"
#include "QGuiApplication"
#include <QPainter>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>


CaptureFrame::CaptureFrame(QWidget* parent)
    :QFrame(parent)
{
    this->setWindowFlags(Qt::Tool|Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedSize( QGuiApplication::primaryScreen()->geometry().size());
}


void CaptureFrame::paintEvent(QPaintEvent *event)
{
    __super::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(0,0,this->width(),this->height(),QColor(1,1,1,1));
    if(!captureArea_.isEmpty())
    {
         painter.fillRect(captureArea_,QColor(125,125,125,125));
    }
}


void CaptureFrame::clearArea()
{
    captureArea_ = QRect();
}

void CaptureFrame::mousePressEvent(QMouseEvent *event)
{
    __super::mousePressEvent(event);

    pressPos_ = event->pos();
}



void CaptureFrame::mouseMoveEvent(QMouseEvent *event)
{
    __super::mouseMoveEvent(event);

    movePos_ = event->pos();

    captureArea_ = QRect(pressPos_,movePos_);
    this->repaint();
}



void CaptureFrame::mouseReleaseEvent(QMouseEvent *event)
{
    __super::mouseReleaseEvent(event);

    emit sigAreaCaptured(captureArea_);
    this->hide();
}
