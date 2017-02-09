#include <QApplication>
#include "websockettest.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WebSocketTest window;
    window.show();
    return app.exec();
}
