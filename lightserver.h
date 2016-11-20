#ifndef LIGHTSERVER_H
#define LIGHTSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QTimer>
#include "backlight.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class LightServer : public QObject
{
    Q_OBJECT
public:
    explicit LightServer(quint16 port, Backlight *backlight, bool debug = false, QObject *parent = Q_NULLPTR);
    ~LightServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void timerTick();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
    Backlight *m_backlight;
    QTimer *timer;
};

#endif //LIGHTSERVER_H
