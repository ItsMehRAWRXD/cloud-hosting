#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Set application metadata
    QApplication::setApplicationName("RawrXD IDE");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("RawrXD");
    QApplication::setOrganizationDomain("rawrxd.dev");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
