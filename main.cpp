#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Finance Tracker");
    app.setOrganizationName("MyFinance");

    MainWindow window;
    window.show();

    return app.exec();
}
