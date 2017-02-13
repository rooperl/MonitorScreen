#include "monitorwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(ORGANIZATION);
    QCoreApplication::setApplicationName(APP_NAME);
    QSettings settings;
    MonitorWindow window(QUrl(settings.value(URI_SETTING).toString()));
    window.show();
    return app.exec();
}
