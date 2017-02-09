#ifndef WEBSOCKETTEST_H
#define WEBSOCKETTEST_H

#include <QMainWindow>
#include <QTextEdit>
#include <QTextBrowser>
#include <QShortcut>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtWebSockets/QtWebSockets>
#include "ui_websockettest.h"

const short PORT = 1234;
const short TICK_LENGTH = 10;
const short HEIGHT_OFFSET = 10;

const QString APP_NAME = "WebSocketTest";
const QString EXIT_CONFIRM_TEXT = "Are you sure you want to quit?";

class Ui::WebSocketTest;
class WebSocketTest : public QMainWindow {
    Q_OBJECT

public:
    explicit WebSocketTest(QWidget *parent = 0);
    ~WebSocketTest();

private slots:
    void connected();
    void messageReceived(QString message);
    void sendMessage();
    void disconnected();
    void on_actionExit_triggered();
    void update();

private:
    Ui::WebSocketTest *ui;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QWebSocketServer *server;
    QList<QWebSocket*> clients;
};

#endif // WEBSOCKETTEST_H
