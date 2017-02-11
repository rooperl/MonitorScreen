#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QLabel>
#include <QToolButton>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <QWebSocket>
#include <QBoxLayout>
#include <QDebug>
#include "ui_monitorwindow.h"

const short TICK_LENGTH = 50;
const short HEIGHT_OFFSET = 50;
const short TEXT_LENGTH_MIN = 7;
const short BUTTON_WIDTH = 128;
const short BUTTON_OFFSET = 8;
const short PARAMETER_THRESHOLD = 2;
const short SIZE_DIFF_TOLERANCE = 1;
const float SIZE_PERCENTAGE = 0.85f;

const QString APP_NAME = "MonitorScreen";
const QString EXIT_CONFIRM_TEXT = "Are you sure you want to exit?";
const QString DISCONNECT_CONFIRM_TEXT = "End current connection?";
const QString DISCONNECTED_TEXT = "Disconnected";
const QString DISCONNECTED_MESSAGE = " disconnected";
const QString CONNECTED_TO_TEXT = " connected to ";
const QString CONNECT_TEXT = "Connect";
const QString WS_URI_TEXT = "WebSocket URI";
const QString STATUS_DELIMITER = " - ";
const QString WSS_SCHEME = "wss";
const QString BUTTON_STYLE = "font-size: 10pt;";

const QString JSON_NAME = "name";
const QString JSON_VALUE = "value";
const QString JSON_TIME = "time";
const QString JSON_URI = "uri";
const QString CONFIG_FILE_NAME = "config.json";

class Ui::MonitorWindow;
class MonitorWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MonitorWindow(QUrl quri = QUrl(), QWidget *parent = 0);
    ~MonitorWindow();

    void closeConnection();
    void resizeText();
    void parameterSelected(QString parameter);
    bool isSimilarSize(int oldN, int newN);
    bool isWss(QUrl uri);
    QString processText(QString text);

private slots:
    void on_actionExit_triggered();
    void on_actionConnect_triggered();
    void connected();
    void disconnected();
    void messageReceived(QString message);
    void update();

private:
    Ui::MonitorWindow *ui;
    QSize windowSize;
    QUrl uri;
    QWebSocket socket;
    QLabel *text;
    QList<QToolButton*> selectButtons;
    QBoxLayout *parameterLayout;
    QString prevText;
    QFont font;
    QSet<QString> parameterSet;
    QString selectedParameter;
    QHash<QString, QString> lastValues;
    QHash<QString, QString> lastTimes;
};

#endif // MONITORWINDOW_H
