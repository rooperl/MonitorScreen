/*!
 * \file monitorwindow.h
 */
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
#include <QShortcut>
#include <QSettings>
#include <QDebug>
#include "ui_monitorwindow.h"

const short TICK_LENGTH = 50; /*!< Timer timeout frequency in milliseconds */
const short HEIGHT_OFFSET = 50; /*!< Height of the title and menu bar */
const short TEXT_LENGTH_MIN = 7; /*!< Text shorter than this value
                                      is processed before display */
const short PARAM_LIST_WIDTH = 128; /*!< Width of the parameter list */
const short PARAM_LIST_OFFSET = 16; /*!< Additional space between text
                                         and the parameter list */
const short PARAM_THRESHOLD = 2; /*!< Number of parameters required for the
                                      parameter list to be shown */
const short SIZE_DIFF_TOLERANCE = 1; /*!< Required difference between two font
                                          point sizes for text to be resized */
const float SIZE_PERCENTAGE = 0.9f; /*!< Text size is multiplied by this value
                                         before display to make sure it fits */
const float WINDOW_INIT_RATIO = 0.5f; /*!< Initial window size is screen size
                                           multiplied by this value */

const QString APP_NAME = "MonitorScreen"; /*!< Used in title bars and
                                               QSettings configuration */
const QString ORGANIZATION = "MonitorScreen"; /*!< Used in QSettings config */
const QString EXIT_CONFIRM_TEXT = "Are you sure you want to exit?";
/*!< Used in exit confirmation window */
const QString CLEAR_CONFIRM_TEXT =
                "Are you sure you want to clear the current parameter list?";
                /*!< Used in parameter clearing confirmation window */
const QString DISCONNECT_CONFIRM_TEXT = "End current connection?";
/*!< Used in disconnect confirmation window */
const QString DISCONNECTED_TEXT = "Disconnected"; /*!< Used in status bar */
const QString DISCONNECTED_MESSAGE = " disconnected"; /*!< Sent to server upon
                                                           disconnection */
const QString CONNECTED_TO_TEXT = " connected to "; /*!< Sent to server
                                                         upon connection */
const QString CONNECT_TEXT = "Connect"; /*!< Used in connection dialog */
const QString WS_URI_TEXT = "WebSocket URI"; /*!< Connection dialog label */
const QString STATUS_DELIMITER = " - "; /*!< Used in status bar to
                                             separate different fields */
const QString WSS_SCHEME = "wss"; /*!< Used when checking connection scheme */
const QString PARAM_LIST_STYLE = "font-size: 12pt;"; /*!< Parameter list
                                                          style sheet */
const QString DELETE_SHORTCUT = "Delete"; /*!< Key sequence for deleting
                                               a single parameter */

const QString JSON_NAME = "name"; /*!< JSON field for parameter name */
const QString JSON_VALUE = "value"; /*!< JSON field for parameter value */
const QString JSON_TIME = "time"; /*!< JSON field for message timestamp */
const QString URI_SETTING = "WebSocketURI"; /*!< Used in QSettings config to
                                                 store the last connection */

class Ui::MonitorWindow;
/*!
 * \brief MonitorWindow class
 */
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
    void parameterDeleted();
    void update();

private:
    Ui::MonitorWindow *ui; /*!< User interface object */
    QSize windowSize; /*!< Used for storing window size and
                           checking if the window is resized */
    QUrl uri; /*!< WebSocket URI */
    QWebSocket socket; /*!< Current WebSocket object */
    QBoxLayout *layout; /*!< Main layout used for displaying text */
    QBoxLayout *parameterLayout; /*!< Layout containing the
                                      parameter list widget */
    QListWidget *parameterList; /*!< Parameter list widget */
    QLabel *text; /*!< Text label for displaying received values */
    QString prevText; /*!< Used for storing the previous value as a string */
    QFont font; /*!< Font object for the text label */
    QSet<QString> parameterSet; /*!< Set of received parameters */
    QString selectedParameter; /*!< Selected parameter name */
    QHash<QString, QString> lastValues; /*!< Last value for each parameter */
    QHash<QString, QString> lastTimes; /*!< Last timestamp for each parameter */
    QSettings settings; /*!< Used for storing program configuration */
    bool autoConnect; /*!< Determines, whether the program should try
                           connecting to a WebSocket server automatically */
};

#endif // MONITORWINDOW_H
