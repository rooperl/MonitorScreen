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
const float BROWSER_RATIO = 1.5f;

const QString APP_NAME = "WebSocketTest";
const QString SEND_SHORTCUT = "Ctrl+Return";
const QString EXIT_CONFIRM_TEXT = "Are you sure you want to quit?";
const QString SERVER_LISTENING_TEXT = "Server listening on port ";
const QString INFO_TEXT = "\nPress Ctrl + Enter to send messages"
                          "\nPress Ctrl + T to send a test message";
const QString TEST_MESSAGE =
"{\"name\":\"Vin\",\"value\":\"14.257 V\",\"time\":\"2017/09/02 - 01:18:30\"}";

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
    void on_actionTest_Message_triggered();
    void update();

private:
    Ui::WebSocketTest *ui;
    QTextEdit *textEdit;
    QTextBrowser *textBrowser;
    QWebSocketServer *server;
    QList<QWebSocket*> clients;
};

#endif // WEBSOCKETTEST_H
