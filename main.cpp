#include "monitorwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QUrl quri = QUrl();
    QFile configFile(CONFIG_FILE_NAME);

    if (configFile.open(QIODevice::ReadOnly)) {
        QByteArray configData = configFile.readAll();
        QJsonDocument configDocument(QJsonDocument::fromJson(configData));
        QJsonObject configObject = configDocument.object();
        quri = QUrl(configObject[JSON_URI].toString());
    }

    MonitorWindow window(quri);
    window.show();
    return app.exec();
}
