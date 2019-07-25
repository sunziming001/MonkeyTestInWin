#ifndef CAPTUREFRAME_H
#define CAPTUREFRAME_H

#include <QWidget>
#include <QFrame>
#include <QPoint>
#include <QQueue>
#include <QPoint>

class CaptureFrame : public QFrame
{
    Q_OBJECT

public:
    enum FrameStatus
    {
        Idel,
        Capturing,
        Display
    };

    CaptureFrame(QWidget* parent);

    void startCapture();
    void startDisplay();

    void clearArea();

    void appendClickedPos(const QPoint& globalPos);

signals:
    void sigAreaCaptured(QRect area);
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
private:
    QRect captureArea_;

    QPoint pressPos_;
    QPoint movePos_;

    FrameStatus status_;

    QQueue<QPoint> clickedPosRecd_;

    const static int MaxClickedPosCnt = 100;
};

#endif // CAPTUREFRAME_H
