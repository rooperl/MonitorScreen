#include "serverlistener.h"

ServerListener::ServerListener(const QUrl &url, QObject *parent) : url(url), QObject(parent) {
    connect(&webSocket, &QWebSocket::connected, this, &ServerListener::connected);
    webSocket.open(url);
}

void ServerListener::connected() {
    connect(&webSocket, &QWebSocket::textMessageReceived, this, &ServerListener::messageReceived);
    webSocket.sendTextMessage("Connected to " + url.toString());
}

void ServerListener::messageReceived(QString message) {
    qDebug() << message;
}
