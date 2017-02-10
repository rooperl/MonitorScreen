#include "websockettest.h"
#include "ui_websockettest.h"

WebSocketTest::WebSocketTest(QWidget *parent) :
    QMainWindow(parent), server(new QWebSocketServer(APP_NAME,
        QWebSocketServer::NonSecureMode, this)), ui(new Ui::WebSocketTest) {

    ui->setupUi(this);
    textBrowser = ui->textBrowser;
    textEdit = ui->textEdit;
    QShortcut *shortcut = new QShortcut(QKeySequence(SEND_SHORTCUT), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(sendMessage()));
    shortcut = new QShortcut(QKeySequence(PREV_SHORTCUT), this);
    QObject::connect(shortcut, SIGNAL(activated()), this, SLOT(prevMessage()));

    if (server->listen(QHostAddress::Any, PORT)) {
        textBrowser->append(SERVER_LISTENING_TEXT + QString::number(PORT)
                                                                + INFO_TEXT);

        QObject::connect(server, SIGNAL(newConnection()), this,
                         SLOT(connected()));
        QObject::connect(server, SIGNAL(closed()), this, SLOT(disconnected()));
    }
    QTimer *timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TICK_LENGTH);
}

WebSocketTest::~WebSocketTest() {
    server->close();
    delete ui;
}

void WebSocketTest::connected() {
    QWebSocket *socket = server->nextPendingConnection();
    QObject::connect(socket, SIGNAL(textMessageReceived(QString)), this,
                     SLOT(messageReceived(QString)));
    QObject::connect(socket, SIGNAL(disconnected()), this,
                     SLOT(disconnected()));
    clients << socket;
}

void WebSocketTest::messageReceived(QString message) {
    for (QWebSocket* client : clients) {
        client->sendTextMessage(message);
    }
    textBrowser->append("> " + message);
}

void WebSocketTest::sendMessage() {
    prevText = textEdit->toPlainText();
    messageReceived(prevText);
    textEdit->clear();
}

void WebSocketTest::prevMessage() {
    textEdit->setText(prevText);
}

void WebSocketTest::disconnected() {
    QWebSocket *client = qobject_cast<QWebSocket*>(sender());
    clients.removeAll(client);
    client->deleteLater();
}

void WebSocketTest::on_actionExit_triggered(){
    if (QMessageBox::question(this, APP_NAME, EXIT_CONFIRM_TEXT,
    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) QApplication::quit();
}

void WebSocketTest::on_actionTest_Message_triggered() {
    prevText = TEST_MESSAGE;
    messageReceived(TEST_MESSAGE);
}

void WebSocketTest::update() {
    textBrowser->resize(this->width(), this->height() / BROWSER_RATIO -
                                                            HEIGHT_OFFSET);
    textEdit->move(0, this->height() / BROWSER_RATIO - HEIGHT_OFFSET);
    textEdit->resize(this->width(), this->height() / (2 * BROWSER_RATIO) -
                                                            HEIGHT_OFFSET);
}
