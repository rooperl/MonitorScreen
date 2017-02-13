#ifndef MONITORWINDOW_H
#define MONITORWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDesktopWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <QWebSocket>
#include <QBoxLayout>
#include <QListWidget>
#include <QDebug>
#include "ui_monitorwindow.h"

const short TICK_LENGTH = 50;
const short HEIGHT_OFFSET = 50;
const short TEXT_LENGTH_MIN = 7;
const short PARAM_LIST_WIDTH = 128;
const short PARAM_LIST_OFFSET = 16;
const short PARAM_THRESHOLD = 2;
const short SIZE_DIFF_TOLERANCE = 1;
const float SIZE_PERCENTAGE = 0.9f;
const float WINDOW_INIT_RATIO = 0.5f;

const QString APP_NAME = "MonitorScreen";
const QString EXIT_CONFIRM_TEXT = "Are you sure you want to exit?";
const QString CLEAR_CONFIRM_TEXT = "Are you sure you want to clear "
                                   "the current parameter list?";
const QString DISCONNECT_CONFIRM_TEXT = "End current connection?";
const QString DISCONNECTED_TEXT = "Disconnected";
const QString DISCONNECTED_MESSAGE = " disconnected";
const QString CONNECTED_TO_TEXT = " connected to ";
const QString CONNECT_TEXT = "Connect";
const QString WS_URI_TEXT = "WebSocket URI";
const QString STATUS_DELIMITER = " - ";
const QString WSS_SCHEME = "wss";
const QString PARAM_LIST_STYLE = "font-size: 12pt;";
const QString THREE_DOTS = "...";

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
    void createParameterList();
    void parameterSelected(QString parameter);
    bool isSimilarSize(int oldN, int newN);
    bool isWss(QUrl uri);
    QString processText(QString text);

private slots:
    void on_actionExit_triggered();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionClear_parameters_triggered();
    void connected();
    void disconnected();
    void messageReceived(QString message);
    void parameterClicked(QListWidgetItem* parameter);
    void update();

private:
    Ui::MonitorWindow *ui;
    QSize windowSize;
    QUrl uri;
    QWebSocket socket;
    QBoxLayout *layout;
    QBoxLayout *parameterLayout;
    QListWidget *parameterList;
    QLabel *text;
    QString prevText;
    QFont font;
    QSet<QString> parameterSet;
    QString selectedParameter;
    QHash<QString, QString> lastValues;
    QHash<QString, QString> lastTimes;
    bool autoConnect;
};

#endif // MONITORWINDOW_H
