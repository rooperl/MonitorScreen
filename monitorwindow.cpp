#include "monitorwindow.h"
#include "ui_monitorwindow.h"

MonitorWindow::MonitorWindow(QUrl quri, QWidget *parent) : uri(quri), QMainWindow(parent), ui(new Ui::MonitorWindow) {
    ui->setupUi(this);
    resize(QDesktopWidget().availableGeometry(this).size() * 0.5);
    setWindowState(Qt::WindowMaximized);
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
    centralWidget()->setLayout(layout);
    windowSize.setWidth(rect().width());
    windowSize.setHeight(rect().height());
    statusBar()->showMessage(DISCONNECTED_TEXT);

    QObject::connect(&socket, SIGNAL(connected()), this, SLOT(connected()));
    QObject::connect(&socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    QObject::connect(&socket, SIGNAL(textMessageReceived(QString)), this, SLOT(messageReceived(QString)));

    text = new QLabel(this);
    text->setAlignment(Qt::AlignCenter);
    layout->addWidget(text);

    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TICK_LENGTH);
}

MonitorWindow::~MonitorWindow() {
    closeConnection();
    delete ui;
}

void MonitorWindow::on_actionExit_triggered() {
    if (QMessageBox::question(this, APP_NAME, EXIT_CONFIRM_TEXT,
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) QApplication::quit();
}

void MonitorWindow::on_actionConnect_triggered() {
    if (socket.state() == QAbstractSocket::ConnectedState) {
        if (QMessageBox::question(this, APP_NAME, DISCONNECT_CONFIRM_TEXT,
                                   QMessageBox::Yes|QMessageBox::No) == QMessageBox::No) return;
    }
    uri = QUrl(QInputDialog::getText(this, CONNECT_TEXT, WS_URI_TEXT, QLineEdit::Normal, uri.toString(), nullptr, Qt::WindowCloseButtonHint));
    closeConnection();
}

void MonitorWindow::connected() {
    socket.sendTextMessage(APP_NAME + CONNECTED_TO_TEXT + uri.toString());
    QJsonObject configObject;
    configObject[JSON_URI] = uri.toString();
    QFile configFile(CONFIG_FILE_NAME);

    if (!configFile.open(QIODevice::WriteOnly)) return;
    QJsonDocument configDocument(configObject);
    configFile.write(configDocument.toJson());
}

void MonitorWindow::disconnected() {
    socket.sendTextMessage(APP_NAME + DISCONNECTED_MESSAGE);
}

void MonitorWindow::messageReceived(QString message) {
    QJsonDocument jsonMessage = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = jsonMessage.object();
    message = jsonObject[JSON_VALUE].toString();

    QString statusMessage = uri.toString();
    if (jsonObject[JSON_NAME].toString() != "") statusMessage += STATUS_DELIMITER + jsonObject[JSON_NAME].toString();
    if (jsonObject[JSON_TIME].toString() != "") statusMessage += STATUS_DELIMITER + jsonObject[JSON_TIME].toString();

    statusBar()->showMessage(statusMessage);
    text->setText(message);
    resizeText();
}

void MonitorWindow::closeConnection() {
    socket.sendTextMessage(APP_NAME + DISCONNECTED_MESSAGE);
    socket.close();
    statusBar()->showMessage(DISCONNECTED_TEXT);
}

void MonitorWindow::resizeText() {
    double widthFactor = QFontMetrics(text->font()).width(processText(text->text())) / double(rect().width());
    double heightFactor = QFontMetrics(text->font()).height() / double(rect().width());
    int pointSize_new = font.pointSize();
    pointSize_new /= widthFactor > heightFactor ? widthFactor / SIZE_PERCENTAGE : heightFactor / SIZE_PERCENTAGE;
    if (pointSize_new > 0) font.setPointSize(pointSize_new);
    text->setFont(font);
    windowSize.setWidth(rect().width());
    windowSize.setHeight(rect().height());
}

QString MonitorWindow::processText(QString text) {
    while (text.length() < TEXT_LENGTH_MIN) {
        text = " " + text + " ";
    }
    return text;
}

void MonitorWindow::update() {
    if (socket.state() == QAbstractSocket::UnconnectedState && !uri.isEmpty()) socket.open(QUrl(uri));
    if (windowSize.width() != rect().width() || windowSize.height() != rect().height()) resizeText();
}
