#ifndef CAPTUREFRAME_H
#define CAPTUREFRAME_H

#include <QWidget>
#include <QFrame>
#include <QPoint>
class CaptureFrame : public QFrame
{
    Q_OBJECT
public:
    CaptureFrame(QWidget* parent);
    void clearArea();
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
};

#endif // CAPTUREFRAME_H
