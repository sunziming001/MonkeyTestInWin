#include "MonkeyTestExecutor.h"
#include <QDebug>
#include <Windows.h>
#include <QTime>
MonkeyTestExecutor::MonkeyTestExecutor(QObject *parent) : QObject(parent),isRunning_(false)
{
    execThread_ = new QThread(this);
    this->moveToThread(execThread_);




    execThread_->start();
}

void MonkeyTestExecutor::startMonkeyTest(const MonkeyTestTask& task)
{

    curTask_ = task;

    isRunning_=true;
    QTime startTime = QTime::currentTime();
    QTime curTime =QTime::currentTime();
    while(isRunning_)
    {
        triggerClick();
        Sleep(curTask_.sepMsecs);

        curTime = QTime::currentTime();
        unsigned int takeTime = abs(curTime.msecsTo(startTime));
        if( takeTime>= curTask_.testMsecs)
        {
            break;
        }
    }

    isRunning_ = false;
}


void MonkeyTestExecutor::stopMonkeyTest()
{
    isRunning_ = false;
}


void MonkeyTestExecutor::triggerClick()
{
    int w = curTask_.area.width();
    int h = curTask_.area.height();
    int x = curTask_.area.x();
    int y = curTask_.area.y();

    int randx = qrand()%w;
    int randy = qrand()%h;
    int toX = x+randx;
    int toY = y+randy;


    POINT win_coords = {toX, toY};
    POINT ctrl_coords = win_coords;


    HWND win_handle = WindowFromPoint(win_coords);

    HWND lastCtrlHandle = win_handle;

    ScreenToClient(lastCtrlHandle, &ctrl_coords);

    LPARAM lParam = MAKELPARAM(ctrl_coords.x, ctrl_coords.y);
    PostMessage(lastCtrlHandle, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
    PostMessage(lastCtrlHandle, WM_LBUTTONUP, 0, lParam);

    QPoint pos(win_coords.x, win_coords.y);

    emit sigClickTriggered(pos);

}
