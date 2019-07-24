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

    //mouseMoveTo(toX,toY);
    //mouseLeftClick();

    POINT win_coords = {toX, toY};
    POINT ctrl_coords = win_coords;


    HWND win_handle = WindowFromPoint(win_coords);
    // Getting the handle of the control to click, from the target window handle and the coordinates relative to it
    HWND lastCtrlHandle = win_handle;

    // Setting coords relative to the target window
    ScreenToClient(lastCtrlHandle, &ctrl_coords);

    qDebug()<<"click";
    HWND childHandle = nullptr;
    do{
        // Clicking using Win32 APIs, with the control handle and the coordinates relative to it as params
        LPARAM lParam = MAKELPARAM(ctrl_coords.x, ctrl_coords.y);
        PostMessage(lastCtrlHandle, WM_LBUTTONDOWN, MK_LBUTTON, lParam);
        PostMessage(lastCtrlHandle, WM_LBUTTONUP, 0, lParam);

        childHandle = ChildWindowFromPoint(lastCtrlHandle, ctrl_coords);
        lastCtrlHandle = childHandle;
        ScreenToClient(lastCtrlHandle, &ctrl_coords);

    }while(childHandle != nullptr);

}


void MonkeyTestExecutor::mouseMoveTo(int toX, int toY)
{
    // get system information regarding screen size / resolution
    double screenRes_width = ::GetSystemMetrics( SM_CXSCREEN )-1;
    double screenRes_height = ::GetSystemMetrics( SM_CYSCREEN )-1;
    // scale the function inputs 'toX and 'toY' appropriately by a
    // factor of 65535
    double dx = toX*(65535.0f / screenRes_width);
    double dy = toY*(65535.0f / screenRes_height);
    // we now have variables 'dx' and 'dy' with the user-desired
    // coordinates in a form that the SendInput function can understand

    // set up INPUT Input, assign values, and move the cursor
    INPUT Input = {0};
    // we want to be sending a MOUSE-type input
    Input.type = INPUT_MOUSE;
    // the mouse input is of the 'MOVE ABSOLUTE' variety
    Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
    Input.mi.dx = LONG(dx);
    Input.mi.dy = LONG(dy);
    // we have given information about the magnitude of the new
    // cursor position; here we will send it
    ::SendInput(1,&Input,sizeof(INPUT));
}

void MonkeyTestExecutor::mouseLeftClick()
{
    INPUT Input = {0};
    UINT v;
    // left down
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    v = SendInput(1,&Input,sizeof(INPUT));

    // left up
    ::ZeroMemory(&Input,sizeof(INPUT));
    Input.type = INPUT_MOUSE;
    Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    ::SendInput(1,&Input,sizeof(INPUT));
}
