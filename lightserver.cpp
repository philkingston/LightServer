#include "lightserver.h"
#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QDebug>

QT_USE_NAMESPACE

LightServer::LightServer(quint16 port, Backlight *backlight, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("LightServer"),
                                            QWebSocketServer::NonSecureMode, this)),
    m_clients(),
    m_debug(debug),
    m_backlight(backlight) {
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug)
            qDebug() << "LightServer listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &LightServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &LightServer::closed);
    } else {
        qDebug() << m_pWebSocketServer->errorString();
    }
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

LightServer::~LightServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void LightServer::timerTick() {
    qDebug() << "tick";
}

void LightServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &LightServer::processTextMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &LightServer::socketDisconnected);

    m_clients << pSocket;
}

void LightServer::processTextMessage(QString message) {
    if (m_debug)
        qDebug() << "Message received:" << message;
    if(message == "fastcycle") {
        // Start fast timer
        timer->start(1000);
    } else if(message == "static") {
        timer->stop();
    } else {
        m_backlight->write(message);
    }
}

void LightServer::socketDisconnected() {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug)
        qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
