#ifndef SERVERLISTENER_H
#define SERVERLISTENER_H

#include <QObject>
#include <QWebSocket>
#include <QBoxLayout>
#include <QDebug>

class ServerListener : public QObject {
    Q_OBJECT

public:
    explicit ServerListener(const QUrl &url, QObject *parent = 0);

private slots:
    void connected();
    void messageReceived(QString message);

private:
    QUrl url;
    QWebSocket webSocket;
};

#endif // SERVERLISTENER_H
