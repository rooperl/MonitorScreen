/*!
 * \file monitorwindow.cpp
 */
#include "monitorwindow.h"
#include "ui_monitorwindow.h"

MonitorWindow::MonitorWindow(QUrl quri, QWidget *parent) : uri(quri),
    QMainWindow(parent), ui(new Ui::MonitorWindow) {

    ui->setupUi(this);
    ui->actionDisconnect->setEnabled(false);
    ui->actionClear_parameters->setEnabled(false);

    resize(QDesktopWidget().availableGeometry(this).size() * WINDOW_INIT_RATIO);
    setWindowState(Qt::WindowMaximized);
    layout = new QBoxLayout(QBoxLayout::RightToLeft);
    parameterLayout = new QBoxLayout(QBoxLayout::TopToBottom);
    layout->addLayout(parameterLayout);
    centralWidget()->setLayout(layout);
    windowSize = rect().size();
    statusBar()->showMessage(DISCONNECTED_TEXT);

    QObject::connect(&socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(&socket, SIGNAL(disconnected()), this,
                                                SLOT(disconnected()));
    QObject::connect(&socket, SIGNAL(textMessageReceived(QString)), this,
                                                SLOT(messageReceived(QString)));

    text = new QLabel(this);
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);
    createParameterList();

    QShortcut *deleteShortcut = new QShortcut(QKeySequence(DELETE_SHORTCUT),
                                                                        this);
    QObject::connect(deleteShortcut, SIGNAL(activated()), this,
                                        SLOT(parameterDeleted()));

    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TICK_LENGTH);
    autoConnect = true;
}

MonitorWindow::~MonitorWindow() {
    closeConnection();
    delete ui;
}

void MonitorWindow::on_actionExit_triggered() {
    /*!
     * \brief Called when exiting via menu
     */
    if (QMessageBox::question(this, APP_NAME, EXIT_CONFIRM_TEXT,
    QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) QApplication::quit();
}

void MonitorWindow::on_actionConnect_triggered() {
    /*! \brief Called when connecting via menu */
    if (socket.state() == QAbstractSocket::ConnectedState
            && QMessageBox::question(this, APP_NAME, DISCONNECT_CONFIRM_TEXT)
            == QMessageBox::No) return;

    bool validUri;
    QUrl newUri = QUrl(QInputDialog::getText(this, CONNECT_TEXT, WS_URI_TEXT,
    QLineEdit::Normal, uri.toString(), &validUri, Qt::WindowCloseButtonHint));

    if (validUri && !isWss(newUri)) uri = newUri;
    closeConnection();
    autoConnect = true;
}

void MonitorWindow::on_actionDisconnect_triggered() {
    /*!
     * \brief Called when closing the connection via menu
     */
    if (socket.state() == QAbstractSocket::ConnectedState
            && QMessageBox::question(this, APP_NAME, DISCONNECT_CONFIRM_TEXT)
            == QMessageBox::Yes) closeConnection();
}

void MonitorWindow::on_actionClear_parameters_triggered() {
    /*!
     * \brief Called when clearing the parameter list via menu
     */
    if (parameterSet.size() && QMessageBox::question(this, APP_NAME,
        CLEAR_CONFIRM_TEXT) == QMessageBox::Yes) {
        parameterSet.clear();
        parameterList->clear();
        parameterLayout->removeWidget(parameterList);
        parameterList->hide();
        ui->actionClear_parameters->setEnabled(false);
    }
}

void MonitorWindow::connected() {
    /*!
     * \brief Called when the client is connected
     */
    socket.sendTextMessage(APP_NAME + CONNECTED_TO_TEXT + uri.toString());
    settings.setValue(URI_SETTING, uri);
    ui->actionDisconnect->setEnabled(true);
    statusBar()->showMessage(uri.toString());
}

void MonitorWindow::disconnected() {
    /*!
     * \brief Called when the client is disconnected
     */
    socket.sendTextMessage(APP_NAME + DISCONNECTED_MESSAGE);
    statusBar()->showMessage(DISCONNECTED_TEXT);
    ui->actionDisconnect->setEnabled(false);
}

void MonitorWindow::messageReceived(QString message) {
    /*!
     * \brief Called when the client receives a message
     */
    QJsonDocument jsonMessage = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonMessage.object();
    QString name = jsonObject[JSON_NAME].toString();
    message = jsonObject[JSON_VALUE].toString();

    if (name != "") {
        if (!(parameterSet.contains(name))) {
            parameterSet.insert(name);
            parameterList->addItem(name);

            if (parameterSet.size() == PARAM_THRESHOLD) {
                parameterLayout->addWidget(parameterList);
                ui->actionClear_parameters->setEnabled(true);
            }

            if (parameterSet.size() >= PARAM_THRESHOLD) parameterList->show();
        }
        lastValues[name] = jsonObject[JSON_VALUE].toString();
        lastTimes[name] = jsonObject[JSON_TIME].toString();
    }
    else if (parameterSet.size()) return;

    QString statusMessage = uri.toString();
    if (name != "") {
        statusMessage += STATUS_DELIMITER + name;
    }

    if (jsonObject[JSON_TIME].toString() != "") {
        statusMessage += STATUS_DELIMITER + jsonObject[JSON_TIME].toString();
    }

    if (selectedParameter == name || parameterSet.count() < PARAM_THRESHOLD) {
        statusBar()->showMessage(statusMessage);
        text->setText(message);
    }
    resizeText();
}

void MonitorWindow::closeConnection() {
    /*!
     * \brief Closes the current WebSocket connection
     */
    if (socket.state() == QAbstractSocket::ConnectedState) {
        socket.sendTextMessage(APP_NAME + DISCONNECTED_MESSAGE);
        socket.close();
    }
    statusBar()->showMessage(DISCONNECTED_TEXT);
    autoConnect = false;
}

void MonitorWindow::resizeText() {
    /*!
     * \brief Scales the displayed text to fit the current window size
     */
    double widthFactor =
            QFontMetrics(text->font()).width(processText(text->text()))
            / (double(rect().width()) - PARAM_LIST_WIDTH - PARAM_LIST_OFFSET);

    double heightFactor = QFontMetrics(text->font()).height()
                                    / (double(rect().height()) - HEIGHT_OFFSET);

    double pointSize_old = font.pointSize();
    double pointSize_new = pointSize_old;

    pointSize_new /= widthFactor > heightFactor ?
            widthFactor / SIZE_PERCENTAGE : heightFactor / SIZE_PERCENTAGE;

    if (pointSize_new > 0 && !isSimilarSize(pointSize_old, pointSize_new)) {
        font.setPointSize(pointSize_new);
    }
    text->setFont(font);
    windowSize = rect().size();
}

void MonitorWindow::createParameterList() {
    /*!
     * \brief Creates the parameter list widget
     */
    parameterList = new QListWidget;
    parameterList->setFixedWidth(PARAM_LIST_WIDTH);
    parameterList->setStyleSheet(PARAM_LIST_STYLE);

    QObject::connect(parameterList,
                     SIGNAL(itemPressed(QListWidgetItem*)), this,
                     SLOT(parameterClicked(QListWidgetItem*)));

    QObject::connect(parameterList,
                     SIGNAL(itemActivated(QListWidgetItem*)), this,
                     SLOT(parameterClicked(QListWidgetItem*)));
}

void MonitorWindow::parameterSelected(QString parameter) {
    /*!
     * \brief Called when a parameter is selected
     */
    selectedParameter = parameter;
    QString statusMessage = uri.toString();
    statusMessage += STATUS_DELIMITER + parameter;

    if (lastTimes[parameter] != "") {
        statusMessage += STATUS_DELIMITER + lastTimes[parameter];
    }
    statusBar()->showMessage(statusMessage);
    text->setText(lastValues[parameter]);
    resizeText();
}

bool MonitorWindow::isSimilarSize(int oldN, int newN) {
    /*!
     * \brief Returns true if the difference between two values
     *        is less than or equal to a specified tolerance value
     */
    return (oldN > newN ? oldN - newN : newN - oldN) <= SIZE_DIFF_TOLERANCE;
}

QString MonitorWindow::processText(QString text) {
    /*!
     * \brief Processes the text to be displayed
     */
    while (text.length() < TEXT_LENGTH_MIN) {
        text = " " + text + " ";
    }
    return text;
}

bool MonitorWindow::isWss(QUrl uri) {
    /*!
     * \brief Returns true if an URI uses the WSS scheme
     */
    return uri.scheme() == WSS_SCHEME;
}

void MonitorWindow::parameterClicked(QListWidgetItem* parameter) {
    /*!
     * \brief Called when a parameter is selected
     */
    parameterSelected(parameter->text());
}

void MonitorWindow::parameterDeleted() {
    /*!
     * \brief Removes the selected parameter from the parameter list
     */
    if (parameterList->currentItem()) {
        parameterSet.remove(parameterList->currentItem()->text());
        delete parameterList->currentItem();

        if (parameterSet.isEmpty()) {
            parameterList->hide();
            ui->actionClear_parameters->setEnabled(false);
        }
        else parameterSelected(parameterList->currentItem()->text());
    }
}

void MonitorWindow::update() {
    /*!
     * \brief Called every time the timer timeouts,
     *        frequency set by const short TICK_LENGTH
     */
    if (socket.state() == QAbstractSocket::UnconnectedState
            && !uri.isEmpty() && autoConnect) socket.open(QUrl(uri));

    if (windowSize != rect().size()) resizeText();
}
