#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <functional>
#include <QDebug>

MonkeyTestExecutor *gMonkeyTestExecutor = nullptr;

LRESULT CALLBACK hookproc( int nCode,
                                       WPARAM wParam,
                                       LPARAM lParam)
{

    DWORD code = PKBDLLHOOKSTRUCT(lParam)->vkCode;
    if (nCode == HC_ACTION)
    {
        switch (wParam)
        {
            // Pass KeyDown/KeyUp messages for Qt class to logicize
            case WM_KEYDOWN:
                if(code == VK_ESCAPE && gMonkeyTestExecutor != nullptr)
                {
                    qDebug()<<"esc Pressed";
                    gMonkeyTestExecutor->stopMonkeyTest();
                }

                break;
            case WM_KEYUP:

                break;
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),glhHook(nullptr),glhInstance(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle("MonkeyTest");

    testTask_.sepMsecs = ui->sp_sep->value();
    testTask_.testMsecs = ui->sp_dur->value()*1000*60;

    monkeyTestExecutor_ = new MonkeyTestExecutor(nullptr);
    gMonkeyTestExecutor = monkeyTestExecutor_;
    captureFrame_ =new CaptureFrame(nullptr);
    connect(this
            , &MainWindow::startTest
            , monkeyTestExecutor_
            , &MonkeyTestExecutor::startMonkeyTest
            , Qt::QueuedConnection);

    connect(monkeyTestExecutor_
            , &MonkeyTestExecutor::sigClickTriggered
            , this
            , &MainWindow::onClickedTriggered
            , Qt::QueuedConnection);

    connect(captureFrame_, &CaptureFrame::sigAreaCaptured,this,&MainWindow::onAreaCaptured);
    connect(ui->sp_sep, SIGNAL(valueChanged(int)),this,SLOT(onSepValChanged(int)));
    connect(ui->sp_dur, SIGNAL(valueChanged(int)),this,SLOT(onDurValChanged(int)));



    startHook();
}



bool MainWindow::startHook()
{
    bool bResult=false;

    glhHook=SetWindowsHookEx(WH_KEYBOARD_LL, hookproc, 0, 0);
    if(glhHook)
        bResult = true;
    return bResult;
}

bool MainWindow::stopHook()
{
    bool bResult=false;
    if(glhHook)
    {
        bResult= UnhookWindowsHookEx(glhHook);
        if(glhHook)
            bResult=true;
    }
    return bResult;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    stopHook();
    __super::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;


}

void MainWindow::on_btn_start_clicked()
{
    emit startTest(testTask_);
}

void MainWindow::onSepValChanged(int msec)
{
    testTask_.sepMsecs = msec;
}

void MainWindow::onDurValChanged(int min)
{
    testTask_.testMsecs = min*1000*60;
}

void MainWindow::on_btn_stop_clicked()
{
    monkeyTestExecutor_->stopMonkeyTest();
}

void MainWindow::on_btn_setArea_clicked()
{
    captureFrame_->startCapture();
}

void MainWindow::onAreaCaptured(QRect area )
{
    testTask_.area = area;
    QString format="area: (%1,%2,%3x%4)";
    QString text = format.arg(area.x()).arg(area.y()).arg(area.width()).arg(area.height());
    ui->lb_area->setText(text);
    captureFrame_->startDisplay();
}

void MainWindow::onClickedTriggered(QPoint globalPos)
{
    captureFrame_->appendClickedPos(globalPos);
}

void MainWindow::on_pushButton_clicked()
{
    static int cnt = 0;
}
