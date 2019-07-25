#ifndef MONKEYTESTEXECUTOR_H
#define MONKEYTESTEXECUTOR_H

#include <QObject>
#include <QRect>
#include <QTimer>
#include <QThread>

struct MonkeyTestTask
{
    unsigned int sepMsecs;
    unsigned int testMsecs;
    QRect area;
};

class MonkeyTestExecutor : public QObject
{
    Q_OBJECT
public:
    explicit MonkeyTestExecutor(QObject *parent = 0);
signals:
    void sigClickTriggered(QPoint globalPos);
public slots:
    void startMonkeyTest(const MonkeyTestTask& task);
    void stopMonkeyTest();

private:
    void triggerClick();
private:
    QTimer timer_;
    QThread *execThread_;
    MonkeyTestTask curTask_;

    bool isRunning_;
};

Q_DECLARE_METATYPE(struct MonkeyTestTask);

#endif // MONKEYTESTEXECUTOR_H
