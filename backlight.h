#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QObject>
#include <queue>
#include <pthread.h>

using namespace std;

class Backlight: public QObject {
    Q_OBJECT

private:
    queue<QString> dataQueue;

public slots:
    bool write(const QString &data);

public:
    pthread_t thread;
    queue<QString> getQueue();
    QString queuePop();

    static void *queueProcessor(void *arg);

    Backlight();
    ~Backlight();
};

#endif // BACKLIGHT_H
