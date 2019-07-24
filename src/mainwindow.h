#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MonkeyTestExecutor.h"
#include "CaptureFrame.h"
#include "afx.h"


namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    virtual void closeEvent(QCloseEvent *);
signals:
    void startTest(const MonkeyTestTask&);

private slots:
    void on_btn_start_clicked();

    void on_btn_stop_clicked();

    void on_btn_setArea_clicked();

    void onAreaCaptured(QRect area);

    void onSepValChanged(int msec);

    void onDurValChanged(int min);

    void on_pushButton_clicked();

private:
    bool startHook();
    bool stopHook();
    LRESULT  CALLBACK  KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam);
private:
    Ui::MainWindow *ui;

    MonkeyTestTask testTask_;
    MonkeyTestExecutor *monkeyTestExecutor_;
    CaptureFrame *captureFrame_;

    HHOOK glhHook;         //安装的鼠标勾子句柄
    HINSTANCE glhInstance; //DLL实例句柄
};

#endif // MAINWINDOW_H
